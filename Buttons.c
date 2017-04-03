#include "Buttons.h"
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

int togglePin = PD5;
int upPin = PD6;
int downPin = PD7;

void buttons_init() {
  // set button pins as input
  DDRD &= ~(1 << togglePin);
  DDRD &= ~(1 << upPin);
  DDRD &= ~(1 << downPin);

  //enable pull up resistors on button pins
  PORTD |= (1 << togglePin);
  PORTD |= (1 << upPin);
  PORTD |= (1 << downPin);

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
    volatile static uint8_t pcint_temp;
    uint8_t difference;
    uint8_t reg;

    // Get the port byte.
    reg = PIND;

    // XOR saved reg with current reg to get difference.
    difference = reg ^ pcint_temp;

    // Save the new value of PORTB.
    pcint_temp = reg;

    if (difference & (1 << togglePin))
    {
        // PINB = (1<<LED_1);
    }
    if (difference & (1 << upPin))
    {
        // PINB = (1<<LED_2);
    }
    if (difference & (1 << downPin))
    {
        // PINB = (1<<LED_2);
    }
}
