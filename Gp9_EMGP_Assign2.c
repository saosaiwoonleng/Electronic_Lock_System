/*
 * File:   Gp9_EMGP_Assign2_source_G1770470U.c
 * Author: Bryan Zelig Lee Wei Ming
 *
 * Created on March 1, 2018, 9:49 PM
 */


#pragma config PLLDIV = 1       
#pragma config CPUDIV = OSC1_PLL2 S
#pragma config USBDIV = 1       

// CONFIG1H
#pragma config FOSC = INTOSCIO_EC 
#pragma config FCMEN = OFF      
#pragma config IESO = OFF       

// CONFIG2L
#pragma config PWRT = OFF       
#pragma config BOR = ON         
#pragma config BORV = 3         
#pragma config VREGEN = OFF     

// CONFIG2H
#pragma config WDT = OFF        
#pragma config WDTPS = 32768    

// CONFIG3H
#pragma config CCP2MX = ON      
#pragma config PBADEN = ON      
#pragma config LPT1OSC = OFF    
#pragma config MCLRE = ON       

// CONFIG4L
#pragma config STVREN = ON      
#pragma config LVP = OFF        
#pragma config ICPRT = OFF      
#pragma config XINST = OFF     

// CONFIG5L
#pragma config CP0 = OFF        
#pragma config CP1 = OFF        
#pragma config CP2 = OFF        
#pragma config CP3 = OFF        

// CONFIG5H
#pragma config CPB = OFF        
#pragma config CPD = OFF        

// CONFIG6L
#pragma config WRT0 = OFF       
#pragma config WRT1 = OFF       
#pragma config WRT2 = OFF       
#pragma config WRT3 = OFF       

// CONFIG6H
#pragma config WRTC = OFF       
#pragma config WRTB = OFF       
 #pragma config WRTD = OFF       

// CONFIG7L
#pragma config EBTR0 = OFF      
#pragma config EBTR1 = OFF      
#pragma config EBTR2 = OFF      
#pragma config EBTR3 = OFF      

// CONFIG7H
#pragma config EBTRB = OFF      

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
TXSTAbits.TX9 = 0;          
TXSTAbits.TXEN = 1;         
TXSTAbits.SYNC = 0;         
TXSTAbits.BRGH = 1;         
RCSTAbits.SPEN = 1;        
RCSTAbits.RX9 = 0;          
RCSTAbits.CREN = 1;         
BAUDCONbits.BRG16 = 0;      
SPBRG = 25;                 
int i=0,stage=0,keyflag=0,tries=3;

unsigned char ReadData;
char x;
char pswd[3]="123";                // tyinging password
char org_pswd[3]="200";           // main password 
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
     if((PIR1bits.RCIF == 0)&&(PIR1bits.TXIF == 1)) 
        {
            while(PIR1bits.RCIF==0&&!C1&&!C2&&!C3); // wait until, get a trigger from Keypad / Bluetooth 
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
            while(PIR1bits.RCIF==1)     
                ReadData=RCREG;

            while(keyflag==0)           
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
        for(int j=0;j<3;j++)
{
            if(pswd[j]!=org_pswd[j])
                b=0;
            }
        if(b==1)
        {   // if password is correct
            RED=0;GREEN=1;BLUE=0;  
           for(i=0;i<14;i++)
           {
            while(PIR1bits.TXIF == 0);
            if(Relay)           
              TXREG = ulk[i];   
            else
              TXREG = lck[i];   
           }
            __delay_ms(1000);   
            RED=0;GREEN=0;
            for(i=0;i<5;i++)    
            {
                BLUE=~BLUE;
                __delay_ms(200);
            }
            Relay=~Relay;       
            tries=3;
            stage=0;            
        }
        else    // If password is wrong
        {
            tries--;
            RED=1;GREEN=0;BLUE=0;   
                if(Relay){
                     for(i=0;i<26;i++){            
                     while(PIR1bits.TXIF == 0);
                     TXREG = unlock[i];
                        }
                    }
                else{
                     for(i=20;i<42;i++){            
                        while(PIR1bits.TXIF == 0);
                        TXREG = lock[i];
                        }
                    }
                if(tries==2)
                    {
                    for(i=0;i<14;i++){            
                    while(PIR1bits.TXIF == 0);
                    TXREG = snd[i];
                    }
                    }
                if(tries==1)
                    {
                    for(i=0;i<14;i++){            
                    while(PIR1bits.TXIF == 0);
                    TXREG = lst[i];
                    }
                    }            
            stage=3;    
        }
    }
else if(stage==3)       
    {
    if(tries<1)       
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
    else        
        {
            i=0;keyflag=0;
            stage=1;
        }
    }
else if(stage==4)       
    {
        for(i=0;i<21;i++){            
            while(PIR1bits.TXIF == 0);
            TXREG = new[i];
        }
        stage=5;i=0;keyflag=0;
    }
else if(stage==5)       
    {
     keyboard_Inti();
        while(PIR1bits.TXIF == 0);  // send the input from keypad to bluetooth module
        if((PIR1bits.RCIF == 0)&&(PIR1bits.TXIF == 1)) 
        {
            while(PIR1bits.RCIF==0&&!C1&&!C2&&!C3); 
            if(PIR1bits.RCIF==1)        
            {
            if(i<3)
               new_pswd[i]=RCREG;
                i++;
                keyflag=1;
                if(i>2){
                    stage=6;keyflag=3;
                    }
            }
        
            while(keyflag==0)           
                {
               char y=GetKeyboard();
                 if(y!=0xff)
                    {
                    new_pswd[i]=y;
                    i++;
                    while(PIR1bits.RCIF==1)
                        x=RCREG;
                    while(PIR1bits.TXIF == 0);  
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
        for(i=0;i<23;i++){                      
            while(PIR1bits.TXIF == 0);
            TXREG = cfm[i];
        }
        stage=6;i=0;keyflag=0;
        }
    }
else if(stage==6)       
    {
     keyboard_Inti();
        while(PIR1bits.TXIF == 0);  
        
     if((PIR1bits.RCIF == 0)&&(PIR1bits.TXIF == 1)) 
        {
            while(PIR1bits.RCIF==0&&!C1&&!C2&&!C3);
            if(PIR1bits.RCIF==1)        
            {
            if(i<3)
               cfm_pswd[i]=RCREG;
                i++;

                keyflag=1;
                if(i>2){
                    stage=7;keyflag=3;}
                }

            while(keyflag==0)           
                {
               char y=GetKeyboard();
                 if(y!=0xff)
                    {
                    cfm_pswd[i]=y;
                    i++;
                    while(PIR1bits.RCIF==1)
                        x=RCREG;
                    while(PIR1bits.TXIF == 0);  
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
        if(keyflag=3)       
            {
            int b=1;
            for(int j=0;j<3;j++)
{
            if(new_pswd[j]!=cfm_pswd[j])
                b=0;
            }
        
        if(b==1)        
            {
            for(int j=0;j<3;j++)
{
                org_pswd[j]=new_pswd[j];  
            }
            for(i=0;i<16;i++){                      
                while(PIR1bits.TXIF == 0);
                TXREG = udt[i];
            }
            tries=3;stage=0;
        }
        else
        {
            for(i=0;i<29;i++){                      
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
	int segment[]={63,6,91,79,102,109,125,7,127,103,0};  	
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

void beep(void)                 
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


