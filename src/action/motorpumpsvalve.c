#include "hardware/gpio.h"
#include "pico/stlib.h"
#include <stdint.h>
#include "hardware/pwm.h"


typedef struct{
	int pin;
	int target;
	int id;
	int pwm;
	int type;
}action;

action pumplist[8];
action valvelist[8];
action motorlist[8];

int ActionGpioInit(action *action,int pin,int target,int id,int pwm,int type){
	action->pin=pin;
	if(action->pwm) {
		gpio_set_function(pin,GPIO_FUNC_PWM);
	}
	else {
		gpio_init(action->pin);
		gpio_set_dir(action->pin,0);
		gpio_put(action->pin,0);
		int tmp=1;
		tmp<<=pin;
	}
	switch(type){
		case 0:
			pumplist[id-1]=*action;
			break;
		case 1:
			valvelist[id-1]=*action;
			break;
		case 2:
			motorlist[id-1]=*action;
			break;
	}
	return 0;
}

int updatePumpValve(uint8_t mask){
	for(int k=0;k<8;k++){
		int state=mask&1;
		gpio_put(pumplist[k].pin,state);
		mask>>=1;
	}
	return 0;
}

int updateMotor(uint8_t mask){
	for(int k=0;k<8;k++){
		int state=mask&1;


















		


























































/*static uint32_t valuegpiomask=0;
static uint32_t gpiomask=0;

typedef struct{
	int pin;
	int target;
	int id;
	int pwm;
	int type;
}action;


action actionlist[3][5];


int ActionGpioInit(action *action,int pin,int id,int type){
	action->pin=pin;
	if(action->pwm) {
		gpio_set_function(pin,GPIO_FUNC_PWM);
	}
	else {
	gpio_init(action->pin);
	gpio_set_dir(action->pin,0);
	gpio_put(action->pin,0);
	int tmp=1;
	tmp<<=pin;
	gpiomask|=tmp;
	}
	actionlist[type][id-1]=*action;
	return 0;
}



int getmask(uint8_t mask,action actionlist[],int type){
	for(int i=0;i<8;i++){
		int tmp=mask&1;
		for(int k=0;k<5;k++){
			if(actionlist[type][k].type==type && actionlist[type][k].id==i && actionlist[type][k].pwm!=1){
				tmp<<=actionlist[k].pin;
				valuegpiomask|=tmp;
			}
			else{
				actionlist[type][k].target=tmp;
			}
		mask>>=1;
		}
	}
}

int execute(){
	for(int k=0;k<10;k++){
		if(actionlist[k].pwm==1){
			uint slicenum = pwm_gpio_to_slice_num(actionlist[k].pin);
			uint chan = pwm_gpio_to_channel(actionlist[k].pin);	
			if(actionlist[k].target==0){
				pwm_set_enabled(slicenum,0);
			}
			else{
				pwm_set_wrap(slicenum,6250);

			}
		}
	}
	gpio_put_masked(gpiomask, valuegpiomask);
}

int main() {
	action	

} */
