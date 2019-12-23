/*
 * File:   PSB.c
 * Created on March 2, 2018, 10:00 PM
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

void keyboard_Inti();   // to initialize the keypad
int GetKeyboard();      // get char from keypad
void _7Segment(int);    // to print number on 7Segment
void beep(void);        // for beep sound
void Alarm(void);       // for alarm sound
void main ()
{
    ADCON1=0x0f;
    TRISA=0b00000000;
    keyboard_Inti();
    TRISC=0b00000001;
    TRISD=0b00000000;
    TRISE=0b00000000;
    
    char wel[]="\nWELCOME TO ELECTRONIC LOCKING SYSTEM\n";     // Messages to display
    char wrg[]="\nWRONG PASSWORD";
    char ulk[]="\n! UNLOCKED !";
    char lck[]="\n!! LOCKED !!";
    char try[]="\nPLEASE ENTER LOCK/UNLOCK PASSWORD : ";
    char alr[]="\n ALARM WARNING !!!";
    char lock[]="\nPRESS * TO RESET / ENTER PASSWORD TO LOCK :";
    char unlock[]="\nENTER PASSWORD TO UNLOCK :";
    char snd[]="(SECOND TRY) :";
    char lst[]="(LAST  TRY)  :";
    char new[]="\nENTER NEW PASSWORD :";
    char cfm[]="\nCONFIRM NEW PASSWORD :";
    char mis[]="\nPASSWORD MISMATCH, TRY AGAIN";
    char udt[]="\nPASSWORD UPDATED";
OSCCONbits.IRCF0 = 0;
OSCCONbits.IRCF1 = 1;
OSCCONbits.IRCF2 = 1;

// USART configuration
TXSTAbits.TX9 = 0;          // 8-bit transmission
TXSTAbits.TXEN = 1;         // transmit enabled
TXSTAbits.SYNC = 0;         // asynchronous mode
TXSTAbits.BRGH = 1;         // High speed Baud rate
RCSTAbits.SPEN = 1;         // enable serial port (configures RX/DT and TX/CK pins as serial port pins)
RCSTAbits.RX9 = 0;          // 8-bit reception
RCSTAbits.CREN = 1;         // enable receiver
BAUDCONbits.BRG16 = 0;      // 8-bit baud rate generator
SPBRG = 25;                 // "64" calculated using formula
int i=0,stage=0,keyflag=0,tries=3;

unsigned char ReadData;
char x;
char pswd[3]="123";                // tyinging password
char org_pswd[3]="200";           // main password **************** important **************8
char new_pswd[3]="000";           // new password
char cfm_pswd[3]="000";           // confirm password

RED=BLUE=GREEN=0;               // off all LEDs
_7Segment(10);                   // Off 7 segment
Relay=0;                        // off the relay
__delay_ms(1000);

while(1)                        // continues loop 
{
while(PIR1bits.RCIF==1)
    ReadData=RCREG;
if(stage==0)
    {
    RED=0;GREEN=0;BLUE=0;   // off all 3 LEDS
    
      for(i=0;i<37;i++){                // print welcome msg
         while(PIR1bits.TXIF == 0);
         TXREG = wel[i];
            }
     
    if(Relay){
         for(i=0;i<27;i++){            // enter password msg to Unlock
         while(PIR1bits.TXIF == 0);
         TXREG = unlock[i];
            }
        }
    else{
         for(i=0;i<42;i++){            // enter password msg to lock
            while(PIR1bits.TXIF == 0);
            TXREG = lock[i];
            }
        }
            stage=1;i=0;keyflag=0; 
    }
else if(stage==1)
    {
      keyboard_Inti();
     if((PIR1bits.RCIF == 0)&&(PIR1bits.TXIF == 1)) // Receiver buffer is empty && Transmitter buffer is empty 
        {
            while(PIR1bits.RCIF==0&&!C1&&!C2&&!C3);// wait until, get a trigger from Keypad / Bluetooth 
            if(PIR1bits.RCIF==1)        // trigger/input from keypad
            {
            if(i<3)
               pswd[i]=RCREG;
                i++;
                keyflag=1;
                if(i>2)
                    stage=2;
                if(pswd[0]=='*')
                    stage=4;
            }
            while(PIR1bits.RCIF==1)     // clear if any extra inputs from bluetooth
                ReadData=RCREG;

            while(keyflag==0)           // input from keypad
                {
               char y=GetKeyboard();
                 if(y!=0xff)
                    {
                    pswd[i]=y;
                    i++;
                    while(PIR1bits.RCIF==1)
                        x=RCREG;
                    while(PIR1bits.TXIF == 0);  // send the input from keypad to bluetooth module
                        TXREG = pswd[i-1];
                    _7Segment(y-'0');
                    beep();
                    if(pswd[0]=='*'){
                        stage=4;keyflag=3;
                    }
                    }
                if(i>2){
                    keyflag=3;stage=2;}
                }
        }
    }
else if(stage==2)
    {
    int b=1;
        for(int j=0;j<3;j++){// cross check the password
            if(pswd[j]!=org_pswd[j])
                b=0;
            }
        if(b==1)
        {   // if password is correct
            RED=0;GREEN=1;BLUE=0;  // on green led if password is correct
           for(i=0;i<14;i++)
           {
            while(PIR1bits.TXIF == 0);
            if(Relay)           
              TXREG = ulk[i];   // send unlock message
            else
              TXREG = lck[i];   // send lock message
           }
            __delay_ms(1000);   
            RED=0;GREEN=0;
            for(i=0;i<5;i++)    // Indicate Blue LED when locking / unlocking
            {
                BLUE=~BLUE;
                __delay_ms(200);
            }
            Relay=~Relay;       // Toggle Th Relay value LOCK<->UNLOCK
            tries=3;
            stage=0;            // Goto Stage 0/ first Stage
        }
        else    // If password is wrong
        {
            tries--;
            RED=1;GREEN=0;BLUE=0;   // On RED LED when password is wrong
                if(Relay){
                     for(i=0;i<26;i++){            // enter password msg to Unlock
                     while(PIR1bits.TXIF == 0);
                     TXREG = unlock[i];
                        }
                    }
                else{
                     for(i=20;i<42;i++){            // enter password msg to lock
                        while(PIR1bits.TXIF == 0);
                        TXREG = lock[i];
                        }
                    }
                if(tries==2)
                    {
                    for(i=0;i<14;i++){            // enter password msg to lock
                    while(PIR1bits.TXIF == 0);
                    TXREG = snd[i];
                    }
                    }
                if(tries==1)
                    {
                    for(i=0;i<14;i++){            // enter password msg to lock
                    while(PIR1bits.TXIF == 0);
                    TXREG = lst[i];
                    }
                    }            
            stage=3;    // stage 5 will give retry
        }
    }
else if(stage==3)       // for retry
    {
    if(tries<1)       // all tries are wrong
        {
        for(int j=0;j<5;j++){
            for(i=0;i<16;i++)
            {
                while(PIR1bits.TXIF == 0);
                TXREG = alr[i];
            }
            Alarm();
            __delay_ms(1000);
        }
        tries=3;
        stage=0;
        }
    else        // for try one more time
        {
            i=0;keyflag=0;
            stage=1;
        }
    }
else if(stage==4)       // for enter new password msg
    {
        for(i=0;i<21;i++){            // enter password msg to lock
            while(PIR1bits.TXIF == 0);
            TXREG = new[i];
        }
        stage=5;i=0;keyflag=0;
    }
else if(stage==5)       // for enter new password
    {
     keyboard_Inti();
        while(PIR1bits.TXIF == 0);  // send the input from keypad to bluetooth module
        if((PIR1bits.RCIF == 0)&&(PIR1bits.TXIF == 1)) // Receiver buffer is empty && Transmitter buffer is empty 
        {
            while(PIR1bits.RCIF==0&&!C1&&!C2&&!C3);// wait until, get a trigger from Keypad / Bluetooth 
            if(PIR1bits.RCIF==1)        // trigger/input from keypad
            {
            if(i<3)
               new_pswd[i]=RCREG;
                i++;
                keyflag=1;
                if(i>2){
                    stage=6;keyflag=3;
                    }
            }
        
            while(keyflag==0)           // input from keypad
                {
               char y=GetKeyboard();
                 if(y!=0xff)
                    {
                    new_pswd[i]=y;
                    i++;
                    while(PIR1bits.RCIF==1)
                        x=RCREG;
                    while(PIR1bits.TXIF == 0);  // send the input from keypad to bluetooth module
                        TXREG = new_pswd[i-1];
                    _7Segment(y-'0');
                    beep();
                    }
                if(i>2){
                    keyflag=3;stage=6;}
                }
            
        }
        if(keyflag==3)
        {
        for(i=0;i<23;i++){                      // msg to confirm new password
            while(PIR1bits.TXIF == 0);
            TXREG = cfm[i];
        }
        stage=6;i=0;keyflag=0;
        }
    }
else if(stage==6)       // to confirm new password
    {
     keyboard_Inti();
        while(PIR1bits.TXIF == 0);  // send the input from keypad to bluetooth module
        
     if((PIR1bits.RCIF == 0)&&(PIR1bits.TXIF == 1)) // Receiver buffer is empty && Transmitter buffer is empty 
        {
            while(PIR1bits.RCIF==0&&!C1&&!C2&&!C3);// wait until, get a trigger from Keypad / Bluetooth 
            if(PIR1bits.RCIF==1)        // trigger/input from keypad
            {
            if(i<3)
               cfm_pswd[i]=RCREG;
                i++;

                keyflag=1;
                if(i>2){
                    stage=7;keyflag=3;}
                }

            while(keyflag==0)           // input from keypad
                {
               char y=GetKeyboard();
                 if(y!=0xff)
                    {
                    cfm_pswd[i]=y;
                    i++;
                    while(PIR1bits.RCIF==1)
                        x=RCREG;
                    while(PIR1bits.TXIF == 0);  // send the input from keypad to bluetooth module
                        TXREG = cfm_pswd[i-1];
                    _7Segment(y-'0');
                    beep();
                    }
                if(i>2){
                    keyflag=3;stage=7;}
                }
        }
    }
else
    {
        if(keyflag=3)       // verify the password
            {
            int b=1;
            for(int j=0;j<3;j++){// cross check the password
            if(new_pswd[j]!=cfm_pswd[j])
                b=0;
            }
        
        if(b==1)        // if password is correct
            {
            for(int j=0;j<3;j++){// cross check the password
                org_pswd[j]=new_pswd[j];  
            }
            for(i=0;i<16;i++){                      // msg to confirm new password
                while(PIR1bits.TXIF == 0);
                TXREG = udt[i];
            }
            tries=3;stage=0;
        }
        else
        {
            for(i=0;i<29;i++){                      // msg to confirm new password
                while(PIR1bits.TXIF == 0);
                TXREG = mis[i];
            }
            stage=4;
        }
        }
    }   // end of else
}   // End of while(1)
}   // End of Main())



void _7Segment(int x)
{	
	int segment[]={63,6,91,79,102,109,125,7,127,103,0};  	// Decimal no corresponding to the no pattern in binary
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

void beep(void)                 // Function to make beep sound
	{
    TRISD=0b00000000;
	for(int i=0;i<100;i++)
	{
	Buzzer=~Buzzer;
    __delay_us(200);
	}
    TRISD=0b10000000;
	}

void Alarm(void)
	{
    TRISD=0b00000000;
	for(int i=0;i<100;i++)
	{
	Buzzer=~Buzzer;
	__delay_us(200);
	}
            __delay_ms(10);
   for(int i=0;i<100;i++)
	{
	Buzzer=~Buzzer;
	__delay_us(300);
	}
             __delay_ms(10);
    for(int i=0;i<100;i++)
	{
	Buzzer=~Buzzer;
    __delay_us(400);
	}
            __delay_ms(10);
  for(int i=0;i<100;i++)
	{ 
	Buzzer=~Buzzer;
	__delay_us(400);
	}
    RED=~RED;     
    TRISD=0b10000000;
	}
