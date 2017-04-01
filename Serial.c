#include <avr/io.h>
#include "Serial.h"

/*
  sci_init - Initialize the SCI port
*/
void sci_init(void) {
	UBRR0 = MYUBRR;				// Set baud rate
	UCSR0B |= (1 << TXEN0);		// Turn on transmitter
	UCSR0B |= (1 << RXEN0);		// Turn on recv
	UCSR0C = (3 << UCSZ00);		// Set for asynchronous operation, no parity,
										// one stop bit, 8 data bits
}

/*
	sci_out - Output a byte to SCI port
*/
void sci_out(unsigned char ch)
{
	while ( (UCSR0A & (1<<UDRE0)) == 0);
	UDR0 = ch;
}

/*
	sci_outs - Print the contents of the character string "s" out the SCI
	port. The string must be terminated by a zero byte.
*/
void sci_outs(unsigned char *s)
{
	unsigned char ch;

	while ((ch = *s++) != (unsigned char) '\0')
		sci_out(ch);
}

/*
serial_in  - Read a byte  from  the  USART0  and  return  it
*/
char sci_in()
{
	while ( !( UCSR0A & (1 << RXC0)) );
	return  UDR0;
}
