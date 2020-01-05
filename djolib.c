#include "xc.h"
#include "djolib.h"
#include "string.h"
#include "delay.h"

void delay(int delay_ms) {
    int i;
    for (i = 0; i < delay_ms; i++) {
    wait_1ms();
    }
}

void lcd_cmd(char Package) {
    I2C2CONbits.SEN = 1;
    while (I2C2CONbits.SEN == 1);
    IFS3bits.MI2C2IF = 0;  //clear flag
    I2C2TRN = 0b01111100; //slave address
    while (IFS3bits.MI2C2IF == 0);
    IFS3bits.MI2C2IF = 0;
    I2C2TRN = 0b00000000; //control bytes
    while (IFS3bits.MI2C2IF == 0);
    IFS3bits.MI2C2IF = 0;
    I2C2TRN = Package; //data/command bytes
    while (IFS3bits.MI2C2IF == 0);
    IFS3bits.MI2C2IF = 0;
    I2C2CONbits.PEN = 1;
    while (I2C2CONbits.PEN == 1);
    
}

void initLCD(void) {
    //wait 50ms;
    delay(50);
    lcd_cmd(0b00111000); // function set, normal instruction mode
    lcd_cmd(0b00111001); // function set, extended instruction mode
    lcd_cmd(0b00010100); // interval osc
    lcd_cmd(0b01110000); // contrast C3-C0
    lcd_cmd(0b01011110); // Ion, Bon, C5-C4
    lcd_cmd(0b01101100); // follower control
    //wait 200ms;
    delay(200);
    lcd_cmd(0b00111000); // function set, normal instruction mode
    lcd_cmd(0b00001100); // Display On
    lcd_cmd(0b00000001); // Clear Display
    //wait 2ms;
    delay(2);
}
 void lcd_setCursor(char row, char col) {
     int i = 0x40 * row + col;    
     int pak = i | 0x80;
     lcd_cmd(pak);
 }
 
 void lcd_printChar(char Package) {
    I2C2CONbits.SEN = 1;
    while (I2C2CONbits.SEN == 1); 
    IFS3bits.MI2C2IF = 0;  //clear flag
    I2C2TRN = 0b01111100; //slave address
    while (IFS3bits.MI2C2IF == 0); //wait for ACK
    IFS3bits.MI2C2IF = 0; //reset ACK
    I2C2TRN = 0b01000000; //control bytes
    while (IFS3bits.MI2C2IF == 0); //wait for ACK
    IFS3bits.MI2C2IF = 0;
    I2C2TRN = Package; //data bytes
    while (IFS3bits.MI2C2IF == 0); //wait for ACK
    IFS3bits.MI2C2IF = 0;
    I2C2CONbits.PEN = 1;
    while (I2C2CONbits.PEN == 1);
    
}
 
 void lcd_printStr(const char *s) {
    I2C2CONbits.SEN = 1;
    while (I2C2CONbits.SEN == 1);
    IFS3bits.MI2C2IF = 0;  //clear flag   
    I2C2TRN = 0b01111100; //slave address
    while (IFS3bits.MI2C2IF == 0);
    IFS3bits.MI2C2IF = 0;
    size_t data_len = strlen(s);
    int i;
     for (i = 0; i < data_len; i++) {
        I2C2TRN = 0b11000000; //control bytes
        while (IFS3bits.MI2C2IF == 0);
        IFS3bits.MI2C2IF = 0;
        I2C2TRN = s[i]; //data
        while (IFS3bits.MI2C2IF == 0);
        IFS3bits.MI2C2IF = 0;
     }
    I2C2CONbits.PEN = 1;
    while (I2C2CONbits.PEN == 1);   
 }
 
