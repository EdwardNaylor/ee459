#ifndef Serial_h
#define Serial_h

#define FOSC 7372800			// Clock frequency 7372800
#define BAUD 9600              // Baud rate used by the LCD 104ns
#define MYUBRR FOSC/16/BAUD-1   // Value for UBRR0 register

void sci_init(void);
void sci_out(unsigned char);
void sci_outs(unsigned char *);
char sci_in();

#endif
