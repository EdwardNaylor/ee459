#include "Buttons.h"
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include "Timer.h"

// Button definitions
#define PINTOGGLE PD5
#define PINUP PD6
#define PINDOWN PD7

// LED definitions
#define LEDRED PC3
#define LEDYELLOW PC2
#define LEDGREEN PC1

unsigned long last_interrupt_time = 0;

void buttons_init() {
	// set button pins as input
	DDRD &= ~(1 << PINTOGGLE);
	DDRD &= ~(1 << PINUP);
	DDRD &= ~(1 << PINDOWN);

	// enable pull up resistors on button pins
	PORTD |= (1 << PINTOGGLE);
	PORTD |= (1 << PINUP);
	PORTD |= (1 << PINDOWN);

	// set LED as outputs
	DDRC |= (1 << PC1);
	DDRC |= (1 << PC2);
	DDRC |= (1 << PC3);

	//set status to green initially
	status = 0;
	PORTC |= (1 << LEDGREEN);

	//set PORTD mask for interrupts
	PCMSK2 |= (1 << PCINT21);
	PCMSK2 |= (1 << PCINT22);
	PCMSK2 |= (1 << PCINT23);

	//enable interrupts on PORTD
	PCICR |= (1 << PCIE2);
}

// ISR for PORTD change interrupt.
ISR(PCINT2_vect)
{
		//determine which pin changed
		uint8_t PIND_saved = PIND;
		uint8_t difference;
		uint8_t reg;

		// Get the port byte.
		PIND_saved = PIND;

		unsigned long interrupt_time = millis();
		if (interrupt_time - last_interrupt_time > 200) {
			if ((PIND_saved & (1 << PINTOGGLE)) == 0)
			{
				switch (status) {
					case 0:
						PORTC &= ~(1 << LEDGREEN);
						break;
					case 1:
						PORTC &= ~(1 << LEDYELLOW);
						break;
					case 2:
						PORTC &= ~(1 << LEDRED);
						break;
				}

				status += 1;
				if (status == 3) {
					status = 0;
				}

				switch (status) {
					case 0:
						PORTC |= (1 << LEDGREEN);
						break;
					case 1:
						PORTC |= (1 << LEDYELLOW);
						break;
					case 2:
						PORTC |= (1 << LEDRED);
						break;
				}
			}
		}
		last_interrupt_time = interrupt_time;
}
