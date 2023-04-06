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

bool pwmrampe(struct repeating_timer *t);

int actionGpioInit(action *action,int pin,int id,int pwm,int type);
