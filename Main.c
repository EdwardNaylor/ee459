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

	gps_init();				// Initialize the GPS

	shift_init();			// Initialize the Shift Registers

	buttons_init();

	//enable global interrupts
	sei();

	shift_out(0xFF, 0xFF);

	while (1) {
		_delay_ms(10);
		lcd_clear();
		sprintf(buffer,"%ld", millis());
    lcd_out(0, buffer);	// Print string on line 1
		// sprintf(buffer,"%ld", compass_get_x());
		// lcd_out(40, compass_get_x());
		// sprintf(buffer,"%ld",  compass_get_y());
		// lcd_out(20, buffer);
		//
		// shift_out(0xFF, 0xFF);
		// _delay_ms(500);
		// shift_out(0x00, 0x00);
		// _delay_ms(500);
	}

	return 0;   /* never reached */
}
