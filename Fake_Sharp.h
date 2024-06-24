#ifndef __FAKE_SHARP_H__  //__FAKE_SHARP_H__
#define __FAKE_SHARP_H__  //__FAKE_SHARP_H__

//[STD Lib Include]
#include <stdio.h>

//Must Declare in Main Code
//#define FAKE_SHARP_ON()
//#define FAKE_SHARP_OFF()

//[Typedef Declare]
typedef unsigned char   FK_SHARP_U8;        //Fake SHARP U8
typedef unsigned int    FK_SHARP_U16;       //Fake SHARP U16
typedef unsigned long   FK_SHARP_U32;       //Fake SHARP U32

typedef signed char     FK_SHARP_S8;        //Fake SHARP S8
typedef signed int      FK_SHARP_S16;       //Fake SHARP S16
typedef signed long     FK_SHARP_S32;       //Fake SHARP S32


//[Macro Declare]
#define FK_SHARP_N                0                     //Numeric Fine-Tune
#define FK_SHARP_BASE             1                     //26.3 (1Clk)
#define FK_SHARP_CARR_CNT         20  + FK_SHARP_N      //320 / 26.3 = 12
#define FK_SHARP_L1_CNT           128 + FK_SHARP_N      //(2000-320)/26.3 = 64 * 2 = 128
#define FK_SHARP_L0_CNT           52  + FK_SHARP_N      //(1000-320)/26.3 = 26 * 2 = 52


//Size
#define FK_SHARP_ADDR_SIZE        5                     //Address Size
#define FK_SHARP_CMD_SIZE         8                     //Command Size
#define FK_SHARP_EXP_SIZE         1                     //Expend Size
#define FK_SHARP_CHK_SIZE         1                     //Check Size

//[Struct Declare]
typedef struct _FK_SHARP
{
  FK_SHARP_U32  SHARP_Cnt;
  FK_SHARP_U16  i,j;
} FK_SHARP_t;


//SHARP Declare
#define FK_SHARP_DECLARE()       FK_SHARP_t SHARP


//SHARP Tick
#define FK_SHARP_TICK()                               \
{                                                     \
  if(SHARP.SHARP_Cnt)                                 \
    SHARP.SHARP_Cnt--;                                \
}


//SHARP Delay
#define FK_SHARP_DELAY(Tick)                          \
{                                                     \
  SHARP.SHARP_Cnt = Tick;                             \
  while(SHARP.SHARP_Cnt);                             \
}


//SHARP Carrier
#define FK_SHARP_CARRIER()                            \
{                                                     \
  FK_SHARP_ON();                                      \
  FK_SHARP_DELAY(FK_SHARP_BASE);                      \
  FK_SHARP_OFF();                                     \
  FK_SHARP_DELAY(FK_SHARP_BASE);                      \
}


//SHARP Logic-0
#define FK_SHARP_LOGIC0()                             \
{                                                     \
  SHARP.i = FK_SHARP_CARR_CNT;                        \
  while(SHARP.i--)                                    \
    FK_SHARP_CARRIER();                               \
  FK_SHARP_OFF();                                     \
  FK_SHARP_DELAY(FK_SHARP_L0_CNT);                    \
}


//SHARP Logic-1
#define FK_SHARP_LOGIC1()                             \
{                                                     \
  SHARP.i = FK_SHARP_CARR_CNT;                        \
  while(SHARP.i--)                                    \
    FK_SHARP_CARRIER();                               \
  FK_SHARP_OFF();                                     \
  FK_SHARP_DELAY(FK_SHARP_L1_CNT);                    \
}


//SHARP Send
#define FK_SHARP_SEND(ADDR,CMD,EXP,CHK)               \
{                                                     \
  /* Send Address [5B] */                             \
  for(SHARP.j=0;SHARP.j<FK_SHARP_ADDR_SIZE;SHARP.j++) \
  {                                                   \
    if(ADDR & (1<<SHARP.j))                           \
    { FK_SHARP_LOGIC1();  }                           \
    else                                              \
    { FK_SHARP_LOGIC0();  }                           \
  }                                                   \
  /* Send Command [8B] */                             \
  for(SHARP.j=0;SHARP.j<FK_SHARP_CMD_SIZE;SHARP.j++)  \
  {                                                   \
    if(CMD & (1<<SHARP.j))                            \
    { FK_SHARP_LOGIC1();  }                           \
    else                                              \
    { FK_SHARP_LOGIC0();  }                           \
  }                                                   \
  /* Send Expend [1B] */                              \
  if(EXP) { FK_SHARP_LOGIC1();  }                     \
  else    { FK_SHARP_LOGIC0();  }                     \
  /* Send Check [1B]*/                                \
  if(CHK) { FK_SHARP_LOGIC1();  }                     \
  else    { FK_SHARP_LOGIC0();  }                     \
}

#endif  //__FAKE_SHARP_H__
