#ifndef Radio_h
#define Radio_h

#include <avr/io.h>

typedef int bool;
#define false 0
#define true 1

void radio_out(unsigned char);
char radio_in();
bool check_radio();

#endif
