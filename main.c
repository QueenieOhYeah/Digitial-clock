#include <p24Fxxxx.h>
#include <xc.h>
#include "djolib.h"
#include "circularbuffer.h"
#include "Keypad.h"
#include <stdio.h> // for sprintf

// PIC24FJ64GA002 Configuration Bit Settings
// CW1: FLASH CONFIGURATION WORD 1 (see PIC24 Family Reference Manual 24.1)
#pragma config ICS = PGx1          // Comm Channel Select (Emulator EMUC1/EMUD1 pins are shared with PGC1/PGD1)
#pragma config FWDTEN = OFF        // Watchdog Timer Enable (Watchdog Timer is disabled)
#pragma config GWRP = OFF          // General Code Segment Write Protect (Writes to program memory are allowed)
#pragma config GCP = OFF           // General Code Segment Code Protect (Code protection is disabled)
#pragma config JTAGEN = OFF        // JTAG Port Enable (JTAG port is disabled)
// CW2: FLASH CONFIGURATION WORD 2 (see PIC24 Family Reference Manual 24.1)
#pragma config POSCMOD = NONE           // Primary Oscillator Select (Primary oscillator disabled. 					// Primary Oscillator refers to an external osc connected to the OSC1 and OSC2 pins)
#pragma config I2C1SEL = PRI       // I2C1 Pin Location Select (Use default SCL1/SDA1 pins)
#pragma config IOL1WAY = OFF       // IOLOCK Protection (IOLOCK may be changed via unlocking seq)
#pragma config OSCIOFNC = ON       // OSC2/CLKO/RC15 functions as port I/O (RC15)
#pragma config FCKSM = CSECME      // Clock Switching and Monitor (Clock switching is enabled,                                      // Fail-Safe Clock Monitor is enabled)
#pragma config FNOSC = FRCPLL      // Oscillator Select (Fast RC Oscillator with PLL module (FRCPLL))
#define TSize 2
int HOUR[TSize];
int MIN[TSize];
int SEC[TSize];
int A_HOUR[TSize];
int A_MIN[TSize];
int A_SEC[TSize];
int TEMP[TSize];
int A_ON = 0;
char print_time[10];

/*Functions for clock*/

/*Print time on LCD*/
void print_t (int* hour, int* min, int*sec) {
     sprintf(print_time, "%d%d:%d%d:%d%d",hour[0],hour[1], min[0], min[1], sec[0], sec[1]);     
     lcd_setCursor(0, 0); 
     lcd_printStr(print_time);     
 }

/* Refresh time, work for every second*/
 void refresh_time (void) 
 {
     SEC[1]++;
     if (SEC[1] == 10) 
     {
         SEC[0]++;
         SEC[1] = 0;
     }
     if (SEC[0] == 6) 
     {
         MIN[1]++;
         SEC[0] = 0;         
     }
     if (MIN[1] == 10) 
     {
         MIN[0]++;
         MIN[1] = 0;
     }
     if (MIN[0] == 6) 
     {
         HOUR[1]++;
         MIN[0] = 0;         
     }
     if (HOUR[0] == 2) 
     {        
         if (HOUR[1] == 4) 
         {
             HOUR[0] = 0;
             HOUR[1] = 0;             
         }
     } 
     else 
     {
         if (HOUR[1] == 10) 
         {
             HOUR[0]++;
             HOUR[1] = 0;
         }
     }
}
 
 /*setup timer 1 and initial values for clock*/
 void initClock (void) {    
    //Set up timer, every 1s
    T1CON = 0;
    T1CONbits.TCKPS1 = 1;
    T1CONbits.TCKPS0 = 1;
    TMR1 = 0;
    PR1 = 62499; //1s
    //Set up interrupt
    _T1IF = 0;   
    _T1IE = 1;
    _T1IP = 5; //Timer 1 used to refresh every second have the highest priority
    HOUR[0] = 0;
    HOUR[1] = 0;
    MIN[0] = 0;
    MIN[1] = 0;
    SEC[0] = 0;
    SEC[1] = 0;
    A_HOUR[0] = 0;
    A_HOUR[1] = 0;
    A_MIN[0] = 0;
    A_MIN[1] = 0;
    A_SEC[0] = 0;
    A_SEC[1] = 0;   
    TEMP[0] = 9;
    TEMP[1] = 9;
}
 
 /*Function to set up the time for alarm
  * if users press "#" on keypad, then the user is going to change the next digit of the time displayed
  * if users press "*" on keypad, then the user is going to change the previous digit of the time displayed
  * if users press "C" on keypad, it is going to return to the clock without setting alarm
  * if press "D", then confirm the alarm and return to clock
  */
 
void setup_alarm (void) {
    int* location = &A_HOUR[0];
    int flag = 0; 
    lcd_setCursor(1, 0);
    lcd_printStr("SetAlarm");
    print_t(A_HOUR, A_MIN, A_SEC);
    while (1) {
        char res = readKeypad();
        if (res != 'N' && flag == 0) {  // When button is pressed,
            if (res == '#') { //if readkeypad = "#" move location pointing to next digit e.g HOUR[0] -> HOUR[1]
                if (location == &A_HOUR[0]) location = &A_HOUR[1];
                else if (location == &A_HOUR[1]) location = &A_MIN[0];
                else if (location == &A_MIN[0]) location = &A_MIN[1];
                else location = &A_HOUR[0];
            }
            else if (res == '*') {  //if readkeypad = "*", HOUR[1] -> HOUR[0]
                if (location == &A_HOUR[1]) location = &A_HOUR[0];
                else if (location == &A_HOUR[0]) location = &A_MIN[1];
                else if (location == &A_MIN[1]) location = &A_MIN[0];
                else location = &A_HOUR[1];
            }
            else if (res == 'A'){} 
            else if (res == 'B'){} 
            else if (res == 'C') break; //return
            else if (res == 'D') { //confirm
                A_ON = 1; //indicate the alarm is on
                break;    
            }
            else {
                if (location == &A_HOUR[0] && res <= 50) *location = res - 48; //transform char to corresponding int
                else if (location == &A_HOUR[1] && res < 65) {
                    if (A_HOUR[0] == 2 && res > 51) ;
                    else *location = res-48;
                }
                else if (location == &A_MIN[0] && res <= 53) *location = res -48;
                else if (location == &A_MIN[1]) *location = res - 48;
                else print_t(A_HOUR, A_MIN, A_SEC);
            }
            flag = 1;       
            print_t(A_HOUR, A_MIN, A_SEC); //execute this section of code, once during the pressing of buttons
        }                   
        if (res == 'N' && flag == 1) flag = 0;  //When button is released, reset the flag
    }
}

// set up temperature alarm
void setTemp(void) {
    lcd_setCursor(1,0);
    lcd_printStr("Set Temp");//second row will display set temp
    int* location = &TEMP[0];
    char* Str[10];
    lcd_printStr(Str);
    int flag = 0;
        while (1) {
            char res = readKeypad(); //define a variable
            // it read value from keypad
            if (res != 'N' && flag == 0) {  // When button is pressed,(read something from keypad)
                if (res == '#') //when keypad press #, it could change ten digits number first
                {
                    location = &TEMP[1];
                }
                else if (res == '*') {
                    location = &TEMP[0];
                }
                else if (res == 'A' ){}
                else if (res == 'B' ){}
                else if (res == 'C' ){}
                else if (res == 'D') { //confirm  
                    // when press D, it will break 
                    break;
                }
                else {
                    *location = res - 48;
                }
                flag = 1;      
                sprintf(Str, "%d%d    \337C",TEMP[0], TEMP[1]); 
                lcd_setCursor(0,0);
                lcd_printStr(Str); 
            }                   
            if (res == 'N' && flag == 1) flag = 0;  //When button is released, reset the flag
    }
}

/* Function allows the user to input initial time for the clock when it is turned on, 
 * also used to reset clock later if wanted.
 * if users press "#" on keypad, then the user is going to change the next digit of the time displayed
 * if users press "*" on keypad, then the user is going to change the previous digit of the time displayed
 * if press "D", then confirm the clock
 */

void setup_clock (void) {
    int* location = &HOUR[0];
    int flag = 0;
    lcd_setCursor(1, 0); 
    lcd_printStr("Set Time"); 
    print_t(HOUR, MIN, SEC);
    while (1) {
        char res = readKeypad();
        if (res != 'N' && flag == 0) {  
            if (res == '#') {
                if (location == &HOUR[0]) location = &HOUR[1];
                else if (location == &HOUR[1]) location = &MIN[0];
                else if (location == &MIN[0]) location = &MIN[1];
                else location = &HOUR[0];
            }
            else if (res == '*') {
                if (location == &HOUR[1]) location = &HOUR[0];
                else if (location == &HOUR[0]) location = &MIN[1];
                else if (location == &MIN[1]) location = &MIN[0];
                else location = &HOUR[1];
            }
            else if (res == 'A'){} 
            else if (res == 'B'){} 
            else if (res == 'C'){} 
            else if (res == 'D'){
                SEC[0] = 0; //reset second to be 0 before break, 
                SEC[1] = 0; //otherwise second is refreshing every second in the mode of reset clock
                break;
            }
            else {
                if (location == &HOUR[0] && res <= 50) *location = res - 48;
                else if (location == &HOUR[1] && res < 65) {
                    if (HOUR[0] == 2 && res > 51) ;
                    else *location = res-48;
                }
                else if (location == &MIN[0] && res <= 53) *location = res -48;
                else if (location == &MIN[1]) *location = res - 48;
                else {}
            }
            flag = 1;        
            SEC[0] = 0; //Keep display on the LCD for seconds maintains to be 00,
            SEC[1] = 0; //otherwise second is refreshing every second in the mode of reset clock
            print_t(HOUR, MIN, SEC);
        }   
        if (res == 'N' && flag == 1) flag = 0;  //When button is released, reset the flag         
    }
 }

/* Function that used to match button pressing to different mode*/
 void clock_funs (char res) {
    if (res == 'A') setup_clock(); //reset the clock
    else if (res == 'B') setup_alarm(); //set alarm
    else if (res == 'C') setDuty(0); //stop the buzzer when rings
    else if (res == '*') setTemp();
    else {}
}

 //include setup for ADC, temperature sensor part
 void setup(void) {
    CLKDIVbits.RCDIV = 0;
    AD1PCFG = 0x9fff; // all digital
    TRISBbits.TRISB15=1;
    LATB = 0xffff; 
    //Initialize Baud Rate Generator
    I2C2CONbits.I2CEN = 0;
    I2C2BRG = 0x9D; 
    I2C2CONbits.I2CEN = 1;
    AD1PCFGbits.PCFG9=0;	//AN9
    TRISAbits.TRISA0=1;    
    // ASAM disabled; ADSIDL disabled; DONE disabled; FORM Absolute decimal result, unsigned, right-justified; SAMP disabled; SSRC TMR3; ADON enabled; 
    AD1CON1 = 0x8044;   
    // CSCNA enabled; VCFG0 AVDD; VCFG1 AVSS; ALTS disabled; BUFM disabled; SMPI Generates interrupt after completion of every sample/conversion operation; 
    AD1CON2 = 0x0000;
    // SAMC 12; ADRC RC clock; ADCS 0; 
    AD1CON3 = 0x0C00;
    // CH0SA AN0; CH0SB AN0; CH0NB AVSS; CH0NA AVSS; 
    AD1CHS = 0x0909;
    // CSSL2 disabled; CSSL15 disabled; CSSL1 disabled; CSSL0 enabled; CSSL12 disabled; CSSL11 disabled; CSSL10 disabled; CSSL9 disabled; CSSL5 disabled; CSSL4 disabled; CSSL3 disabled; 
    AD1CSSL = 0x00;   
    // Enabling ADC1 interrupt.
    IEC0bits.AD1IE = 1;
    //Period = 0.0625 s; Frequency = 2000000 Hz; PR3 15624;  
    PR3 = 0x3D08;
    _T3IP = 3; //lower priority. //Ziqian Qiu 
    //TCKPS 1:8; TON enabled; TSIDL disabled; TCS FOSC/2; TGATE disabled; 
    T3CON = 0x8010;   
    T3CONbits.TON = 1;    
}
 
/*
 * Main Application
 */

uint16_t adValue;

int main(void) {
    char adStr[20];
    setup();
    initKeypad();
    initBuffer();
    initLCD();  
    initBuzzer();

    initClock ();
    setup_clock();
    T1CONbits.TON = 1;
    int flag = 0;
    int T_ON = 0;
    while (1) {
        delay(100);
        char res = readKeypad();// it used to detect whether new button pressed
        if (res != 'N' && flag == 0) {  //If yes, it will switch to the corresponding mode
            flag = 1;                   //If not, it will still display temperature and clock
        } else {
            print_t(HOUR, MIN, SEC);   //display clock
            sprintf(adStr, "%4.1f  \337C",((adValue/1024.0)*3.3-0.5)*100); 
            lcd_setCursor(1, 0); 
            lcd_printStr(adStr); //display temp
        }
        if (res == 'N' && flag == 1) flag = 0;
        // if the alarm is on and the current time matches the alarm, buzzer rings.
        if (A_ON == 1 && HOUR[0] == A_HOUR[0] && HOUR[1] == A_HOUR[1] && MIN[0] == A_MIN[0] && MIN[1] == A_MIN[1]) {
            setDuty(50);
            A_ON = 0;
        }
        // when time passed and temperature is higher than settings. Buzzer will alarm.
        if (((adValue/1024.0)*3.3-0.5)*100 >= TEMP[0] * 10 + TEMP[1] && T_ON == 0) { // when temp is high,
            setDuty(50);
            T_ON = 1; // execute this section of code once, otherwise cannot stop the buzzer
        }
        if (((adValue/1024.0)*3.3-0.5)*100 < TEMP[0] * 10 + TEMP[1] && T_ON == 1) T_ON = 0; // when temp is lower than setting, reset T_ON = 0            
    }
}

void __attribute__ ( ( __interrupt__ , auto_psv ) ) _ADC1Interrupt ( void )
{
	if(IFS0bits.AD1IF)
	{
		putVal(ADC1BUF0);
		// clear the ADC interrupt flag
		IFS0bits.AD1IF = 0;
	}
}

void __attribute__((interrupt, auto_psv)) _T1Interrupt() {
    IFS0bits.T1IF = 0;
    adValue = getAvg();
    refresh_time();
   
}
/*
 * End of File
 */
