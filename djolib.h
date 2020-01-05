/* 
 * File:   djolib.h
 * Author: Ziqian
 *
 * Created on April 3, 2019, 8:57 AM
 */

#ifndef DJOLIB_H
#define	DJOLIB_H

#include <xc.h> // include processor files - each processor file is guarded. 

#ifdef	__cplusplus
extern "C" {
#endif

  // Insert declarations
void delay(int delay_ms);
void lcd_cmd (char command);
void initLCD(void);
void lcd_setCursor(char x, char y);
void lcd_printChar(char myChar);
void lcd_printStr(const char *s);


#ifdef	__cplusplus
}
#endif

#endif	/* DJOLIB_H */

