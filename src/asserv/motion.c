#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>

#include "pico/stdlib.h"
#include "hardware/pwm.h"
#include "pico/time.h"
#include "hardware/irq.h"
#include "hardware/gpio.h"

#include "./header/encoder.h"
#include "./header/PID.h"
#include "./header/motor.h"       
#include "./header/motion.h"

#define MAX(a, b) (a > b) ? a : b;  
int consigne; // How far we want to go,int cause ticks, it's the distance or angle we want
float kP_right=0.7;
float kD_right=0;
float kI_right=0;

float kP_left=0.7;
float kD_left=0;
float kI_left=0;

float kP_trans=0;
float kD_trans=1;
float kI_trans=0;

float kP_rot=0;
float kD_rot=1;
float kI_rot=0;

int timer=1;
int lidar=0;
int cancelmove=0;

errors errorpid;
struct repeating_timer pidtimer;
int setpoint;

void init_all_enc_mot(){

    init_encoder(Signal_A_Right);
    init_encoder(Signal_A_Left);
    init_encoder(Signal_B_Right);
    init_encoder(Signal_B_Left);

    init_motor(Motor_R_For,&slice_R_For, &channel_R_For);
    init_motor(Motor_R_Rev,&slice_R_Rev, &channel_R_Rev);
    init_motor(Motor_L_For,&slice_L_For, &channel_L_For);
    init_motor(Motor_L_Rev,&slice_L_Rev, &channel_L_Rev);
	cancelmove=0;
	counter_Left=0;
	counter_Right=0;
	Actual_left=0;
	Actual_right=0;
	Actual_rot=0;
	Actual_trans=0;

}

void init_interrupt(){
    
    gpio_set_irq_enabled_with_callback(Signal_A_Right,GPIO_IRQ_EDGE_RISE | GPIO_IRQ_EDGE_FALL, true, read_encoder);
    gpio_set_irq_enabled(Signal_A_Left, GPIO_IRQ_EDGE_RISE | GPIO_IRQ_EDGE_FALL, true);
    gpio_set_irq_enabled(Signal_B_Right, GPIO_IRQ_EDGE_RISE, true);
    gpio_set_irq_enabled(Signal_B_Left, GPIO_IRQ_EDGE_RISE, true);

}

//Forward: positive distance ; Reverse: negative distance   
//HL job to convert the distance we want to go to a certain number of ticks and it give it to us as consigne 
//add distance between the two encoder, the delta in the command to the motors, see how to do that

bool updatErrors(struct repeating_timer *t){
	
	Actual_left = counter_Left;
    Actual_right = counter_Right;

    if (setpoint<0){ 
        Actual_trans= -((Actual_left+Actual_right)/2); 
    }
    if (setpoint>0){
        Actual_trans= (Actual_left+Actual_right)/2;  
    }
	
	Actual_rot= (Actual_right - Actual_left)/2;   
	
	errorpid.globalerror=setpoint-Actual_trans;
	errorpid.globalsum+=errorpid.globalerror;
	errorpid.lefterror=setpoint-Actual_left;
	errorpid.leftsum+=errorpid.lefterror;
	errorpid.righterror=setpoint-Actual_right;
	errorpid.rightsum+=errorpid.righterror;
	errorpid.rotglobalerror=setpoint-Actual_rot;
	errorpid.rotglobalsum+=errorpid.rotglobalerror;
	errorpid.negativelefterror=-setpoint-Actual_left;
	errorpid.negativelefterror+=errorpid.negativelefterror;
}


void move_translate (int consigne){
	setpoint=consigne;

	const uint LED_PIN = PICO_DEFAULT_LED_PIN;
	gpio_init(LED_PIN);
	gpio_set_dir(LED_PIN, GPIO_OUT);


    
    Actual_left = counter_Left;
    Actual_right = counter_Right;

    if (consigne <0){ 
        Actual_trans= -((Actual_left+Actual_right)/2); 
    }
    if (consigne>0){
        Actual_trans= (Actual_left+Actual_right)/2;  
    }

    float alpha_right= MAX(1 - (Actual_right/consigne), 0);
    float alpha_left= MAX(1 - (Actual_left/consigne), 0);

    dif_right= consigne - Actual_right ;
    dif_left = consigne - Actual_left ;
	
	if(timer){
	 add_repeating_timer_ms(5,updatErrors,NULL,&pidtimer);
	 timer=0;
	}

    Output_trans = PID(kP_trans, kI_trans,kD_trans,errorpid.globalerror,errorpid.globalsum,last_error_trans);
    Output_left = PID(kP_left,kI_left,kD_left,errorpid.lefterror,errorpid.leftsum,last_error_left);
    Output_right = PID(kP_right,kI_right, kD_right,errorpid.righterror,errorpid.rightsum, last_error_right);

	
    command_left = Output_trans + ((1-alpha_left) * pwm_base) + ( alpha_left*Output_left);
    command_right = Output_trans + ((1-alpha_right) * pwm_base) + (alpha_right*Output_right);

	if( lidar || cancelmove){
	//	gpio_put(LED_PIN,0);
        //We turn off both motors
        command_motors (slice_R_Rev,slice_R_For, channel_R_Rev,channel_R_For,0, dif_right);
        command_motors (slice_L_Rev,slice_L_For,channel_L_Rev,channel_L_For,0, dif_left);
	}
	else{
	//	gpio_put(LED_PIN,1);
		//Command PWM right motor
        command_motors (slice_R_Rev,slice_R_For, channel_R_Rev,channel_R_For,command_right, dif_right);
        //Command PWM left motor
        command_motors (slice_L_Rev,slice_L_For,channel_L_Rev,channel_L_For,command_left, dif_left);
	}   
	last_error_trans=errorpid.globalerror;
	last_error_left=errorpid.lefterror;
	last_error_right=errorpid.righterror;
}


//angle converted to a certain number of ticks // HL job
//Clockwise : positive consigne ; Counter Clockwise : negative consigne (not sure at all, to check)
void move_rotate (int consigne){  
	setpoint=consigne;	

    Actual_left = counter_Left;
    Actual_right = counter_Right;

    Actual_rot= (Actual_right - Actual_left)/2;      
     
	if(timer){
	 add_repeating_timer_ms(5,updatErrors,NULL,&pidtimer);
	 timer=0;
	}          

    float alpha_right=MAX( 1 - (Actual_right/consigne),0);
    float alpha_left=MAX( 1 - (Actual_left/(-consigne)),0);

    dif_right= consigne - Actual_right ;
    dif_left= (-consigne) - Actual_left;

    Output_rot = PID(kP_rot, kI_rot,kD_rot,errorpid.rotglobalerror,errorpid.rotglobalsum,last_error_rot); 
    Output_left = PID(kP_left,kI_left,kD_left,errorpid.negativelefterror,errorpid.negativeleftsum,last_error_left);
    Output_right = PID(kP_right,kI_right, kD_right,errorpid.righterror,errorpid.rightsum,last_error_right);


    command_left = Output_rot + ((1-alpha_left) * pwm_base) + (alpha_left*Output_left ) ;
    command_right = Output_rot + ((1-alpha_right)* pwm_base) + (alpha_right*Output_right);
	
	if(lidar || cancelmove){
        //We turn off both motors
        command_motors (slice_R_Rev,slice_R_For, channel_R_Rev,channel_R_For,0, dif_right);
        command_motors (slice_L_Rev,slice_L_For,channel_L_Rev,channel_L_For,0, dif_left);
	}
	else{
		//Command PWM right motor
        command_motors (slice_R_Rev,slice_R_For, channel_R_Rev,channel_R_For,command_right, dif_right);
        //Command PWM left motor
        command_motors (slice_L_Rev,slice_L_For,channel_L_Rev,channel_L_For,command_left, dif_left);
	} 
	last_error_rot=errorpid.rotglobalerror;
	last_error_right=errorpid.righterror;
	last_error_left=errorpid.negativelefterror;
}


bool translate (int consigne){ 

    dif_right= consigne - Actual_right ;
    dif_left = consigne - Actual_left ;

    dif =dif_left*dif_left+dif_right*dif_right;

    if (abs(dif)>2500){
        return true;
    }
    else {

    //Command PWM right motor
    command_motors (slice_R_Rev,slice_R_For, channel_R_Rev, channel_R_For, 0, dif_right); 

    //Command PWM left motor
    command_motors ( slice_L_Rev, slice_L_For,channel_L_Rev,channel_L_For, 0, dif_left);

        return false;
    }
	cancel_repeating_timer(&pidtimer);
	timer=1;

}


bool rotate (int consigne){
    dif_right= consigne - Actual_right ;
    dif_left= (-consigne) - Actual_left;

    dif =dif_right*dif_right+dif_left*dif_left;


    if (abs(dif)>2500){
        return true;
    }
    else{
        command_motors (slice_R_Rev,slice_R_For, channel_R_Rev,channel_R_For,0, dif_right);
        command_motors (slice_L_Rev,slice_L_For,channel_L_Rev,channel_L_For,0, dif_left);

        return false;
    }
	cancel_repeating_timer(&pidtimer);
	timer=1;

}


