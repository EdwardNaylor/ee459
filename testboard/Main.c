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
	char str[100];
	int len = strlen(text);
	int sum = 0;
	for (int i = 0; i < len; i++) {
		sum += text[i];
	}
	strcpy(str, text);
	char sumBuffer[20];
	sprintf(sumBuffer, "%d", sum);
	strcat(str, sumBuffer);
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
				send_serial("$Leavy,0,3402166,-11828296;");
				break;
			case 2:
				send_serial("$Wemes,1,3402098,-11829677;");
				break;
			case 4:
				send_serial("$Colliseum,2,3401536,-11828830;");
				break;
			case 8:
				send_serial("$Engemann,0,3402506,-11828882;");
				break;
		}
		_delay_ms(10);
	}
	return 0; //never reached
}
