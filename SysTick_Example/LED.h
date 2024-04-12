/**
  ******************************************************************************
  * @file    LED.h 
  * @author  sbKim
  * @version V1.0.0
  * @date    2015/05/21
  * @brief   Header for LED.h  module
  ******************************************************************************
**/
#ifndef __LED_H
#define __LED_H

/* Includes ------------------------------------------------------------------*/
#include "main.h"

#define LED_ON  1
#define LED_OFF 0

//#ifndef HPA_36C
#if (MACHINE == HPA_130W)
  #define LED_PLASMA_FONT   1
  #define LED_DIS_FONT      2
  #define LED_ION_FONT      3
  #define LED_RESERVE_FONT  4
  #define LED_POWER_FONT    5
  #define LED_PID_FONT      6
  #define LED_POWER_SIG1    7
  #define LED_POWER_SIG2    8
  #define LED_MODE_SIG1     9
  #define LED_MODE_SIG2     10
  #define LED_SETTING_SIG1  11
  #define LED_SETTING_SIG2  12
  #define LED_PLASMA_ON     13
  #define LED_DIS_ON        14
  #define LED_ION_ON        15
  #define LED_RESERVE_ON    16
  #define LED_POWER_ON      17
  #define LED_PID_ON        18
//#else   // HPA_36C
#elif (MACHINE == HPA_36C)
  #define LED_INTENSITY_0   1
  #define LED_INTENSITY_1   2
  #define LED_INTENSITY_2   3
  #define LED_INTENSITY_3   4
  #define LED_BATTERY_GREEN 5
  #define LED_BATTERY_RED   6
  #ifdef  LPP_V1_00_BD
    #define LED_BAT_GRN1    10 
    #define LED_BAT_GRN2    11 
    #define LED_BAT_RED1    12 
    #define LED_BAT_RED2    9 
  #endif
//  #define LED_INTENSITY_0   7
//  #define LED_INTENSITY_1   8
//  #define LED_INTENSITY_2   9
//  #define LED_INTENSITY_3   10
//  #define LED_BATTERY_GREEN 11
//  #define LED_BATTERY_RED   12
  #define LED_PLASMA_ON     13
  #define LED_DIS_ON        14
  #define LED_ION_ON        15
  #define LED_RESERVE_ON    16
//  #define LED_POWER_ON      17
  #define LED_PID_ON        18
#endif  // MACHINE   HPA_36C


/* Exported types ------------------------------------------------------------*/

/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
void ledPortInit();
void ledControl(unsigned int control, unsigned char onoff);
void segmentControl(unsigned int num);
void ledComControl(unsigned char num, unsigned char onoff);
void timerLed();
void ledAllOff();
void segmentAlphaControl(unsigned char ch1, unsigned char ch2);
void PIRLedTimer();
void REDLed();
void WhiteLed();
#endif /* __LED_H */
