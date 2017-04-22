#include <avr/io.h>
#include <util/delay.h>
#include "LCD.h"
#include "Serial.h"

/*
  lcd_init - Initialize the LCD
*/
void lcd_init()
{
	_delay_ms(250);			// Wait 500msec for the LCD to start up
	_delay_ms(250);
	sci_out(0xfe);				// Clear the screen
	sci_out(0x51);
}

/*
  lcd_out - Print the contents of the character string "s" starting at LCD
  location "col" and "row".  The upper left corner is col=1, row=1.
  The string must be terminated by a zero byte.
*/
void lcd_out(int pos, unsigned char *s)
{
	//set demux bit to LCD
	PORTD &= ~(1 << DEMUXSELECT);

	sci_out((unsigned char) 0xfe);	// Set the cursor position
	sci_out((unsigned char) 0x45);
	sci_out((unsigned char) pos);

	sci_outs(s);							// Output the string
}

void lcd_clear()
{
	//set demux bit to LCD
	PORTD &= ~(1<< DEMUXSELECT);

	sci_out(0xfe);
	sci_out(0x51);
}
