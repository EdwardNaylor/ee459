#include "LCD.h"
#include "GPS.h"
#include "Timer.h"
#include <avr/io.h>
#include <util/delay.h>

// unsigned char str1[] = "1234567890";
// unsigned char str2[] = ">> USC EE459L <<";
// unsigned char str3[] = ">> at328-6.c <<<";
// unsigned char str4[] = "-- April 11, 2011 --";

int main(void) {
	sci_init();						// Initialize the SCI port

	lcd_init();						// Initialize the LCD

	//lcd_out(55, (unsigned char *) str1);				// Print string on line 1

  // enable timer 0 overflow interrupt
  sbi(TIMSK0, TOIE0);

	while (1) {
    sci_out(0xfe);				// Clear the screen
    sci_out(0x51);
    lcd_out(0, (unsigned char *) millis());				// Print string on line 1
	}

	return 0;   /* never reached */
}
