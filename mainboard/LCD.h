#ifndef LCD_h
#define LCD_h

#define LCD_LINE_ONE 0x00
#define LCD_LINE_TWO 0x40
#define LCD_LINE_THREE 0x14
#define LCD_LINE_FOUR 0x54

void lcd_init(void);
void lcd_out(int, unsigned char *);
void lcd_clear();

#endif
