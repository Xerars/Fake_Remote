//[STC Lib Include]
#include "STC8A8K64D4.h"

//[Fake Lib Include]
#include "Fake_NEC.h"

//Test Parameter:
//Chip 								: STC8A8K64D4
//Input IRC Frequency : 40Mhz
//Instruction Time 		: (1/40M) * 12 = 300ns


//[Prototype Declare]
void GPIO_Init(void);
void TMR0_Init(void);
void TMR1_Init(void);


//[ISR Declare]
void TMR0_ISR(void);
void TMR1_ISR(void);


//[Macro Function Declare]
#define FK_NEC_ON()						P00 = 1
#define FK_NEC_OFF()					P00 = 0


//[Variable Declare]
unsigned long Count = 0;
FK_NEC_DECLARE();


//Main Code
void main(void)
{
	GPIO_Init();
	TMR0_Init();
	TMR1_Init();
	EA  = 1;				//Enable Global Interrupt

	FK_NEC_SEND(0x59,0x16);
	FK_NEC_DELAY(100)
	FK_NEC_EXT_SEND(0xAA,0x59,0x16);
	while(1)
	{}
}



///@Function: 
///@Descript:
void TMR0_ISR(void) interrupt 1 using 1
{
	//P00 = !P00;
	FK_NEC_TICK();
}



///@Function: 
///@Descript: 
void TMR1_ISR(void) interrupt 3 using 1
{	
	P01 = !P01;
}

///@Function: 
///@Descript: 
void TMR0_Init(void)
{
	TMOD |= 0x00;		//Timer0/1 Mode
	
	//f = 11.0592Mhz
	//Formula : 65536 - 11.0592M / 12 /1000
	TL0 = 0xCE;
	TH0 = 0xFF;
	
	TR0 = 1;				//Timer0 Enable
	ET0 = 1;				//Enable Timer0 Interrupt
}
	

///@Function: 
///@Descript: 
void TMR1_Init(void)
{
	TMOD |= 0x00;		//Timer0/1 Mode

	//f = 11.0592Mhz
	//Formula : 65536 - 11.0592M / 12 /1000
	TL1 = 0xCE;			//1:500ns		CC:36K
	TH1 = 0xFF;
	
	TR1 = 1;				//Timer1 Enable
	ET1 = 1;				//Enable Timer1 Interrupt
}


///@Function: 
///@Descript: 
void GPIO_Init(void)
{
	//Set P0-P7 (Push Pull)
	P0M0 = 0xFF;		//1111-1111
	P0M1 = 0x00;		//0000-0000

	//Set Initial Value
	P00 = 0;
	P01 = 0;
}
