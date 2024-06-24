#ifndef __FAKE_NEC_H__  //__FAKE_NEC_H__
#define __FAKE_NEC_H__  //__FAKE_NEC_H__

//[STD Lib Include]
#include <stdio.h>

//Must Declare in Main Code
//#define FAKE_NEC_ON()
//#define FAKE_NEC_OFF()

//[Typedef Declare]
typedef unsigned char   FK_NEC_U8;        //Fake NEC U8
typedef unsigned int    FK_NEC_U16;       //Fake NEC U16
typedef unsigned long   FK_NEC_U32;       //Fake NEC U32

typedef signed char     FK_NEC_S8;        //Fake NEC S8
typedef signed int      FK_NEC_S16;       //Fake NEC S16
typedef signed long     FK_NEC_S32;       //Fake NEC S32


//[Macro Declare]
#define FK_NEC_N                0                   //Numeric Fine-Tune
#define FK_NEC_BASE             1                   //26.3 (1Clk)
#define FK_NEC_CARR_CNT         21  + FK_NEC_N      //560 / 26.3 = 21
#define FK_NEC_L1_CNT           128 + FK_NEC_N      //(2250-560)/26.3 = 64 * 2 = 128
#define FK_NEC_L0_CNT           42  + FK_NEC_N      //(1120-560)/26.3 = 21 * 2 = 42

//Start
#define FK_NEC_CARR_START_CNT   342 + FK_NEC_N      //9000 / 26.3 = 342
#define FK_NEC_LOW_START_CNT    342 + FK_NEC_N      //4500 / 26.3 = 171 * 2 = 342

//Size
#define FK_NEC_ADDR_SIZE        8                   //Address Size
#define FK_NEC_CMD_SIZE         8                   //Command Size


//[Struct Declare]
typedef struct _FK_NEC
{
  FK_NEC_U32  NEC_Cnt;
  FK_NEC_U16  i,j;
} FK_NEC_t;


//NEC Declare
#define FK_NEC_DECLARE()       FK_NEC_t NEC


//NEC Tick
#define FK_NEC_TICK()                         \
{                                             \
  if(NEC.NEC_Cnt)                             \
    NEC.NEC_Cnt--;                            \
}


//NEC Delay
#define FK_NEC_DELAY(Tick)                    \
{                                             \
  NEC.NEC_Cnt = Tick;                         \
  while(NEC.NEC_Cnt);                         \
}


//NEC Carrier
#define FK_NEC_CARRIER()                      \
{                                             \
  FK_NEC_ON();                                \
  FK_NEC_DELAY(FK_NEC_BASE);                  \
  FK_NEC_OFF();                               \
  FK_NEC_DELAY(FK_NEC_BASE);                  \
}


//NEC Logic-0
#define FK_NEC_LOGIC0()                       \
{                                             \
  NEC.i = FK_NEC_CARR_CNT;                    \
  while(NEC.i--)                              \
    FK_NEC_CARRIER();                         \
  FK_NEC_OFF();                               \
  FK_NEC_DELAY(FK_NEC_L0_CNT);                \
}


//NEC Logic-1
#define FK_NEC_LOGIC1()                       \
{                                             \
  NEC.i = FK_NEC_CARR_CNT;                    \
  while(NEC.i--)                              \
    FK_NEC_CARRIER();                         \
  FK_NEC_OFF();                               \
  FK_NEC_DELAY(FK_NEC_L1_CNT);                \
}


//NEC Start
#define FK_NEC_START()                        \
{                                             \
  NEC.i = FK_NEC_CARR_START_CNT;              \
  while(NEC.i--)                              \
    FK_NEC_CARRIER();                         \
  FK_NEC_OFF();                               \
  FK_NEC_DELAY(FK_NEC_LOW_START_CNT);         \
}


//NEC Stop
#define FK_NEC_STOP()                         \
{                                             \
  NEC.i = FK_NEC_CARR_CNT;                    \
  while(NEC.i--)                              \
    FK_NEC_CARRIER();                         \
}


//NEC Send
#define FK_NEC_SEND(ADDR,CMD)                 \
{                                             \
  /* Send Start (S1) [1B]*/                   \
  FK_NEC_START();                             \
  /* Send Address [8B] */                     \
  for(NEC.j=0;NEC.j<FK_NEC_ADDR_SIZE;NEC.j++) \
  {                                           \
    if(ADDR & (1<<NEC.j))                     \
    { FK_NEC_LOGIC1();  }                     \
    else                                      \
    { FK_NEC_LOGIC0();  }                     \
  }                                           \
  /* Send Address(Reverse) [8B] */            \
  for(NEC.j=0;NEC.j<FK_NEC_ADDR_SIZE;NEC.j++) \
  {                                           \
    if(ADDR & (1<<NEC.j))                     \
    { FK_NEC_LOGIC0();  }  /* Reverse */      \
    else                                      \
    { FK_NEC_LOGIC1();  }  /* Reverse */      \
  }                                           \
  /* Send Command [8B] */                     \
  for(NEC.j=0;NEC.j<FK_NEC_CMD_SIZE;NEC.j++)  \
  {                                           \
    if(CMD & (1<<NEC.j))                      \
    { FK_NEC_LOGIC1();  }                     \
    else                                      \
    { FK_NEC_LOGIC0();  }                     \
  }                                           \
  /* Send Command(Reverse) [8B] */            \
  for(NEC.j=0;NEC.j<FK_NEC_CMD_SIZE;NEC.j++)  \
  {                                           \
    if(CMD & (1<<NEC.j))                      \
    { FK_NEC_LOGIC0();  }  /* Reverse */      \
    else                                      \
    { FK_NEC_LOGIC1();  }  /* Reverse */      \
  }                                           \
  /* Send Stpp (560us) */                     \
  FK_NEC_STOP();                              \
}


//NEC Extended Send
#define FK_NEC_EXT_SEND(HADDR,LADDR,CMD)      \
{                                             \
  /* Send Start (S1) [1B]*/                   \
  FK_NEC_START();                             \
  /* Send Address [8B] */                     \
  for(NEC.j=0;NEC.j<FK_NEC_ADDR_SIZE;NEC.j++) \
  {                                           \
    if(LADDR & (1<<NEC.j))                    \
    { FK_NEC_LOGIC1();  }                     \
    else                                      \
    { FK_NEC_LOGIC0();  }                     \
  }                                           \
  /* Send Address [8B] */                     \
  for(NEC.j=0;NEC.j<FK_NEC_ADDR_SIZE;NEC.j++) \
  {                                           \
    if(HADDR & (1<<NEC.j))                    \
    { FK_NEC_LOGIC1();  }  /* Reverse */      \
    else                                      \
    { FK_NEC_LOGIC0();  }  /* Reverse */      \
  }                                           \
  /* Send Command [8B] */                     \
  for(NEC.j=0;NEC.j<FK_NEC_CMD_SIZE;NEC.j++)  \
  {                                           \
    if(CMD & (1<<NEC.j))                      \
    { FK_NEC_LOGIC1();  }                     \
    else                                      \
    { FK_NEC_LOGIC0();  }                     \
  }                                           \
  /* Send Command(Reverse) [8B] */            \
  for(NEC.j=0;NEC.j<FK_NEC_CMD_SIZE;NEC.j++)  \
  {                                           \
    if(CMD & (1<<NEC.j))                      \
    { FK_NEC_LOGIC0();  }  /* Reverse */      \
    else                                      \
    { FK_NEC_LOGIC1();  }  /* Reverse */      \
  }                                           \
  /* Send Stpp (560us) */                     \
  FK_NEC_STOP();                              \
}
#endif  //__FAKE_NEC_H__
