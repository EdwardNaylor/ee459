#include "LCD.h"
#include "GPS.h"
#include "ShiftOut.h"
#include "Compass.h"
#include "Timer.h"
#include "Buttons.h"
#include "Serial.h"
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <stdio.h>
#include <stdlib.h>

// unsigned char str1[] = "1234567890";
// unsigned char str2[] = ">> USC EE459L <<";
// unsigned char str3[] = ">> at328-6.c <<<";
// unsigned char str4[] = "-- April 11, 2011 --";

unsigned char buffer[20];

int main(void) {
	sci_init();						// Initialize the SCI port

	lcd_init();						// Initialize the LCD

	timer_init();				// Initialize the timer

	compass_init();			// Initialize the Compass

	//gps_init();				// Initialize the GPS

	shift_init();			// Initialize the Shift Registers

	//buttons_init();

	//enable global interrupts
	sei();


	while (1) {
		lcd_clear();
		sprintf(buffer,"%ld", millis());
		lcd_out(0, buffer);	// Print string on line 1
		short x, y, z;
		x = compass_get_x();
		y = compass_get_y();
		z = compass_get_z();

		sprintf(buffer,"x:%hd y:%hd z:%hd", x, y, z);
		lcd_out(40, buffer);

		double theta = compass_get_north();
		int position = theta / 45;

		dtostrf(theta, -10, 3, buffer);
		lcd_out(20, buffer);

		shift_out(1 << position, 1 << position);


		// while (!gps_encode(sci_in())) {
		//
		// }
		//
		// sprintf(buffer,"lat:%ld %d", _latitude, _gps_data_good);
		// lcd_out(20, buffer);
		/*char buf[100];
		unsigned char i = 0;
		bool flag = false;

		while (1)
		{
			char c = sci_in();
			if(c == '$')
			{
				flag = true;
			}
			if(flag)
			{
				buf[i] = c;
				i++;
			}
			if(i == 20)
			{
				buf[19] = '\0';
				lcd_out(0, buf);
				i = 0;
				flag = false;
			}
		}*/

		_delay_ms(50);

		//
		// shift_out(0xFF, 0xFF);
		// _delay_ms(500);
		// shift_out(0x00, 0x00);
		// _delay_ms(500);
	}

	return 0;   /* never reached */
}
