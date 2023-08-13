#include "lcd.h"
#include "xc.h"

#define CONTRAST 0b01110000

void delay_ms(unsigned int ms){
    while(ms-- > 0){
        asm("repeat #15998");
        asm("nop");
    }
}

void lcd_cmd(char Package){
    I2C2CONbits.SEN = 1;            //Initiate start
    while(I2C2CONbits.SEN == 1);    //Wait for start bit to clear
    IFS3bits.MI2C2IF = 0;           
    I2C2TRN = 0b01111100;           //Shift slave address and read bit
    while(IFS3bits.MI2C2IF == 0);   //Wait for slave to acknowledge 
    IFS3bits.MI2C2IF = 0;           
    I2C2TRN = 0b00000000;           //control byte
    while(IFS3bits.MI2C2IF == 0);   
    IFS3bits.MI2C2IF = 0;
    I2C2TRN = Package;              //Shift package byte
    while(IFS3bits.MI2C2IF == 0);
    IFS3bits.MI2C2IF = 0;
    I2C2CONbits.PEN = 1;            //Stop bit
    while(I2C2CONbits.PEN == 1);    //Wait for slave to acknowledge stop
}

void lcd_init(void){
    I2C2CONbits.I2CEN = 0;
    IFS3bits.MI2C2IF = 0;
    I2C2BRG = 0x9d;
    I2C2CONbits.I2CEN = 1;
    
    delay_ms(40);
    
    lcd_cmd(0b00111000);    //function set, normal instruction mode
    lcd_cmd(0b00111001);    //function set, extended instruction mode
    lcd_cmd(0b00010100);    //interval osc
    lcd_cmd(CONTRAST);      //contrast C3-C0
    lcd_cmd(0b01010110);    //Ion, Bon, C5-C4
    lcd_cmd(0b01101100);    //follower control
    
    delay_ms(200);
    
    lcd_cmd(0b00111000);    //function set, normal instruction mode
    lcd_cmd(0b00001100);    //Display on
    lcd_cmd(0b00000001);    //Clear Display
    
    delay_ms(1);
}

void lcd_setCursor(char x, char y){
    unsigned char address = (0x40 * y) + x;     //equation to find address in DDRAM
    address |= 0x80;                            //mask the address so 1 is the first bit
    lcd_cmd(address);                           
}

void printChar(char myChar){
    I2C2CONbits.SEN = 1;            //Initiate start
    while(I2C2CONbits.SEN);    //Wait for start bit to clear
    IFS3bits.MI2C2IF = 0;           
    I2C2TRN = 0b01111100;           //Shift slave address and read bit
    while(!IFS3bits.MI2C2IF);   //Wait for slave to acknowledge 
    IFS3bits.MI2C2IF = 0;           
    I2C2TRN = 0b01000000;           //control byte
    while(!IFS3bits.MI2C2IF);   
    IFS3bits.MI2C2IF = 0;
    I2C2TRN = myChar;              //Shift package byte
    while(!IFS3bits.MI2C2IF);
    IFS3bits.MI2C2IF = 0;
    I2C2CONbits.PEN = 1;            //Stop bit
    while(I2C2CONbits.PEN);    //Wait for slave to acknowledge stop
}

void lcd_printStr(const char s[]){
    int flag = 0;
    int i = 0;
    I2C2CONbits.SEN = 1;
    while(I2C2CONbits.SEN);
    IFS3bits.MI2C2IF = 0;
    I2C2TRN = 0b01111100;
    while(!IFS3bits.MI2C2IF);
    IFS3bits.MI2C2IF = 0;
    while(!flag){
        if(s[i] != '\0'){
           I2C2TRN = 0b11000000;
           while(!IFS3bits.MI2C2IF);   
           IFS3bits.MI2C2IF = 0;
           I2C2TRN = s[i];
           while(!IFS3bits.MI2C2IF);
           IFS3bits.MI2C2IF = 0; 
           i++;
        }
        if(s[i] == '\0'){
            I2C2TRN = 0b01000000;
            while(!IFS3bits.MI2C2IF);   
            IFS3bits.MI2C2IF = 0;
            I2C2TRN = s[i];
            while(!IFS3bits.MI2C2IF);
            IFS3bits.MI2C2IF = 0;
            flag = 1;
        }     
    }
    I2C2CONbits.PEN = 1;
    while(I2C2CONbits.PEN);
}
