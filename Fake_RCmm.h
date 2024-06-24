#ifndef __FAKE_RCMM_H__  //__FAKE_RCMM_H__
#define __FAKE_RCMM_H__  //__FAKE_RCMM_H__

//[STD Lib Include]
#include <stdio.h>

//Must Declare in Main Code
//#define FAKE_RCMM_ON()
//#define FAKE_RCMM_OFF()

//[Typedef Declare]
typedef unsigned char   FK_RCMM_U8;
typedef unsigned int    FK_RCMM_U16;
typedef unsigned long   FK_RCMM_U32;

typedef signed char     FK_RCMM_S8;
typedef signed int      FK_RCMM_S16;
typedef signed long     FK_RCMM_S32;

//[Macro Declare]
#define FK_RCMM_N                 0                     //Numberic Fine-Turning
#define FK_RCMM_BASE              1                     //27.7(36Khz)
#define FK_RCMM_CARR_CNT          6  + FK_RCMM_N        //166.7 / 27.7 = 6
#define FK_RCMM_CNT_00            8  + FK_RCMM_N        //(277.8-166.7) / 27.7 = 4  * 2 = 8
#define FK_RCMM_CNT_01            20 + FK_RCMM_N        //(444.4-166.7) / 27.7 = 10 * 2 = 20
#define FK_RCMM_CNT_10            32 + FK_RCMM_N        //(611.1-166.7) / 27.7 = 16 * 2 = 32
#define FK_RCMM_CNT_11            44 + FK_RCMM_N        //(777.8-166.7) / 27.7 = 22 * 2 = 44

//Start
#define FK_RCMM_CARR_START_CNT    15 + FK_RCMM_N        //416.7/27.7 = 15
#define FK_RCMM_LOW_START_CNT     20 + FK_RCMM_N        //277.8/27.7 = 10 * 2 = 20

//Size
#define FK_RCMM_MODE_SIZE         2                     //Mode Size
#define FK_RCMM_EXT_MODE_SIZE     4                     //Extended Mode Size
#define FK_RCMM_ADDR_SIZE         2                     //Address Size
#define FK_RCMM_CMD_SIZE          8                     //Command Size
#define FK_RCMM_EXT_CMD_SIZE      20                    //Extended Command Size

//[Enum Declare]
typedef enum _RCMM_12B_MODE
{
  RCMM_12B_EXTEDND      = 0,        //12-Bit Extended Mode
  RCMM_12B_MOUSE        = 1,        //12-Bit Mouse    Mode
  RCMM_12B_KEYBOARD     = 2,        //12-Bit Keyboard Mode
  RCMM_12B_GAMING       = 3,        //12-Bit Gaming   Mode
} RCMM_12B_MODE_E;


//[Struct Declare]
typedef struct _FK_RCMM
{
  FK_RCMM_U32  RCMM_Cnt;
  FK_RCMM_U16  i,j;
} FK_RCMM_t;


//RCMM Declare
#define FK_RCMM_DECLARE()       FK_RCMM_t RCMM

//RCMM Tick
#define FK_RCMM_TICK()                          \
{                                               \
  if(RCMM.RCMM_Cnt)                             \
    RCMM.RCMM_Cnt--;                            \
}

//RCMM Delay
#define FK_RCMM_DELAY(Tick)                     \
{                                               \
  RCMM.RCMM_Cnt = Tick;                         \
  while(RCMM.RCMM_Cnt);                         \
}

//RCMM Carrier
#define FK_RCMM_CARRIER()                       \
{                                               \
  FK_RCMM_ON();                                 \
  FK_RCMM_DELAY(FK_RCMM_BASE);                  \
  FK_RCMM_OFF();                                \
  FK_RCMM_DELAY(FK_RCMM_BASE);                  \
}

//RCMM Logic-00
#define FK_RCMM_LOGIC00()                       \
{                                               \
  RCMM.i = FK_RCMM_CARR_CNT;                    \
  while(RCMM.i--)                               \
    FK_RCMM_CARRIER();                          \
  FK_RCMM_OFF();                                \
  FK_RCMM_DELAY(FK_RCMM_CNT_00);                \
}

//RCMM Logic-01
#define FK_RCMM_LOGIC01()                       \
{                                               \
  RCMM.i = FK_RCMM_CARR_CNT;                    \
  while(RCMM.i--)                               \
    FK_RCMM_CARRIER();                          \
  FK_RCMM_OFF();                                \
  FK_RCMM_DELAY(FK_RCMM_CNT_01);                \
}

//RCMM Logic-10
#define FK_RCMM_LOGIC10()                       \
{                                               \
  RCMM.i = FK_RCMM_CARR_CNT;                    \
  while(RCMM.i--)                               \
    FK_RCMM_CARRIER();                          \
  FK_RCMM_OFF();                                \
  FK_RCMM_DELAY(FK_RCMM_CNT_10);                \
}

//RCMM Logic-11
#define FK_RCMM_LOGIC11()                       \
{                                               \
  RCMM.i = FK_RCMM_CARR_CNT;                    \
  while(RCMM.i--)                               \
    FK_RCMM_CARRIER();                          \
  FK_RCMM_OFF();                                \
  FK_RCMM_DELAY(FK_RCMM_CNT_11);                \
}

//RCMM Start
#define FK_RCMM_START()                         \
{                                               \
  RCMM.i = FK_RCMM_CARR_START_CNT;              \
  while(RCMM.i--)                               \
    FK_RCMM_CARRIER();                          \
  FK_RCMM_OFF();                                \
  FK_RCMM_DELAY(FK_RCMM_LOW_START_CNT);         \
}

//RCMM Send
#define FK_RCMM_SEND(MODE,ADDR,CMD)             \
{                                               \
  /* Send Start [2B]*/                          \
  FK_RCMM_START();                              \
  /* Send Mode [2B]*/                           \
  switch(MODE)                                  \
  {                                             \
    case 0: FK_RCMM_LOGIC00();  break;          \
    case 1: FK_RCMM_LOGIC01();  break;          \
    case 2: FK_RCMM_LOGIC10();  break;          \
    case 3: FK_RCMM_LOGIC11();  break;          \
    default:  break;                            \
  }                                             \
  /* Send Address [2B] */                       \
  switch(ADDR)                                  \
  {                                             \
    case 0: FK_RCMM_LOGIC00();  break;          \
    case 1: FK_RCMM_LOGIC01();  break;          \
    case 2: FK_RCMM_LOGIC10();  break;          \
    case 3: FK_RCMM_LOGIC11();  break;          \
    default:  break;                            \
  }                                             \
  /* Send Command [5B] */                       \
  RCMM.j = FK_RCMM_CMD_SIZE;                    \
  while(RCMM.j)                                 \
  {                                             \
    switch((CMD & (3<<(RCMM.j-2)))>>(RCMM.j-2)) \
    {                                           \
      case 0: FK_RCMM_LOGIC00();  break;        \
      case 1: FK_RCMM_LOGIC01();  break;        \
      case 2: FK_RCMM_LOGIC10();  break;        \
      case 3: FK_RCMM_LOGIC11();  break;        \
      default:  break;                          \
    }                                           \
    RCMM.j-=2;  /* Shift 2 */                   \
  }                                             \
  /* Send Stop */                               \
  RCMM.i = FK_RCMM_CARR_CNT;                    \
  while(RCMM.i--)                               \
    FK_RCMM_CARRIER();                          \
}

#endif  //__FAKE_RCMM_H__
