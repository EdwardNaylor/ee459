/*************************************************************
*       at328-6.c - Demonstrate interface to a serial LCD display
*
*       This program will print a message on an LCD display
*       using a serial interface.  It is designed to work with a
*       Matrix Orbital LK204-25 using an RS-232 interface.
*
* Revision History
* Date     Author      Description
* 11/07/07 A. Weber    First Release
* 02/26/08 A. Weber    Code cleanups
* 03/03/08 A. Weber    More code cleanups
* 04/22/08 A. Weber    Added "one" variable to make warning go away
* 04/11/11 A. Weber    Adapted for ATmega168
* 11/18/13 A. Weber    Renamed for ATmega328P
*************************************************************/

#include <avr/io.h>
#include <util/delay.h>
#include "LCD.h"
#include "Serial.h"

/*
	First line:		0-19
	Second line:	40-59
	Third line:		20-39
	Fourth line:	??-??

	Fourth line doesnt seem to work well
*/

// #define FOSC 7372800			// Clock frequency 7372800
// #define BAUD 9600              // Baud rate used by the LCD 104ns
// #define MYUBRR FOSC/16/BAUD-1   // Value for UBRR0 register

// int main(void) {
//
// 	sci_init();						// Initialize the SCI port
//
// 	lcd_init();						// Initialize the LCD
//
// 	lcd_out(55, (unsigned char *) str1);				// Print string on line 1
// 	//lcd_out(2, 1, (unsigned char *) str2);		// Print string on line 2
// 	//lcd_out(3, 1, (unsigned char *) str3);		// Print string on line 3
// 	//lcd_out(4, 1, (unsigned char *) str4);		// Print string on line 4
//
// 	while (1) {               // Loop forever
// 	}
//
// 	return 0;   /* never reached */
// }

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
	sci_out((unsigned char) 0xfe);	// Set the cursor position
	sci_out((unsigned char) 0x45);
	sci_out((unsigned char) pos);

	sci_outs(s);							// Output the string
}
