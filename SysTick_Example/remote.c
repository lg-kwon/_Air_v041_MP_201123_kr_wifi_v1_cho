#include "remote.h"


volatile unsigned char m_oldBit;
volatile unsigned short m_count, m_count2;
extern unsigned int m_remoteState;
extern unsigned int g_remoteFlag;
extern unsigned int g_RemoteOzoneSensorValue;

unsigned char pBuf[MAX_SIZE_PRINT];
unsigned int pPointer;
unsigned char m_data[4];
unsigned char m_dataBit;
unsigned char m_dataCount;
unsigned char m_dataBitCount;
unsigned char m_repeatCount;
unsigned char m_dataCheck;
unsigned int g_extOzoneSenseFlag;

void uart_printf(unsigned char *buf, unsigned int size)
{
  
  for(int i = 0; i < size; i++)
  {
    pBuf[pPointer] = buf[i];
    if(++pPointer > MAX_SIZE_PRINT)
      pPointer = 0;
  }
}


unsigned int testFlag, testFlag2;
unsigned int testCount;
//0.1ms
void remoteTimerInit()
{
  TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
  TIM_OCInitTypeDef  TIM_OCInitStructure;
  NVIC_InitTypeDef  NVIC_InitStructure;

  /* Enable TIM2, TIM3 and TIM4 clocks */
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);

  // (84000000Hz / 1680) / 5 = 10000Hz = 100us  
  /* TIM2 configuration */
  TIM_TimeBaseStructure.TIM_Period = 4;
  TIM_TimeBaseStructure.TIM_Prescaler = 839;//1679;
  TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;    
  TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  
  TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);
  TIM_OCStructInit(&TIM_OCInitStructure);

  /* Immediate load of TIM2 Precaler values */
  TIM_PrescalerConfig(TIM3, 839, TIM_PSCReloadMode_Immediate);

  /* Clear TIM2 update pending flags */
  TIM_ClearFlag(TIM3, TIM_FLAG_Update);

  /* Enable the TIM2 Interrupt */
  NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);

  /* Enable TIM4 Update interrupts */
  TIM_ITConfig(TIM3, TIM_IT_Update, ENABLE);

  /* TIM2, TIM3 and TIM4 enable counters */
  TIM_Cmd(TIM3, ENABLE);
}

void remoteInit()
{
  GPIO_InitTypeDef  GPIO_InitStructure;
  
  /* Enable the key Clock */
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);
  
  /* Configure the Relay pin */
#ifdef  INCLUDE_DIP_SWITCH
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2 | GPIO_Pin_8 | GPIO_Pin_9 | GPIO_Pin_10;
#else
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
#endif
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIOD, &GPIO_InitStructure);
  testFlag = FALSE;
  testCount = TRUE;
  m_remoteState = 0;
}

unsigned int testSec;

unsigned int bufferSize(unsigned char *buffer)
{
  unsigned int size = 0;
  while(buffer[size] != '\0')
  {
    size++;  
  }
  return size;
}

void remoteCheck()
{
  unsigned char getBit = 0;

  m_count++;
  if ((GPIOD->IDR & GPIO_Pin_2) != (uint32_t)Bit_RESET)
  {
    getBit = 1;
  }
  else 
  {
    getBit = 0;
  }
  
  if(m_oldBit != getBit)
  {
    m_oldBit = getBit;

    switch(m_remoteState)
    {
    case REMOTE_IDLE:
      if(getBit == 0)
      {
        m_remoteState = REMOTE_READY_LOW;
        m_count = 0;
        m_dataBitCount = 0;
        m_dataCount = 0;
      }
      break;
    case REMOTE_READY_LOW://9ms
      if(m_count <= (TIME_READY_LOW + REMOTE_MARGIN) && m_count >= (TIME_READY_LOW - REMOTE_MARGIN))
      {
        m_count = 0;
        m_remoteState = REMOTE_READY_HIGH;
      }
      break;
    case REMOTE_READY_HIGH://4.5ms
      if(m_count <= (TIME_READY_HIGH + REMOTE_MARGIN) && m_count >= (TIME_READY_HIGH - REMOTE_MARGIN))
      {
        m_count = 0;
        m_remoteState = REMOTE_DATA_LOW;
      }
      else if(m_count <= (TIME_REPEAT + REMOTE_MARGIN) && m_count >= (TIME_REPEAT - REMOTE_MARGIN))
      {
        m_count = 0;
        m_remoteState = REMOTE_IDLE;
        //printf("repeat : %d\r\n", m_repeatCount); 
        if(m_dataCheck == TRUE)
        {
          if(++m_repeatCount > 5)
          {
            m_repeatCount = 0;
            m_dataCheck = FALSE;
            remoteFlag(m_data, REMOTE_LONG_FLAG);
          }
        }
      }
      break;
    case REMOTE_DATA_LOW://
      if(m_count <= (TIME_DATA_LOW + REMOTE_MARGIN) && m_count >= (TIME_DATA_LOW - REMOTE_MARGIN))
      {
        m_remoteState = REMOTE_DATA_HIGH;
        m_count = 0;
      }
      else
      {
        //m_count = 0;
      }
      break;
    case REMOTE_DATA_HIGH:
      if(m_count <= (TIME_DATA_BIT_LOW + REMOTE_MARGIN) && m_count >= (TIME_DATA_BIT_LOW - REMOTE_MARGIN))
      {//0.56ms
        m_count = 0;
        m_remoteState = REMOTE_DATA_LOW;
        m_dataBit |= (0 << m_dataBitCount);
      }
      else if(m_count <= (TIME_DATA_BIT_HIGH + REMOTE_MARGIN) && m_count >= (TIME_DATA_BIT_HIGH - REMOTE_MARGIN))
      {
        m_count = 0;
        m_remoteState = REMOTE_DATA_LOW;
        m_dataBit |= (1 << m_dataBitCount);
      }
      else
      {
        //printf("error\r\n");
      }
      if(++m_dataBitCount % 8 == 0)
      {
        //sprintf((char*)buf, "m_dataBitCount : %d\r\n", m_dataBitCount);
        //uart_printf(buf, bufferSize(buf));
        m_data[m_dataCount] = m_dataBit;
        m_dataBit = 0;
        m_dataBitCount = 0;
        m_dataCount++;
      }
      if(m_dataCount == 4)
      {
        m_remoteState = REMOTE_IDLE;
        m_dataCount = 0;
        m_count = 0;
        m_repeatCount = 0;
        m_dataCheck = TRUE;
        //sprintf((char*)buf, "[t : %d][0x%x][0x%x][0x%x][0x%x]\r\n", m_count, m_data[0], m_data[1], m_data[2], m_data[3]);  
        //uart_printf(buf, bufferSize(buf));
      }
      break;
    }
  }

  if(m_dataCheck == FALSE && m_count > REMOTE_ERROR)
  {
    m_count = 0;
    m_remoteState = REMOTE_IDLE;  
  }
  
  if(m_dataCheck == TRUE && m_repeatCount <= 5 && m_count > TIME_FLAME_INTERVAL * 1)
  {
    m_remoteState = REMOTE_IDLE;
    m_repeatCount = 0;
    m_count = 0;
    m_dataCheck = FALSE;
    
    printf("\r\n****** kwon : remoteFlag enter  *******");
    
      remoteFlag(m_data, REMOTE_NORMAL_FLAG);
  }
}

void remoteFlag(unsigned char *data, unsigned char type)
{
  printf("[R/C] %02x %02x  %02x %02x [t=%02x]\r\n",data[0], data[1], data[2], data[3], type);
  if (*data == REMOTE_CODE_FIRST && *(data + 1) == REMOTE_CODE_SECOND) {
    if(type == REMOTE_NORMAL_FLAG) {
      if(*(data + 2) == REMOTE_POWER)                g_remoteFlag = REMOTE_POWER_FLAG;
      else if(*(data + 2) == REMOTE_STERILIZATION)   g_remoteFlag = REMOTE_STERILIZATION_FLAG;
      else if(*(data + 2) == REMOTE_O3)              g_remoteFlag = REMOTE_O3_FLAG;
      else if(*(data + 2) == REMOTE_TIMER)           g_remoteFlag = REMOTE_TIMER_FLAG;
      else if(*(data + 2) == REMOTE_WIND)            g_remoteFlag = REMOTE_WIND_FLAG;
      else if(*(data + 2) == REMOTE_ION)             g_remoteFlag = REMOTE_ION_FLAG;
      else if(*(data + 2) == REMOTE_PLASMA)          g_remoteFlag = REMOTE_PLASMA_FLAG;
      else if(*(data + 2) == REMOTE_UP)              g_remoteFlag = REMOTE_UP_FLAG;
      else if(*(data + 2) == REMOTE_MENU)            g_remoteFlag = REMOTE_MENU_FLAG;
      else if(*(data + 2) == REMOTE_OK)              g_remoteFlag = REMOTE_OK_FLAG;
      else if(*(data + 2) == REMOTE_DOWN)            g_remoteFlag = REMOTE_DOWN_FLAG;
      else if(*(data + 2) == REMOTE_RESERVE)         g_remoteFlag = REMOTE_RESERVE_FLAG;
#ifdef  ADD_REMOTE_OZONE_SENSOR
      else if(*(data + 2) == REMOTE_OZONE_1)
        {g_remoteFlag = REMOTE_OZ1_FLAG;  g_extOzoneSenseFlag = TRUE; g_RemoteOzoneSensorValue = 0;}//5
      else if(*(data + 2) == REMOTE_OZONE_2)
        {g_remoteFlag = REMOTE_OZ2_FLAG;  g_extOzoneSenseFlag = TRUE; g_RemoteOzoneSensorValue = 15;}//15
      else if(*(data + 2) == REMOTE_OZONE_3)
        {g_remoteFlag = REMOTE_OZ3_FLAG;  g_extOzoneSenseFlag = TRUE; g_RemoteOzoneSensorValue = 25;}//25
      else if(*(data + 2) == REMOTE_OZONE_4)
        {g_remoteFlag = REMOTE_OZ4_FLAG;  g_extOzoneSenseFlag = TRUE; g_RemoteOzoneSensorValue = 35;}//35
      else if(*(data + 2) == REMOTE_OZONE_5)
        {g_remoteFlag = REMOTE_OZ5_FLAG;  g_extOzoneSenseFlag = TRUE; g_RemoteOzoneSensorValue = 45;}//45
      else if(*(data + 2) == REMOTE_OZONE_6)
        {g_remoteFlag = REMOTE_OZ6_FLAG;  g_extOzoneSenseFlag = TRUE; g_RemoteOzoneSensorValue = 55;}//55
      else if(*(data + 2) == REMOTE_OZONE_7)
        {g_remoteFlag = REMOTE_OZ7_FLAG;  g_extOzoneSenseFlag = TRUE; g_RemoteOzoneSensorValue = 65;}//65
      else if(*(data + 2) == REMOTE_OZONE_8)
        {g_remoteFlag = REMOTE_OZ8_FLAG;  g_extOzoneSenseFlag = TRUE; g_RemoteOzoneSensorValue = 75;}//75
//      else if(*(data + 2) == REMOTE_OZONE_9)         g_remoteFlag = REMOTE_OZ9_FLAG;  //85
//      else if(*(data + 2) == REMOTE_OZONE_10)         g_remoteFlag = REMOTE_OZ10_FLAG;  //95        
      //printf("g_remoteFlag : [%x], type[%d]\r\n", g_remoteFlag, type);
#endif
    }
    else if(type == REMOTE_LONG_FLAG) {
      if(*(data + 2) == REMOTE_POWER)                g_remoteFlag = REMOTE_POWER_LONG_FLAG;
      else if(*(data + 2) == REMOTE_STERILIZATION)   g_remoteFlag = REMOTE_STERILIZATION_LONG_FLAG;
      else if(*(data + 2) == REMOTE_O3)              g_remoteFlag = REMOTE_O3_LONG_FLAG;
      else if(*(data + 2) == REMOTE_TIMER)           g_remoteFlag = REMOTE_TIMER_LONG_FLAG;
      else if(*(data + 2) == REMOTE_WIND)            g_remoteFlag = REMOTE_WIND_LONG_FLAG;
      else if(*(data + 2) == REMOTE_ION)             g_remoteFlag = REMOTE_ION_LONG_FLAG;
      else if(*(data + 2) == REMOTE_PLASMA)          g_remoteFlag = REMOTE_PLASMA_LONG_FLAG;
      else if(*(data + 2) == REMOTE_UP)              g_remoteFlag = REMOTE_UP_LONG_FLAG;
      else if(*(data + 2) == REMOTE_MENU)            g_remoteFlag = REMOTE_MENU_LONG_FLAG;
      else if(*(data + 2) == REMOTE_OK)              g_remoteFlag = REMOTE_OK_LONG_FLAG;
      else if(*(data + 2) == REMOTE_DOWN)            g_remoteFlag = REMOTE_DOWN_LONG_FLAG;
      else if(*(data + 2) == REMOTE_RESERVE)         g_remoteFlag = REMOTE_RESERVE_LONG_FLAG;
    }
//    printf("g_remoteFlag : [%x], type[%d]\r\n", g_remoteFlag, type);
  }
#ifdef  USE_119SP_REMOTE
  else if (*data == OKC_119SP_CODE_FIRST && *(data + 1) == OKC_119SP_CODE_SECOND) {
    if(type == REMOTE_NORMAL_FLAG) {
      if(*(data + 2) == OKC_119SP_POWER)                g_remoteFlag = REMOTE_POWER_FLAG;
      else if(*(data + 2) == OKC_119SP_STERILIZATION)   g_remoteFlag = REMOTE_STERILIZATION_FLAG;
      else if(*(data + 2) == OKC_119SP_O3)              g_remoteFlag = REMOTE_O3_FLAG;
      else if(*(data + 2) == OKC_119SP_TIMER)           g_remoteFlag = REMOTE_TIMER_FLAG;
      else if(*(data + 2) == OKC_119SP_WIND)            g_remoteFlag = REMOTE_WIND_FLAG;
      else if(*(data + 2) == OKC_119SP_ION)             g_remoteFlag = REMOTE_ION_FLAG;
      else if(*(data + 2) == OKC_119SP_PLASMA)          g_remoteFlag = REMOTE_PLASMA_FLAG;
      else if(*(data + 2) == OKC_119SP_UP)              g_remoteFlag = REMOTE_UP_FLAG;
      else if(*(data + 2) == OKC_119SP_MENU)            g_remoteFlag = REMOTE_MENU_FLAG;
      else if(*(data + 2) == OKC_119SP_OK)              g_remoteFlag = REMOTE_OK_FLAG;
      else if(*(data + 2) == OKC_119SP_DOWN)            g_remoteFlag = REMOTE_DOWN_FLAG;
#ifdef USE_TNY_1311S_REMOTE
      else if(*(data + 2) == TNY_1311S_POWER)           g_remoteFlag = TNY_POWER_FLAG;
      else if(*(data + 2) == TNY_1311S_MODE)            g_remoteFlag = TNY_MODE_FLAG;
      else if(*(data + 2) == TNY_1311S_LEFT)            g_remoteFlag = TNY_LEFT_FLAG;
      else if(*(data + 2) == TNY_1311S_OK)              g_remoteFlag = TNY_OK_FLAG;
      else if(*(data + 2) == TNY_1311S_RIGHT)           g_remoteFlag = TNY_RIGHT_FLAG;
      else if(*(data + 2) == TNY_1311S_TIMER)           g_remoteFlag = TNY_TIMER_FLAG;
      else if(*(data + 2) == TNY_1311S_HUMAN)           g_remoteFlag = TNY_HUMAN_FLAG;
      else if(*(data + 2) == TNY_1311S_SETUP)           g_remoteFlag = TNY_SETUP_FLAG;
      else if(*(data + 2) == TNY_1311S_HOME)            g_remoteFlag = TNY_HOME_FLAG;
#endif
      
      printf("\r\n *********** KWON: R/C g_remoteFlag %02x  ***********\r\n",g_remoteFlag);
      
    }
    else if(type == REMOTE_LONG_FLAG) {
      if(*(data + 2) == OKC_119SP_POWER)                g_remoteFlag = REMOTE_POWER_LONG_FLAG;
      else if(*(data + 2) == OKC_119SP_STERILIZATION)   g_remoteFlag = REMOTE_STERILIZATION_LONG_FLAG;
      else if(*(data + 2) == OKC_119SP_O3)              g_remoteFlag = REMOTE_O3_LONG_FLAG;
      else if(*(data + 2) == OKC_119SP_TIMER)           g_remoteFlag = REMOTE_TIMER_LONG_FLAG;
      else if(*(data + 2) == OKC_119SP_WIND)            g_remoteFlag = REMOTE_WIND_LONG_FLAG;
      else if(*(data + 2) == OKC_119SP_ION)             g_remoteFlag = REMOTE_ION_LONG_FLAG;
      else if(*(data + 2) == OKC_119SP_PLASMA)          g_remoteFlag = REMOTE_PLASMA_LONG_FLAG;
      else if(*(data + 2) == OKC_119SP_UP)              g_remoteFlag = REMOTE_UP_LONG_FLAG;
      else if(*(data + 2) == OKC_119SP_MENU)            g_remoteFlag = REMOTE_MENU_LONG_FLAG;
      else if(*(data + 2) == OKC_119SP_OK)              g_remoteFlag = REMOTE_OK_LONG_FLAG;
      else if(*(data + 2) == OKC_119SP_DOWN)            g_remoteFlag = REMOTE_DOWN_LONG_FLAG;
#ifdef USE_TNY_1311S_REMOTE
      else if(*(data + 2) == TNY_1311S_POWER)           g_remoteFlag = TNY_POWER_LONG_FLAG;
      else if(*(data + 2) == TNY_1311S_MODE)            g_remoteFlag = TNY_MODE_LONG_FLAG;
      else if(*(data + 2) == TNY_1311S_LEFT)            g_remoteFlag = TNY_LEFT_LONG_FLAG;
      else if(*(data + 2) == TNY_1311S_OK)              g_remoteFlag = TNY_OK_LONG_FLAG;
      else if(*(data + 2) == TNY_1311S_RIGHT)           g_remoteFlag = TNY_RIGHT_LONG_FLAG;
      else if(*(data + 2) == TNY_1311S_TIMER)           g_remoteFlag = TNY_TIMER_LONG_FLAG;
      else if(*(data + 2) == TNY_1311S_HUMAN)           g_remoteFlag = TNY_HUMAN_LONG_FLAG;
      else if(*(data + 2) == TNY_1311S_SETUP)           g_remoteFlag = TNY_SETUP_LONG_FLAG;
      else if(*(data + 2) == TNY_1311S_HOME)            g_remoteFlag = TNY_HOME_LONG_FLAG;
#endif
    }
  }
#endif  // USE_119SP_REMOTE
#if 0
  //#ifdef  USE_TNY_1311S_REMOTE
  else if (*data == TNY_1311S_CODE_FIRST && *(data + 1) == TNY_1311S_CODE_SECOND) {
    if(type == REMOTE_NORMAL_FLAG) {
      if(*(data + 2) == TNY_1311S_POWER)                g_remoteFlag = REMOTE_POWER_FLAG;
      else if(*(data + 2) == OKC_119SP_STERILIZATION)   g_remoteFlag = REMOTE_STERILIZATION_FLAG;
      else if(*(data + 2) == OKC_119SP_O3)              g_remoteFlag = REMOTE_O3_FLAG;
      else if(*(data + 2) == OKC_119SP_TIMER)           g_remoteFlag = REMOTE_TIMER_FLAG;
      else if(*(data + 2) == OKC_119SP_WIND)            g_remoteFlag = REMOTE_WIND_FLAG;
      else if(*(data + 2) == OKC_119SP_ION)             g_remoteFlag = REMOTE_ION_FLAG;
      else if(*(data + 2) == OKC_119SP_PLASMA)          g_remoteFlag = REMOTE_PLASMA_FLAG;
      else if(*(data + 2) == TNY_1311S_UP)              g_remoteFlag = REMOTE_UP_FLAG;
      else if(*(data + 2) == OKC_119SP_MENU)            g_remoteFlag = REMOTE_MENU_FLAG;
      else if(*(data + 2) == OKC_119SP_OK)              g_remoteFlag = REMOTE_OK_FLAG;
      else if(*(data + 2) == TNY_1311S_DOWN)            g_remoteFlag = REMOTE_DOWN_FLAG;
    }
    else if(type == REMOTE_LONG_FLAG) {
      if(*(data + 2) == TNY_1311S_POWER)                g_remoteFlag = REMOTE_POWER_LONG_FLAG;
      else if(*(data + 2) == OKC_119SP_STERILIZATION)   g_remoteFlag = REMOTE_STERILIZATION_LONG_FLAG;
      else if(*(data + 2) == OKC_119SP_O3)              g_remoteFlag = REMOTE_O3_LONG_FLAG;
      else if(*(data + 2) == OKC_119SP_TIMER)           g_remoteFlag = REMOTE_TIMER_LONG_FLAG;
      else if(*(data + 2) == OKC_119SP_WIND)            g_remoteFlag = REMOTE_WIND_LONG_FLAG;
      else if(*(data + 2) == OKC_119SP_ION)             g_remoteFlag = REMOTE_ION_LONG_FLAG;
      else if(*(data + 2) == OKC_119SP_PLASMA)          g_remoteFlag = REMOTE_PLASMA_LONG_FLAG;
      else if(*(data + 2) == TNY_1311S_UP)              g_remoteFlag = REMOTE_UP_LONG_FLAG;
      else if(*(data + 2) == OKC_119SP_MENU)            g_remoteFlag = REMOTE_MENU_LONG_FLAG;
      else if(*(data + 2) == OKC_119SP_OK)              g_remoteFlag = REMOTE_OK_LONG_FLAG;
      else if(*(data + 2) == TNY_1311S_DOWN)            g_remoteFlag = REMOTE_DOWN_LONG_FLAG;
    }
  }
#endif  // USE_TNY_1311S_REMOTE
}