#include "Serial.h"
#include "Radio.h"
#include <avr/io.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "GPS.h"
#include "Buttons.h"
#include "LCD.h"
#include "Timer.h"

void radio_init() {
  strcpy(radio_name[0], "Mainboard");
}

void radio_send_message() {
  strcpy(send_str, "$");
  strcat(send_str, radio_name[0]);
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
    radio_sum_buffer[radio_index] = radio_in();
    if (radio_sum_buffer[radio_index] == ';') {
      radio_sum_buffer[radio_index] = '\0';
      radio_sum = atoi(radio_sum_buffer);
      break;
    }
    radio_index++;
    timeout++;
  }

  //checksum
  int sum = 0;
  int len = strlen(radio_buffer);
  for (int i = 0; i < len; i++) {
    sum += radio_buffer[i];
  }

  // unsigned char buffer[20];
  // sprintf(buffer,"calc %d radio %s", sum, radio_sum_buffer);
  // lcd_out(LCD_LINE_FOUR, buffer);

  if (sum == radio_sum - '$' - ';') {
    //checksum success
    int count = 0;
    char *pt;
    pt = strtok (radio_buffer,",");
    int user_insert_index = -1;
    while (pt != NULL && pt != "") {
        if (count == 0) {
          //name
          for (int i = 0; i < max_users; i++) {
            if (strcmp(radio_name[i], pt) == 0) {
              user_insert_index = i;
            }
          }
          if (user_insert_index == -1) {
            max_users++;
            user_insert_index = max_users - 1;
          }

          strcpy(radio_name[user_insert_index], pt);
          radio_fix[user_insert_index] = millis();
        } else if (count == 1) {
          radio_status[user_insert_index] = atoi(pt);
        } else if (count == 2) {
          radio_lat[user_insert_index] = strtol(pt, 0, 10);
        } else if (count == 3) {
          radio_lon[user_insert_index] = strtol(pt, 0, 10);
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
