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
	buttons_init();

	sci_init();						// Initialize the SCI port

	radio_init();						// Initialize the radio

	lcd_init();						// Initialize the LCD

	timer_init();				// Initialize the timer

	compass_init();			// Initialize the Compass

	shift_init();			// Initialize the Shift Registers

	gps_init();			// Initialize the GPS


	// UCSR0B = (1 << RXCIE0) | (1 << RXEN0);

	//enable global interrupts
	sei();

	float my_lat;
	float my_lon;
	float their_lat;
	float their_lon;

	float distance;
	float bearing;

	while (1) {
		short x, y, z;
		x = compass_get_x();
		y = compass_get_y();
		z = compass_get_z();

		if (check_radio()) {
			radio_read(); //blocking
		}

		double theta = compass_get_north();
		int position =  (int) round(theta / 45);
		if (position == 8) {
			position = 0;
		}

		uint8_t theta_buffer[10];
		dtostrf(theta, -3, 2, theta_buffer);

		uint8_t red = 0x00;
		if (user_index != 0) {
			if (_gps_data_ever_good) {
				get_position(&my_lat, &my_lon);
				their_lat = radio_lat[user_index] / 100000.0;
				their_lon = radio_lon[user_index] / 100000.0;

				distance = distance_between(my_lat, my_lon, their_lat, their_lon);
				bearing = bearing_between(my_lat, my_lon, their_lat, their_lon);

				double phi = theta - bearing;
				if (phi > 360.0) {
					phi -= 360.0;
				}
				if (phi < 0) {
					phi += 360.0;
				}

				int phi_int = (int) round(phi / 45);
				if (phi_int == 8) {
					phi_int = 0;
				}

				red = (1 << phi_int);
			}
		}

		shift_out(1 << position, red);

		if (printLCD == 0 || printOverride == 1) {
			radio_send_message();

			lcd_clear();

			char status_string[10];
			if (radio_status[user_index] == 0) {
				strcpy(status_string, "ok");
			} else if (radio_status[user_index] == 1) {
				strcpy(status_string, "warning");
			}	else if (radio_status[user_index] == 2) {
			 strcpy(status_string, "danger");
		  }

			sprintf(buffer,"%s: %s", radio_name[user_index], status_string);
			lcd_out(LCD_LINE_ONE, buffer);

			if (user_index == 0) {
				if (_gps_data_ever_good) {
					float lat;
					float lon;
					get_position(&lat, &lon);
					uint8_t lat_buffer[10];
					uint8_t lon_buffer[10];
					dtostrf(lat, -3, 2, lat_buffer);
					dtostrf(lon, -3, 2, lon_buffer);

					sprintf(buffer,"%s%c, %s%c %s", lat_buffer, 0xDF, lon_buffer, 0xDF, compass_get_dir(theta));
					lcd_out(LCD_LINE_TWO, buffer);
				} else {
					sprintf(buffer, "%s", theta_buffer);
					lcd_out(LCD_LINE_TWO, buffer);
				}
			} else {
				if (_gps_data_ever_good) {
					uint8_t distance_buffer[10];
					dtostrf(distance, -3, 2, distance_buffer);

					sprintf(buffer,"%sm %s", distance_buffer, compass_get_dir(bearing));
					lcd_out(LCD_LINE_TWO, buffer);
				} else {
					sprintf(buffer, "Need GPS fix");
					lcd_out(LCD_LINE_TWO, buffer);
				}
			}

			if (user_index == 0) {
				if (_gps_data_ever_good) {
					sprintf(buffer, "%ld %ld", _time, _date);
					lcd_out(LCD_LINE_THREE, buffer);
				} else {
					// sprintf(buffer,"x:%hd y:%hd z:%hd", x, y, z);
					// lcd_out(LCD_LINE_THREE, buffer);
					sprintf(buffer, "Need GPS fix");
					lcd_out(LCD_LINE_THREE, buffer);
				}
			} else {
				int seconds = (millis() - radio_fix[user_index]) / 1000;
				sprintf(buffer, "Heard %d s ago", seconds);
				lcd_out(LCD_LINE_THREE, buffer);
			}


			if (!_gps_data_ever_good) {
				sprintf(buffer,"GPS fixing...");
				lcd_out(LCD_LINE_FOUR, buffer);
			}

			// while (!gps_encode(gps_in())) {};

			printOverride = 0;
		}

		printLCD++;
		if (printLCD == 100) {
			printLCD = 0;
		}
	}
}
