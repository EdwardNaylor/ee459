#ifndef COMPASS_H
#define COMPASS_H

void compass_init(void);
short compass_get_x(void);
short compass_get_y(void);
short compass_get_z(void);
double compass_get_north(void);
const char* compass_get_dir(void);

#endif
