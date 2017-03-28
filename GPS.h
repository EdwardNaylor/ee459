#ifndef GPS_h
#define GPS_h

typedef int bool;
#define false 0
#define true 1

typedef unsigned char byte;

enum {_GPS_SENTENCE_GPGGA, _GPS_SENTENCE_GPRMC, _GPS_SENTENCE_OTHER};

#define _GPRMC_TERM   "GPRMC"
#define _GPGGA_TERM   "GPGGA"

#define _GPS_MPH_PER_KNOT 1.15077945
#define _GPS_MPS_PER_KNOT 0.51444444
#define _GPS_KMPH_PER_KNOT 1.852
#define _GPS_MILES_PER_METER 0.00062137112
#define _GPS_KM_PER_METER 0.001

enum {GPS_INVALID_AGE = 0xFFFFFFFF, GPS_INVALID_ANGLE = 999999999, GPS_INVALID_ALTITUDE = 999999999, GPS_INVALID_DATE = 0,
  GPS_INVALID_TIME = 0xFFFFFFFF, GPS_INVALID_SPEED = 999999999, GPS_INVALID_FIX_TIME = 0xFFFFFFFF};

// properties
unsigned long _time, _new_time;
unsigned long _date, _new_date;
long _latitude, _new_latitude;
long _longitude, _new_longitude;
long _altitude, _new_altitude;
unsigned long  _speed, _new_speed;
unsigned long  _course, _new_course;

unsigned long _last_time_fix, _new_time_fix;
unsigned long _last_position_fix, _new_position_fix;

// parsing state variables
byte _parity;
bool _is_checksum_term;
char _term[15];
byte _sentence_type;
byte _term_number;
byte _term_offset;
bool _gps_data_good;

void setup();
void loop();
// Get and process GPS data
void gpsdump();
// Feed data as it becomes available
bool feedgps();
float distance_between(float lat1, float long1, float lat2, float long2);
void get_position(long *latitude, long *longitude, unsigned long *fix_age);

//internal utilities
int from_hex(char a);
unsigned long parse_decimal();
unsigned long parse_degrees();
bool term_complete();
bool gpsisdigit(char c);
long gpsatol(const char *str);
int gpsstrcmp(const char *str1, const char *str2);

#endif
