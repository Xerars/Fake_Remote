#ifndef __FAKE_RC6_H__  //__FAKE_RC6_H__
#define __FAKE_RC6_H__  //__FAKE_RC6_H__

//[STD Lib Include]
#include <stdio.h>

//Must Declare in Main Code
//#define FAKE_RC6_ON()
//#define FAKE_RC6_OFF()

//[Typedef Declare]
typedef unsigned char   FK_RC6_U8;    //Fake RC6 U8
typedef unsigned int    FK_RC6_U16;   //Fake RC6 U16
typedef unsigned long   FK_RC6_U32;   //Fake RC6 U32

typedef signed char     FK_RC6_S8;    //Fake RC6 S8
typedef signed int      FK_RC6_S16;   //Fake RC6 S16
typedef signed long     FK_RC6_S32;   //Fake RC6 S32

//[Macro Declare]
#define FK_RC6_N              0                   //Numeric Fine-Tune
#define FK_RC6_BASE           1                   //27.7(36Khz)
//Data
#define FK_RC6_CARR_DATA_CNT  16 + FK_RC6_N       //444.4/ 27.7 = 16
#define FK_RC6_DATA_CNT       32 + FK_RC6_N       //444.4/ 27.7 = 16 * 2 = 32

//Toggle
#define FK_RC6_CAR_TOG_CNT    32 + FK_RC6_N       //889  / 27.7 = 32
#define FK_RC6_CNT            64 + FK_RC6_N       //889  / 27.7 = 32 * 2 = 64

//Start
#define FK_RC6_CARR_START_CNT 96  + FK_RC6_N      //2666 / 27.7 / = 96
#define FK_RC6_START_CNT      192 + FK_RC6_N      //2666 / 27.7 = 96 * 2 = 192

//Size
#define FK_RC6_FIELD_SIZE     3                   //Field Size
#define FK_RC6_ADDR_SIZE      8                   //Address Size
#define FK_RC6_CMD_SIZE       8                   //Command Size

//[Struct Declare]
typedef struct _FK_RC6
{
  FK_RC6_U32  RC6_Cnt;
  FK_RC6_U16  i,j;
} FK_RC6_t;


//RC6 Declare
#define FK_RC6_DECLARE()       FK_RC6_t RC6

//RC6 Tick
#define FK_RC6_TICK()                         \
{                                             \
  if(RC6.RC6_Cnt)                             \
    RC6.RC6_Cnt--;                            \
}

//RC6 Delay
#define FK_RC6_DELAY(Tick)                    \
{                                             \
  RC6.RC6_Cnt = Tick;                         \
  while(RC6.RC6_Cnt);                         \
}

//RC6 Carrier
#define FK_RC6_CARRIER()                      \
{                                             \
  FK_RC6_ON();                                \
  FK_RC6_DELAY(FK_RC6_BASE);                  \
  FK_RC6_OFF();                               \
  FK_RC6_DELAY(FK_RC6_BASE);                  \
}

//RC6 Logic-0
#define FK_RC6_LOGIC0()                       \
{                                             \
  FK_RC6_OFF();                               \
  FK_RC6_DELAY(FK_RC6_DATA_CNT);              \
  RC6.i = FK_RC6_CARR_DATA_CNT;               \
  while(RC6.i--)                              \
    FK_RC6_CARRIER();                         \
}

//RC6 Logic-1
#define FK_RC6_LOGIC1()                       \
{                                             \
  RC6.i = FK_RC6_CARR_DATA_CNT;               \
  while(RC6.i--)                              \
    FK_RC6_CARRIER();                         \
  FK_RC6_OFF();                               \
  FK_RC6_DELAY(FK_RC6_DATA_CNT);              \
}


//RC6 Start
#define FK_RC6_START()                        \
{                                             \
  /* S1 */                                    \
  RC6.i = FK_RC6_CARR_START_CNT;              \
  while(RC6.i--)                              \
    FK_RC6_CARRIER();                         \
  FK_RC6_OFF();                               \
  FK_RC6_DELAY(FK_RC6_CNT);                   \
  /* T */                                     \
  FK_RC6_LOGIC1();                            \
}

//RC6 Field
#define FK_RC6_FIELD(MODE)                    \
{                                             \
  /* Send Address [3B] */                     \
  RC6.j = FK_RC6_FIELD_SIZE;                  \
  while(RC6.j--)                              \
  {                                           \
    if(MODE &(1<<RC6.j))                      \
    { FK_RC6_LOGIC1();  }                     \
    else                                      \
    { FK_RC6_LOGIC0();  }                     \
  }                                           \
}

//RC6 Toggle-0
#define FK_RC6_TOGGLE0()                      \
{                                             \
  FK_RC6_OFF();                               \
  FK_RC6_DELAY(FK_RC6_CNT);                   \
  RC6.i = FK_RC6_CAR_TOG_CNT;                 \
  while(RC6.i--)                              \
    FK_RC6_CARRIER();                         \
}

//RC6 Toggle-1
#define FK_RC6_TOGGLE1()                      \
{                                             \
  RC6.i = FK_RC6_CAR_TOG_CNT;                 \
  while(RC6.i--)                              \
    FK_RC6_CARRIER();                         \
  FK_RC6_OFF();                               \
  FK_RC6_DELAY(FK_RC6_CNT);                   \
}

//RC6 Toggle
#define FK_RC6_TOGGLE(TOG)                    \
{                                             \
  if(TOG)                                     \
    { FK_RC6_TOGGLE1(); }                     \
  else                                        \
    { FK_RC6_TOGGLE0(); }                     \
}

//RC6 Send
#define FK_RC6_SEND(MODE,TOG,ADDR,CMD)        \
{                                             \
  /* Send Start (S1+T) [2B] */                \
  FK_RC6_START();                             \
  /* Send Field [3B] */                       \
  FK_RC6_FIELD(MODE);                         \
  /* Send Toggle[1B] */                       \
  FK_RC6_TOGGLE(TOG)                          \
  /* Send Address [8B] */                     \
  RC6.j = FK_RC6_ADDR_SIZE;                   \
  while(RC6.j--)                              \
  {                                           \
    if(ADDR &(1<<RC6.j))                      \
    { FK_RC6_LOGIC1();  }                     \
    else                                      \
    { FK_RC6_LOGIC0();  }                     \
  }                                           \
  /* Send Command [8B] */                     \
  RC6.j = FK_RC6_CMD_SIZE;                    \
  while(RC6.j--)                              \
  {                                           \
    if(CMD &(1<<RC6.j))                       \
    { FK_RC6_LOGIC1();  }                     \
    else                                      \
    { FK_RC6_LOGIC0();  }                     \
  }                                           \
}

#endif  //__FAKE_RC6_H__
