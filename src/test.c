#include "pico/platform.h"
#include "pico/time.h"
#include <stepper.h>
#include <pico/stdlib.h>


int main(){
	int target[2]={100,100};
	int target1[2]={-100,-100};
	stepper stepper2;
	endstop endstop1;
	stepper stepper1;
	endstop endstop2;
	endstopInit(&endstop1,10,1);
	endstopInit(&endstop2,11,2);
	stepperInit(&stepper1,2,9,8,10,1);
	stepperInit(&stepper2,1,7,6,10,2);
	homming(&stepper2,&endstop1,1);
	homming(&stepper1,&endstop2,1);
	sleep_ms(10000);
	armMove(target);
	sleep_ms(15000);
	armMove(target1);



















	/*stepper motors1;
	stepper motors2;
	int id[2] = {1,2};
	int dir[2] = {0,0};
	int dir1[2] = {0,1};
	int target[2] = {100,100};
	stepperInit(&motors1,1,7,6,10,5);
	stepperInit(&motors2,2,9,8,10,5);
	motorValueStepper(steppers,id,dir,target,2);
	sleep_ms(20000);
	motorValueStepper(steppers,id,dir1,target,2);
	sleep_ms(20000);
	motorValueStepper(steppers,id,dir,target,2);*/
	while(1){
	tight_loop_contents();
	}
}

