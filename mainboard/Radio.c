#include "Serial.h"
#include <avr/io.h>

void radio_out(unsigned char ch)
{
  //set demux bit to radio
  PORTD &= ~(1 << DEMUXSELECT);

  sci_out(ch);
}

char radio_in()
{
  //set mux bit to radio
  PORTD |= (1 << MUXSELECT);

  return sci_in();
}
