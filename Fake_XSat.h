#ifndef __FAKE_XSAT_H__  //__FAKE_XSAT_H__
#define __FAKE_XSAT_H__  //__FAKE_XSAT_H__

//[STD Lib Include]
#include <stdio.h>

//Must Declare in Main Code
//#define FAKE_XSAT_ON()
//#define FAKE_XSAT_OFF()

//[Typedef Declare]
typedef unsigned char   FK_XSAT_U8;        //Fake XSAT U8
typedef unsigned int    FK_XSAT_U16;       //Fake XSAT U16
typedef unsigned long   FK_XSAT_U32;       //Fake XSAT U32

typedef signed char     FK_XSAT_S8;        //Fake XSAT S8
typedef signed int      FK_XSAT_S16;       //Fake XSAT S16
typedef signed long     FK_XSAT_S32;       //Fake XSAT S32


//[Macro Declare]
#define FK_XSAT_N                 0                     //Numeric Fine-Tune
#define FK_XSAT_BASE              1                     //26.3 (1Clk)
#define FK_XSAT_CARR_CNT          20  + FK_XSAT_N       //526 / 26.3 = 20
#define FK_XSAT_L1_CNT            112 + FK_XSAT_N       //(2000-526)/26.3 = 56 * 2 = 112
#define FK_XSAT_L0_CNT            36  + FK_XSAT_N       //(1000-526)/26.3 = 18 * 2 = 36

//Start
#define FK_XSAT_CARR_START_CNT    304 + FK_XSAT_N       //8000 / 26.3 = 304
#define FK_XSAT_LOW_START_CNT     320 + FK_XSAT_N       //4000 / 26.3 = 152 * 2 = 304

//Size
#define FK_XSAT_ADDR_SIZE         8                     //Address Size
#define FK_XSAT_CMD_SIZE          8                     //Command Size


//[Struct Declare]
typedef struct _FK_XSAT
{
  FK_XSAT_U32  XSAT_Cnt;
  FK_XSAT_U16  i,j;
} FK_XSAT_t;


//XSAT Declare
#define FK_XSAT_DECLARE()       FK_XSAT_t XSAT


//XSAT Tick
#define FK_XSAT_TICK()                            \
{                                                 \
  if(XSAT.XSAT_Cnt)                               \
    XSAT.XSAT_Cnt--;                              \
}


//XSAT Delay
#define FK_XSAT_DELAY(Tick)                       \
{                                                 \
  XSAT.XSAT_Cnt = Tick;                           \
  while(XSAT.XSAT_Cnt);                           \
}


//XSAT Carrier
#define FK_XSAT_CARRIER()                         \
{                                                 \
  FK_XSAT_ON();                                   \
  FK_XSAT_DELAY(FK_XSAT_BASE);                    \
  FK_XSAT_OFF();                                  \
  FK_XSAT_DELAY(FK_XSAT_BASE);                    \
}


//XSAT Logic-0
#define FK_XSAT_LOGIC0()                          \
{                                                 \
  XSAT.i = FK_XSAT_CARR_CNT;                      \
  while(XSAT.i--)                                 \
    FK_XSAT_CARRIER();                            \
  FK_XSAT_OFF();                                  \
  FK_XSAT_DELAY(FK_XSAT_L0_CNT);                  \
}


//XSAT Logic-1
#define FK_XSAT_LOGIC1()                          \
{                                                 \
  XSAT.i = FK_XSAT_CARR_CNT;                      \
  while(XSAT.i--)                                 \
    FK_XSAT_CARRIER();                            \
  FK_XSAT_OFF();                                  \
  FK_XSAT_DELAY(FK_XSAT_L1_CNT);                  \
}


//XSAT Start
#define FK_XSAT_START()                           \
{                                                 \
  XSAT.i = FK_XSAT_CARR_START_CNT;                \
  while(XSAT.i--)                                 \
    FK_XSAT_CARRIER();                            \
  FK_XSAT_OFF();                                  \
  FK_XSAT_DELAY(FK_XSAT_LOW_START_CNT);           \
}


//XSAT Send
#define FK_XSAT_SEND(ADDR,CMD)                    \
{                                                 \
  /* Send Start (S1) [1B]*/                       \
  FK_XSAT_START();                                \
  /* Send Address [8B] */                         \
  for(XSAT.j=0;XSAT.j<FK_XSAT_ADDR_SIZE;XSAT.j++) \
  {                                               \
    if(ADDR & (1<<XSAT.j))                        \
    { FK_XSAT_LOGIC1();  }                        \
    else                                          \
    { FK_XSAT_LOGIC0();  }                        \
  }                                               \
  /* Send Carrier & Period */                     \
  XSAT.i = FK_XSAT_CARR_CNT;                      \
  while(XSAT.i--)                                 \
    FK_XSAT_CARRIER();                            \
  FK_XSAT_OFF();                                  \
  FK_XSAT_DELAY(FK_XSAT_LOW_START_CNT);           \
  /* Send Command [8B] */                         \
  for(XSAT.j=0;XSAT.j<FK_XSAT_CMD_SIZE;XSAT.j++)  \
  {                                               \
    if(CMD & (1<<XSAT.j))                         \
    { FK_XSAT_LOGIC1();  }                        \
    else                                          \
    { FK_XSAT_LOGIC0();  }                        \
  }                                               \
  /* Send Carrier */                              \
  XSAT.i = FK_XSAT_CARR_CNT;                      \
  while(XSAT.i--)                                 \
    FK_XSAT_CARRIER();                            \
  FK_XSAT_OFF();                                  \
}

#endif  //__FAKE_XSAT_H__
