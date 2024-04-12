/**
  * 0.5v - �ö�� ��忡�� ������ ������ ���� ��.(���� ��忡 �°�)
  *        ���� ���� �߰�
  * 0.6v - �ö�� ��忡�� ��ü���� ���� ����.
  *        ��ü ���� �߿��� ���� ���� �۵� ����.
  *        �ö�� ��忡�� ���� ���� ���� ����(0~3 -> 0~2)
  * 0.7v - ����� ��忡�� UV Lamp�� ���۽�Ŵ
  *        �ö�� ��忡�� �⺻ ���⸦ 1�� ����, ���� ����(0~2 -> 1~3)
  * 0.8v - �ö�� ��� ���� �� ���� ��ư�� ������ ���� ���Ⱑ ǥ�� ��.
  *        �Ҹ�ǰ ��ü Warningǥ�� �� Ȯ�� ��ư�� ������ Main���� �Ѿ� ��.
  */

/* Includes ------------------------------------------------------------------*/

//git 테스트를 해 본다.  다시바꾸어본다
#include "main.h"`

/** @addtogroup STM32F4xx_StdPeriph_Examples
  * @{
  */

/** @addtogroup SysTick_Example
  * @{
  */ 

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
unsigned int testLedBlink = FALSE;
unsigned char rx_buffer[MAX_RX_BUFFER];
unsigned char rx_head;
unsigned char rx_tail;
unsigned char setRTCFlag;
unsigned int m_remoteState;
extern unsigned char pBuf[];
extern unsigned int pPointer;
unsigned int oldPpointer;
unsigned char printfTest;
unsigned int segBlinkOnTime;

unsigned int currentState, isFirstEntrance;   // SJM 190711 never used , isReadyEntrance;
unsigned int destState;         // SJM 201121 added for consumable
unsigned char pidDetect;
unsigned int oneSecFlag;
unsigned int g_remoteFlag;

SYSTEMINFO sysConfig;
PLASMAINFO plasmaInfo;
DISINFO disInfo;
IONINFO ionInfo;

GPIO_InitTypeDef GPIO_InitStructure;
static __IO uint32_t TimingDelay;

void USART_SendString(USART_TypeDef* USARTx, uint8_t* string);
  
/* Private function prototypes -----------------------------------------------*/
void Delay(__IO uint32_t nTime);

/* Private functions ---------------------------------------------------------*/
extern void GetUARTData();
extern void handler();
extern void changeState(unsigned char state, unsigned char write);
extern void voicePlay(unsigned int voice, unsigned int delay);
extern void voicePortInit();

#ifdef  INCLUDE_IWDG
unsigned char isWatchDog = FALSE;
void IWDG_Init();
#endif  
/**
  * @brief   Main program
  * @param  None
  * @retval None
  */
#ifdef  INCLUDE_RETURN_TO_CONTINUOUS_MODE
extern unsigned char returnToContinuousOperation;
extern unsigned char prevOperation;
extern unsigned char continueOperation;
#endif

unsigned char  maxDispPower = 4;      // SJM 201117 used for HPA_36C only
//#ifdef  HPA_36C
#if ( MACHINE == HPA_36C)
extern unsigned char  quarterSecFlag;
short curBatLevel = 4;
unsigned int voltageValue;
unsigned int voltageLevel[MAX_LEVEL_VOLTAGE] = {SHUTDOWN_VOLTAGE,
          LOW_VOLTAGE, LOW_VOLTAGE_WARN,SAFE_VOLTAGE_WARN, SAFE_VOLTAGE};
extern unsigned char   blinkLED;
  #ifdef  INCLUDE_STOP_MODE
  extern unsigned char countIdleTime;  // flag to count Idle Time
  extern unsigned short idleTimeSec;
  extern unsigned char enterStopMode;
  extern unsigned char stopFlag;
  extern unsigned char intCounter;
  extern unsigned char stabilizedVolt;
  extern void setIdleTimer();
  extern void prepareRemoteSensor();      // remote port
  #define NORMAL_STOP 0
  #define FORCED_STOP 1
  #endif
  #ifdef  EXCLUDE_SUPPLIED_POWER
  unsigned char extPowerSupplied,prevExtPower;
  #endif

void inputPortInit();
void control_relayAllOff();
void checkSTOP(unsigned char mode);

void enterForcedSTOP()
{
  voicePlay(SWITCH_KEY, DELAY_KEY); Delay(DELAY_KEY);
  voicePlay(SWITCH_KEY, DELAY_KEY); Delay(DELAY_KEY);
  voicePlay(SWITCH_KEY, DELAY_KEY); Delay(DELAY_KEY);
  voicePlay(SWITCH_KEY, DELAY_KEY); Delay(DELAY_KEY);
  checkSTOP(FORCED_STOP);
}

#ifdef  LPP_V1_00_BD
  #ifdef  INCLUDE_CHARGE_CONTROL
  unsigned char chargeControl;
  #endif
#endif

unsigned char isBatteryInstalled()
{
#ifdef  LPP_V1_00_BD
  return GPIO_ReadInputDataBit(GPIOD,GPIO_Pin_3);
#else
  return TRUE;    // No Capability to find Battery installed
#endif
}

void dispBatteryStatus()
{
//  extern int voltageValue;
  if (GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_5)) {
    // External Power Supplied
    curBatLevel = LEVEL_SAFE_VOLTAGE;
    if (isBatteryInstalled()) {
      ledControl(LED_BAT_GRN1,LED_ON);
      ledControl(LED_BAT_GRN2,LED_ON);
      ledControl(LED_BAT_RED1,LED_OFF);
      ledControl(LED_BAT_RED2,LED_OFF);
      // Green On, Red Off  --- for prior LPP_V1_00_BD
      ledControl(LED_BATTERY_GREEN,LED_ON);
      ledControl(LED_BATTERY_RED,LED_OFF);
    }
    else {
      ledControl(LED_BAT_GRN1,LED_OFF);
      ledControl(LED_BAT_GRN2,LED_OFF);
      ledControl(LED_BAT_RED1,LED_OFF);
      ledControl(LED_BAT_RED2,LED_OFF);
    }
#ifdef  EXCLUDE_SUPPLIED_POWER
      extPowerSupplied = TRUE;
      if (prevExtPower!=extPowerSupplied) {
        setIdleTimer();   // reset
        prevExtPower=extPowerSupplied;
      }
#endif
  }
  else {
#ifdef  EXCLUDE_SUPPLIED_POWER
      extPowerSupplied = FALSE;
      if (prevExtPower!=extPowerSupplied) {
        setIdleTimer();   // set if required
        prevExtPower=extPowerSupplied;
      }
#endif
    if ((curBatLevel>LEVEL_SHUTDOWN_VOLTAGE)&&(voltageValue<=(voltageLevel[curBatLevel-1]-5)))
      curBatLevel--;
    else if ((curBatLevel<LEVEL_SAFE_VOLTAGE)&&(voltageValue>=(voltageLevel[curBatLevel]+5)))
             curBatLevel++;
//    if (curBatLevel<0)  curBatLevel=LEVEL_SHUTDOWN_VOLTAGE;
//    else if (curBatLevel>=MAX_LEVEL_VOLTAGE)  curBatLevel=LEVEL_SAFE_VOLTAGE;
#ifdef INCLUDE_BATTERY_CHECKER
    switch(curBatLevel) {
      case LEVEL_SHUTDOWN_VOLTAGE :
        // ShutDown
        control_relayAllOff();    // ledAllOff() will be called in checkSTOP()
        ledControl(LED_BATTERY_GREEN,LED_OFF);
        ledControl(LED_BATTERY_RED,LED_OFF);
  #ifdef  LPP_V1_00_BD
        ledControl(LED_BAT_GRN1,LED_OFF);
        ledControl(LED_BAT_GRN2,LED_OFF);
        ledControl(LED_BAT_RED1,blinkLED);
        ledControl(LED_BAT_RED2,blinkLED);
  #endif
        if (stabilizedVolt) enterForcedSTOP();
        break;
      case LEVEL_LOW_VOLTAGE :
      // Green Off, Red On
        ledControl(LED_BATTERY_GREEN,LED_OFF);
        ledControl(LED_BATTERY_RED,LED_ON);
  #ifdef  LPP_V1_00_BD
        ledControl(LED_BAT_GRN1,LED_OFF);
        ledControl(LED_BAT_GRN2,LED_OFF);
        ledControl(LED_BAT_RED1,LED_OFF);
        ledControl(LED_BAT_RED2,blinkLED);
  #endif
        break;
      case LEVEL_LOW_VOLTAGE_WARN :
        ledControl(LED_BATTERY_GREEN,LED_OFF);
        ledControl(LED_BATTERY_RED,blinkLED);       // blink
  #ifdef  LPP_V1_00_BD
        ledControl(LED_BAT_GRN1,LED_OFF);
        ledControl(LED_BAT_GRN2,LED_OFF);
        ledControl(LED_BAT_RED1,LED_OFF);
        ledControl(LED_BAT_RED2,LED_ON);
  #endif
        break;
      case LEVEL_SAFE_VOLTAGE_WARN :
        ledControl(LED_BATTERY_GREEN,blinkLED);    // blink
        ledControl(LED_BATTERY_RED,LED_OFF);
  #ifdef  LPP_V1_00_BD
        ledControl(LED_BAT_GRN1,LED_OFF);
        ledControl(LED_BAT_GRN2,LED_ON);
        ledControl(LED_BAT_RED1,LED_OFF);
        ledControl(LED_BAT_RED2,LED_OFF);
  #endif
        break;
      case LEVEL_SAFE_VOLTAGE :
        // Green On, Red Off
        ledControl(LED_BATTERY_GREEN,LED_ON);
        ledControl(LED_BATTERY_RED,LED_OFF);
  #ifdef  LPP_V1_00_BD
        ledControl(LED_BAT_GRN1,LED_ON);
        ledControl(LED_BAT_GRN2,LED_ON);
        ledControl(LED_BAT_RED1,LED_OFF);
        ledControl(LED_BAT_RED2,LED_OFF);
  #endif
        break;
    }
  #if 0
    if (voltageValue<=SHUTDOWN_VOLTAGE) {
      // ShutDown
      control_relayAllOff();    // ledAllOff() will be called in checkSTOP()
      ledControl(LED_BATTERY_GREEN,LED_OFF);
      ledControl(LED_BATTERY_RED,LED_OFF);
      // checkSTOP();   // immediately!
    }
    else if (voltageValue<=LOW_VOLTAGE) {
      // Green Off, Red On
      ledControl(LED_BATTERY_GREEN,LED_OFF);
      ledControl(LED_BATTERY_RED,LED_ON);
    }
    else if (voltageValue<=LOW_VOLTAGE_WARN) {
      ledControl(LED_BATTERY_GREEN,LED_OFF);
      ledControl(LED_BATTERY_RED,blinkLED);       // blink
    }
    else if (voltageValue<=SAFE_VOLTAGE_WARN) {
      ledControl(LED_BATTERY_GREEN,blinkLED);    // blink
      ledControl(LED_BATTERY_RED,LED_OFF);
    }
    else {
      // Green On, Red Off
      ledControl(LED_BATTERY_GREEN,LED_ON);
      ledControl(LED_BATTERY_RED,LED_OFF);
    }
  #endif
#else
      // Green On, Red Off
      ledControl(LED_BATTERY_GREEN,LED_ON);
      ledControl(LED_BATTERY_RED,LED_OFF);
#endif
  }
}

void intensityLED(unsigned char strength)
{
  ledControl(LED_INTENSITY_3,LED_OFF);
  ledControl(LED_INTENSITY_2,LED_OFF);
  ledControl(LED_INTENSITY_1,LED_OFF);
  ledControl(LED_INTENSITY_0,LED_OFF);
  switch(strength) {
    case 4 :  ledControl(LED_INTENSITY_3,LED_ON); // fall-through
    case 3 :  ledControl(LED_INTENSITY_2,LED_ON); // fall-through
    case 2 :  ledControl(LED_INTENSITY_1,LED_ON); // fall-through
    case 1 :  ledControl(LED_INTENSITY_0,LED_ON); // fall-through
      break;
  }
}

void dispIntensityStatus()
{
  static unsigned char curPower = 0;
    
  if (quarterSecFlag) {
    quarterSecFlag = FALSE;
    switch (currentState) {
      case STATE_POWER_OFF :
        intensityLED(0);
        break;
      case STATE_STER :           // fall-through
      case STATE_DIS :            // fall-through
      case STATE_ION :            // fall-through
      case STATE_DESTRUCTION :    // fall-through
        intensityLED(curPower);
        curPower++;
        if (curPower>maxDispPower) curPower = 0;
        break;
      default :
        intensityLED(maxDispPower);
        break;
    }
  }
}

void displayHandler()
{
  dispBatteryStatus();
  dispIntensityStatus();
}
#endif // ( MACHINE == HPA_36C)

#ifdef INCLUDE_STOP_MODE
void RTC_Alarm_IRQHandler()
{
    if(RTC_GetITStatus(RTC_IT_ALRA) != RESET) {
 
        RTC_ClearITPendingBit(RTC_IT_ALRA);
        EXTI_ClearITPendingBit(EXTI_Line17);
        printf("\r\n [RTC_Alarm occured!!!]\r\n"); 
        NVIC_SystemReset();
//        GPIOC->ODR &= ~GPIO_Pin_11;
//        delay_ms(200);
//        GPIOC->ODR |= GPIO_Pin_11;
    }
}

void initStop(unsigned short hour, unsigned short min, unsigned short sec)
{
  EXTI_InitTypeDef EXTI_InitStructure;
  RTC_AlarmTypeDef alarm;

  // configure EXTI Line 17
  EXTI_ClearITPendingBit(EXTI_Line17);
  EXTI_InitStructure.EXTI_Line = EXTI_Line17;
  EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
  EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;
  EXTI_InitStructure.EXTI_LineCmd = ENABLE;
  EXTI_Init(&EXTI_InitStructure);
  // Enable the RTC Alarm Interrupt
  RTC_ITConfig(RTC_IT_ALRA,ENABLE);
  // configure the RTC
/**
  * @brief  Set the specified RTC Alarm.
  * @note   The Alarm register can only be written when the corresponding Alarm
  *         is disabled (Use the RTC_AlarmCmd(DISABLE)).    
  * @param  RTC_Format: specifies the format of the returned parameters.
  *          This parameter can be one of the following values:
  *            @arg RTC_Format_BIN: Binary data format 
  *            @arg RTC_Format_BCD: BCD data format
  * @param  RTC_Alarm: specifies the alarm to be configured.
  *          This parameter can be one of the following values:
  *            @arg RTC_Alarm_A: to select Alarm A
  *            @arg RTC_Alarm_B: to select Alarm B  
  * @param  RTC_AlarmStruct: pointer to a RTC_AlarmTypeDef structure that 
  *                          contains the alarm configuration parameters.     
  * @retval None
  */
//void RTC_SetAlarm(uint32_t RTC_Format, uint32_t RTC_Alarm, RTC_AlarmTypeDef* RTC_AlarmStruct)
  RTC_AlarmCmd(RTC_Alarm_A,DISABLE);
  RTC_AlarmStructInit(&alarm);
  alarm.RTC_AlarmTime.RTC_Hours = hour;
  alarm.RTC_AlarmTime.RTC_Minutes = min;
  alarm.RTC_AlarmTime.RTC_Seconds = sec;
  alarm.RTC_AlarmMask = RTC_AlarmMask_All & (~RTC_AlarmMask_Hours)
    & (~RTC_AlarmMask_Minutes) & (~RTC_AlarmMask_Seconds);
  RTC_SetAlarm(RTC_Format_BIN,RTC_Alarm_A,&alarm);
  // NVIC enable
  NVIC_EnableIRQ(RTC_Alarm_IRQn);
  RTC_AlarmCmd(RTC_Alarm_A,ENABLE);
}

void checkSTOP(unsigned char mode)
{
  short gap, hour;
  
  if (plasmaInfo.rsvOn == TRUE) {
    RTC_GetTime(RTC_Format_BIN, &RTC_TimeStructure);
    (void)RTC->DR;
    // Do not Enter STOP mode 5 or less minutes before reservation time.
//    if (RTC_TimeStructure.RTC_Minutes>=60-TIME_TO_RESERVATION) {
    if ((RTC_TimeStructure.RTC_Minutes>=60-TIME_TO_RESERVATION)&&(mode==NORMAL_STOP)) {
      gap = plasmaInfo.rsvTime - RTC_TimeStructure.RTC_Hours;
      if ((gap==1)||(gap==-23)) {
        printf("\r\n [%02d:%02d => Do Not Enter STOP Mode ] reserve at %02dH\r\n",
               RTC_TimeStructure.RTC_Hours,RTC_TimeStructure.RTC_Minutes,plasmaInfo.rsvTime);
       countIdleTime = FALSE;
       enterStopMode = FALSE;
       idleTimeSec = 0;
       return;
      }
    }
    else {
      hour = plasmaInfo.rsvTime-1;
      if (hour<0) hour = 23;
      printf("\r\n[%02d:%02d => AlarmSet ] rsv = %02d, %02d:59:50\r\n",
             RTC_TimeStructure.RTC_Hours,RTC_TimeStructure.RTC_Minutes,
             plasmaInfo.rsvTime,hour);
      // set RTC Alarm
      initStop(hour,59,50);
    }
  }
  // enter STOP mode
    ledAllOff();
    currentState = STATE_POWER_OFF; // SJM 200825 because of pir_White ???
    sysConfig.stopped = TRUE;
    systemWrite();
    GPIO_DeInit(GPIOC);     // alternative to currentState = STATE_POWER_OFF;
    printf("\r\n [prepareRemoteSensor]\r\n");
    prepareRemoteSensor();
    printf("\r\n [POWER OFF! PA15->0 ]\r\n");
    GPIO_WriteBit(GPIOA,GPIO_Pin_15,Bit_RESET);
    Delay(100);
    stopFlag = 1;
    intCounter = 0;
    printf("\r\n [Enter STOP Mode] => ON,WFI\r\n");
    PWR_EnterSTOPMode(PWR_Regulator_LowPower,PWR_STOPEntry_WFI);
}
#endif
//#ifdef  MONEY_STERILIZER
#if ( MACHINE == MONEY_STERILIZER)
void keyHandler()
{
  
  if (g_keyFlag) {
    printf("\r\n Key = 0x%08x \r\n", g_keyFlag);
    if ((g_keyFlag&KEY_START)||(g_keyFlag&KEY_START_LONG))
      changeState(STATE_MONEY_STER,TRUE);
    else if ((g_keyFlag&KEY_INC)||(g_keyFlag&KEY_INC_LONG)) {
      sysConfig.sterTime += 10;
      if (sysConfig.sterTime>MAX_STER_TIME)
        sysConfig.sterTime = MIN_STER_TIME;
      sysConfig.decompTime = sysConfig.sterTime/2;
    }
    g_keyFlag = 0;
  }
  if (sysConfig.sterTime>=MAX_STER_TIME)
    segmentControl(99);
  else
    segmentControl(sysConfig.sterTime);
}
#endif // ( MACHINE == MONEY_STERILIZER)
#ifdef  INCLUDE_BATTERY_CHECKER
extern unsigned char  batteryCheckFlag;
void printBatteryVoltage()
{
  if (batteryCheckFlag) {
    batteryCheckFlag = FALSE;
    printf("\r\n Battery1 = %d, Level=%d, Ext=%d\r\n",voltageValue,curBatLevel,
           extPowerSupplied);
#ifdef  LPP_V1_00_BD
  #ifdef  INCLUDE_CHARGE_CONTROL
    printf("BatteryInstalled = %d, ChargeControl = %d\r\n", isBatteryInstalled(),
           chargeControl);
  #else
    printf("BatteryInstalled = %d\r\n", isBatteryInstalled());
  #endif  // INCLUDE_CHARGE_CONTROL
#endif  // LPP_V1_00_BD
    printf(" countIdleTime=%d, idleTimeSec=%d, enterStopMode=%d\r\n",
           countIdleTime, idleTimeSec, enterStopMode);
  }
}
#endif

#ifdef  INCLUDE_DIP_SWITCH
  unsigned char statusDIP1 = 1;                   // SJM 200619 default value = 1;
#endif

//void USART6_SendString(char *str){
//  while(*str){
//      while(USART_GetFlagStatus(USART6, USART6_FLAG_TXE) == RESET);
//      USART_SendData(USART6, *str);
//      str++;
//  }
//}

int counter = 0;

// ms ������ ������ �Լ�
void delay_ms(uint32_t ms) {
    volatile uint32_t i, j;
    for (i = 0; i < ms; i++)
        for (j = 0; j < 3195; j++);
}

// UART3�� ���� �����ϴ� �Լ�
void UART3_Write(char c) {
    while (!(USART3->SR & USART_SR_TXE)); // ���� ���۰� ����ִ��� Ȯ��
    USART3->DR = c; // ���� ����
}

// ���� �������� �����͸� ����ϴ� �Լ�
void printBinary(uint8_t data) {
    for (int i = 7; i >= 0; i--) {
        if (data & (1 << i))
            UART3_Write('1');
        else
            UART3_Write('0');
    }
    
     UART3_Write('\n'); // �� �ٲ� ���� ����
}


//kwon: uart6��  ����Ÿ ����
void USART_SendString(USART_TypeDef* USARTx, uint8_t* string)
{
  
   printf("\r\n USARt_SendString ");
    while (*string)
    {
        // Wait until transmit data register is empty
//       printf("\r\n data sent  before");
//       printf("\r\n  char = %c", *string);


        // Send a character to usart6
      USART_SendData(USARTx, *string & 0xff) ;

       while (USART_GetFlagStatus(USART6, USART_FLAG_TC) == RESET) {}
        string++;

        
//        printf("\r\n data sent success");

        
    }
}

void USART6_Transmit(uint8_t data) {
    while (!(USART6->SR & USART_SR_TXE));
    USART6->DR = (data & (uint16_t)0x01FF);
}



//kwon: 2024-4-9 : uart6�� ������ ������ ó���ϴ� ��ƾ
void serial_input_cmd_handler() {
  
  
  //swtichInfo �� �ִ� ������ �о ó���� �Ѵ�. 
  
   if(strlen(switchInfo.switchType) >0 ){
          
            printf("main.c switch type = %s \r\n", switchInfo.switchType);
            printf("main.c switch state = %s \r\n", switchInfo.switchState);
            
          
            if(strcmp(switchInfo.switchType, "power")  == 0 && strcmp(switchInfo.switchState, "on") == 0) {
               printf("\r\n rx power on from serial-6 \r\n");
               
               g_remoteFlag = TNY_POWER_FLAG;
                      
            }
            else if(strcmp(switchInfo.switchType, "power")  == 0 && strcmp(switchInfo.switchState, "off") == 0) {
               printf("\r\n rx power off from serial-6 \r\n");
               
               g_remoteFlag = TNY_POWER_FLAG;
                      
            }
                      
            else if(strcmp(switchInfo.switchType, "wind")  == 0 && strcmp(switchInfo.switchState, "1") == 0) {
               printf("\r\n rx wind weak from serial-6 \r\n");
             
            }
            else if(strcmp(switchInfo.switchType, "wind")  == 0 && strcmp(switchInfo.switchState, "2") == 0) {
               printf("\r\n rx wind medium from serial-6 \r\n");
             
            }
            else if(strcmp(switchInfo.switchType, "wind")  == 0 && strcmp(switchInfo.switchState, "3") == 0) {
                           printf("\r\n rx wind strong medium from serial-6 \r\n");
                         
            } 
             else if(strcmp(switchInfo.switchType, "duration")  == 0 && strcmp(switchInfo.switchState, "30") == 0) {
               printf("\r\n rx duration - STATE_SERIAL_READY_DIS : 30 from serial-6 \r\n");
              //  g_remoteFlag = TNY_MODE_FLAG;

               changeState(STATE_SERIAL_READY_DIS, FALSE);
                         
            } 
             else if(strcmp(switchInfo.switchType, "duration")  == 0 && strcmp(switchInfo.switchState, "60") == 0) {
               printf("\r\n rx duration: 60 from serial-6 \r\n");
               g_remoteFlag = TNY_MODE_FLAG;
                         
            } 
             else if(strcmp(switchInfo.switchType, "duration")  == 0 && strcmp(switchInfo.switchState, "90") == 0) {
               printf("\r\n rx duration: 90 from serial-6 \r\n");
               g_remoteFlag = TNY_MODE_FLAG;
                         
            } 
            
            strcpy(switchInfo.switchState, "");
            strcpy(switchInfo.switchType, "");
            
        }
}


int main(void)
{
  GPIO_InitTypeDef  GPIO_InitStructure;
  
  /* Enable the GPIO_LED Clock */
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);
  
  /* Configure the GPIO_LED pin */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIOB, &GPIO_InitStructure);

  if (SysTick_Config(SystemCoreClock / 1000)) { 
    /* Capture error */ 
    while (1);
  }

  TIM_Config();
  relayControlInit();   //relay port Init
                        // SJM 200820 Move to here because of MainPowerControl
  remoteTimerInit();
  remoteInit();
  Uart_init();
  
  //kwon ozs port uart6 init
  Uart6_init();
  
  
//#ifdef  HPA_36C
#if ( MACHINE == HPA_36C)
  inputPortInit();
#endif

#if 0
  if (RCC_GetFlagStatus(RCC_FLAG_IWDGRST) != RESET) { 
    /* WWDGRST flag set */
    /* Turn on LED1 */
    //STM_EVAL_LEDOn(LED1);
    printf("\r\n IWDG Reset occured!\r\n");
    /* Clear reset flags */
//    RCC_ClearFlag();
  }
  else {
    printf("\r\n IWDG Reset was not occured!\r\n");
  }
#endif
  pirInit();
  sEE_Init();    //24LC512 Init
//  relayControlInit();   //relay port Init => Move to upper. SJM 200820
//#ifndef HPA_36C     // SJM 200820 Key is not used for HPA_36C
#if ( MACHINE != HPA_36C)   // SJM 201112 Key port is also used in MoneySterilizer
  keyPortInit();
#endif
  ledPortInit();
  ISD1760_init();
  voicePortInit(); //voice Port init

  ozoneSensorInit();    // SJM 200820 ADC is used for Vbat in HPA_36C
  ADC_SoftwareStartConv(ADC1);
  
  GPIOB->BSRRL = GPIO_Pin_8;
  Delay(10);
  
  rtc_check();
  rtc_init();
  
  printfTest = FALSE;
//#ifdef  HPA_36C
#if ( MACHINE == HPA_36C)
  printf("\r\n  Compact Type : HPA-36C\r\n");
  #ifdef  EXCLUDE_SUPPLIED_POWER
  extPowerSupplied = prevExtPower = GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_5);
  #endif
//#else
#elif ( MACHINE == HPA_130W )
  printf("\r\n  Wall Type  : HPA-130W\r\n");
#endif

  printf("\r\n  kwonilgun : git hub test 2024-4-11 \r\n");



  printf("\r\n   Ver %d (Language:%d)\r\n", SW_VERSION,LANGUAGE);
  printf("\r\n  HealthWell Medical Inc. 0401-01 \r\n");
  
  
   // Send "Hello, World!" through USART6
 
   
  
  
  
//#ifndef   MONEY_STERILIZER

 
  /* Check if the system has resumed from WWDG reset */
  if (RCC_GetFlagStatus(RCC_FLAG_IWDGRST) != RESET) { 
    /* WWDGRST flag set */
    /* Turn on LED1 */
    isWatchDog = TRUE;
    //STM_EVAL_LEDOn(LED1);
    printf("\r\n IWDG Reset occured!\r\n");
    /* Clear reset flags */
    RCC_ClearFlag();
  }
  else{
    isWatchDog = FALSE;
  }
  IWDG_Init();



    changeState(STATE_POWER_OFF, FALSE);  // SJM 201120 change to changeState()
    //system loading
    systemRead();
 


//#ifndef HPA_36C     // SJM below line is used for HPA-130W
#if ( MAHINE == HPA_130W )
  #ifdef  INCLUDE_DIP_SWITCH
  statusDIP1 = GPIO_ReadInputDataBit(GPIOD,GPIO_Pin_8);
  #endif
#endif  // HPA-36C
  
  while (1) {
#ifdef  INCLUDE_IWDG     
    IWDG_ReloadCounter();
#endif
    GetUARTData();
#ifdef  INCLUDE_BATTERY_CHECKER
    printBatteryVoltage();
#endif
#ifdef INCLUDE_STOP_MODE
    if (enterStopMode)
      checkSTOP(NORMAL_STOP);
#endif
    handler();
//#ifdef  HPA_36C
#if ( MACHINE == HPA_36C )
    displayHandler();
#endif
    if (g_keyFlag) {
      if ((g_keyFlag&KEY_UPGRADE)==KEY_UPGRADE) {
        voicePlay(SWITCH_KEY, DELAY_KEY);
        Delay(DELAY_KEY);
        UpgradeEEPWrite(UpgradeEEPdata);
        NVIC_SystemReset();
        g_keyFlag = 0;      // SJM 200427 clear flag only when expected key.
                            // Fortunately, here is the only place to check key....
      }
    }
    if(testLedBlink == TRUE) {
      testLedBlink = FALSE;
      GPIOB->ODR ^= GPIO_Pin_8;
      //GPIOA->ODR ^= GPIO_Pin_14;
    }
//#ifndef MONEY_STERILIZER
#if ( MACHINE != MONEY_STERILIZER )
         // SJM 190704 ???
//    if(pidDetect == TRUE) pidDetect = FALSE;
//    if (currentState != STATE_POWER_OFF)      SJM 200409 Remove to eliminate confusion
//      ledControl(LED_PID_FONT,pidDetect);
    //���� ����.
  #ifndef INCLUDE_RETURN_TO_CONTINUOUS_MODE
    if(currentState == STATE_POWER_OFF || currentState == STATE_READY_STER ||
       currentState == STATE_READY_ION || currentState == STATE_READY_DIS ) {
  #else
    if(currentState == STATE_POWER_OFF || currentState == STATE_READY_STER || currentState == STATE_STER_STOP ||
       currentState == STATE_READY_ION || currentState == STATE_ION_STOP || 
       currentState == STATE_READY_DIS || currentState == STATE_DIS_STOP ||
       (((currentState==STATE_DIS)||(currentState==STATE_ION))&&(continueOperation))) {
  #endif
      //1�ʸ���.
      if(oneSecFlag == TRUE) {
        
        //kwon :2024-4-9 , power on off 
        
        serial_input_cmd_handler();
         
         
         
          oneSecFlag = FALSE;
          RTC_GetTime(RTC_Format_BIN, &RTC_TimeStructure);
          (void)RTC->DR;
          if(plasmaInfo.rsvOn == TRUE) {
          // SJM 190928 �ð��� check�ϸ� �߰��� �ߴ��ص� 1�ð������� ��� �ݺ��� ����� ����.
          // ==> �ѹ� ���۽�Ų ���Ŀ��� �ٽ� ������ �ȵ�.
            if ((RTC_TimeStructure.RTC_Hours == plasmaInfo.rsvTime)&&
                (RTC_TimeStructure.RTC_Minutes == 0)&&(RTC_TimeStructure.RTC_Seconds == 0)) {
    #ifdef  INCLUDE_RETURN_TO_CONTINUOUS_MODE
//              if (((currentState==STATE_DIS)||(currentState==STATE_ION))&&(continueOperation))
//                returnToContinuousOperation = TRUE;
//              else  returnToContinuousOperation = FALSE;
              switch (currentState) {
                case STATE_DIS       :  if (continueOperation) {
                                          returnToContinuousOperation = TRUE;
                                          prevOperation = STATE_DIS;
                                        }
                                        else {
                                          returnToContinuousOperation = FALSE;
                                          prevOperation = STATE_READY_DIS;
                                        }
                                        break;
                case STATE_ION       :  if (continueOperation) {
                                          returnToContinuousOperation = TRUE;
                                          prevOperation = STATE_ION;
                                        }
                                        else {
                                          returnToContinuousOperation = FALSE;
                                          prevOperation = STATE_READY_ION;
                                        }
                                        break;
                case STATE_INIT      :  // fall-through
                case STATE_POWER_OFF :  prevOperation = STATE_POWER_OFF;
                                        returnToContinuousOperation = FALSE;
                                        break;
                case STATE_READY_ION :  // fall-through
                case STATE_ION_STOP :   prevOperation = STATE_READY_ION;
                                        returnToContinuousOperation = FALSE;
                                        break;
                case STATE_READY_STER : // fall-through
                case STATE_STER_STOP :  prevOperation = STATE_READY_STER;
                                        returnToContinuousOperation = FALSE;
                                        break;
                case STATE_READY_DIS :  // fall-through
                case STATE_DIS_STOP :   
                default :               prevOperation = STATE_READY_DIS;
                                        returnToContinuousOperation = FALSE;
                                        break;
              }
//              if (currentState==STATE_INIT) prevOperation = STATE_POWER_OFF;
//              else                          prevOperation = currentState;
    #endif  // INCLUDE_RETURN_TO_CONTINUOUS_MODE
              changeState(STATE_STER,TRUE);    // SJM 201113 FALSE-->TRUE
                                               // to detect EEPROM Error
              plasmaInfo.isScheduled = TRUE;
            }
          }
      }
    }
#endif  // MONEY_STERILIZER
    if(pPointer != oldPpointer) {
      printf("%c", pBuf[oldPpointer]);
      if(++oldPpointer > MAX_SIZE_PRINT)
        oldPpointer = 0;
    }
  }
}

/**
  * @brief  Inserts a delay time.
  * @param  nTime: specifies the delay time length, in milliseconds.
  * @retval None
  */
void Delay(__IO uint32_t nTime)
{ 
  TimingDelay = nTime;

  while(TimingDelay != 0);
}

/**
  * @brief  Decrements the TimingDelay variable.
  * @param  None
  * @retval None
  */
void TimingDelay_Decrement(void)
{
  if (TimingDelay != 0x00)
  { 
    TimingDelay--;
  }
}

#ifdef  USE_FULL_ASSERT

/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t* file, uint32_t line)
{ 
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

  /* Infinite loop */
  while (1)
  {
  }
}

uint32_t sEE_TIMEOUT_UserCallback(void)
{
  
  printf("eeprom error11\r\n");
  
  /* Block communication and all processes */
  /*while (1)
  {   
  }  */
  return 0;
}

#endif

/**
  * @}
  */ 

/**
  * @}
  */ 

/******************* (C) COPYRIGHT 2011 STMicroelectronics *****END OF FILE****/
