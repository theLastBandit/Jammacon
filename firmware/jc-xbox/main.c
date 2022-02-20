/*
     .k8GOGGNqkSFS5XkqXPSkSkkqXXFS5kSkSS15U22F2515U2uujuu1U1u2U1U2uUuFS.
   :0qE     JS5uuJuuFFX51jU2SSk12jU2SSXF5uuu15SFS5k12ujj21S5kFS5S12jJYu11
  5XS:        1UYYLu.   vUUX    U22r     SUF         SUF           ;YYLuU5
 1F5i  NNSkS7  2uLJui   51u     S5.      .PX         .XX           LJvLLu1.
 kUk  0iLk5FFu vuYY2:   5F    Xkk7        78    E0    i0    GEXPXk2uLLvLLuk
X25, 8O   2kX0  5YJUi   M    555    PkXk   i    q1FU   7    ONNkP12YLvLvLYS
S25  8888  888  5uY5         FuS    PS50   .    FuUU   7          uJvLvLLJ2i
kUF             SJjU.      P02UF    P25k   .    Su2Y   v          2LLvLvLL17
S21  XJj88  0u  1uY2.        X2k           .    k11E   v    7;ii:JuJvLvLvJ2:
2257 jqv   Pqq  1LJur         PP.          7    EX:    q    OqqXP51JYvLvYYS.
 X2F  kXkXSXk  kJYLU:   O     ,Z    0PXZ   i    ii    q0    i:::,,.jLLvLLuF'
 ik1k  ;qkPj  .uJvYu:   UN      :   XU2F   :         S5S           iJLLvjUF8
  :PSq       72uLLLui   uSi    .;   2uY1   r.       72j1           LYYLYJSU88
    XqE2   rP12juJuu1FX55U5FqXXSXkXF1juUkkPSXSPXPXPF1Jju5FkFSFXFSF5uujUu5j28V
      .uGOZESS5S5SFkkPkPkXkPXqXPXqXXFkSkkPXPXPkqSkSS1521252121U2u2u12Suv7


* Arduino Micro (Leonardo) Classic XBOX Pad emulator firmware
*
* Copyright (c) 2016
* Bruno Freitas - bruno@brunofreitas.com
* Jon Wilson    - degenatrons@gmail.com
* Kevin Mackett - kevin@sharpfork.com
*
* This program is free software: you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation, either version 3 of the License, or
* (at your option) any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program. If not, see <http://www.gnu.org/licenses/>.
*/


/* Jamma pin mappings 

Modified for use with JammaCon 

Up				PD7
Down			PB4
Left			PB5
Right			PB6
Start			PE6
Coin			PF0
Button1			PC6
Button2			PC7
Button3			PE2
Button4			PF7
Button5			PF6
Button6			PF5
Button7			PF4
Button8			PF1	

*/


#include "XBOXPad.h"
#include "util.h"

void setup_pins(void);

int main(void) {
	uint8_t pad_up, pad_down, pad_left, pad_right, pad_y, pad_b, pad_x, pad_a, pad_black,
	pad_white, pad_start, pad_select, pad_l3, pad_r3, pad_l, pad_r, pad_left_analog_x,
	pad_left_analog_y, pad_right_analog_x, pad_right_analog_y, joystick_mode, start_counter;

	// Set clock @ 16Mhz
	CPU_PRESCALE(0);

	// Disable JTAG
	bit_set(MCUCR, 1 << JTD);
	bit_set(MCUCR, 1 << JTD);

	// Setup pins
	setup_pins();

	// Init XBOX pad emulation
	xbox_init(true);

	// Set Joystick mapping to D-Pad by default
	joystick_mode = 1;
	
	start_counter = 0;
	
	// Pins polling and gamepad status updates
	for (;;) {
		xbox_reset_watchdog();
	
		// Read all button states
		pad_up = !bit_check(PIND, 7);
		pad_down = !bit_check(PINB, 4);
		pad_left = !bit_check(PINB, 5);
		pad_right = !bit_check(PINB, 6);
		pad_y = !bit_check(PINF, 7);
		pad_b = !bit_check(PINC, 7);
		pad_x = !bit_check(PINE, 2);
		pad_a = !bit_check(PINC, 6);
		pad_black =  !bit_check(PINF, 1);
		pad_white =  !bit_check(PINF, 4);
		pad_start =  !bit_check(PINE, 6);
		pad_select =  !bit_check(PINF, 0);
//		pad_l3 =  !bit_check(PINB, 5);
//		pad_r3 =  !bit_check(PINB, 6);
		pad_l = !bit_check(PINF, 6);
		pad_r = !bit_check(PINF, 5);

		pad_left_analog_x = pad_left_analog_y = pad_right_analog_x = pad_right_analog_y = 0x7F;
		pad_l3 = 0;
		pad_r3 = 0;

		//	Set Joystick mapping mode
		// Mode 0: Up + Start = D-pad
		// Mode 1: Left + Start = Left stick
		// Mode 2: Right + Start = Right stick

		if (pad_start & pad_up) {
			joystick_mode = 0;
			pad_start = 0;
			start_counter = 0;
		}
		if (pad_start & pad_left) {
			joystick_mode = 1;
			pad_start = 0;
			start_counter = 0;
			pad_left = pad_right = pad_up = pad_down = 0;
		}
		if (pad_start & pad_right) {
			joystick_mode = 2;
			pad_start = 0;
			start_counter = 0;
			pad_left = pad_right = pad_up = pad_down = 0;
		}	

/*
		if(!bit_check(PINB, 1)) {
			pad_left_analog_x = 0x00;
		} else if(!bit_check(PINB, 3)) {
			pad_left_analog_x = 0xFF;
		}

		if(!bit_check(PINF, 0)) {
			pad_left_analog_y = 0x00;
		} else if(!bit_check(PINF, 1)) {
			pad_left_analog_y = 0xFF;
		}

		if(!bit_check(PINF, 4)) {
			pad_right_analog_x = 0x00;
		} else if(!bit_check(PINF, 5)) {
			pad_right_analog_x = 0xFF;
		}

		if(!bit_check(PINF, 6)) {
			pad_right_analog_y = 0x00;
		} else if(!bit_check(PINF, 7)) {
			pad_right_analog_y = 0xFF;
		}
*/
		// Joystick mapped to D-Pad
		if ( joystick_mode == 0 ){		
			pad_up    ? bit_set(gamepad_state.digital_buttons, XBOX_DPAD_UP)    : bit_clear(gamepad_state.digital_buttons, XBOX_DPAD_UP);
			pad_down  ? bit_set(gamepad_state.digital_buttons, XBOX_DPAD_DOWN)  : bit_clear(gamepad_state.digital_buttons, XBOX_DPAD_DOWN);
			pad_left  ? bit_set(gamepad_state.digital_buttons, XBOX_DPAD_LEFT)  : bit_clear(gamepad_state.digital_buttons, XBOX_DPAD_LEFT);
			pad_right ? bit_set(gamepad_state.digital_buttons, XBOX_DPAD_RIGHT) : bit_clear(gamepad_state.digital_buttons, XBOX_DPAD_RIGHT);
		}


		// Joystick mapped to Left stick override the analog stick values
		if (joystick_mode == 1){
			if (pad_left) {
				pad_left_analog_x = 0x00;
			} else if (pad_right) {
				pad_left_analog_x = 0xff;
			} 
				
			if (pad_up) {
				pad_left_analog_y = 0x00;
			} else if (pad_down) {
				pad_left_analog_y = 0xff;
			}
		}
		
		// Joystick mapped to Right stick override the analog stick values
		if (joystick_mode == 2){
			if (pad_left) {
				pad_right_analog_x = 0x00;
			} else if (pad_right) {
				pad_right_analog_x = 0xff;
			} 
				
			if (pad_up) {
				pad_right_analog_y = 0x00;
			} else if (pad_down) {
				pad_right_analog_y = 0xff;
			}			
		}
		
		
		// Add button mappings using Start as the modifer
		
		// Start + A = White
		if (pad_start & pad_a) {
			pad_white = 1;
			pad_start = 0;
			pad_a = 0;
			start_counter = 0;	
		}
		// Start + B = Black
		if (pad_start & pad_b)	{
			pad_black = 1;
			pad_start = 0;
			pad_b = 0;
			start_counter = 0;
		}
		// Start + X = L3
		if (pad_start & pad_x) {
			pad_l3 = 1;
			pad_start = 0;
			pad_x = 0;
			start_counter = 0;
		}
		// Start + Y = R3
		if (pad_start & pad_y)	{
			pad_r3 = 1;
			pad_start = 0;
			pad_y = 0;
			start_counter = 0;
		}		
			
		
		gamepad_state.a = pad_a * 0xFF;
		gamepad_state.b = pad_b * 0xFF;
		gamepad_state.x = pad_x * 0xFF;
		gamepad_state.y = pad_y * 0xFF;
		gamepad_state.black = pad_black * 0xFF;
		gamepad_state.white = pad_white * 0xFF;
		
/*		
		We don't send the pad values unless Joystick mode 0
		
		pad_up    ? bit_set(gamepad_state.digital_buttons, XBOX_DPAD_UP)    : bit_clear(gamepad_state.digital_buttons, XBOX_DPAD_UP);
		pad_down  ? bit_set(gamepad_state.digital_buttons, XBOX_DPAD_DOWN)  : bit_clear(gamepad_state.digital_buttons, XBOX_DPAD_DOWN);
		pad_left  ? bit_set(gamepad_state.digital_buttons, XBOX_DPAD_LEFT)  : bit_clear(gamepad_state.digital_buttons, XBOX_DPAD_LEFT);
		pad_right ? bit_set(gamepad_state.digital_buttons, XBOX_DPAD_RIGHT) : bit_clear(gamepad_state.digital_buttons, XBOX_DPAD_RIGHT);
*/
		// Get a count of start presses, if its high enough the assume Start pressed on its own
		if (pad_start) {
			start_counter++;
			if (start_counter > 100 ) {
				bit_set(gamepad_state.digital_buttons, XBOX_START);
				start_counter = 0;
			}
		} else {
			bit_clear(gamepad_state.digital_buttons, XBOX_START);
		}
		
		
		pad_select ? bit_set(gamepad_state.digital_buttons, XBOX_BACK)        : bit_clear(gamepad_state.digital_buttons, XBOX_BACK);
		pad_l3     ? bit_set(gamepad_state.digital_buttons, XBOX_LEFT_STICK)  : bit_clear(gamepad_state.digital_buttons, XBOX_LEFT_STICK);
		pad_r3     ? bit_set(gamepad_state.digital_buttons, XBOX_RIGHT_STICK) : bit_clear(gamepad_state.digital_buttons, XBOX_RIGHT_STICK);


		gamepad_state.l_x = pad_left_analog_x * 257 + -32768;
		gamepad_state.l_y = pad_left_analog_y * -257 + 32767;
		gamepad_state.r_x = pad_right_analog_x * 257 + -32768;
		gamepad_state.r_y = pad_right_analog_y * -257 + 32767;

		gamepad_state.l = pad_l * 0xFF;
		gamepad_state.r = pad_r * 0xFF;

		xbox_send_pad_state();
	}
}

void setup_pins(void) {
/*
	// Setup pins
	bit_clear(DDRF, 1 << 7);
	bit_set(PORTF, 1 << 7);

	bit_clear(DDRF, 1 << 6);
	bit_set(PORTF, 1 << 6);

	bit_clear(DDRF, 1 << 5);
	bit_set(PORTF, 1 << 5);

	bit_clear(DDRF, 1 << 4);
	bit_set(PORTF, 1 << 4);

	bit_clear(DDRF, 1 << 1);
	bit_set(PORTF, 1 << 1);

	bit_clear(DDRF, 1 << 0);
	bit_set(PORTF, 1 << 0);

	bit_clear(DDRB, 1 << 3);
	bit_set(PORTB, 1 << 3);

	bit_clear(DDRB, 1 << 1);
	bit_set(PORTB, 1 << 1);

	bit_clear(DDRD, 1 << 6);
	bit_set(PORTD, 1 << 6);

	bit_clear(DDRB, 1 << 7);
	bit_set(PORTB, 1 << 7);

	bit_clear(DDRB, 1 << 6);
	bit_set(PORTB, 1 << 6);

	bit_clear(DDRB, 1 << 5);
	bit_set(PORTB, 1 << 5);

	bit_clear(DDRB, 1 << 4);
	bit_set(PORTB, 1 << 4);

	bit_clear(DDRE, 1 << 6);
	bit_set(PORTE, 1 << 6);

	bit_clear(DDRD, 1 << 7);
	bit_set(PORTD, 1 << 7);

	bit_clear(DDRC, 1 << 6);
	bit_set(PORTC, 1 << 6);

	bit_clear(DDRD, 1 << 4);
	bit_set(PORTD, 1 << 4);

	bit_clear(DDRD, 1 << 0);
	bit_set(PORTD, 1 << 0);

	bit_clear(DDRD, 1 << 1);
	bit_set(PORTD, 1 << 1);

	bit_clear(DDRD, 1 << 2);
	bit_set(PORTD, 1 << 2);

	bit_clear(DDRD, 1 << 3);
	bit_set(PORTD, 1 << 3);

	bit_clear(DDRB, 1 << 0);
	bit_set(PORTB, 1 << 0);

	bit_clear(DDRB, 1 << 2);
	bit_set(PORTB, 1 << 2);

	bit_clear(DDRC, 1 << 7);
	bit_set(PORTC, 1 << 7);
*/	
	DDRB = 0b10001111;
	DDRC = 0b00111111;
	DDRD = 0b01111111;
	DDRE = 0b10111011;
	DDRF = 0b00001100;
	
	PORTB = 0xff;
	PORTC = 0xff;
	PORTD = 0xff;
	PORTE = 0xff;
	PORTF = 0xFF;
}
