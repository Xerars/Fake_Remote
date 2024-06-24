# Fake Remote

---

Fake Remote is a Lightweight GPIO to Simulate IR Remote Library Written in C Language.

All Libraries of Fake Series are Simulated by GPIO. Similar Libraries with Prefix will be Released in the Future

## Feature

* LightWeight (Only One Header File per Mode)

* Use Macro Function API Instead of Function API

* Already Verified on STC8051 in Order to Make this Function Library more Efficient in Transplanting on other MCUs, Since each Instructions take Longer Time in 8051 Instruction Set.

* Macro function is used to eliminate both Overhead of Entering and Leaving Function

## Usage

1. You need to Include the IR Remote Header First

```c
#include "Fake_XXX.h"
```

2. Need to Implement GPIO On/Off Function (I Use Macro Function Method)

```c
#define FK_XXX_ON()                     P00 = 1
#define FK_XXX_OFF()                    P00 = 0
```

3. Variables need to be Defined. Each Lib has been Defined and can be Called Directly.

```c
FK_XXX_DECLARE();
```

4. Basic Timer Unit and Timer Interrupt Service Routine need to be Defined.
   
   Basic Timer Unit needs to Use Carrier Time as the Basic Time , and the Data needs to be Defined is Display on the Lib.

```c
void TMR0_ISR(void) interrupt 1 using 1
{
    //P00 = !P00;
    FK_XXX_TICK();
}
```

5. The Last, You can Put the Code You need to Send Anywhere.

```c
FK_XXX_SEND(0x59,0x16);
```

6. The Control Time may not be Accurate. You can Adjust the N-Value to make Slight Adjustments.

```c
#define FK_XXX_N        0
```

## Example

```c
//[STC Lib Include]
#include "STC8A8K64D4.h"

//[Fake Lib Include]
#include "Fake_NEC.h"

//Test Parameter:
//Chip                  : STC8A8K64D4
//Input IRC Frequency   : 40Mhz
//Instruction Time      : (1/40M) * 12 = 300ns

//[Prototype Declare]
void GPIO_Init(void);
void TMR0_Init(void);
void TMR1_Init(void);

//[ISR Declare]
void TMR0_ISR(void);
void TMR1_ISR(void);

//[Macro Function Declare]
#define FK_NEC_ON()                     P00 = 1
#define FK_NEC_OFF()                    P00 = 0

//[Variable Declare]
unsigned long Count = 0;
FK_NEC_DECLARE();

//Main Code
void main(void)
{
    GPIO_Init();
    TMR0_Init();
    TMR1_Init();
    EA  = 1;                //Enable Global Interrupt

    FK_NEC_SEND(0x59,0x16);
    FK_NEC_DELAY(100)
    FK_NEC_EXT_SEND(0xAA,0x59,0x16);
    while(1)
    {}
}

///@Function: TMR0_ISR
///@Descript: TMR0 Interrupt Service Routine
void TMR0_ISR(void) interrupt 1 using 1
{
    FK_NEC_TICK();
}

///@Function: TMR1_ISR
///@Descript: TMR1 Interrupt Service Routine
void TMR1_ISR(void) interrupt 3 using 1
{    
    P01 = !P01;
}

///@Function: TMR0_Init
///@Descript: TMR0 Initial
void TMR0_Init(void)
{
    TMOD |= 0x00;         //Timer0/1 Mode
    //f = 11.0592Mhz
    //Formula : 65536 - 11.0592M / 12 /1000
    TL0 = 0xCE;
    TH0 = 0xFF;

    TR0 = 1;              //Timer0 Enable
    ET0 = 1;              //Enable Timer0 Interrupt
}

///@Function: TMR1_Init
///@Descript: TMR1 Initial
void TMR1_Init(void)
{
    TMOD |= 0x00;        //Timer0/1 Mode
    //f = 11.0592Mhz
    //Formula : 65536 - 11.0592M / 12 /1000
    TL1 = 0xCE;          //1:500ns        CC:36K
    TH1 = 0xFF;

    TR1 = 1;            //Timer1 Enable
    ET1 = 1;            //Enable Timer1 Interrupt
}

///@Function: GPIO_Init
///@Descript: GPIO Initial
void GPIO_Init(void)
{
    //Set P0-P7 (Push Pull)
    P0M0 = 0xFF;        //1111-1111
    P0M1 = 0x00;        //0000-0000
    //Set Initial Value
    P00 = 0;
    P01 = 0;
}
```

### Tests

`Fake Remote Library Function` Currently Verified on STC8051 since it Requires 12 Cycle for 8051 Instruction to Place an Instruction; Therefore I believe `Fake  Remote Library Function` can be Tranplanted on Other Platform much Easily if it no Problem with 8051

| IRC   | Instructions Time | ~10u Executed Instructions | Test Result |
| ----- | ----------------- | -------------------------- | ----------- |
| 24Mhz | 500ns             | 20                         | `PASS`      |
| 40Mhz | 300ns             | 33                         | `PASS`      |
| 45Mhz | 260ns             | 38                         | `PASS`      |

#### License

[The MIT License (MIT)](http://opensource.org/licenses/mit-license.php)
