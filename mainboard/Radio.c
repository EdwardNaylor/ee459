#include "Serial.h"
#include "Radio.h"
#include <avr/io.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "GPS.h"
#include "Buttons.h"

void radio_send_message() {
  strcpy(send_str, "$");
  strcat(send_str, "mainboard");
  strcat(send_str, ",");
  char statusBuffer[10];
  itoa(status, statusBuffer, 10);
  strcat(send_str, statusBuffer);
  strcat(send_str, ",");
  char latBuffer[20];
  sprintf(latBuffer,"%ld", _latitude);
  strcat(send_str, latBuffer);
  strcat(send_str, ",");
  char lonBuffer[20];
  sprintf(lonBuffer,"%ld", _longitude);
  strcat(send_str, lonBuffer);
  strcat(send_str, ";");
  int len = strlen(send_str);
  char lenBuffer[10];
  itoa(len, lenBuffer, 10);
  strcat(send_str, lenBuffer);
  strcat(send_str, ";");

  radio_out(send_str);
}

void radio_out(unsigned char * str)
{
  //set demux bit to radio
  PORTD |= (1 << DEMUXSELECT);

  sci_outs(str);
}

void radio_read() {
  int timeout = 0;
  while (true) {
    if (check_radio()) {
      timeout = 0;
    } else {
      timeout++;
      if (timeout > 100) {
        return;
      }
    }
    if (radio_in() == '$') {
      break;
    }
  };

  //get buffer
  radio_index = 0;
  timeout = 0;
  while (true) {
    if (timeout > 50) {
      return;
    }
    radio_buffer[radio_index] = radio_in();
    if (radio_buffer[radio_index] == ';') {
      radio_buffer[radio_index] = '\0';
      break;
    }
    radio_index++;
    timeout++;
  }

  //length
  radio_index = 0;
  timeout = 0;
  while (true) {
    if (timeout > 50) {
      return;
    }
    radio_len_buffer[radio_index] = radio_in();
    if (radio_len_buffer[radio_index] == ';') {
      radio_len_buffer[radio_index] = '\0';
      radio_length = atoi(radio_len_buffer);
      break;
    }
    radio_index++;
    timeout++;
  }

  if (strlen(radio_buffer) == radio_length - 2) {
    int count = 0;
    char *pt;
    pt = strtok (radio_buffer,",");
    while (pt != NULL) {
        if (count == 0) {
          strcpy(radio_name, pt);
        } else if (count == 1) {
          radio_status = atoi(pt);
        } else if (count == 2) {
          radio_lat = strtol(pt, 0, 10);
        } else if (count == 3) {
          radio_lon = strtol(pt, 0, 10);
        }
        count++;
        pt = strtok (NULL, ",");
    }
  }
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
