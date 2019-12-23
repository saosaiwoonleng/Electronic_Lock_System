
#include <xc.h>
#include <pic18f4550.h>
#include<string.h>

#define _XTAL_FREQ 4000000

#define Buzzer PORTDbits.RD7
#define Relay  PORTAbits.RA0

#define RED PORTEbits.RE2
#define GREEN PORTEbits.RE1
#define BLUE PORTEbits.RE0




void beep(void);
void main ()
{
    ADCON1=0x0f;
    TRISA=0b00000000;
    TRISB=0b11111111;
    TRISC=0b00000001;
    TRISD=0b00000000;
    TRISE=0b00000000;
    
    
    
 while(1){   
    RED=1;BLUE=0;GREEN=0; 
    beep();
    Relay=1;
    __delay_ms(2000);
        RED=0;BLUE=1;GREEN=0; 
        beep();Relay=0;
        
    __delay_ms(2000);
        RED=0;BLUE=0;GREEN=1;
        beep();
        Relay=1;
        __delay_ms(2000);

        RED=0;BLUE=1;GREEN=0;
        beep();
        Relay=0;
        __delay_ms(2000);    
}
    
}
    

void beep(void)
	{
	for(int i=0;i<100;i++)
	{
	Buzzer=~Buzzer;
    __delay_us(200);
	}
	}
