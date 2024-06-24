#ifndef __FAKE_JVC_H__  //__FAKE_JVC_H__
#define __FAKE_JVC_H__  //__FAKE_JVC_H__

//[STD Lib Include]
#include <stdio.h>

//Must Declare in Main Code
//#define FAKE_JVC_ON()
//#define FAKE_JVC_OFF()

//[Typedef Declare]
typedef unsigned char   FK_JVC_U8;        //Fake JVC U8
typedef unsigned int    FK_JVC_U16;       //Fake JVC U16
typedef unsigned long   FK_JVC_U32;       //Fake JVC U32

typedef signed char     FK_JVC_S8;        //Fake JVC S8
typedef signed int      FK_JVC_S16;       //Fake JVC S16
typedef signed long     FK_JVC_S32;       //Fake JVC S32


//[Macro Declare]
#define FK_JVC_N                0                     //Numeric Fine-Tune
#define FK_JVC_BASE             1                     //26.3 (1Clk)
#define FK_JVC_CARR_CNT         20  + FK_JVC_N        //526 / 26.3 = 20
#define FK_JVC_L1_CNT           120 + FK_JVC_N        //(2110-526)/26.3 = 60 * 2 = 120
#define FK_JVC_L0_CNT           40  + FK_JVC_N        //(1050-526)/26.3 = 20 * 2 = 40

//Start
#define FK_JVC_CARR_START_CNT   320 + FK_JVC_N        //8400 / 26.3 = 320
#define FK_JVC_LOW_START_CNT    320 + FK_JVC_N        //4200 / 26.3 = 160 * 2 = 320

//Size
#define FK_JVC_ADDR_SIZE        8                     //Address Size
#define FK_JVC_CMD_SIZE         8                     //Command Size


//[Struct Declare]
typedef struct _FK_JVC
{
  FK_JVC_U32  JVC_Cnt;
  FK_JVC_U16  i,j;
} FK_JVC_t;


//JVC Declare
#define FK_JVC_DECLARE()       FK_JVC_t JVC


//JVC Tick
#define FK_JVC_TICK()                         \
{                                             \
  if(JVC.JVC_Cnt)                             \
    JVC.JVC_Cnt--;                            \
}


//JVC Delay
#define FK_JVC_DELAY(Tick)                    \
{                                             \
  JVC.JVC_Cnt = Tick;                         \
  while(JVC.JVC_Cnt);                         \
}


//JVC Carrier
#define FK_JVC_CARRIER()                      \
{                                             \
  FK_JVC_ON();                                \
  FK_JVC_DELAY(FK_JVC_BASE);                  \
  FK_JVC_OFF();                               \
  FK_JVC_DELAY(FK_JVC_BASE);                  \
}


//JVC Logic-0
#define FK_JVC_LOGIC0()                       \
{                                             \
  JVC.i = FK_JVC_CARR_CNT;                    \
  while(JVC.i--)                              \
    FK_JVC_CARRIER();                         \
  FK_JVC_OFF();                               \
  FK_JVC_DELAY(FK_JVC_L0_CNT);                \
}


//JVC Logic-1
#define FK_JVC_LOGIC1()                       \
{                                             \
  JVC.i = FK_JVC_CARR_CNT;                    \
  while(JVC.i--)                              \
    FK_JVC_CARRIER();                         \
  FK_JVC_OFF();                               \
  FK_JVC_DELAY(FK_JVC_L1_CNT);                \
}


//JVC Start
#define FK_JVC_START()                        \
{                                             \
  JVC.i = FK_JVC_CARR_START_CNT;              \
  while(JVC.i--)                              \
    FK_JVC_CARRIER();                         \
  FK_JVC_OFF();                               \
  FK_JVC_DELAY(FK_JVC_LOW_START_CNT);         \
}


//JVC Send
#define FK_JVC_SEND(ADDR,CMD)                 \
{                                             \
  /* Send Start (S1) [1B]*/                   \
  FK_JVC_START();                             \
  /* Send Address [8B] */                     \
  for(JVC.j=0;JVC.j<FK_JVC_ADDR_SIZE;JVC.j++) \
  {                                           \
    if(ADDR & (1<<JVC.j))                     \
    { FK_JVC_LOGIC1();  }                     \
    else                                      \
    { FK_JVC_LOGIC0();  }                     \
  }                                           \
  /* Send Command [8B] */                     \
  for(JVC.j=0;JVC.j<FK_JVC_CMD_SIZE;JVC.j++)  \
  {                                           \
    if(CMD & (1<<JVC.j))                      \
    { FK_JVC_LOGIC1();  }                     \
    else                                      \
    { FK_JVC_LOGIC0();  }                     \
  }                                           \
}

#endif  //__FAKE_JVC_H__
