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

			
volatile uint8_t byteCount;		

uint8_t replyMsgLen; 
extern uint8_t deviceMode;				

extern bool config;		
uint8_t replyData[21];	
extern controllerStatus data;

uint8_t dev79[21] = {0xFF, 0x79, 0x5A, 0xFF, 0xFF, 0x7F, 0x7F, 0x7F, 0x7F,0,0,0,0,0,0,0,0,0,0,0,0};
uint8_t dev73[9] = {0xFF, 0x73, 0x5A, 0xFF, 0xFF, 0x7F, 0x7F, 0x7F, 0x7F};
uint8_t dev41[5] = {0xFF, 0x41, 0x5A, 0xFF, 0xFF};
uint8_t devCfg[5] = {0xFF, 0xF3, 0x5A, 0xFF, 0xFF};

uint8_t msg41[9] = {0xFF,0x41,0x5A,0x00,0x00,0x00,0x00,0x00,0x00};
uint8_t msg43[9] = {0xFF,0xF3,0x5A,0xFF,0xFF,0x7F,0x7F,0x7F,0x7F};
uint8_t msg44[9] = {0xFF,0xF3,0x5A,0x00,0x00,0x00,0x00,0x00,0x00};
uint8_t msg45[9] = {0xFF,0xF3,0x5A,0x01,0x02,0x01,0x02,0x01,0x00};
uint8_t msg46_1[9] = {0xFF,0xF3,0x5A,0x00,0x00,0x01,0x02,0x00,0x0A};
uint8_t msg46_2[9] = {0xFF,0xF3,0x5A,0x00,0x00,0x01,0x01,0x01,0x14};
uint8_t msg47[9] = {0xFF,0xF3,0x5A,0x00,0x00,0x02,0x00,0x01,0x00};
uint8_t msg4C_1[9] = {0xFF,0xF3,0x5A,0x00,0x00,0x00,0x04,0x00,0x00};
uint8_t msg4C_2[9] = {0xFF,0xF3,0x5A,0x00,0x00,0x00,0x07,0x00,0x00};
uint8_t msg4D[9] = {0xFF,0xF3,0x5A,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF};
uint8_t msg80[9] = {0xFF,0xF3,0x5A,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF};


ISR(PCINT0_vect)
{ 
	byteCount = 0;
	SPDR = 0xFF;
	// While ATT is held low, we complete a full packet exhange.
	while (!(SPI_PIN & (1 << ATT))){
		processByte();
	}
}


void ack(void)
{
		DDRB &= ~(1<<1); // make input
		PORTB |= (1<<1); // tri state
		DDRB |= (1<<1); // make output
		PORTB &= ~(1<<1); // drive low
		_delay_us(3);
		DDRB &= ~(1<<1);
		PORTB |= (1<<1);
}
	
void setReply(uint8_t src[], uint8_t msgSize)
{
	uint8_t i;
	for (i = 0; i < msgSize; i++)
	{
		replyData[i] = src[i];
	}
}

void processByte(void)
{
	static uint8_t psxCmd;       		
	static uint8_t lastCmd;
	static uint8_t lastDeviceMode;
	uint8_t psxDataIn;
	
	// Read data byte
	while(!(SPSR & (1<<SPIF)))
	{
		// Check Attention, if High then reset
		if (bit_is_set(PINB, 2))
		{
			SPDR = 0xFF;
			byteCount = 0;
			return;
		}
	}	
	
	psxDataIn = SPDR;
	
	if (byteCount == 0)
	{
		// We should receive a 0x01 as the first byte or something wrong
		if (psxDataIn == 0x01) 
		{
			if(config)
			{
				SPDR = 0xF3;
				deviceMode = 0xF3;
				ack();
				byteCount++;
			}
			else
			{ 
				SPDR = deviceMode;			
				ack();
				byteCount++;
			}
		}
		else
		{
			byteCount = 0;
			SPDR = 0xFF;
			return;
		}
		return;
	}
	else if (byteCount == 1)
	{
		lastCmd = psxCmd;
		psxCmd = psxDataIn;
		SPDR = 0x5A;
		ack();
		byteCount++;
		return;
	}
	
	switch (psxCmd)
	{
		case 0x41:
			replyMsgLen = 9;
			setReply(msg41,9);
			byteCount++;
			SPDR = replyData[byteCount];
			if (byteCount < (replyMsgLen))
			{
				ack();
			}
		break;
		
		case 0x42:
			if (deviceMode == 0x41)
			{
				setReply(dev41,5);
				replyMsgLen = 5;
			}
			else if (deviceMode == 0x73)
			{
				setReply(dev73,9);
				replyMsgLen = 9;
			}
			else if (deviceMode == 0x79)
			{
				setReply(dev79,21);
				replyMsgLen = 21;
			}

			// update reply bytes with buttons
			getButtonData();
			byteCount++;
			SPDR = replyData[byteCount];
			if (byteCount < (replyMsgLen))
			{
				ack();
			}
		break;
		
		case 0x43:
			replyMsgLen = 9;
			setReply(msg43,9);
			if ((byteCount == 3 ) && (psxDataIn == 0x01))
			{
				config = true;
				lastDeviceMode = deviceMode;
				deviceMode = 0xF3;
			}
			else if ((byteCount == 3 ) && (psxDataIn == 0x00))
			{
				config = false;
				deviceMode = lastDeviceMode;
			}
			getButtonData();
			byteCount++;
			SPDR = replyData[byteCount];
			if (byteCount < (replyMsgLen))
			{
				ack();
			}
		break;
		
		case 0x44:
			replyMsgLen = 9;
			setReply(msg44,9);
		//	if ((byteCount == 3 ) && (psxDataIn == 0x01))
		//	{
				// Test Analog-Digital Switch 
				if (bit_is_clear(PINB,0))
				{
					deviceMode = 0x73;
					lastDeviceMode = 0x73;
				}
				else 
				{
					deviceMode = 0x41;
					lastDeviceMode = 0x41;
				}
		//	} 
		//	else 
		//	{
		//		deviceMode = 0x41;
		//	}
			byteCount++;
			SPDR = replyData[byteCount];
			if (byteCount < (replyMsgLen))
			{
				ack();
			}
		break;
		
		case 0x45:
			replyMsgLen = 9;
			setReply(msg45,9);
			byteCount++;
			SPDR = replyData[byteCount];
			if (byteCount < (replyMsgLen))
			{
				ack();
			}

		break;
		
		case 0x46:
			replyMsgLen = 9;
			if (lastCmd != 0x46)
			{
				setReply(msg46_1,9);
			}
			else
			{
				setReply(msg46_2,9);
			}
			byteCount++;
			SPDR = replyData[byteCount];
			if (byteCount < (replyMsgLen))
			{
				ack();
			}
		break;
		
		case 0x47:
			replyMsgLen = 9;
			setReply(msg47,9);
			byteCount++;
			SPDR = replyData[byteCount];
			if (byteCount < (replyMsgLen))
			{
				ack();
			}
		break;
		
		case 0x4C:
			replyMsgLen = 9;
			if (lastCmd != 0x4C)
			{
				setReply(msg4C_1,9);
			}
			else
			{
				setReply(msg4C_2,9);
			}
			byteCount++;
			SPDR = replyData[byteCount];
			if (byteCount < (replyMsgLen))
			{
				ack();
			}
		break;
		
		case 0x4D:
			replyMsgLen = 9;
			setReply(msg4D,9);
			byteCount++;
			SPDR = replyData[byteCount];
			if (byteCount < (replyMsgLen))
			{
				ack();
			}
		break;
		case 0x80:
			replyMsgLen = 9;
			setReply(msg80,9);
			byteCount++;
			SPDR = replyData[byteCount];
			if (byteCount < (replyMsgLen))
			{
				ack();
			}
		break;
	}
}

void getButtonData()
{
// set the buttons - for the packet, 0 means pressed, 1 means off
	replyData[3] = ( (!data.selectOn) << 0 |
		 (!data.l3On) << 1 |
		 (!data.r3On) << 2 |
		 (!data.startOn) << 3 |
		 (!data.dpadUpOn) << 4 |
		 (!data.dpadRightOn) << 5 |
		 (!data.dpadDownOn) << 6 |
		 (!data.dpadLeftOn) << 7    );

	replyData[4] = ( (!data.l2On) << 0 |
		 (!data.r2On) << 1 |
		 (!data.l1On) << 2 |
		 (!data.r1On) << 3 |
		 (!data.triangleOn) << 4 |
		 (!data.circleOn) << 5 |
		 (!data.crossOn) << 6 |
		 (!data.squareOn) << 7    );

	replyData[5] = data.rightStickX;
	replyData[6] = data.rightStickY;
	replyData[7] = data.leftStickX;
	replyData[8] = data.leftStickY;
}
