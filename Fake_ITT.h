#ifndef __FAKE_ITT_H__  //__FAKE_ITT_H__
#define __FAKE_ITT_H__  //__FAKE_ITT_H__

//[STD Lib Include]
#include <stdio.h>

//Must Declare in Main Code
//#define FAKE_ITT_ON()
//#define FAKE_ITT_OFF()

//[Typedef Declare]
typedef unsigned char   FK_ITT_U8;        //Fake ITT U8
typedef unsigned int    FK_ITT_U16;       //Fake ITT U16
typedef unsigned long   FK_ITT_U32;       //Fake ITT U32

typedef signed char     FK_ITT_S8;        //Fake ITT S8
typedef signed int      FK_ITT_S16;       //Fake ITT S16
typedef signed long     FK_ITT_S32;       //Fake ITT S32


//[Macro Declare]
#define FK_ITT_N                0                     //Numeric Fine-Tune
#define FK_ITT_BASE             1                     //None
#define FK_ITT_CARR_CNT         1  + FK_ITT_N         //None
#define FK_ITT_L0_CNT           10 + FK_ITT_N         //(100-10)/10 = 10
#define FK_ITT_L1_CNT           20 + FK_ITT_N         //(200-10)/10 = 20

//Start
#define FK_ITT_LOW_START_CNT    30 + FK_ITT_N         //300 / 10 = 30

//Stop
#define FK_ITT_LOW_STOP_CNT     36 + FK_ITT_N         //360 / 10 = 36

//Size
#define FK_ITT_ADDR_SIZE        4                     //Address Size
#define FK_ITT_CMD_SIZE         6                     //Command Size


//[Struct Declare]
typedef struct _FK_ITT
{
  FK_ITT_U32  ITT_Cnt;
  FK_ITT_U16  i,j;
} FK_ITT_t;


//ITT Declare
#define FK_ITT_DECLARE()       FK_ITT_t ITT


//ITT Tick
#define FK_ITT_TICK()                         \
{                                             \
  if(ITT.ITT_Cnt)                             \
    ITT.ITT_Cnt--;                            \
}


//ITT Delay
#define FK_ITT_DELAY(Tick)                    \
{                                             \
  ITT.ITT_Cnt = Tick;                         \
  while(ITT.ITT_Cnt);                         \
}


//ITT Carrier
#define FK_ITT_CARRIER()                      \
{                                             \
  FK_ITT_ON();                                \
  FK_ITT_DELAY(FK_ITT_BASE);                  \
  FK_ITT_OFF();                               \
  FK_ITT_DELAY(FK_ITT_BASE);                  \
}


//ITT Logic-0
#define FK_ITT_LOGIC0()                       \
{                                             \
  FK_ITT_CARRIER();                           \
  FK_ITT_OFF();                               \
  FK_ITT_DELAY(FK_ITT_L0_CNT);                \
}


//ITT Logic-1
#define FK_ITT_LOGIC1()                       \
{                                             \
  FK_ITT_CARRIER();                           \
  FK_ITT_OFF();                               \
  FK_ITT_DELAY(FK_ITT_L1_CNT);                \
}


//ITT Start
#define FK_ITT_START()                        \
{                                             \
  FK_ITT_CARRIER();                           \
  FK_ITT_DELAY(FK_ITT_LOW_START_CNT);         \
  FK_ITT_LOGIC0();                            \
}

//ITT Stop
#define FK_ITT_STOP()                         \
{                                             \
  FK_ITT_CARRIER();                           \
  FK_ITT_DELAY(FK_ITT_LOW_STOP_CNT);          \
  FK_ITT_LOGIC0();                            \
}

//ITT Send
#define FK_ITT_SEND(ADDR,CMD)                 \
{                                             \
  /* Send Start (S1) [1B]*/                   \
  FK_ITT_START();                             \
  /* Send Address [4B] */                     \
  for(ITT.j=0;ITT.j<FK_ITT_ADDR_SIZE;ITT.j++) \
  {                                           \
    if(ADDR & (1<<ITT.j))                     \
    { FK_ITT_LOGIC1();  }                     \
    else                                      \
    { FK_ITT_LOGIC0();  }                     \
  }                                           \
  /* Send Command [6B] */                     \
  for(ITT.j=0;ITT.j<FK_ITT_CMD_SIZE;ITT.j++)  \
  {                                           \
    if(CMD & (1<<ITT.j))                      \
    { FK_ITT_LOGIC1();  }                     \
    else                                      \
    { FK_ITT_LOGIC0();  }                     \
  }                                           \
  /* Send Stop */                             \
  FK_ITT_STOP();                              \
}

#endif  //__FAKE_ITT_H__
