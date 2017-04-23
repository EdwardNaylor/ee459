#include "Timer.h"
#include "Serial.h"
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <stdlib.h>
#include <string.h>

#define SWITCH_MASK 0x0F

uint8_t get_swtich()
{
	return PINC & SWITCH_MASK;
}

void send_serial(char * text) {
	char str[80];
	int len = strlen(text);
	strcpy(str, text);
	char lenBuffer[10];
	itoa(len, lenBuffer, 10);
	strcat(str, lenBuffer);
	strcat(str, ";");
	sci_outs(str);
}

int main(void) {
	sci_init();						// Initialize the SCI port

	timer_init();				// Initialize the timer

	//enable global interrupts
	sei();

	//set button ports as inputs
	DDRC &= ~(1 << PC0);
	DDRC &= ~(1 << PC1);
	DDRC &= ~(1 << PC2);
	DDRC &= ~(1 << PC3);

	//set buttons pull up resistors
	PORTC |= (1 << PC0);
	PORTC |= (1 << PC1);
	PORTC |= (1 << PC2);
	PORTC |= (1 << PC3);

	while (1) {
		switch(get_swtich())
		{
			case 1:
				send_serial("$testA,0,12,12;");
				break;
			case 2:
				send_serial("$testB,1,123,345;");
				break;
			case 4:
				send_serial("$testC,2,345,456;");
				break;
			case 8:
				send_serial("$testD,3,45,41;");
				break;
		}
		_delay_ms(10);
	}
	return 0; //never reached
}
