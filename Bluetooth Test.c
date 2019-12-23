

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
    char te[]="\nENTER PIN TO UNLOCK SYSTEM :";
     
OSCCONbits.IRCF0 = 0;
OSCCONbits.IRCF1 = 1;
OSCCONbits.IRCF2 = 1;
//GIE=1;

// USART configuration
TXSTAbits.TX9 = 0; // 8-bit transmission
TXSTAbits.TXEN = 1; // transmit enabled
TXSTAbits.SYNC = 0; // asynchronous mode
TXSTAbits.BRGH = 1; // High speed Baud rate
RCSTAbits.SPEN = 1; // enable serial port (configures RX/DT and TX/CK pins as serial port pins)
RCSTAbits.RX9 = 0; // 8-bit reception
RCSTAbits.CREN = 1; // enable receiver
BAUDCONbits.BRG16 = 0; // 8-bit baud rate generator
SPBRG = 25; // "64" calculated using formula
int rcreg_flag=0,count=0,step=0,n_tyms=3,is_open=0,loop=0,i=0;
unsigned char Rdata;
  while(1)
{

    while(PIR1bits.RCIF==1)
        Rdata=RCREG;
         while(PIR1bits.TXIF == 0);
         TXREG = te[i];  
         i++;
         
        if(i>29)
     {i=0;__delay_ms(2000);
     }
  }

}
    

