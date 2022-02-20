/*
* Jammacon Playstation Adapter
* Copyright (c) 2017 Shane Kiely - shane.kiely@gmail.com
*
* This program is free software: you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation, either version 3 of the License, or
* (at your option) any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
 
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include "controller.h"
#include <stdbool.h>


controllerStatus data;
uint8_t deviceMode;
bool config;

void readJamma(void)
{
	static uint8_t directional = DPAD;
	
	data.crossOn = 0;
	data.circleOn = 0;
	data.squareOn = 0;
	data.triangleOn = 0;
	data.l1On = 0;
	data.l2On = 0;
	data.l3On = 0;
	data.r1On = 0;
	data.r2On = 0;
	data.r3On = 0;
	data.startOn = 0;
	data.selectOn = 0;
	data.leftStickX = 0x7F;
	data.leftStickY = 0x7F;
	data.rightStickX = 0x7F;
	data.rightStickY = 0x7F;

	if (JAMMA_B1 && JAMMA_START) 
	{
		_delay_ms(1);
		if(JAMMA_B1 && JAMMA_START)
		{
			data.selectOn = 1;
		}
	}
	else 
	{
		data.crossOn = JAMMA_B1;
		data.startOn = JAMMA_START;
	}
	
	if(JAMMA_UP && JAMMA_START)
	{
		_delay_ms(1);
		if(JAMMA_UP && JAMMA_START)
		{
			directional = DPAD;
		}
	}
	if(JAMMA_LEFT && JAMMA_START)
	{
		_delay_ms(1);
		if(JAMMA_LEFT && JAMMA_START)
		{
			directional = LSTICK;
		}
	}
	if(JAMMA_RIGHT && JAMMA_START)
	{
		_delay_ms(1);
		if(JAMMA_RIGHT && JAMMA_START)
		{
			directional = RSTICK;
		}

	}
	
	switch(directional)
	{
		case DPAD:
			data.dpadUpOn = (JAMMA_UP);
			data.dpadDownOn = (JAMMA_DOWN);
			data.dpadLeftOn = (JAMMA_LEFT);
			data.dpadRightOn = (JAMMA_RIGHT);
		break;
		case LSTICK:
			if(JAMMA_UP){
				data.leftStickY = 0x00;
			}
			if(JAMMA_DOWN){
				data.leftStickY = 0xff;	
			}
			if(JAMMA_LEFT){
				data.leftStickX = 0x00;
			}
			if(JAMMA_RIGHT){
				data.leftStickX = 0xff;
			}
		break;
		case RSTICK:
			if(JAMMA_UP){
				data.rightStickY = 0x00;
			}
			if(JAMMA_DOWN){
				data.rightStickY = 0xff;	
			}
			if(JAMMA_LEFT){
				data.rightStickX = 0x00;
			}
			if(JAMMA_RIGHT){
				data.rightStickX = 0xff;
			}
		break;
	}

	data.circleOn = JAMMA_B2;
	data.squareOn = JAMMA_B3;
	data.triangleOn = JAMMA_B4;
	data.l1On = JAMMA_B5;
	data.r1On = JAMMA_B6;
	data.l2On = JAMMA_B7;
	data.r2On = JAMMA_B8;
	data.selectOn = JAMMA_COIN;	
}


int main(void)
{	
	// Initialise the Hardware
	DDRB = 0b00010010;
	PORTB = 0xFF;
	DDRC = 0b00000000;
	PORTC = 0xFF;
	DDRD = 0b00000000;	
	PORTD = 0xFF;


	//PORTB setup
	SPI_DDR &= ~(1<<ATT);		//input
	SPI_PORT |= (1<<ATT);		//pull-up enabled	
	SPI_DDR &= ~(1<<CMD);		//input
	SPI_PORT |= (1<<CMD);		//pull-up enabled
	SPI_DDR |= (1<<DATA);		//output
	SPI_PORT |= (1<<DATA);		//set HIGH
	SPI_DDR &= ~(1<<CLK);		//input
	SPI_PORT |= (1<<CLK);		//pull-up enabled
		
	//SPI initialization
	SPCR = 0x00;
	SPCR |= (1<<CPOL);			// SCK HIGH when idle
	SPCR |= (1<<CPHA);			// setup data on falling edge of CLK, read on rising edge
	SPCR &= ~(1<<MSTR);			// select SLAVE mode
	SPCR |= (1<<DORD);			// data order to LSB first
	SPCR |= (1<<SPE);			// SPI enable

	SPDR = 0xFF;				

	deviceMode = 0x41;
	config = false;

	_delay_ms(1000);

	PCMSK0 = 0b00000100; 
	PCICR |= 1 << PCIE0;
	sei();	
			
	
	while(1)				
	{
		readJamma();
		_delay_ms(2);
	}	
	return 0;
}

