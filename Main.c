#include "LCD.h"
#include "GPS.h"
#include "Timer.h"
#include "Serial.h"
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <stdio.h>

// unsigned char str1[] = "1234567890";
// unsigned char str2[] = ">> USC EE459L <<";
// unsigned char str3[] = ">> at328-6.c <<<";
// unsigned char str4[] = "-- April 11, 2011 --";

unsigned char str[20];

int main(void) {
	sci_init();						// Initialize the SCI port

	lcd_init();						// Initialize the LCD

	timer_init();

	//enable global interrupts
	sei();

	while (1) {
		_delay_ms(10);
    sci_out(0xfe);				// Clear the screen
    sci_out(0x51);
		sprintf(str,"%ld", millis());
    lcd_out(0, str);				// Print string on line 1
		// lcd_out(0, str1);
	}

	return 0;   /* never reached */
}
