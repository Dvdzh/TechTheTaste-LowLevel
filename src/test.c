#include "pico/time.h"
#include <pico/stdlib.h>
#include <math.h>
#include "hardware/pwm.h"

double clockDivider(int speed){
	return ceil(125000000.0/(speed*4096))/16;
}

int	wrapCalculator(int speed){
	double clkfreq = 125000000*16/ceil(125000000.0/(speed*4096));
	return floor(clkfreq/speed);
}




int main(){
	gpio_set_function(7, GPIO_FUNC_PWM);
	int pwmSlice=pwm_gpio_to_slice_num(7);
	int pwmChan=pwm_gpio_to_channel(7);
	for(int k=0;k<5000;k+=10){
		int wrap=2;
		pwm_set_clkdiv(pwmSlice,100);
		pwm_set_wrap(pwmSlice,wrap);
		pwm_set_chan_level(pwmSlice,pwmChan,wrap/2);
		pwm_set_enabled(pwmSlice,true);
		sleep_ms(1000);
	}








	while(1){
	tight_loop_contents();
	}
}

