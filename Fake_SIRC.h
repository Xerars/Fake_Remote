#ifndef __FAKE_SIRC_H__  //__FAKE_SIRC_H__
#define __FAKE_SIRC_H__  //__FAKE_SIRC_H__

//[STD Lib Include]
#include <stdio.h>

//Must Declare in Main Code
//#define FAKE_SIRC_ON()
//#define FAKE_SIRC_OFF()

//[Typedef Declare]
typedef unsigned char   FK_SIRC_U8;        //Fake SIRC U8
typedef unsigned int    FK_SIRC_U16;       //Fake SIRC U16
typedef unsigned long   FK_SIRC_U32;       //Fake SIRC U32

typedef signed char     FK_SIRC_S8;        //Fake SIRC S8
typedef signed int      FK_SIRC_S16;       //Fake SIRC S16
typedef signed long     FK_SIRC_S32;       //Fake SIRC S32


//[Macro Declare]
#define FK_SIRC_N                0                      //Numeric Fine-Tune
#define FK_SIRC_BASE             1                      //25 (1Clk)
#define FK_SIRC_CARR_L_CNT       24 + FK_SIRC_N         //600 / 25 = 24
#define FL_SIRC_CARR_H_CNT       48 + FK_SIRC_N         //1200 / 25 = 48
#define FK_SIRC_L_CNT            48 + FK_SIRC_N         //600 / 25 = 24 * 2 = 48

//Start
#define FK_SIRC_CARR_START_CNT   96 + FK_SIRC_N         //2400 / 25 = 96
#define FK_SIRC_LOW_START_CNT    48 + FK_SIRC_N         //600 / 25 = 24 * 2 = 48

#define FK_SIRC_ADDR_SIZE        7                      //Address Size
#define FK_SIRC_CMD_SIZE         5                      //Command Size

//[Enum Declare]
typedef enum _FK_SIRC_TYPE
{
  SIRC_TYPE_12 = 0,       //12 (7 Cmd + 5 Addr)
  SIRC_TYPE_15 = 1,       //15 (7 Cmd + 8 Addr)
  SIRC_TYPE_20 = 2,       //20 (7 Cmd + 5 Addr + 8 Extend)
} FK_SIRC_TYPE_E;

//[Struct Declare]
typedef struct _FK_SIRC
{
  FK_SIRC_U32  SIRC_Cnt;
  FK_SIRC_U16  i,j;
} FK_SIRC_t;


//SIRC Declare
#define FK_SIRC_DECLARE()       FK_SIRC_t SIRC


//SIRC Tick
#define FK_SIRC_TICK()                            \
{                                                 \
  if(SIRC.SIRC_Cnt)                               \
    SIRC.SIRC_Cnt--;                              \
}


//SIRC Delay
#define FK_SIRC_DELAY(Tick)                       \
{                                                 \
  SIRC.SIRC_Cnt = Tick;                           \
  while(SIRC.SIRC_Cnt);                           \
}


//SIRC Carrier
#define FK_SIRC_CARRIER()                         \
{                                                 \
  FK_SIRC_ON();                                   \
  FK_SIRC_DELAY(FK_SIRC_BASE);                    \
  FK_SIRC_OFF();                                  \
  FK_SIRC_DELAY(FK_SIRC_BASE);                    \
}


//SIRC Logic-0
#define FK_SIRC_LOGIC0()                          \
{                                                 \
  SIRC.i = FK_SIRC_CARR_L_CNT;                    \
  while(SIRC.i--)                                 \
    FK_SIRC_CARRIER();                            \
  FK_SIRC_OFF();                                  \
  FK_SIRC_DELAY(FK_SIRC_L_CNT);                   \
}


//SIRC Logic-1
#define FK_SIRC_LOGIC1()                          \
{                                                 \
  SIRC.i = FL_SIRC_CARR_H_CNT;                    \
  while(SIRC.i--)                                 \
    FK_SIRC_CARRIER();                            \
  FK_SIRC_OFF();                                  \
  FK_SIRC_DELAY(FK_SIRC_L_CNT);                   \
}


//SIRC Start
#define FK_SIRC_START()                           \
{                                                 \
  SIRC.i = FK_SIRC_CARR_START_CNT;                \
  while(SIRC.i--)                                 \
    FK_SIRC_CARRIER();                            \
  FK_SIRC_OFF();                                  \
  FK_SIRC_DELAY(FK_SIRC_LOW_START_CNT);           \
}


//SIRC Send
#define FK_SIRC_SEND(ADDR,CMD)                    \
{                                                 \
  /* Send Start (S1) [1B]*/                       \
  FK_SIRC_START();                                \
  /* Send Address [8B] */                         \
  for(SIRC.j=0;SIRC.j<FK_SIRC_ADDR_SIZE;SIRC.j++) \
  {                                               \
    if(ADDR & (1<<SIRC.j))                        \
    { FK_SIRC_LOGIC1();  }                        \
    else                                          \
    { FK_SIRC_LOGIC0();  }                        \
  }                                               \
  /* Send Command [8B] */                         \
  for(SIRC.j=0;SIRC.j<FK_SIRC_CMD_SIZE;SIRC.j++)  \
  {                                               \
    if(CMD & (1<<SIRC.j))                         \
    { FK_SIRC_LOGIC1();  }                        \
    else                                          \
    { FK_SIRC_LOGIC0();  }                        \
  }                                               \
}

#endif  //__FAKE_SIRC_H__
