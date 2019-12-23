
#include <xc.h>
#include <pic18f4550.h>
#include<string.h>

#define _XTAL_FREQ 4000000

#define Buzzer PORTDbits.RD7
#define Relay  PORTAbits.RA0

#define RED PORTEbits.RE2
#define GREEN PORTEbits.RE1
#define BLUE PORTEbits.RE0


void _7Segment(int);

void main ()
{
    ADCON1=0x0f;
    TRISA=0b00000000;
    TRISB=0b11111111;
    TRISC=0b00000001;
    TRISD=0b00000000;
    TRISE=0b00000000;

while(1)
{

    for(int j=0;j<10;j++){
_7Segment(j);
__delay_ms(1000);
    }
}
    
}


void _7Segment(int x)
{	
	int segment[]={63,6,91,79,102,109,125,7,127,103};  	// Decimal no corresponding to the no pattern in binary
			PORTD=segment[x];			
}

