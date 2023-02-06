#include <stdio.h>
#include <stdint.h>
#include "pico/stdlib.h"
#include "hardware/uart.h"
#include "hardware/irq.h"
#include "pico/time.h"
#include <com.h>
#include <function.h>

void (*fonction[16]) (unsigned int comp, unsigned short arg0, unsigned short arg1) = {
	*lidarStop,
	*move, 
	*rotate,
	*cancelMove,
	*motorValue,
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

int main() {
	BufferInit(&buffer);
	uartInit();
	uartIrqSetup();
    while (1) {
		if(buffer.BufferOrderNumber>orderExecuted){
			int flag=!ReadNewOrder(order,&buffer);
			id = getID(order[0]);
			comp = getCOMP(order[0]);
			arg0 = (((unsigned short) order[1]) << 8) + ((unsigned short) order[2]);
			arg1 = (((unsigned short) order[3]) << 8) + ((unsigned short) order[4]);
			if(flag){
				fonction[id](comp,arg0,arg1);
				orderExecuted++;
			}
		}
	}
}



