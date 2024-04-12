/**
  ******************************************************************************
  * @file    relay.h 
  * @author  sbKim
  * @version V1.0.0
  * @date    2015/05/21
  * @brief   Header for relay.h   module
  ******************************************************************************
**/
#ifndef __ISD1760_H
#define __ISD1760_H

/* Includes ------------------------------------------------------------------*/
#include "main.h"
/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */


//======================================================================================================================
//                                                  ISD1760
//                                              Defined in isd1760.c
//======================================================================================================================
//======================================================================================================================
// Command defines
//======================================================================================================================
#define                 VOICE_POWER(X)          (X == 1)?GPIO_SetBits(GPIOB, GPIO_Pin_9) : GPIO_ResetBits(GPIOB, GPIO_Pin_9)

#define                 COMMAND_PU              (0x01 | 0x10)
#define                 COMMAND_STOP            (0x02 | 0x10)
#define                 COMMAND_RESET           (0x03 | 0x10)
#define                 COMMAND_CLR_INT         (0x04 | 0x10)
#define                 COMMAND_RD_STATUS       (0x05 | 0x10)
#define                 COMMAND_PD              (0x07 | 0x10)
#define                 COMMAND_G_ERASE         (0x43 | 0x10)
#define                 COMMAND_RD_APC          (0x44 | 0x10)
#define                 COMMAND_WR_NVCFG        (0x46 | 0x10)
#define                 COMMAND_LD_NVCFG        (0x47 | 0x10)
#define                 COMMAND_WR_APC2         (0x65 | 0x10)
#define                 COMMAND_SET_PLAY        (0x80 | 0x10)
#define                 COMMAND_SET_REC         (0x81 | 0x10)
#define                 COMMAND_SET_ERASE       (0x82 | 0x10)

#define                 CMD_ERR                 0x0100
#define                 CHECK_RDY               0x0001
#define                 CHECK_INT               0x100000
#define                 CHECK_PU                0x40000
#define                 CHECK_PLAYING           0x000004
#define                 CHECK_CMDERR            0x010000

#define                 DEFAULT_APC             0x440

#define                 DELAY_TPUD              50

#define                 ISD1760_TPUD_TIMEOUT    2
#define                 ISD1760_TIMEOUT         10
#define                 ISD1760_INT_TIMEOUT     1000
#define                 ISD1760_TIMEOUT_COUNTER 2000

void                    TestISD1760             (void);
void                    ISD1760_init            (void);
unsigned    short       ISD1760_powerUp         (void);
unsigned    short       ISD1760_powerDown       (void);
unsigned    short       ISD1760_stop            (void);
unsigned    short       ISD1760_clearInt        (void);
unsigned    long        ISD1760_readStatus      (void);
unsigned    short       ISD1760_reset           (void);
unsigned    long        ISD1760_readAPC         (void);
unsigned    short       ISD1760_writeNVCFG      (void);
unsigned    short       ISD1760_loadNVCFG       (void);
unsigned    short       ISD1760_globalErase     (void);
unsigned    long        ISD1760_writeAPC2       (unsigned short data);
void                    ISD1760_setRecord       (unsigned short startAddr, unsigned short endAddr);
void                    ISD1760_setPlay         (unsigned short startAddr, unsigned short endAddr, unsigned char volume);
void                    ISD1760_setErase        (unsigned short startAddr, unsigned short endAddr);
void                    ISD1760_setVolume       (unsigned char volume);
void                    ISD1760_setAnalogPath   (void);




#endif /* __ISD1760_H */