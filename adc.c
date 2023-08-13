#include "adc.h"
#include "xc.h"

// Buffer size is defined to be 256 elements because this is number of samples
// we need to implement oversampling.
#define BUFSIZE 256

// Array used to implement the circular buffer.
int adc_buffer[BUFSIZE];

// Index to insert elements at in the circular buffer.
int buffer_index = 0;

// Function to initialize the ADC.
void adc_init(void){
    TRISAbits.TRISA1 = 1;  
    AD1PCFGbits.PCFG1 = 0;
    
    TMR3 = 0;
    T3CON = 0;
    T3CONbits.TCKPS = 0b10;
    PR3 = 487;
    T3CONbits.TON = 1; 

    AD1CON1 = 0;
    AD1CON2 = 0;
    AD1CON3 = 0;
    
    AD1CHSbits.CH0SA = 0b00001;
    
    AD1CON2bits.BUFM = 0;  // 16 bit buffers, 16 of them
    AD1CON2bits.SMPI = 0b1111;  // 16 conversions per interrupt, must unload after
    
    AD1CON2bits.VCFG = 0b001;
    AD1CON3bits.ADCS = 0b00000000;
    AD1CON1bits.SSRC = 0b010;
    AD1CON1bits.FORM = 0b00;
    
    AD1CON1bits.ASAM = 1;
    AD1CON1bits.ADON = 1;
    
    _AD1IF = 0;
    _AD1IE = 1;
}

// Function to initialize all elements in the buffer to zero.
void initBuffer(void){
    int i;
    for(i = 0; i < BUFSIZE; i++){
        adc_buffer[i] = 0;
    }
}

// Function to place an integer adc reading into the buffer.
void putVal(int ADCvalue){
    adc_buffer[buffer_index] =  ADCvalue;
    buffer_index++;
    if(buffer_index >= BUFSIZE - 1){
        buffer_index = 0;
    }
}

// Function to get the oversampled ADC reading.
unsigned long int getAvg(void){
    
    // Sum of the elements in the buffer.
    unsigned long int sum = 0;
    
    int i;
    for(i = 0; i < BUFSIZE; i++){
        sum += adc_buffer[i];
    }
    
    // Divide by 16 (shift right by 4) to get the average representing a 14-bit
    // ADC reading.
    unsigned long int avg = sum >> 4;
    
    return avg;
}
