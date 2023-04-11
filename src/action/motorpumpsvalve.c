#include <pico/stdlib.h>
#include <stdint.h>
#include "hardware/pwm.h"
#include "pico/time.h"
#include "hardware/gpio.h"

typedef struct{
	int pin;
	int id;
	int pwm;
	int type;
	struct repeating_timer pwmmotor;
}action;

action pumplist[10];
action valvelist[10];
action motorlist[10];
int motorpwmlevel[2]={0,0};



bool pwmrampe(struct repeating_timer *t){
	for(int k=0;k<8;k++){
		if(&motorlist[k].id==t->user_data){
			if(motorpwmlevel[k]<2000){
				motorpwmlevel[k]+=10;
				int sliceNum = pwm_gpio_to_slice_num(motorlist[k].pin);
				int chan = pwm_gpio_to_channel(motorlist[k].pin); 
				pwm_set_chan_level(sliceNum,chan,motorpwmlevel[k]);
			}
			else{
				cancel_repeating_timer(t);
			}

		}
	}
	return true;
}


int actionGpioInit(action *action,int pin,int id,int pwm,int type){
	action->pin=pin;
	switch(type){
		case 0:
			action->type=0;
			action->pwm=0;
			action->id=id;
			pumplist[id-1]=*action;
			break;
		case 1:
			action->type=1;
			action->pwm=0;
			action->id=id;
			valvelist[id-1]=*action;
			break;
		case 2:
			action->type=2;
			action->pwm=0;
			action->id=id;
					break;
	}
	if(pwm) {
		action->pwm=1;
		gpio_set_function(pin,GPIO_FUNC_PWM);
	}
	else{
		gpio_init(action->pin);
		gpio_set_dir(action->pin,GPIO_OUT);
		gpio_put(action->pin,0);
		gpio_is_pulled_down(action->pin);
	}
	motorlist[id-1]=*action;

	return 0;
}

int updatePumpValve(uint8_t mask, int type,int lenght){
	for(int k=0;k<lenght;k++){
		volatile int state=mask&1;
		if(type==0){
			gpio_put(pumplist[k].pin,state);
		}
		else{
			gpio_put(valvelist[k].pin,state);
		}
		mask>>=1;
	}
	return 0;
}

int updateMotor(uint8_t mask,int lenght){
	for(int k=0;k<lenght;k++){
		int state=mask&1;
		if(motorlist[k].pwm==1){
			int sliceNum= pwm_gpio_to_slice_num(motorlist[k].pin);
			int channel =pwm_gpio_to_channel(motorlist[k].pin);
			if(state){
				pwm_set_wrap(sliceNum,6250);                                //we set the wrap at 6250 for a frequency of 20kHz
				pwm_set_chan_level(sliceNum,channel,0);
				pwm_set_enabled(sliceNum,true);
				add_repeating_timer_ms(10,pwmrampe,&motorlist[k].id,&motorlist[k].pwmmotor);
			}
			else{
				pwm_set_enabled(sliceNum,false);
				motorpwmlevel[k]=0;
			}
		}
		else{
			gpio_put(motorlist[k].pin,state);
		}
		mask>>=1;
	}
	return 0;
}


int actionInit1A(){
	action pump1;
	action valve1;
	actionGpioInit(&pump1,13,1,0,0);
	actionGpioInit(&valve1,18,1,0,1);

}

int actionInit2A(){
}





