#ifndef __FAKE_RECS_H__  //__FAKE_RECS_H__
#define __FAKE_RECS_H__  //__FAKE_RECS_H__

//[STD Lib Include]
#include <stdio.h>

//Must Declare in Main Code
//#define FAKE_RECS_ON()
//#define FAKE_RECS_OFF()

//[Typedef Declare]
typedef unsigned char   FK_RECS_U8;        //Fake RECS U8
typedef unsigned int    FK_RECS_U16;       //Fake RECS U16
typedef unsigned long   FK_RECS_U32;       //Fake RECS U32

typedef signed char     FK_RECS_S8;        //Fake RECS S8
typedef signed int      FK_RECS_S16;       //Fake RECS S16
typedef signed long     FK_RECS_S32;       //Fake RECS S32


//[Macro Declare]
#define FK_RECS_N                0                      //Numeric Fine-Tune
#define FK_RECS_BASE             1                      //26.3 (1Clk)
#define FK_RECS_CARR_CNT         6   + FK_RECS_N        //158 / 26.3 = 6
#define FK_RECS_L1_CNT           564 + FK_RECS_N        //(7590-158)/26.3 = 282 * 2 = 564
#define FK_RECS_L0_CNT           372 + FK_RECS_N        //(5060-158)/26.3 = 186 * 2 = 372

//Start
#define FK_RECS_CARR_START_CNT   320 + FK_RECS_N        //8400 / 26.3 = 320
#define FK_RECS_LOW_START_CNT    320 + FK_RECS_N        //4200 / 26.3 = 160 * 2 = 320

//Size
#define FK_RECS_ADDR_SIZE        3                      //Address Size
#define FK_RECS_ADDR_EXT_SIZE    4                      //Address Size
#define FK_RECS_CMD_SIZE         6                      //Command Size

//Normal Mode Address (Address Must Use it)
#define RECS_NORMAL_MODE_1       0x07                   //111
#define RECS_NORMAL_MODE_2       0x00                   //000
#define RECS_NORMAL_MODE_3       0x01                   //001
#define RECS_NORMAL_MODE_4       0x02                   //010
#define RECS_NORMAL_MODE_5       0x03                   //011
#define RECS_NORMAL_MODE_6       0x04                   //100
#define RECS_NORMAL_MODE_7       0x05                   //101

//Extended Mode Address (Address Must Use it)
#define RECS_EXT_MODE_8          0x00                   //0000
#define RECS_EXT_MODE_9          0x08                   //1000
#define RECS_EXT_MODE_10         0x04                   //0100
#define RECS_EXT_MODE_11         0x0C                   //1100
#define RECS_EXT_MODE_12         0x01                   //0001
#define RECS_EXT_MODE_13         0x09                   //1001
#define RECS_EXT_MODE_14         0x05                   //0101
#define RECS_EXT_MODE_15         0x0D                   //1101
#define RECS_EXT_MODE_16         0x02                   //1010
#define RECS_EXT_MODE_17         0x06                   //0110
#define RECS_EXT_MODE_18         0x0E                   //1110
#define RECS_EXT_MODE_19         0x07                   //0111
#define RECS_EXT_MODE_20         0x0F                   //1111


//[Struct Declare]
typedef struct _FK_RECS
{
  FK_RECS_U32  RECS_Cnt;
  FK_RECS_U16  i,j;
} FK_RECS_t;


//RECS Declare
#define FK_RECS_DECLARE()       FK_RECS_t RECS


//RECS Tick
#define FK_RECS_TICK()                          \
{                                               \
  if(RECS.RECS_Cnt)                             \
    RECS.RECS_Cnt--;                            \
}


//RECS Delay
#define FK_RECS_DELAY(Tick)                     \
{                                               \
  RECS.RECS_Cnt = Tick;                         \
  while(RECS.RECS_Cnt);                         \
}


//RECS Carrier
#define FK_RECS_CARRIER()                       \
{                                               \
  FK_RECS_ON();                                 \
  FK_RECS_DELAY(FK_RECS_BASE);                  \
  FK_RECS_OFF();                                \
  FK_RECS_DELAY(FK_RECS_BASE);                  \
}


//RECS Logic-0
#define FK_RECS_LOGIC0()                        \
{                                               \
  RECS.i = FK_RECS_CARR_CNT;                    \
  while(RECS.i--)                               \
    FK_RECS_CARRIER();                          \
  FK_RECS_OFF();                                \
  FK_RECS_DELAY(FK_RECS_L0_CNT);                \
}


//RECS Logic-1
#define FK_RECS_LOGIC1()                        \
{                                               \
  RECS.i = FK_RECS_CARR_CNT;                    \
  while(RECS.i--)                               \
    FK_RECS_CARRIER();                          \
  FK_RECS_OFF();                                \
  FK_RECS_DELAY(FK_RECS_L1_CNT);                \
}


//RECS Start
#define FK_RECS_START()                         \
{                                               \
  FK_RECS_LOGIC1();                             \
  FK_RECS_LOGIC1();                             \
}


//RECS Send
#define FK_RECS_SEND(ADDR,CMD)                  \
{                                               \
  /* Send Start (S1) [1B]*/                     \
  FK_RECS_START();                              \
  /* Send Address [8B] */                       \
  RECS.j = FK_RECS_ADDR_SIZE;                   \
  while(RECS.j--)                               \
  {                                             \
    if(ADDR &(1<<RECS.j))                       \
    { FK_RECS_LOGIC1();  }                      \
    else                                        \
    { FK_RECS_LOGIC0();  }                      \
  }                                             \
  /* Send Command [5B] */                       \
  RECS.j = FK_RECS_CMD_SIZE;                    \
  while(RECS.j--)                               \
  {                                             \
    if(CMD &(1<<RECS.j))                        \
    { FK_RECS_LOGIC1();  }                      \
    else                                        \
    { FK_RECS_LOGIC0();  }                      \
  }                                             \
  FK_RECS_LOGIC1();                             \
}


//RECS Extend Send
#define FK_RECS_EXT_SEND(TOG,ADDR,CMD)          \
{                                               \
  /* Send Start (S1) [2B]*/                     \
  FK_RECS_START();                              \
  /* Send Toggle [1B] */                        \
  if(TOG)                                       \
  { FK_RECS_LOGIC1();   }                       \
  else                                          \
  { FK_RECS_LOGIC0();   }                       \
  /* Send Address [8B] */                       \
  RECS.j = FK_RECS_ADDR_EXT_SIZE;               \
  while(RECS.j--)                               \
  {                                             \
    if(ADDR &(1<<RECS.j))                       \
    { FK_RECS_LOGIC1();  }                      \
    else                                        \
    { FK_RECS_LOGIC0();  }                      \
  }                                             \
  /* Send Command [5B] */                       \
  RECS.j = FK_RECS_CMD_SIZE;                    \
  while(RECS.j--)                               \
  {                                             \
    if(CMD &(1<<RECS.j))                        \
    { FK_RECS_LOGIC1();  }                      \
    else                                        \
    { FK_RECS_LOGIC0();  }                      \
  }                                             \
  FK_RECS_LOGIC1();                             \
}

#endif  //__FAKE_RECS_H__
