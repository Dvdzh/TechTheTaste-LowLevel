#include <stdio.h>
#include <time.h>
#include <stdbool.h>

#include "./header/encoder.h"
#include "./header/motor.h"
#include "./header/PID.h"
#include "./header/motion.h"

#include "hardware/gpio.h"
#include "pico/stdlib.h"
#include "hardware/pwm.h"
#include "pico/time.h"
#include "hardware/irq.h"
#include <stdio.h>

bool callback(struct repeating_timer *t){
		uart_putc(uart0,0X0F);
		return true;
}


#include "hardware/uart.h"

/// \tag::hello_uart[]

#define UART_ID uart0
#define BAUD_RATE 115200

// We are using pins 0 and 1, but see the GPIO function select table in the
// datasheet for information on which other pins can be used.
#define UART_TX_PIN 0
#define UART_RX_PIN 1

int main() {
    // Set up our UART with the required speed.
    uart_init(UART_ID, BAUD_RATE);

    // Set the TX and RX pins by using the function select on the GPIO
    // Set datasheet for more information on function select
    gpio_set_function(UART_TX_PIN, GPIO_FUNC_UART);
    gpio_set_function(UART_RX_PIN, GPIO_FUNC_UART);

/*	const uint LED_PIN = PICO_DEFAULT_LED_PIN;
    gpio_init(LED_PIN);
    gpio_set_dir(LED_PIN, GPIO_OUT);

    stdio_init_all();       //Permet de lire sur le minicom

    init_all_enc_mot();     //Initialise tous les gpio  moteurs et encodeurs

    init_interrupt(); 	    //Set up the interruptions for the encoders so that they start counting 
    
    sleep_ms(1000);
    
    //demander type de mouvement : translation ou rotation
    //demander consgine : combien de ticks 


    consigne  = 1000; 


   while (translate(consigne)){
        move_translate(consigne);
    }
	gpio_put(LED_PIN,1);
	*/
	
	
	struct repeating_timer timer;

	add_repeating_timer_ms(100,callback,NULL,&timer);
    return 0;
}

