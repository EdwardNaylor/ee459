#ifndef Radio_h
#define Radio_h

#include <avr/io.h>

typedef int bool;
#define false 0
#define true 1

char send_str[100];
void radio_send_message();
void radio_out(unsigned char *);
char radio_in();
bool check_radio();
void radio_read();
int radio_sum;
int radio_index;
char radio_buffer[100];
char radio_sum_buffer[40];

void radio_init();

char radio_name[10][40];
int radio_status[10];
unsigned long radio_fix[10];
long radio_lon[10];
long radio_lat[10];

#endif
