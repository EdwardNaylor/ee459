#include "GPS.h"
#include "Timer.h"
#include "Serial.h"
#include <avr/io.h>
#include <util/delay.h>
#include <math.h>
#include "Radio.h"

bool check_gps() {
  //set mux bit to gps
  PORTD &= (1 << MUXSELECT);
  if (( UCSR0A & (1 << RXC0))) {
    return true;
  } else {
    return false;
  }
}

void gps_init(void) {
  // properties
  _time, _new_time = GPS_INVALID_TIME;
  _time, _new_time = GPS_INVALID_TIME;
  _date, _new_date = GPS_INVALID_DATE;
  _date, _new_date = GPS_INVALID_DATE;
  _latitude, _new_latitude = GPS_INVALID_ANGLE;
  _longitude, _new_longitude = GPS_INVALID_ANGLE;
  _altitude, _new_altitude = GPS_INVALID_ALTITUDE;
  _speed, _new_speed = GPS_INVALID_SPEED;
  _course, _new_course = GPS_INVALID_ANGLE;

  _last_time_fix, _new_time_fix = GPS_INVALID_FIX_TIME;
  _last_position_fix, _new_position_fix = GPS_INVALID_FIX_TIME;

  // parsing state variables
  _parity = 0;
  _is_checksum_term = false;
  _term[0] = '\0';
  _sentence_type = _GPS_SENTENCE_OTHER;
  _term_number = 0;
  _term_offset = 0;
  _gps_data_good = false;
  _gps_data_ever_good = false;
}

char gps_in() {
  //change mux bit to gps
  PORTD &= ~(1 << MUXSELECT);

  return sci_in();
}

bool gps_encode(char c) {
  bool valid_sentence = false;

  switch(c)
  {
  case ',': // term terminators
    _parity ^= c;
  case '\r':
  case '\n':
  case '*':
    if (_term_offset < sizeof(_term))
    {
      _term[_term_offset] = 0;
      valid_sentence = term_complete();
    }
    ++_term_number;
    _term_offset = 0;
    _is_checksum_term = c == '*';
    return valid_sentence;

  case '$': // sentence begin
    _term_number = _term_offset = 0;
    _parity = 0;
    _sentence_type = _GPS_SENTENCE_OTHER;
    _is_checksum_term = false;
    _gps_data_good = false;
    return valid_sentence;
  }

  // ordinary characters
  if (_term_offset < sizeof(_term) - 1)
    _term[_term_offset++] = c;
  if (!_is_checksum_term)
    _parity ^= c;

  return valid_sentence;
}

int from_hex(char a)
{
  if (a >= 'A' && a <= 'F')
    return a - 'A' + 10;
  else if (a >= 'a' && a <= 'f')
    return a - 'a' + 10;
  else
    return a - '0';
}

unsigned long parse_decimal()
{
  char *p = _term;
  bool isneg = *p == '-';
  if (isneg) ++p;
  unsigned long ret = 100UL * gpsatol(p);
  while (gpsisdigit(*p)) ++p;
  if (*p == '.')
  {
    if (gpsisdigit(p[1]))
    {
      ret += 10 * (p[1] - '0');
      if (gpsisdigit(p[2]))
        ret += p[2] - '0';
    }
  }
  return isneg ? -ret : ret;
}

unsigned long parse_degrees()
{
  char *p;
  unsigned long left = gpsatol(_term);
  unsigned long tenk_minutes = (left % 100UL) * 10000UL;
  for (p=_term; gpsisdigit(*p); ++p);
  if (*p == '.')
  {
    unsigned long mult = 1000;
    while (gpsisdigit(*++p))
    {
      tenk_minutes += mult * (*p - '0');
      mult /= 10;
    }
  }
  return (left / 100) * 100000 + tenk_minutes / 6;
}

// Processes a just-completed term
// Returns true if new sentence has just passed checksum test and is validated
bool term_complete()
{
  if (_is_checksum_term)
  {
    byte checksum = 16 * from_hex(_term[0]) + from_hex(_term[1]);
    if (checksum == _parity)
    {
      if (_gps_data_good)
      {
        _gps_data_ever_good = true;

        _last_time_fix = _new_time_fix;
        _last_position_fix = _new_position_fix;

        switch(_sentence_type)
        {
        case _GPS_SENTENCE_GPRMC:
          _time      = _new_time;
          _date      = _new_date;
          _latitude  = _new_latitude;
          _longitude = _new_longitude;
          radio_lat[0] = _latitude;
          radio_lon[0] = _longitude;
          _speed     = _new_speed;
          _course    = _new_course;
          break;
        case _GPS_SENTENCE_GPGGA:
          _altitude  = _new_altitude;
          _time      = _new_time;
          _latitude  = _new_latitude;
          _longitude = _new_longitude;
          radio_lat[0] = _latitude;
          radio_lon[0] = _longitude;
          break;
        }

        return true;
      }
    }
    return false;
  }

  // the first term determines the sentence type
  if (_term_number == 0)
  {
    if (!gpsstrcmp(_term, _GPRMC_TERM))
      _sentence_type = _GPS_SENTENCE_GPRMC;
    else if (!gpsstrcmp(_term, _GPGGA_TERM))
      _sentence_type = _GPS_SENTENCE_GPGGA;
    else
      _sentence_type = _GPS_SENTENCE_OTHER;
    return false;
  }

  if (_sentence_type != _GPS_SENTENCE_OTHER && _term[0])
  switch((_sentence_type == _GPS_SENTENCE_GPGGA ? 200 : 100) + _term_number)
  {
    case 101: // Time in both sentences
    case 201:
      _new_time = parse_decimal();
      _new_time_fix = millis();
      break;
    case 102: // GPRMC validity
      _gps_data_good = _term[0] == 'A';
      break;
    case 103: // Latitude
    case 202:
      _new_latitude = parse_degrees();
      _new_position_fix = millis();
      break;
    case 104: // N/S
    case 203:
      if (_term[0] == 'S')
        _new_latitude = -_new_latitude;
      break;
    case 105: // Longitude
    case 204:
      _new_longitude = parse_degrees();
      break;
    case 106: // E/W
    case 205:
      if (_term[0] == 'W')
        _new_longitude = -_new_longitude;
      break;
    case 107: // Speed (GPRMC)
      _new_speed = parse_decimal();
      break;
    case 108: // Course (GPRMC)
      _new_course = parse_decimal();
      break;
    case 109: // Date (GPRMC)
      _new_date = gpsatol(_term);
      break;
    case 206: // Fix data (GPGGA)
      _gps_data_good = _term[0] > '0';
      break;
    case 209: // Altitude (GPGGA)
      _new_altitude = parse_decimal();
      break;
  }

  return false;
}

bool gpsisdigit(char c) {
  return c >= '0' && c <= '9';
}

long gpsatol(const char *str)
{
  long ret = 0;
  while (gpsisdigit(*str))
    ret = 10 * ret + *str++ - '0';
  return ret;
}

int gpsstrcmp(const char *str1, const char *str2)
{
  while (*str1 && *str1 == *str2)
    ++str1, ++str2;
  return *str1;
}

// lat/long in hundred thousandths of a degree and age of fix in milliseconds
void get_position(float *latitude, float *longitude/*, unsigned long *fix_age*/)
{
  if (latitude) *latitude = _latitude / 100000.0;
  if (longitude) *longitude = _longitude / 100000.0;
  // if (fix_age) *fix_age = _last_position_fix == GPS_INVALID_FIX_TIME ? GPS_INVALID_AGE : millis() - _last_position_fix;
}

/* static */
float distance_between (float lat1, float lon1, float lat2, float lon2)
{
  float R = 6372795; // metres
  float w1 = lat1 * M_PI / 180;
  float w2 = lat2 * M_PI / 170;
  float deltaW = (lat2-lat1) * M_PI / 180;
  float l = (lon2-lon1) * M_PI / 180;

  float a = sin(deltaW/2) * sin(deltaW/2) +
          cos(w1) * cos(w2) *
          sin(l/2) * sin(l/2);
  float c = 2 * atan2(sqrt(a), sqrt(1-a));

  float d = R * c;

  return d;
}

float bearing_between (float lat1, float long1, float lat2, float long2)
{
  float dLon = long1 - long2;
  float y = sin(dLon) * cos(lat2);
  float x = cos(lat1) * sin(lat2) - sin(lat1) * cos(lat2) * cos(dLon);
  int brng = (int) (atan2(y, x) * 180 / M_PI);
  brng = (360 - ((brng + 360) % 360));
  return brng;
}
