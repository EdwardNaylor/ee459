#include "LCD.h"
#include "GPS.h"
#include "Radio.h"
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
#include <string.h>

unsigned char buffer[20];

int main(void) {
	sci_init();						// Initialize the SCI port

	lcd_init();						// Initialize the LCD

	timer_init();				// Initialize the timer

	compass_init();			// Initialize the Compass

	shift_init();			// Initialize the Shift Registers

	gps_init();			// Initialize the GPS

	buttons_init();

	// UCSR0B = (1 << RXCIE0) | (1 << RXEN0);

	//enable global interrupts
	sei();

	while (1) {
		radio_send_message();

		lcd_clear();
		sprintf(buffer,"%s", send_str);
		lcd_out(LCD_LINE_ONE, buffer);	// Print string on line 1
		short x, y, z;
		x = compass_get_x();
		y = compass_get_y();
		z = compass_get_z();

		if (check_radio()) {
			radio_read(); //blocking
		}
		sprintf(buffer,"%s %d %ld %ld", radio_name, radio_status, radio_lat, radio_lon);
		lcd_out(LCD_LINE_TWO, buffer);

		double theta = compass_get_north();
		int position =  (int) round(theta / 45);
		if (position == 8) {
			position = 0;
		}

		uint8_t theta_buffer[10];
		dtostrf(theta, -10, 0, theta_buffer);
		sprintf(buffer, "%s %s", compass_get_dir(), theta_buffer);
		lcd_out(LCD_LINE_THREE, buffer);

		shift_out(~(1 << position), 1 << position);

		if (_gps_data_ever_good) {
			float lat;
			float lon;
			unsigned long age;
			get_position(&lat, &lon, &age);
			uint8_t lat_buffer[10];
			uint8_t lon_buffer[10];
			dtostrf(lat, -3, 2, lat_buffer);
			dtostrf(lon, -3, 2, lon_buffer);
			sprintf(buffer,"%s %s", lat_buffer, lon_buffer);
			lcd_out(LCD_LINE_FOUR, buffer);
		} else {
			sprintf(buffer,"GPS fixing...");
			lcd_out(LCD_LINE_FOUR, buffer);
		}

		int timeout = 0;
		while (!gps_encode(gps_in())) {
			if (timeout > 1000) {
				break;
			}
			timeout++;
		}

		//_delay_ms(250);
	}
		//
		// sprintf(buffer,"lat:%ld %d", _latitude, _gps_data_good);
		// lcd_out(20, buffer);
		/*char buf[100];
		unsigned char i = 0;
		bool flag = false;

		while (1)
		{
			char c = gps_in();
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
		}

		_delay_ms(50);

		//
		// shift_out(0xFF, 0xFF);
		// _delay_ms(500);
		// shift_out(0x00, 0x00);
		// _delay_ms(500);
	}

	return 0;	 /* never reached */
}
