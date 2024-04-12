#include "setting.h"
#include "eeprom.h"

extern unsigned int AsynchPrediv;
extern unsigned int SynchPrediv;

char *msg[4] = {"=>Setting Main Menu",
                  "=> 1 : DateSetting",
                  "=> 2 : TimeSetting",
                  "=> 3 : FACTORY RESET" 
};

func_t getShellCommand(unsigned char commandNum)
{
  switch(commandNum) {
  case '1':
    return dateSetting;
    break;
  case '2':
    return timeSetting;
    break;
  case '3':
    return factoryReset;
    break;
  }
  return NULL;
}

void settingMainMenu(void)
{
  unsigned char ch;
  digit_idx = 0;
  func_t fptr = NULL;
  
  for(int i = 0; i < sizeof(msg) / sizeof(msg[0]); i++)
    printf("%s\r\n", *(msg+i));  
  
  while(1) {
    IWDG_ReloadCounter();       // SJM 201102 added
    if(rx_head != rx_tail) {
      ch = rx_buffer[rx_tail];
      if (++rx_tail >= MAX_RX_BUFFER) rx_tail=0;
      fptr = getShellCommand(ch);
      if (fptr!=NULL)           // SJM 201113 add to avoid hang-up
        fptr();
      break;
    }
  }
}

void dateSetting()
{
  unsigned int yy, mm, dd, ww = 0;
  unsigned char nDatesOfMonth[13] = {0, 31, 29, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
  
  RTC_Config();
  
  SynchPrediv = 0xFF;
  AsynchPrediv = 0x7F;
  
  /* Configure the RTC data register and RTC prescaler */
  RTC_InitStructure.RTC_AsynchPrediv = 0x7F;//AsynchPrediv;
  RTC_InitStructure.RTC_SynchPrediv = 0xFF;//SynchPrediv;
  RTC_InitStructure.RTC_HourFormat = RTC_HourFormat_24;
  
  if (RTC_Init(&RTC_InitStructure) == ERROR)
  {
    printf("\n\r        /!\\***** RTC Prescaler Config failed ********/!\\ \n\r");
    return;
  }
  
  RTC_TimeStructure.RTC_H12     = RTC_H12_AM;
  
  printf("=>date Setting\r\n");

  while(1)
  {
    IWDG_ReloadCounter();
    printf("=>Year(00~99)\r\n");
    yy = getNum();
     
     if(yy > 0 && yy < 99)
     {
        printf("=>Year : %d\r\n", yy);
        RTC_DateStructure.RTC_Year = yy;
        break;  
     }
     else
     {
        printf("Wrong Number\r\n");
     }
  }


  while(1)
  {
    IWDG_ReloadCounter();
    printf("=>Month(1~12)\r\n");
    mm = getNum();
    
    if(mm > 0 && mm < 13)
    {
      printf("=>Month : %d\r\n", mm);
      RTC_DateStructure.RTC_Month = mm;
      break;
    }
    else
    {
      printf("Wrong Number\r\n");
    }
  }
  
    //
    // Calculate simple leap month.
    //
    if((yy % 4) == 0)
    {
        nDatesOfMonth[2] = 29;
    }
    else
    {
        nDatesOfMonth[2] = 28;
    }
  

  while(1)
  {
    IWDG_ReloadCounter();
    printf("=>Enter date(1-%d) : ", nDatesOfMonth[mm]);
    
    dd = getNum();
    if(dd >= 1 && dd <= nDatesOfMonth[mm])
    {
      printf("=>Day : %d\r\n", dd);
      RTC_DateStructure.RTC_Date = dd;
      break;
    }
    else
    {
      printf("=>Wrong number\r\n");  
    }
  }
  
  while(1)
  {
    IWDG_ReloadCounter();
    printf("=>Weekday(1~7, 1 is Monday)\r\n");
    ww = getNum();
    
    if(ww > 0 && ww < 8)
    {
      printf("=>Weekday : %d\r\n", ww);
      RTC_DateStructure.RTC_WeekDay = ww - 1;  
      break;
    }
    else
    {
      printf("=>Wrong number\r\n"); 
    }
  }

  if(RTC_SetDate(RTC_Format_BIN, &RTC_DateStructure) == ERROR)
  {
    printf("=>> !! RTC Set Date failed. !! <<\n\r");
  } 
  else
  {
    printf("=>> !! RTC Set Date success. !! <<\n\r");
    RTC_DateShow();
    /* Indicator for the RTC configuration */
    RTC_WriteBackupRegister(RTC_BKP_DR0, 0x32F2);
  }
  //Date Save!!!
}

void factoryReset()
{
//#ifndef HPA_36C  // SJM 200413 No UV-Lamp and Ozpne-Lamp in HPA_36C
#if ( MACHINE == HPA_130W)
      sysConfig.uvLampCountMin = 0;
      sysConfig.ozoneLampCountMin = 0;
      // SJM 200511 added Initialization
      sysConfig.ozoneLimit = OZONE_LIMIT_DEFAULT;
#endif
      sysConfig.filterCountMin = 0;
      sysConfig.rciOperatingMin = 0;
      sysConfig.prevState = DEFAULT_STATE;
      plasmaInfo.rsvOn = FALSE;
      plasmaInfo.rsvTime = 1;
      plasmaInfo.pidOn = TRUE;
      plasmaInfo.pwr = MAX_PLASMA_PWR;
      plasmaInfo.isScheduled = FALSE;
      
      systemWrite();          // Factory Reset by Remote Controller
//#ifndef HPA_36C  // SJM 200413 No UV-Lamp and Ozpne-Lamp in HPA_36C
#if ( MACHINE == HPA_130W)
  printf("factory reset(UV Lamp, ozone Lamp, RCI cell, Filter)\r\n");
#else
  printf("factory reset(RCI cell, Filter)\r\n");
#endif
}

void timeSetting()
{
  signed int hh, mm, ss = 0;
  
  if(rtc_init() == RTC_FAIL) return;
  
  RTC_TimeStructure.RTC_H12     = RTC_H12_AM;
  
  printf("=>time Setting\r\n");
  
  while(1)
  {
    IWDG_ReloadCounter();
    printf("=>Hour Setting(0~24)\r\n");
    hh = getNum();
    
    if(hh >= 0 && hh < 25)
    {
      printf("=>Hour : %d\r\n", hh);
      RTC_TimeStructure.RTC_Hours = hh;  
      break;
    }
    else
    {
      printf("=>Wrong number\r\n"); 
    }
  }
  
  while(1)
  {
    IWDG_ReloadCounter();
    printf("=>Minute Setting(0~59)\r\n");
    mm = getNum();
    
    if(mm >= 0  && mm < 60)
    {
      printf("=>Minute : %d\r\n", mm);
      RTC_TimeStructure.RTC_Minutes = mm;  
      break;
    }
    else
    {
      printf("=>Wrong number\r\n"); 
    }
  }
  
  while(1)
  {
    IWDG_ReloadCounter();
    printf("=>Second Setting(0~59)\r\n");
    ss = getNum();
    
    if(ss >= 0 && ss < 60)
    {
      printf("=>Second : %d\r\n", ss);
      RTC_TimeStructure.RTC_Seconds = ss;  
      break;
    }
    else
    {
      printf("=>Wrong number\r\n"); 
    }
  }
  
  /* Configure the RTC time register */
  if(RTC_SetTime(RTC_Format_BIN, &RTC_TimeStructure) == ERROR)
  {
    printf("\n\r>> !! RTC Set Time failed. !! <<\n\r");
  } 
  else
  {
    printf("\n\r>> !! RTC Set Time success. !! <<\n\r");
    RTC_TimeShow();
    /* Indicator for the RTC configuration */
    RTC_WriteBackupRegister(RTC_BKP_DR0, 0x32F2);
  }
}

unsigned int getNum(void)
{
    unsigned char ch = 0;
    char* ptrDigitNum;
    
    ptrDigitNum = digitNum;
    printf("=>");
    while(ch != 0xd)
    {
      IWDG_ReloadCounter();
      if(rx_head != rx_tail)
      {
       
        ch = rx_buffer[rx_tail];
        if(++rx_tail > MAX_RX_BUFFER) rx_tail = 0;
        digitNum[digit_idx] = ch;
        
          if(ch == 0xd)
          {
                  digitNum[digit_idx] = 0;
                  if (digit_idx!=0) {
                      printf("\r\n");
                      digit_idx = 0;
                      return digit_analysis(ptrDigitNum);
                  } 
                  digit_idx = 0;
                  printf("\r\n=>");
          }
          else
          {
              if (ch==0x8) {
                  if (digit_idx>0) {
                      --digit_idx;
                      digitNum[digit_idx] = 0;
                      printf("%c %c",ch,ch);
                  }  
              }
              else {
                  if (ch == 9) ch=0x20;
                  if (digit_idx<COMMAND_STR_MAX-2) {
                      if (ch==0x20 || (ch>='0' && ch <='9'))
                      {
                        printf("%c",ch);
                        digitNum[digit_idx++] = ch;
                        digitNum[digit_idx] = 0;
                      }
                  }
              }
          }
        }
    }
    
    //For error
    return 10000;
}



int digit_analysis(char *string)
{
    int minus    = 0;
    unsigned int result   = 0;
    int lastIndex;    

    if(string[0]=='-')
    {
        minus = 1;
        string++;
    }
    if(string[0]==0xd)
    {
      return 1000;
    }
    
    lastIndex = strlen((char*)string) - 1;
    
    if(lastIndex<0)
        return -1;
    
    result = atoi((char*)string);
    result = minus ? (-1*result):result;
    
    return result;
}

