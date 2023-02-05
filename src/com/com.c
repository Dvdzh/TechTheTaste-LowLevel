#include <stdio.h>
#include <stdint.h>
#include <inttypes.h>
#include <com_lib.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "pico/stdlib.h"
#include "hardware/uart.h"
#include "hardware/irq.h"

CircularBuffer buffer;
char order[5];
int orderExecuted=0;
unsigned int id=0;
unsigned int comp=0;
unsigned short arg0=0;
unsigned short arg1=0;




/*definition of uart communication parameter */

#define UART_ID uart0
#define BAUD_RATE 115200
#define DATA_BITS 8
#define STOP_BITS 1
#define PARITY    UART_PARITY_NONE
#define UART_TX_PIN 0
#define UART_RX_PIN 1


/*UART init*/

void uartInit(){ // Setup uart parameters
	uart_init(UART_ID, 2400);
    gpio_set_function(UART_TX_PIN,GPIO_FUNC_UART);
    gpio_set_function(UART_RX_PIN, GPIO_FUNC_UART);
    uart_set_baudrate(UART_ID, BAUD_RATE);
    uart_set_hw_flow(UART_ID, false, false);
    uart_set_format(UART_ID, DATA_BITS, STOP_BITS, PARITY);
    uart_set_fifo_enabled(UART_ID, false);
    }

void uartIrqSetup(){ //Setup receive irq on byts 
    int UART_IRQ = UART_ID == uart0 ? UART0_IRQ : UART1_IRQ;
    irq_set_exclusive_handler(UART_IRQ, receive);
    irq_set_enabled(UART_IRQ, true);
    uart_set_irq_enables(UART_ID, true, false);
 
	}


/* Routine used for communication wich deal with uart*/


void receive() { //receive data and write the buffer 
	volatile char ch=uart_getc(UART_ID);
	WriteBuffer(ch,&buffer);
}

void acknowledge(char order[5]){ //acknowledgement for recieved order 
	uart_putc(UART_ID,order[0]>>4);
	for(int k=0; k<4;k++){
		uart_putc(UART_ID, 0x00);
		}
	}

void finish(char order[5]){ //acknowledgement for finished order 
	uart_putc(UART_ID,0x10+(order[0]>>4));
	for(int k=0; k<4;k++){
		uart_putc(UART_ID, 0x00);
		}
	}
	
/*Buffer management routines*/

void BufferInit(CircularBuffer *Buffer){ //init buffer parameter
	Buffer->Head=0;
	Buffer->Tail=0;
	Buffer->BufferSize=25;
	Buffer->BufferFullFlag=0;
	Buffer->BufferEmptyFlag=1;
	Buffer->BufferOrderNumber=0;
	};

int WriteBuffer(char data, CircularBuffer *Buffer){ //write buffer and update related flags
	if(Buffer->BufferFullFlag){ 
		return 1;
		}
	else{
		Buffer->Buffer[Buffer->Head] = data;
		Buffer->Head = (Buffer->Head+1)%(Buffer->BufferSize);
		Buffer->BufferEmptyFlag=0;
		if(Buffer->Head == Buffer->Tail){
			Buffer->BufferFullFlag=1;
			}
		if(Buffer->Head%5==0){
			Buffer->BufferOrderNumber++;
			}
		return 0;
		}
	}


int IsBufferEmpty(CircularBuffer *Buffer){ //check if the buffer is empty and update related flags
	int temp=0;
	if(Buffer->Head == Buffer->Tail &&  Buffer->BufferFullFlag!=1){
		temp=1;
		}
	Buffer->BufferEmptyFlag=temp;
	return temp;
	}


int ReadBuffer(char *data,  CircularBuffer *Buffer){ //read the buffer and update related flags
	if(IsBufferEmpty(Buffer)){
		return 1;
		}
	else{
		*data=Buffer->Buffer[Buffer->Tail];
		Buffer->Tail= (Buffer->Tail+1)%(Buffer->BufferSize);
		Buffer->BufferFullFlag=0;
		return 0;
		}
	}

int ReadNewOrder(char order[5],CircularBuffer *Buffer ){ //extract order from the buffer 
	int result=0;
	for(int k=0; k<5;k++){
		char temp;
		result = ReadBuffer(&temp,Buffer);
		order[k]=temp;
		}
	return result;
	}

/* Parsing routines */

unsigned long extraire_bit(unsigned long data, int n) {
	unsigned long MASK = 0x1;
    unsigned long bit = (data >> n) & MASK;
	return bit;
	}

unsigned int concatene16(unsigned int octet1, unsigned int octet2){
    unsigned int result = (uint16_t)(octet1 << 8)|octet2;
    return result;
	}

unsigned int concatene32(unsigned int double_octet1, unsigned int double_octet2){
    unsigned int result = (uint32_t)(double_octet1 << 8)|double_octet2;
    return result;
	}


int getID(char octet0) {
    char MASK = 0x0f;
    return (octet0 >> 4) & MASK;
	}

unsigned int getCOMP(unsigned int octet0){
    return octet0 & 0x0f;
	}

unsigned int getARG(unsigned int double_octet){
    return double_octet >> 0;
	}

float getARG_float(unsigned short buffer_reception[])
{
    unsigned short id = getID(buffer_reception[0]);
    unsigned short comp = getCOMP(buffer_reception[0]);

    unsigned short arg0 = (((unsigned short) buffer_reception[1]) << 8) + ((unsigned short) buffer_reception[2]);
    unsigned short arg1 = (((unsigned short) buffer_reception[3]) << 8) + ((unsigned short) buffer_reception[4]);
    unsigned int tmp = (((unsigned int) arg0) << 16) + (unsigned int) arg1;
    float floatArg = *(float*) &tmp; //chelou

    return floatArg;
}








	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	


	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
