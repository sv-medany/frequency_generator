#include <reg51.h>
sbit c3=P1^7;
sbit c2=P1^6;
sbit c1=P1^5;
sbit c0=P1^4;
sbit r3=P1^3;
sbit r2=P1^2;
sbit r1=P1^1;
sbit r0=P1^0;
unsigned char begin;
int x;
int count;
int n;
float numfreq;  
int overflow; // count exceeding cycles
float remainder;  
int ovtemp; //to store overflow
sbit out=P3^7; //for outupt as req
float clone=0.0;

//defining where each column and row of keypad is connected
void debounce(){
	unsigned char i =200;
	unsigned char j=200;
	unsigned char k=100;
	for(;i>0;i--)
		for(;j>0;j--)
			for(;k>0;k--)
	;
}                            
void delay_4ms(void) //refresh rate for sev seg
{
	TMOD &= 0xF8; // maintain state of timer 1 and reinitialize timer 0
    TMOD |= 0x01; // timer 0 mode 1 
    TH0 = 0xF1;
    TL0 = 0x99;
    //TH0 AND TL0 CALCULATED AS LECTURES x=4ms / 1.085 micro then 65536 - x , convert to hexa 
    // Start Timer0
    TR0 = 1;
    while (TF0 == 0); //polling
    // Clear Timer0 overflow flag and stop Timer0
    TF0 = 0;
    TR0 = 0;
}
unsigned char Keypad(){
	 static unsigned char Old_Key;
    unsigned char Key=0xFF;
    r0=0;
    if(c2==0) {Key=3;}
    if(c1==0) {Key=2;}
    if(c0==0) {Key=1;}
    r0=1;
    r1=0;
    if(c2==0) {Key=6;}
    if(c1==0) {Key=5;}
    if(c0==0) {Key=4;}
    r1=1;
    r2=0;
    if(c2==0) {Key=9;}
    if(c1==0) {Key=8;}
    if(c0==0) {Key=7;}
    r2=1;
    r3=0;
    if(c1==0) {Key=10;}
    r3=1;
     debounce(); //as mentioned in youtube video we need debounce to anticipate the extra press
    if (Key!=0xFF) {
        if(Key==Old_Key) {
            return 0;
        } else {
            Old_Key=Key;
            return Key;
        }
    }
    Old_Key=0xFF;
    return 0;
}
sbit enable0 = P0^0;  // Enable pin for digit 0
sbit enable1 = P0^1;  // Enable pin for digit 1
sbit enable2 = P0^2;  // Enable pin for digit 2
sbit enable3 = P0^3;  // Enable pin for digit 3
unsigned char digits[]={0xC0,0xF9,0xA4,0xB0,0x99,0x92,0x82,0xF8,0x80,0x98}; //digit patterns to interface seven segment (atafy eh w anwr eh) 
void intdis(unsigned char digit) {
    P2 = digits[digit];  // Set data lines to display the digit
}

void button_initialize(void) {
		EA = 1; //global interrupts
    IT0 = 1; // edge triggered
    EX0 = 1; // Enable external interrupt 0
}
void timer1_initialize(void)
{
		TMOD &= 0x0F; //maintain state of timer 0 and reset timer 1
    TMOD |= 0x10;   //timer 1 mode 1
		if(begin==1){
		out=~out;	
		begin=0;
		}
		TH1 = (unsigned char) ((n>>8)&0xFF ) ;      // n is global consider the case 0xFF89 if we shift to the left 8 bits it will be 0x00FF , so unsinged char is one byte so it will take FF
    TL1 = (unsigned char) (n&0x00FF);	// as it will already take the lower byte only 0xFF89 , it will take 89 only (no need for shifts)
    ET1 = 1;        // Interrupt for timer 1 
}

void button_isr() interrupt 0 { // interrupt 0 written in reference to ivector table ext0 
  static unsigned char flag = 1; // consider the first case where we will enter the number from keypad then press button to confirm (then we will alternate)
	if (flag){
		clone=(float)x;
		//the following is to handle errors in freq generation (inner porcessing)
		if(x>=750 && x<1500){ 
			clone=clone+(0.0789*clone);
		}
		else if(x>=1500 && x <2500){
			clone=clone+(0.122*clone);
			
		}
			else if(x>=2500 && x <3300){
			
			clone=clone+(0.195*clone);
		}
			else if(x>=3300 && x<4000){
			clone=clone+(0.3*clone);
		}
			else if(x>=4000 && x<4750){
			clone=clone+(0.4*clone);
		}
			else if(x>=4750 && x<5500){
			clone=clone+(0.5*clone);
		}
		else if(x>=5500 && x<6500){
			clone=clone+(0.6*clone);
		}
		else if(x>=6500 && x<7000){
			clone=clone+(0.8*clone);
		}
		else if(x>=7000 && x<8100){
			clone=clone+(1.2*clone);
		}
		else if(x>=8100 && x<8800){
			clone=clone+(1.3*clone);
			
		}
		else if(x>=8800 && x<9200){
			clone=clone+(1.5*clone);
			
		}
		else if(x>=9200 && x<=9999){
			clone=clone+(1.75*clone);
		}
		numfreq= (float)(1/(2*(float)clone)); 
		numfreq=(numfreq*1000000)/1.085; // assume division on 1.085 * 10^-6
		overflow=(int)(numfreq/65536);   //10.3-10  
		remainder=(numfreq/65536)-overflow; //to know value to put in timer for remainder (not full cycle)
		overflow++; //code puproses 
		ovtemp = overflow; //store to keep executing continous frequency
		n=remainder*65536; //calculate value that has to be stored in n
		n=65536-n; //value to put in timer
		timer1_initialize();  //10.3
		flag =0; }
	else{
		x=0;
		flag=1;
	}
	TR1 = ~TR1; // Start Timer 1
	debounce();	
}

void timer1_ISR(void) interrupt 3
{
	  overflow--; 

	if(overflow==0){ //consider the case we only need one cycle
		out=~out;
		overflow=ovtemp;
		TH1 = (unsigned char) ((n>>8)&0xFF ) ;      // n is global consider the case 0xFF89 if we shift to the left 8 bits it will be 0x00FF , so unsinged char is one byte so it will take FF
    TL1 = (unsigned char) (n&0x00FF);	// as it will already take the lower byte only 0xFF89 , it will take 89 only (no need for shifts)
	}
	else{ //case we need more than one cycle
		TH1=0;
		TL1=0;
	}
	
}

void display_number(int x){
				intdis(x % 10);  //ones
				enable0 = 0; //stop enable to display digit (so we can see it )
				delay_4ms(); //refresh rate
				enable0 = 1; // return interrupt to 1
				intdis((x/10) % 10);  //tens
				enable1 = 0;
				delay_4ms();
				enable1 = 1;
				intdis((x/100) % 10); 
				enable2 = 0;
				delay_4ms();
				enable2 = 1;   
				intdis((x/1000) % 10); 
				enable3 = 0;
				delay_4ms();
				enable3 = 1;
}

void main(){
	unsigned char keyval;
	
	begin=1;
	button_initialize();
	out=0;
  P1=0xF0; // Set port 1 upper nibble as input for columns and lower nibble as output for rows
	r0=1; r1=1;	r2=1;r3=1;
	

	//SerialSetup();

    while(1){
       keyval = Keypad(); // Scan for a key press
        if(keyval) { // If a key is keyval
					if(keyval == 10) 
						x=(x%1000)*10;	
					else
						x=(x%1000)*10+keyval;
				}
				display_number(x);
        }
}