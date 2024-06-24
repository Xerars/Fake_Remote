#ifndef __FAKE_NRC17_H__  //__FAKE_NRC17_H__
#define __FAKE_NRC17_H__  //__FAKE_NRC17_H__

//[STD Lib Include]
#include <stdio.h>

//Must Declare in Main Code
//#define FAKE_NRC_ON()
//#define FAKE_NRC_OFF()

//[Typedef Declare]
typedef unsigned char   FK_NRC_U8;        //Fake NRC U8
typedef unsigned int    FK_NRC_U16;       //Fake NRC U16
typedef unsigned long   FK_NRC_U32;       //Fake NRC U32

typedef signed char     FK_NRC_S8;        //Fake NRC S8
typedef signed int      FK_NRC_S16;       //Fake NRC S16
typedef signed long     FK_NRC_S32;       //Fake NRC S32


//[Macro Declare]
#define FK_NRC_N                0
#define FK_NRC_BASE             1                   //26.3 (1Clk)
#define FK_NRC_CARR_CNT         19  + FK_NRC_N      //500 / 26.3 = 19
#define FK_NRC_CNT              38  + FK_NRC_N      //500 / 26.3 = 19 * 2 = 38

//Start
#define FK_NRC_CARR_START_CNT   19  + FK_NRC_N      //500 / 26.3 = 19
#define FK_NRC_LOW_START_CNT    190 + FK_NRC_N      //2500 / 26.3 = 95 * 2 = 190

//Size
#define FK_NRC_CMD_SIZE         8                   //Command Size
#define FK_NRC_ADDR_SIZE        4                   //Address Size
#define FK_NRC_SUB_SIZE         4                   //Sub Code Size


//[Struct Declare]
typedef struct _FK_NRC
{
  FK_NRC_U32  NRC_Cnt;
  FK_NRC_U16  i,j;
} FK_NRC_t;


//NRC Declare
#define FK_NRC_DECLARE()       FK_NRC_t NRC


//NRC Tick
#define FK_NRC_TICK()                         \
{                                             \
  if(NRC.NRC_Cnt)                             \
    NRC.NRC_Cnt--;                            \
}


//NRC Delay
#define FK_NRC_DELAY(Tick)                    \
{                                             \
  NRC.NRC_Cnt = Tick;                         \
  while(NRC.NRC_Cnt);                         \
}


//NRC Carrier
#define FK_NRC_CARRIER()                      \
{                                             \
  FK_NRC_ON();                                \
  FK_NRC_DELAY(FK_NRC_BASE);                  \
  FK_NRC_OFF();                               \
  FK_NRC_DELAY(FK_NRC_BASE);                  \
}


//NRC Logic-0
#define FK_NRC_LOGIC0()                       \
{                                             \
  FK_NRC_OFF();                               \
  FK_NRC_DELAY(FK_NRC_CNT);                   \
  NRC.i = FK_NRC_CARR_CNT;                    \
  while(NRC.i--)                              \
    FK_NRC_CARRIER();                         \
}


//NRC Logic-1
#define FK_NRC_LOGIC1()                       \
{                                             \
  NRC.i = FK_NRC_CARR_CNT;                    \
  while(NRC.i--)                              \
    FK_NRC_CARRIER();                         \
  FK_NRC_OFF();                               \
  FK_NRC_DELAY(FK_NRC_CNT);                   \
}


//NRC Start
#define FK_NRC_START()                        \
{                                             \
  /* Send Start */                            \
  NRC.i = FK_NRC_CARR_START_CNT;              \
  while(NRC.i--)                              \
    FK_NRC_CARRIER();                         \
  FK_NRC_OFF();                               \
  FK_NRC_DELAY(FK_NRC_LOW_START_CNT);         \
  /* Send Start Bit*/                         \
  FK_NRC_LOGIC1();                            \
}


//NRC Send
#define FK_NRC_SEND(CMD,ADDR,SUBCODE)         \
{                                             \
  /* Send Start (S1) [1B]*/                   \
  FK_NRC_START();                             \
  /* Send Command [8B] */                     \
  for(NRC.j=0;NRC.j<FK_NRC_CMD_SIZE;NRC.j++)  \
  {                                           \
    if(CMD & (1<<NRC.j))                      \
    { FK_NRC_LOGIC1();  }                     \
    else                                      \
    { FK_NRC_LOGIC0();  }                     \
  }                                           \
  /* Send Address [4B] */                     \
  for(NRC.j=0;NRC.j<FK_NRC_ADDR_SIZE;NRC.j++) \
  {                                           \
    if(ADDR & (1<<NRC.j))                     \
    { FK_NRC_LOGIC1();  }                     \
    else                                      \
    { FK_NRC_LOGIC0();  }                     \
  }                                           \
  /* Send SubCode [4B] */                     \
  for(NRC.j=0;NRC.j<FK_NRC_SUB_SIZE;NRC.j++)  \
  {                                           \
    if(SUBCODE & (1<<NRC.j))                  \
    { FK_NRC_LOGIC1();  }                     \
    else                                      \
    { FK_NRC_LOGIC0();  }                     \
  }                                           \
}

#endif  //__FAKE_NRC17_H__
