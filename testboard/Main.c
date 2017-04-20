#include "Timer.h"
#include "Serial.h"
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

#define SWITCH_MASK 0x0F

uint8_t get_swtich()
{
	return PINC & SWITCH_MASK;
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
			case 0:
				sci_out('u');
				break;
			case 1:
				sci_out(0x0F);
				break;
			case 2:
				sci_out(0xF0);
				break;
			case 4:
				sci_out(0xFF);
				break;
		}
		_delay_ms(500);
	}
	return 0; //never reached
}
