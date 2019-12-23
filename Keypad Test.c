/*
 * File:   .c
 * Author: Bryan Zelig Lee Wei Ming
 *
 * Created on March 2, 2018, 10:58 PM
 */

#pragma config PLLDIV = 1       // PLL Prescaler Selection bits (No prescale (4 MHz oscillator input drives PLL directly))
#pragma config CPUDIV = OSC1_PLL2// System Clock Postscaler Selection bits ([Primary Oscillator Src: /1][96 MHz PLL Src: /2])
#pragma config USBDIV = 1       // USB Clock Selection bit (used in Full-Speed USB mode only; UCFG:FSEN = 1) (USB clock source comes directly from the primary oscillator block with no postscale)

// CONFIG1H
#pragma config FOSC = INTOSCIO_EC// Oscillator Selection bits (Internal oscillator, port function on RA6, EC used by USB (INTIO))
#pragma config FCMEN = OFF      // Fail-Safe Clock Monitor Enable bit (Fail-Safe Clock Monitor disabled)
#pragma config IESO = OFF       // Internal/External Oscillator Switchover bit (Oscillator Switchover mode disabled)

// CONFIG2L
#pragma config PWRT = OFF       // Power-up Timer Enable bit (PWRT disabled)
#pragma config BOR = ON         // Brown-out Reset Enable bits (Brown-out Reset enabled in hardware only (SBOREN is disabled))
#pragma config BORV = 3         // Brown-out Reset Voltage bits (Minimum setting 2.05V)
#pragma config VREGEN = OFF     // USB Voltage Regulator Enable bit (USB voltage regulator disabled)

// CONFIG2H
#pragma config WDT = OFF        // Watchdog Timer Enable bit (WDT disabled (control is placed on the SWDTEN bit))
#pragma config WDTPS = 32768    // Watchdog Timer Postscale Select bits (1:32768)

// CONFIG3H
#pragma config CCP2MX = ON      // CCP2 MUX bit (CCP2 input/output is multiplexed with RC1)
#pragma config PBADEN = ON      // PORTB A/D Enable bit (PORTB<4:0> pins are configured as analog input channels on Reset)
#pragma config LPT1OSC = OFF    // Low-Power Timer 1 Oscillator Enable bit (Timer1 configured for higher power operation)
#pragma config MCLRE = ON       // MCLR Pin Enable bit (MCLR pin enabled; RE3 input pin disabled)

// CONFIG4L
#pragma config STVREN = ON      // Stack Full/Underflow Reset Enable bit (Stack full/underflow will cause Reset)
#pragma config LVP = OFF        // Single-Supply ICSP Enable bit (Single-Supply ICSP disabled)
#pragma config ICPRT = OFF      // Dedicated In-Circuit Debug/Programming Port (ICPORT) Enable bit (ICPORT disabled)
#pragma config XINST = OFF      // Extended Instruction Set Enable bit (Instruction set extension and Indexed Addressing mode disabled (Legacy mode))

// CONFIG5L
#pragma config CP0 = OFF        // Code Protection bit (Block 0 (000800-001FFFh) is not code-protected)
#pragma config CP1 = OFF        // Code Protection bit (Block 1 (002000-003FFFh) is not code-protected)
#pragma config CP2 = OFF        // Code Protection bit (Block 2 (004000-005FFFh) is not code-protected)
#pragma config CP3 = OFF        // Code Protection bit (Block 3 (006000-007FFFh) is not code-protected)

// CONFIG5H
#pragma config CPB = OFF        // Boot Block Code Protection bit (Boot block (000000-0007FFh) is not code-protected)
#pragma config CPD = OFF        // Data EEPROM Code Protection bit (Data EEPROM is not code-protected)

// CONFIG6L
#pragma config WRT0 = OFF       // Write Protection bit (Block 0 (000800-001FFFh) is not write-protected)
#pragma config WRT1 = OFF       // Write Protection bit (Block 1 (002000-003FFFh) is not write-protected)
#pragma config WRT2 = OFF       // Write Protection bit (Block 2 (004000-005FFFh) is not write-protected)
#pragma config WRT3 = OFF       // Write Protection bit (Block 3 (006000-007FFFh) is not write-protected)

// CONFIG6H
#pragma config WRTC = OFF       // Configuration Register Write Protection bit (Configuration registers (300000-3000FFh) are not write-protected)
#pragma config WRTB = OFF       // Boot Block Write Protection bit (Boot block (000000-0007FFh) is not write-protected)
#pragma config WRTD = OFF       // Data EEPROM Write Protection bit (Data EEPROM is not write-protected)

// CONFIG7L
#pragma config EBTR0 = OFF      // Table Read Protection bit (Block 0 (000800-001FFFh) is not protected from table reads executed in other blocks)
#pragma config EBTR1 = OFF      // Table Read Protection bit (Block 1 (002000-003FFFh) is not protected from table reads executed in other blocks)
#pragma config EBTR2 = OFF      // Table Read Protection bit (Block 2 (004000-005FFFh) is not protected from table reads executed in other blocks)
#pragma config EBTR3 = OFF      // Table Read Protection bit (Block 3 (006000-007FFFh) is not protected from table reads executed in other blocks)

// CONFIG7H
#pragma config EBTRB = OFF      // Boot Block Table Read Protection bit (Boot block (000000-0007FFh) is not protected from table reads executed in other blocks)

#include <xc.h>
#include <pic18f4550.h>
#include<string.h>

#define _XTAL_FREQ 4000000

#define Buzzer PORTDbits.RD7
#define Relay  PORTAbits.RA0

#define RED PORTEbits.RE2
#define GREEN PORTEbits.RE1
#define BLUE PORTEbits.RE0

#define R1 LATBbits.LATB0
#define R2 LATBbits.LATB1
#define R3 LATBbits.LATB2
#define R4 LATBbits.LATB3

#define C1 PORTBbits.RB5
#define C2 PORTBbits.RB6
#define C3 PORTBbits.RB7

char const keyPadMatrix[]=
{
    '1','2','3',
    '4','5','6',
    '7','8','9',
    '*','0','#',
    0xff
};


char old_key=0;
void keyboard_Inti();
int GetKeyboard();

void _7Segment(int);
void beep(void);
void main ()
{
    ADCON1=0x0f;
    TRISA=0b00000000;
    keyboard_Inti();
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
char x;
while(1)
{
//    for(int j=0;j<10;j++){
//_7Segment(j);
//__delay_ms(1000);
//    }

//  if(R1&&C1)
//      _7Segment(1);
 x=GetKeyboard();
 if(x!=0xff){
     
         while(PIR1bits.RCIF==1)
        Rdata=RCREG;
         while(PIR1bits.TXIF == 0);
         TXREG = x;       
 }
 
}

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

void _7Segment(int x)
{	
	int segment[]={63,6,91,79,102,109,125,7,127,103};  	// Decimal no corresponding to the no pattern in binary
			PORTD=segment[x];			
}


void keyboard_Inti()
{

    TRISB=0xff;
    PORTB=0x00;
    LATB=0x00;
    TRISB=0xf0;
    PORTB=0x0f;

}

int GetKeyboard(){
    char key=0,row;
    for(row=0b00000001;row<0b00010000;row<<=1)
    {
        {
        R1=(row & 0x0001)>>0;
        R2=(row & 0x0002)>>1;
        R3=(row & 0x0004)>>2;
        R4=(row & 0x0008)>>3;
        __delay_ms(1);
        }
        
        if(C1)break;key++;
        if(C2)break;key++;
        if(C3)break;key++;
    }
    R1=0;R2=0;R3=0;R4=0;
    if(key!=old_key)
    {
    old_key=key; 
        return keyPadMatrix[key];
    }
    else
        return keyPadMatrix[0x0C];
}