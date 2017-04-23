#ifndef Radio_h
#define Radio_h

#include <avr/io.h>

typedef int bool;
#define false 0
#define true 1

char send_str[80];
void radio_send_message();
void radio_out(unsigned char *);
char radio_in();
bool check_radio();
void radio_read();
int radio_length;
int radio_index;
char radio_buffer[40];
char radio_len_buffer[40];
char radio_name[40];
int radio_status;
long radio_lon;
long radio_lat;

#endif
