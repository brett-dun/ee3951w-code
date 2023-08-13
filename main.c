/*
 * File:   main.c
 * Author: Brett Duncan
 *
 * Created on December 8, 2022, 6:43 PM
 */


#include <p24FJ64GA002.h>
#include <stdio.h>

#include "xc.h"
#include "adc.h"
#include "lcd.h"


// CW1: FLASH CONFIGURATION WORD 1 (see PIC24 Family Reference Manual 24.1)
#pragma config ICS = PGx1          // Comm Channel Select (Emulator EMUC1/EMUD1 pins are shared with PGC1/PGD1)
#pragma config FWDTEN = OFF        // Watchdog Timer Enable (Watchdog Timer is disabled)
#pragma config GWRP = OFF          // General Code Segment Write Protect (Writes to program memory are allowed)
#pragma config GCP = OFF           // General Code Segment Code Protect (Code protection is disabled)
#pragma config JTAGEN = OFF        // JTAG Port Enable (JTAG port is disabled)


// CW2: FLASH CONFIGURATION WORD 2 (see PIC24 Family Reference Manual 24.1)
#pragma config I2C1SEL = PRI       // I2C1 Pin Location Select (Use default SCL1/SDA1 pins)
#pragma config IOL1WAY = OFF       // IOLOCK Protection (IOLOCK may be changed via unlocking seq)
#pragma config OSCIOFNC = ON       // Primary Oscillator I/O Function (CLKO/RC15 functions as I/O pin)
#pragma config FCKSM = CSECME      // Clock Switching and Monitor (Clock switching is enabled, 
                                       // Fail-Safe Clock Monitor is enabled)
#pragma config FNOSC = FRCPLL      // Oscillator Select (Fast RC Oscillator with PLL module (FRCPLL))

void setup(void){
    _RCDIV = 0;
    AD1PCFG = 0xffff;
    lcd_init();
    initBuffer();
    adc_init();
}

void __attribute__((interrupt, auto_psv)) _ADC1Interrupt(void){
    // Counter for the number of times the ADC interrupt has been triggered.
    static int counter = 0;
    
    // Clear the interrupt flag.
    _AD1IF = 0;
    
    // Take values out of the ADC hardware buffer and put them in the circular
    // buffer.
    putVal(ADC1BUF0);
    putVal(ADC1BUF1);
    putVal(ADC1BUF2);
    putVal(ADC1BUF3);
    putVal(ADC1BUF4);
    putVal(ADC1BUF5);
    putVal(ADC1BUF6);
    putVal(ADC1BUF7);
    putVal(ADC1BUF8);
    putVal(ADC1BUF9);
    putVal(ADC1BUFA);
    putVal(ADC1BUFB);
    putVal(ADC1BUFC);
    putVal(ADC1BUFD);
    putVal(ADC1BUFE);
    putVal(ADC1BUFF);
    
    // Increment our count of the number of interrupts that have been triggered.
    counter++;
    
    // 16 interrupt triggers * 16 samples = 256 samples
    if(counter == 16){
        
        // Reset the counter for the next 256 samples.
        counter = 0;
        
        // Find the average of the 256 samples in the circular buffer.
        unsigned long int avg = getAvg();
//        unsigned long int avg = ADC1BUF0 << 4;
//        unsigned long int avg = 256 << 4;
        
        // Character array to store the characters for the LCD.
        char lsByte[20];
        char msByte[20];
        
        int digit;
        for(digit = 7; digit >=0; digit--){
            // Check if the digit is a 1 or 0.
            lsByte[7 - digit] = avg & (1 << digit) ? '1' : '0';
        }
        for(digit = 15; digit >=8; digit--){
            // 
            if(15-digit == 1){
                msByte[15-digit] = '.';
                continue;
            }
            if(15-digit == 0){
                msByte[15-digit] = '0';
                continue;
            }
            msByte[15 - digit] = avg & (1 << digit) ? '1' : '0';
        }
        
        // Null character to terminate the string.
        lsByte[9] = '\0';
        msByte[9] = '\0';
        
        // Set the cursor position to the beginning of the top row.
        lcd_setCursor(0,0);
        lcd_printStr(msByte);
        
        // Set the cursor position to the beginning of the bottom row.
        lcd_setCursor(0, 1);
        lcd_printStr(lsByte);
    }
}

int main(void) {
    setup();
    // While loop to run forever. All code is implemented in interrupts.
    while(1){}
    return 0;
}
