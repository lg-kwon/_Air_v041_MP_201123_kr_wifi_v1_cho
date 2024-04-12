#ifndef __CONTROL_H
#define __CONTROL_H

/* Includes ------------------------------------------------------------------*/
#include "main.h"
/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */

//init
void control_ledFont();
void control_initLed();
void segmentOff();
void control_relayAllOff();

//ster control
void control_sterOn();
void control_sterStartLedOn();
void control_sterStopLedOn();
void control_consumableCheckLed();

//Disinfect control
void control_disRelayOn();
//void control_disRelayOff();     SJM 190711 never used
void control_disLed();

//Ion control
void control_IonOn();
//void control_IonOff();          SJM 190711 never used
void control_ionLed();

//Destruction
void control_desLed();
void control_relayDestruction();

//engineer
void control_engineerLed();
#endif
