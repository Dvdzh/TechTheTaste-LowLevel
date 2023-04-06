#include "hardware/gpio.h"
#include "pico/time.h"
#include "pico/stdlib.h"
#include <stdio.h>
#include <stdint.h>
#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <function.h>
#include <com.h>
#include <encoder.h>
#include <motor.h>
#include <PID.h>
#include <motion.h>
#include <stepper.h>
float vitesse = 450;
static char ordermotors[5]={0x70,0x00,0x00,0x00,0x00}; 
static uint nbmotors=0;
static short positionmotors[16]={0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
static uint nbordermotors=0;
static float *pid[16]={&kP_right,&kD_right,&kI_right,&kP_left,&kD_left,&kI_left,&kP_trans,&kD_trans,&kI_trans,&kP_rot,&kD_rot,&kI_rot,&vitesse,NULL,NULL,NULL};
static char ordercancelmove[5]={0x30,0x00,0x00,0x00,0x00};
static char orderlidarstop[5]={0x00,0x00,0x00,0x00,0x00};
static int curve=0;
static struct repeating_timer timer;
	



void lidarStop( unsigned int comp, unsigned short arg0, unsigned short arg1){
	
}


void move( unsigned int comp, unsigned short arg0, unsigned short arg1){
	char received[5];
	acknowledge(order);
	init_all_enc_mot();
	init_interrupt();
	while(translate((short) arg1)){
		move_translate((short) arg1);
		if(cancelmove){
			move_translate((short) arg1);
			finish(ordercancelmove);
			return;
		}
	}
	finish(order);
}


void rotatefunction( unsigned int comp,unsigned short arg0, unsigned short arg1){
	acknowledge(order);
	init_all_enc_mot();
	init_interrupt();
	while(rotate((short) arg1)){
		move_rotate((short) arg1);
		if(cancelmove){
			move_rotate((short) arg1);
			finish(ordercancelmove);
			return;
		}
	}
	finish(order);
}



void cancelMove( unsigned int comp, unsigned short arg0, unsigned short arg1){
	
	
	}


void arm( unsigned int comp, unsigned short arg0, unsigned short arg1){
	acknowledge(order);
	int target[2]={(short) arg0, (short) arg1};
	armMove(target,(int)vitesse);
	}
	


void motorTime( unsigned int comp, unsigned short arg0, unsigned short arg1){

}


void  pumps( unsigned int comp, unsigned short arg0, unsigned short arg1){
	
	
}


void  motors( unsigned int comp, unsigned short arg0, unsigned short arg1){
	acknowledge(order);	
	int target[2]={arg0,arg1};


	/*	acknowledge(order);
	nbmotors=comp;*/
	
}


void motorsArgs( unsigned int comp, unsigned short arg0, unsigned short arg1){
	if(nbmotors>nbordermotors){
		positionmotors[nbordermotors]=arg1;
		nbordermotors++;
		acknowledge(order);
	}
	if(nbmotors==nbordermotors){
		nbordermotors=0;
		finish(ordermotors);
	}
}


void setVar( unsigned int comp, unsigned short arg0, unsigned short arg1){
	acknowledge(order);
	*pid[comp]=getFloat(arg0,arg1);
	finish(order);
	
	}
void getVar( unsigned int comp, unsigned short arg0, unsigned short arg1){
	acknowledge(order);
	sendVar(getInt(*pid[comp]),2,comp);
	finish(order);
	}
void track( unsigned int comp, unsigned short arg0, unsigned short arg1){
	acknowledge(order);
	curve=!curve;
	if(curve){
		add_repeating_timer_ms(100,sendtrack,NULL,&timer);
	
	}
	else{
		cancel_repeating_timer(&timer);
	}
	finish(order);
}

void trololol(unsigned int comp, unsigned short arg0, unsigned short arg1){
	int a=2; 
	a++;
}


void mainprocess(){



	
	void (*fonction[16]) (unsigned int comp, unsigned short arg0, unsigned short arg1) = {
		NULL,
		*move, 
		*rotatefunction,
		NULL,
		*arm,
		*motorTime,
		*pumps,	
		*motors,
		*motorsArgs,
		*setVar,
		*getVar,
		*track,
		NULL,
		NULL,
		NULL,
		NULL
		};
	
	const uint LED_PIN = PICO_DEFAULT_LED_PIN;
	gpio_init(LED_PIN);
	gpio_set_dir(LED_PIN, GPIO_OUT);

	
	BufferInit(&buffer);
	uartInit();
	uartIrqSetup();
    while (1) {
			int flag=!ReadNewOrder(order,&buffer);
			id = getID(order[0]);
			comp = getCOMP(order[0]);
			arg0 = (((unsigned short) order[1]) << 8) + ((unsigned short) order[2]);
			arg1 = (((unsigned short) order[3]) << 8) + ((unsigned short) order[4]);
			if(id==10){	gpio_put(LED_PIN,1);}
			if(flag){
				fonction[id](comp,arg0,arg1);
				orderExecuted++;
			
		}
	}
}








