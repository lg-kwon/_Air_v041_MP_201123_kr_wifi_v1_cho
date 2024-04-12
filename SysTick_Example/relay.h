/**
  ******************************************************************************
  * @file    relay.h 
  * @author  sbKim
  * @version V1.0.0
  * @date    2015/05/21
  * @brief   Header for relay.h   module
  ******************************************************************************
**/
#ifndef __RELAY_H
#define __RELAY_H

/* Includes ------------------------------------------------------------------*/
#include "main.h"

  #define RELAY_AC_FAN1     1
#if ( MACHINE == HPA_130W)
  #define RELAY_OZONE_LAMP  2
  #define RELAY_AC_UV       3
  #define RELAY_AC_FAN2     4
#endif
  #define RELAY_PLASMA      5
  #define RELAY_SPI         6
  #define RELAY_PLASMA2     7
  #define RELAY_RCI         8

#define MAX_RELAY_NUM     8

#define RELAY_ON          1
#define RELAY_OFF         0

/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
void relayControlInit();
void relayControl(unsigned char relay, unsigned char onoff);
#endif /* __RELAY_H */
