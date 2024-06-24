#ifndef __FAKE_RC5_H__  //__FAKE_RC5_H__
#define __FAKE_RC5_H__  //__FAKE_RC5_H__

//[STD Lib Include]
#include <stdio.h>

//Must Declare in Main Code
//#define FAKE_RC5_ON()
//#define FAKE_RC5_OFF()

//[Typedef Declare]
typedef unsigned char   FK_RC5_U8;
typedef unsigned int    FK_RC5_U16;
typedef unsigned long   FK_RC5_U32;

typedef signed char     FK_RC5_S8;
typedef signed int      FK_RC5_S16;
typedef signed long     FK_RC5_S32;

//[Macro Declare]
#define FK_RC5_N              0                   //Numberic Fine-Turning
#define FK_RC5_BASE           1                   //27.7(36Khz)
#define FK_RC5_CARR_CNT       32 + FK_RC5_N       //889 / 27.7 = 32
#define FK_RC5_CNT            64 + FK_RC5_N       //889 / 27.7 = 32 * 2 = 64

//Size
#define FK_RC5_ADDR_SIZE      5                   //Address Size
#define FK_RC5_CMD_SIZE       6                   //Command Size

//[Struct Declare]
typedef struct _FK_RC5
{
  FK_RC5_U32  RC5_Cnt;
  FK_RC5_U16  i,j;
} FK_RC5_t;


//RC5 Declare
#define FK_RC5_DECLARE()       FK_RC5_t RC5

//RC5 Tick
#define FK_RC5_TICK()                   \
{                                       \
  if(RC5.RC5_Cnt)                       \
    RC5.RC5_Cnt--;                      \
}

//RC5 Delay
#define FK_RC5_DELAY(Tick)              \
{                                       \
  RC5.RC5_Cnt = Tick;                   \
  while(RC5.RC5_Cnt);                   \
}

//RC5 Carrier
#define FK_RC5_CARRIER()                \
{                                       \
  FK_RC5_ON();                          \
  FK_RC5_DELAY(FK_RC5_BASE);            \
  FK_RC5_OFF();                         \
  FK_RC5_DELAY(FK_RC5_BASE);            \
}

//RC5 Logic-0
#define FK_RC5_LOGIC0()                 \
{                                       \
  RC5.i = FK_RC5_CARR_CNT;              \
  while(RC5.i--)                        \
    FK_RC5_CARRIER();                   \
  FK_RC5_OFF();                         \
  FK_RC5_DELAY(FK_RC5_CNT);             \
}

//RC5 Logic-1
#define FK_RC5_LOGIC1()                 \
{                                       \
  FK_RC5_OFF();                         \
  FK_RC5_DELAY(FK_RC5_CNT);             \
  RC5.i = FK_RC5_CARR_CNT;              \
  while(RC5.i--)                        \
    FK_RC5_CARRIER();                   \
}

//RC5 Send
#define FK_RC5_SEND(ADDR,CMD)           \
{                                       \
  /* Send Start (S1+S2+T) [3B]*/        \
  FK_RC5_LOGIC1();                      \
  FK_RC5_LOGIC1();                      \
  FK_RC5_LOGIC0();                      \
  /* Send Address [5B] */               \
  RC5.j = FK_RC5_ADDR_SIZE;             \
  while(RC5.j--)                        \
  {                                     \
    if(ADDR &(1<<RC5.j))                \
    { FK_RC5_LOGIC1();  }               \
    else                                \
    { FK_RC5_LOGIC0();  }               \
  }                                     \
  /* Send Command [5B] */               \
  RC5.j = FK_RC5_CMD_SIZE;              \
  while(RC5.j--)                        \
  {                                     \
    if(CMD &(1<<RC5.j))                 \
    { FK_RC5_LOGIC1();  }               \
    else                                \
    { FK_RC5_LOGIC0();  }               \
  }                                     \
}

#endif  //__FAKE_RC5_H__
