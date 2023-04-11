#include "hardware/irq.h"
#include "pico/time.h"
#include <pico/stdlib.h>
#include <function.h>
#include <com.h>
 


int main(){
	void (*fonction[16]) (unsigned int comp, unsigned short arg0, unsigned short arg1) = {
		*lidarStop,
		*move, 
		*rotatefunction,
		*cancelMove,
		*arm,
		*motorTime,
		*pumpsvalvemotors,	
		*motors,
		*motorsArgs,
		*setVar,
		*getVar,
		*track,
		NULL,
		NULL,
		*identification,
		*syncro
		};
			
	BufferInit(&buffer);
	uartInit();
	uartIrqSetup();
	irq_set_priority(UART0_IRQ,0);
	while (1) {
		if(sync){
			BufferInit(&buffer);
			sync=0;
			orderExecuted=0;
		}
		else {
			if(buffer.BufferOrderNumber>orderExecuted){
				int flag=!ReadNewOrder(order,&buffer);
				id = getID(order[0]);
				comp = getCOMP(order[0]);
				arg0 = (((unsigned short) order[1]) << 8) + ((unsigned short) order[2]);
				arg1 = (((unsigned short) order[3]) << 8) + ((unsigned short) order[4]);
				if(flag && id!=0 && id!=3){
					fonction[id](comp,arg0,arg1);
					orderExecuted++;
				}
				else if( id==0 || id==3){
					orderExecuted++;
				}
			}
		}	
		sleep_ms(100);
	}
}


