/**
  ******************************************************************************
  * @file    RTC.h 
  * @author  sbKim
  * @version V1.0.0
  * @date    2015/05/21
  * @brief   Header for RTC.h  module
  ******************************************************************************
**/
#ifndef __RTC_H
#define __RTC_H

/* Includes ------------------------------------------------------------------*/
#include "main.h"
/* Exported types ------------------------------------------------------------*/
#define RTC_SUCCESS 1
#define RTC_FAIL    !RTC_SUCCESS

/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
unsigned int rtc_init();

void RTC_TimeShow();
void RTC_DateShow();
void RTC_TimeStampShow(void);
void rtc_check();
void RTC_Config(void);
void RTC_TimeRegulate(void);

#endif /* __RTC_H */