/**
  ******************************************************************************
  * @file    24LC512.h 
  * @author  sbKim
  * @version V1.0.0
  * @date    2015/05/21
  * @brief   Header for 24LC512.h  module
  ******************************************************************************
**/
#ifndef __24LC512_H
#define __24LC512_H

/* Includes ------------------------------------------------------------------*/
#include "main.h"
/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
void sEE_Init(void);
void sEE_WriteBuffer(uint8_t* pBuffer, uint16_t WriteAddr, uint16_t NumByteToWrite);
uint32_t sEE_ReadBuffer(uint8_t* pBuffer, uint16_t ReadAddr, uint16_t* NumByteToRead);
uint32_t sEE_WaitEepromStandbyState(void);
#endif /* __24LC512_H */