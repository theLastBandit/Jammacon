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
 

#ifndef PSX
#define PSX

// Adjust these pins to match your AVR's particular 
//  pins for dedicated SPI
#define SPI_PORT PORTB
#define SPI_DDR  DDRB
#define SPI_PIN  PINB
#define ATT		2		//SS
#define CMD		3		//MOSI
#define DATA	4		//MISO
#define CLK		5		//CLK

// Jamma Button macros
#define JAMMA_UP 		(bit_is_clear(PIND,2))
#define JAMMA_DOWN 		(bit_is_clear(PIND,3))
#define JAMMA_LEFT 		(bit_is_clear(PIND,4))
#define JAMMA_RIGHT 	(bit_is_clear(PIND,5))
#define JAMMA_B1 		(bit_is_clear(PIND,6))
#define JAMMA_B2 		(bit_is_clear(PIND,7))
#define JAMMA_B3 		(bit_is_clear(PINC,0))
#define JAMMA_B4 		(bit_is_clear(PINC,1))
#define JAMMA_B5 		(bit_is_clear(PINC,2))
#define JAMMA_B6 		(bit_is_clear(PINC,3))
#define JAMMA_B7 		(bit_is_clear(PINC,4))
#define JAMMA_B8 		(bit_is_clear(PINC,5))
#define JAMMA_START 	(bit_is_clear(PIND,1))
#define JAMMA_COIN 		(bit_is_clear(PIND,0))

#define DPAD 0
#define LSTICK 1
#define RSTICK 2

void sendPS2Data(void);
void processByte(void);
void getButtonData(void);

typedef struct controllerStatus
{
	uint8_t triangleOn : 1; 		
	uint8_t circleOn : 1;
	uint8_t squareOn : 1;
	uint8_t crossOn : 1;
	uint8_t l1On : 1;
	uint8_t l2On : 1;
	uint8_t l3On : 1;
	uint8_t r1On : 1;
	uint8_t r2On : 1;
	uint8_t r3On : 1;
	uint8_t selectOn : 1;
	uint8_t startOn : 1;
	uint8_t homeOn : 1;
	uint8_t dpadLeftOn : 1;
	uint8_t dpadUpOn : 1;
	uint8_t dpadRightOn : 1;
	uint8_t dpadDownOn : 1;
	uint8_t leftStickX : 8; 
	uint8_t leftStickY : 8;
	uint8_t rightStickX : 8;
	uint8_t rightStickY : 8;
} controllerStatus;

	
#endif
