#ifndef COMPASS_H
#define COMPASS_H

void compass_init(void);
unsigned char compass_get_x(void);
unsigned char compass_get_y(void);
unsigned char compass_get_z(void);

#endif