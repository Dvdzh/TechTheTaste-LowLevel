#include "pico/time.h"
#include <stdio.h>
#include <stdint.h>
#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <function.h>
#include <com_lib.h>

static char ordermotors[5]={0x70,0x00,0x00,0x00,0x00}; 
static uint nbmotors=0;
static short positionmotors[16]={0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
static uint nbordermotors=0;

void lidarStop( unsigned int comp, unsigned short arg0, unsigned short arg1){
	
	
	}



void move( unsigned int comp, unsigned short arg0, unsigned short arg1){
			
	
	}


void rotate( unsigned int comp, unsigned short arg0, unsigned short arg1){

	
	
	}


void cancelMove( unsigned int comp, unsigned short arg0, unsigned short arg1){
	
	
	}


void motorValue( unsigned int comp, unsigned short arg0, unsigned short arg1){
	acknowledge(order);
	sleep_ms(2000);
	finish(order);
	}
	


void motorTime( unsigned int comp, unsigned short arg0, unsigned short arg1){

}


void  pumps( unsigned int comp, unsigned short arg0, unsigned short arg1){
	
	
	}


void  motors( unsigned int comp, unsigned short arg0, unsigned short arg1){
	acknowledge(order);
	nbmotors=comp;
	
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
	sleep_ms(2000);
	finish(order);
	
	}
void getVar( unsigned int comp, unsigned short arg0, unsigned short arg1){
	acknowledge(order);
	sleep_ms(2000);
	finish(order);
	
	}
void track( unsigned int comp, unsigned short arg0, unsigned short arg1){
	
	
	}







