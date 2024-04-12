/**
  ******************************************************************************
  * @file    Key.h 
  * @author  sbKim
  * @version V1.0.0
  * @date    2015/05/21
  * @brief   Header for Key.h   module
  ******************************************************************************
**/
#ifndef __KEY_H
#define __KEY_H

#define INCLUDE_KEY_CONTROL
/* Includes ------------------------------------------------------------------*/
#include "main.h"
/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
void timer_key_check (void);
void keyPortInit();

/***********************************************************************************************************************
 *                                                  	DEFINES
 **********************************************************************************************************************/
#ifndef INCLUDE_KEY_CONTROL
  #define KEY_PWR_CHECK       0x00000001
  #define KEY_STER_CHECK      0x00000002
  #define KEY_O3_CHECK        0x00000004
  #define KEY_TIME_CHECK      0x00000008

  #define KEY_PWR_FLAG        0x00010000
  #define KEY_STER_FLAG       0x00020000    // 소독 모드
  #define KEY_O3_FLAG         0x00040000    // 살균 모드
  #define KEY_TIME_FLAG       0x00080000
#else
  #define KEY_SW1_CHECK       0x00000001
  #define KEY_SW2_CHECK       0x00000002
  #define KEY_SW3_CHECK       0x00000004
  #define KEY_SW4_CHECK       0x00000008
  #define KEY_SW5_CHECK       0x00000010

  #define KEY_SW1_FLAG        0x00010000
  #define KEY_SW2_FLAG        0x00020000
  #define KEY_SW3_FLAG        0x00040000
  #define KEY_SW4_FLAG        0x00080000
  #define KEY_SW5_FLAG        0x00100000

  #define KEY_SW1_LONG_FLAG   0x01010000
  #define KEY_SW2_LONG_FLAG   0x01020000
  #define KEY_SW3_LONG_FLAG   0x01040000
  #define KEY_SW4_LONG_FLAG   0x01080000
  #define KEY_SW5_LONG_FLAG   0x01100000

  #define KEY_START           KEY_SW3_FLAG
  #define KEY_START_LONG      KEY_SW3_LONG_FLAG
  #define KEY_INC             KEY_SW1_FLAG
  #define KEY_INC_LONG        KEY_SW1_LONG_FLAG
  #define KEY_UPGRADE         0x00180000    // SW4 and SW5
#endif
/***********************************************************************************************************************
 *                                                  KEY STATE DEFINES
 **********************************************************************************************************************/
#define KEY_STATE_NONE                      0x00
#define KEY_STATE_DETECTED                  0x01
#define KEY_STATE_PRESSED                   0x02

#define KEY_TIME_COUNTER_INITIAL            10      // 50 milli for first detection
#define KEY_TIME_COUNTER_1SEC               200     // 1000 milli for 1.5 sec detection
#define KEY_TIME_COUNTER_DECREASE           50      // 250 milli decrease
#define KEY_TIME_COUNTER_LIMIT              20      // 100 milli

#define TYPE_NORMAL                         0
#define TYPE_LONG                           1
#define TYPE_CONTINUE                       2

#endif /* __KEY_H */