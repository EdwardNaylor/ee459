#include <avr/io.h>
#include "ShiftOut.h"

int latchPin = PB1;
int clockPin = PB2;
int dataGreenPin = PB3;
int dataRedPin = PB4;

void shift_init() {
  // set pins as output
  DDRB |= (1 << latchPin);
  DDRB |= (1 << clockPin);
  DDRB |= (1 << dataGreenPin);
  DDRB |= (1 << dataRedPin);
}

void shift_out(byte dataGreen, byte dataRed) {
  //ground latchPin and hold low for as long as you are transmitting
  PORTB &= ~(1 << latchPin);
  // This shifts 8 bits out MSB first,
  //on the rising edge of the clock,
  //clock idles low

  //internal function setup
  int i=0;

  //clear everything out just in case to
  //prepare shift register for bit shifting
  PORTB &= ~(1 << dataGreenPin);
  PORTB &= ~(1 << dataRedPin);
  PORTB &= ~(1 << clockPin);

  //for each bit in the byte myDataOut�
  //NOTICE THAT WE ARE COUNTING DOWN in our for loop
  //This means that %00000001 or "1" will go through such
  //that it will be pin Q0 that lights.
  for (i=7; i>=0; i--)  {
    PORTB &= ~(1 << clockPin);

    //if the value passed to myDataOut and a bitmask result
    // true then... so if we are at i=6 and our value is
    // %11010100 it would the code compares it to %01000000
    // and proceeds to set pinState to 1.
    if ( dataGreen & (1<<i) ) {
      PORTB |= (1 << dataGreenPin);
    }
    else {
      PORTB &= ~(1 << dataGreenPin);

    }

    if ( dataRed & (1<<i) ) {
      PORTB |= (1 << dataRedPin);
    }
    else {
      PORTB &= ~(1 << dataRedPin);
    }

    //register shifts bits on upstroke of clock pin
    PORTB |= (1 << clockPin);
    //zero the data pins after shift to prevent bleed through
    PORTB &= ~(1 << dataGreenPin);
    PORTB &= ~(1 << dataRedPin);
  }

  //stop shifting
  PORTB &= ~(1 << clockPin);

  //no longer needs to listen for information
  PORTB |= (1 << latchPin);
}
