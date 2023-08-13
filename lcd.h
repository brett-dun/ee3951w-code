
#ifndef LCD_H
#define	LCD_H

#include <xc.h>

#ifdef	__cplusplus
extern "C" {
#endif

void lcd_cmd(char Package);
void lcd_init(void);
void lcd_setCursor(char x, char y);
void printChar(char myChar);
void delay_ms(unsigned int ms);
void lcd_printStr(const char s[]);
void lcd_scroll(void);

#ifdef	__cplusplus
}
#endif

#endif	/* LCD_H */