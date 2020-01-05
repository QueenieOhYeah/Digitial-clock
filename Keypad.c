#include <p24FJ64GA002.h>
#include "xc.h"
#include "delay.h"
#include "djolib.h"
#include "math.h"


void initKeypad(void){
                  
    TRISBbits.TRISB11 = 0;       // Set output pins
    TRISBbits.TRISB12 = 0;
    TRISBbits.TRISB13 = 0;
    TRISBbits.TRISB14 = 0;
    CNPU1bits.CN2PUE = 1;        //enable pull-up resistor on RA0
    CNPU1bits.CN3PUE = 1;        //enable pull-up resistor on RA1
    CNPU2bits.CN30PUE = 1;       //enable pull-up resistor on RA2
    CNPU2bits.CN29PUE = 1;       //enable pull-up resistor on RA3
}


char readKeypad(void){                      // The resistor is pulled high so we read input with low
    LATBbits.LATB11 = 0;           
    LATBbits.LATB12 = 1;
    LATBbits.LATB13 = 1;
    LATBbits.LATB14 = 1;                    // Set row 1 as 0
    delay(1);
    int column = PORTA & 0b1111;            // Read the input from the column
        if (column == 0b1110){              // Column 4
            return 'A';                     // returns 0, which means button is pressed on that specific column
        } else if (column == 0b1101){       // Column 3
            return '3';
        } else if (column == 0b1011){       // Column 2
            return '2';
        } else if (column == 0b0111){       // Column 1
            return '1';
        }
    LATBbits.LATB11 = 1;
    LATBbits.LATB12 = 0;
    LATBbits.LATB13 = 1;
    LATBbits.LATB14 = 1;                    // Set row 2 as 0
    delay(1);
    column = PORTA & 0b1111;
        if (column == 0b1110){
            return 'B';
        } else if (column == 0b1101){
            return '6';
        } else if (column == 0b1011){
            return '5';
        } else if (column == 0b0111){
            return '4';
        }
    LATBbits.LATB11 = 1;
    LATBbits.LATB12 = 1;
    LATBbits.LATB13 = 0;
    LATBbits.LATB14 = 1;                    // Set row 3 as 0
    delay(1);
    column = PORTA & 0b1111;
        if (column == 0b1110){
            return 'C';
        } else if (column == 0b1101){
            return '9';
        } else if (column == 0b1011){
            return '8';
        } else if (column == 0b0111){
            return '7';
        }
    LATBbits.LATB11 = 1;
    LATBbits.LATB12 = 1;
    LATBbits.LATB13 = 1;
    LATBbits.LATB14 = 0;                    // Set row 4 as 0
    delay(1);
    column = PORTA & 0b1111;
        if (column == 0b1110){
            return 'D';
        } else if (column == 0b1101){
            return '#';
        } else if (column == 0b1011){
            return '0';
        } else if (column == 0b0111){
            return '*';
        }
    return 'N';
}
