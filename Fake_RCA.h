#ifndef __FAKE_RCA_H__  //__FAKE_RCA_H__
#define __FAKE_RCA_H__  //__FAKE_RCA_H__

//[STD Lib Include]
#include <stdio.h>

//Must Declare in Main Code
//#define FAKE_RCA_ON()
//#define FAKE_RCA_OFF()

//[Typedef Declare]
typedef unsigned char   FK_RCA_U8;        //Fake RCA U8
typedef unsigned int    FK_RCA_U16;       //Fake RCA U16
typedef unsigned long   FK_RCA_U32;       //Fake RCA U32

typedef signed char     FK_RCA_S8;        //Fake RCA S8
typedef signed int      FK_RCA_S16;       //Fake RCA S16
typedef signed long     FK_RCA_S32;       //Fake RCA S32


//[Macro Declare]
#define FK_RCA_N                0                   //Numeric Fine-Tune
#define FK_RCA_BASE             1                   //17.8 (1Clk)
#define FK_RCA_CARR_CNT         28  + FK_RCA_N      //500 / 17.8 = 28
#define FK_RCA_L1_CNT           224 + FK_RCA_N      //(2500-500)/17.8 = 112 * 2 = 224
#define FK_RCA_L0_CNT           112 + FK_RCA_N      //(1500-500)/17.8 = 56 * 2 = 112

//Start
#define FK_RCA_CARR_START_CNT   225                 //4000 / 17.8 = 225
#define FK_RCA_LOW_START_CNT    450                 //4000 / 17.8 = 225 * 2 = 450

#define FK_RCA_ADDR_SIZE        4                   //Address Size
#define FK_RCA_CMD_SIZE         8                   //Command Size


//[Struct Declare]
typedef struct _FK_RCA
{
  FK_RCA_U32  RCA_Cnt;
  FK_RCA_U16  i,j;
} FK_RCA_t;


//RCA Declare
#define FK_RCA_DECLARE()       FK_RCA_t RCA


//RCA Tick
#define FK_RCA_TICK()                         \
{                                             \
  if(RCA.RCA_Cnt)                             \
    RCA.RCA_Cnt--;                            \
}


//RCA Delay
#define FK_RCA_DELAY(Tick)                    \
{                                             \
  RCA.RCA_Cnt = Tick;                         \
  while(RCA.RCA_Cnt);                         \
}


//RCA Carrier
#define FK_RCA_CARRIER()                      \
{                                             \
  FK_RCA_ON();                                \
  FK_RCA_DELAY(FK_RCA_BASE);                  \
  FK_RCA_OFF();                               \
  FK_RCA_DELAY(FK_RCA_BASE);                  \
}


//RCA Logic-0
#define FK_RCA_LOGIC0()                       \
{                                             \
  RCA.i = FK_RCA_CARR_CNT;                    \
  while(RCA.i--)                              \
    FK_RCA_CARRIER();                         \
  FK_RCA_OFF();                               \
  FK_RCA_DELAY(FK_RCA_L0_CNT);                \
}


//RCA Logic-1
#define FK_RCA_LOGIC1()                       \
{                                             \
  RCA.i = FK_RCA_CARR_CNT;                    \
  while(RCA.i--)                              \
    FK_RCA_CARRIER();                         \
  FK_RCA_OFF();                               \
  FK_RCA_DELAY(FK_RCA_L1_CNT);                \
}


//RCA Start
#define FK_RCA_START()                        \
{                                             \
  RCA.i = FK_RCA_CARR_START_CNT;              \
  while(RCA.i--)                              \
    FK_RCA_CARRIER();                         \
  FK_RCA_OFF();                               \
  FK_RCA_DELAY(FK_RCA_LOW_START_CNT);         \
}


//RCA Send
#define FK_RCA_SEND(ADDR,CMD)                 \
{                                             \
  /* Send Start (S1) [1B]*/                   \
  FK_RCA_START();                             \
  /* Send Address [4B] */                     \
  RCA.j = FK_RCA_ADDR_SIZE;                   \
  while(RCA.j--)                              \
  {                                           \
    if(ADDR &(1<<RCA.j))                      \
    { FK_RCA_LOGIC1();  }                     \
    else                                      \
    { FK_RCA_LOGIC0();  }                     \
  }                                           \
  /* Send Command [8B] */                     \
  RCA.j = FK_RCA_CMD_SIZE;                    \
  while(RCA.j--)                              \
  {                                           \
    if(CMD &(1<<RCA.j))                       \
    { FK_RCA_LOGIC1();  }                     \
    else                                      \
    { FK_RCA_LOGIC0();  }                     \
  }                                           \
  /* Send Address [4B] */                     \
  RCA.j = FK_RCA_ADDR_SIZE;                   \
  while(RCA.j--)                              \
  {                                           \
    if(ADDR &(1<<RCA.j))                      \
    { FK_RCA_LOGIC0();  }                     \
    else                                      \
    { FK_RCA_LOGIC1();  }                     \
  }                                           \
  /* Send Command [8B] */                     \
  RCA.j = FK_RCA_CMD_SIZE;                    \
  while(RCA.j--)                              \
  {                                           \
    if(CMD &(1<<RCA.j))                       \
    { FK_RCA_LOGIC0();  }                     \
    else                                      \
    { FK_RCA_LOGIC1();  }                     \
  }                                           \
  FK_RCA_LOGIC1();                            \
}

#endif  //__FAKE_RCA_H__
