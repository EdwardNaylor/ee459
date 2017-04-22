#include "Serial.h"
#include "Radio.h"
#include <avr/io.h>

void radio_out(unsigned char ch)
{
  //set demux bit to radio
  PORTD |= (1 << DEMUXSELECT);

  sci_out(ch);
}

char radio_in()
{
  //set mux bit to radio
  PORTD |= (1 << MUXSELECT);

  return sci_in();
}

bool check_radio() {
  //set mux bit to radio
  PORTD |= (1 << MUXSELECT);
  if (( UCSR0A & (1 << RXC0))) {
    return true;
  } else {
    return false;
  }
}
