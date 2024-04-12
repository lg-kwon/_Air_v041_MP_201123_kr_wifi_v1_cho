#include "LED.h"

extern unsigned int g_60hz;
extern unsigned char pidDetectedStart;
//extern unsigned int pidLEDOnFlag;                   SJM 190711 never used

unsigned int G1_SF;
unsigned int G2_SF;
unsigned int G3_SF;
unsigned int G4_SF;
unsigned int G5_SF;

/***********************************************************************************************************************
 * key Port Init
 *
 **********************************************************************************************************************/
void ledPortInit()
{
  GPIO_InitTypeDef  GPIO_InitStructure;
  
  /* Enable the Clock */
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE);
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);
    
  /* Configure the pin : GPIOB => LED_5TH~LED_1ST */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9 | GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;//GPIO_PuPd_UP;////
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIOB, &GPIO_InitStructure);
  
  /* Configure the pin : GPIOC => DP0,DP1, LED_G~LED_C */
#ifdef  ENABLE_DP
// kwon  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3 | GPIO_Pin_6 | GPIO_Pin_7 | GPIO_Pin_8 | GPIO_Pin_9 | GPIO_Pin_10 | GPIO_Pin_11;
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3 | GPIO_Pin_8 | GPIO_Pin_9 | GPIO_Pin_10 | GPIO_Pin_11;
#else
//  kwon  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7 | GPIO_Pin_8 | GPIO_Pin_9 | GPIO_Pin_10 | GPIO_Pin_11;
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8 | GPIO_Pin_9 | GPIO_Pin_10 | GPIO_Pin_11;
#endif
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;//GPIO_PuPd_UP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIOC, &GPIO_InitStructure);
  
  /* Configure the pin : GPIOE => LED_B~LED_A */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;//GPIO_PuPd_UP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIOE, &GPIO_InitStructure);
 
#ifdef  INCLUDE_CHARGE_CONTROL    // SJM 201015 BAT_CNT added
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIOD, &GPIO_InitStructure);
#endif

  /* Configure the pin : GPIOD => LED1~LED3 ==>PIR B/D */
// kwon GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7;
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;//GPIO_PuPd_UP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIOD, &GPIO_InitStructure); 
}

/*
void ledTest()
{
//  printf("ledTest!\r\n");
 
    GPIOB->BSRRL = GPIO_Pin_9;// | GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15;
//    GPIOC->BSRRL = GPIO_Pin_6 | GPIO_Pin_7 | GPIO_Pin_8 | GPIO_Pin_9 | GPIO_Pin_10 | GPIO_Pin_11;
//    GPIOE->BSRRL = GPIO_Pin_0 | GPIO_Pin_1;
//    GPIOD->BSRRL = GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7;    
    
    Delay(1000);
    
    GPIOB->BSRRH = GPIO_Pin_9;// | GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15;
//    GPIOC->BSRRH = GPIO_Pin_6 | GPIO_Pin_7 | GPIO_Pin_8 | GPIO_Pin_9 | GPIO_Pin_10 | GPIO_Pin_11;
//    GPIOE->BSRRH = GPIO_Pin_0 | GPIO_Pin_1;
//    GPIOD->BSRRH = GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7;
}
*/
void ledTest()
{
  
  printf("ledTest!\r\n");

    ledControl(7, LED_ON);
    ledControl(8, LED_ON);
    G1_SF = 0;
    G5_SF = 0;
//    GPIOE->BSRRL = GPIO_Pin_0 | GPIO_Pin_1;  
//    GPIOB->BSRRH = GPIO_Pin_13;
//    GPIOB->BSRRL = GPIO_Pin_9 | GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15;
//    GPIOC->BSRRL = GPIO_Pin_6 | GPIO_Pin_7 | GPIO_Pin_8 | GPIO_Pin_9 | GPIO_Pin_10 | GPIO_Pin_11;
//    GPIOE->BSRRL = GPIO_Pin_0 | GPIO_Pin_1;

    
    Delay(1000);

    ledControl(7, LED_OFF);
    ledControl(8, LED_OFF);
    G1_SF = 0;
    G5_SF = 0;
//    GPIOE->BSRRL = GPIO_Pin_0 | GPIO_Pin_1;    
//    GPIOB->BSRRL = GPIO_Pin_13;
//    GPIOB->BSRRH = GPIO_Pin_9 | GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15;
//    GPIOC->BSRRH = GPIO_Pin_6 | GPIO_Pin_7 | GPIO_Pin_8 | GPIO_Pin_9 | GPIO_Pin_10 | GPIO_Pin_11;
//    GPIOE->BSRRH = GPIO_Pin_0 | GPIO_Pin_1;

}

void ledAllOff()
{
    G1_SF = 0;
    G2_SF = 0;
    G5_SF = 0;
    G3_SF = 0;      // SJM 190715 add to off segment
    G4_SF = 0;      // SJM 190715 add to off segment

    GPIOD->BSRRH = GPIO_Pin_5;
    GPIOD->BSRRH = GPIO_Pin_6;
    GPIOD->BSRRH = GPIO_Pin_7;    
}

void ledAllOn()         // SJM 190703 newly added to be used in handleBoardDebug()
{
      G1_SF = 0xff;
      G2_SF = 0xff;
      G3_SF = 0xff;
      G4_SF = 0xff;
      G5_SF = 0xff;
}

void segmentOff()
{
  G3_SF = 0;
  G4_SF = 0;
}

void ledControl(unsigned int control, unsigned char onoff)
{
  switch(control) {
#if ( MACHINE == HPA_130W )
  case LED_PLASMA_FONT: if(onoff == LED_ON) G1_SF |= 0x01; else G1_SF &= ~0x01; break;
  case LED_DIS_FONT: if(onoff == LED_ON) G1_SF |= 0x02; else G1_SF &= ~0x02; break;
  case LED_ION_FONT: if(onoff == LED_ON) G1_SF |= 0x04; else G1_SF &= ~0x04; break;
  case LED_RESERVE_FONT: if(onoff == LED_ON) G1_SF |= 0x08; else G1_SF &= ~0x08; break;
  
  case LED_POWER_FONT: if(onoff == LED_ON) G1_SF |= 0x10; else G1_SF &= ~0x10; break;
  case LED_PID_FONT: if(onoff == LED_ON) G1_SF |= 0x20; else G1_SF &= ~0x20; break;
  
  case LED_POWER_SIG1:  if(onoff == LED_ON) G2_SF |= 0x01; else G2_SF &= ~0x01; break;
  case LED_POWER_SIG2: if(onoff == LED_ON) G2_SF |= 0x02; else G2_SF &= ~0x02; break;
  case LED_MODE_SIG1: if(onoff == LED_ON) G2_SF |= 0x04; else G2_SF &= ~0x04; break;
  case LED_MODE_SIG2: if(onoff == LED_ON) G2_SF |= 0x08; else G2_SF &= ~0x08; break;
  
  case LED_SETTING_SIG1: if(onoff == LED_ON) G2_SF |= 0x10; else G2_SF &= ~0x10; break;
  case LED_SETTING_SIG2: if(onoff == LED_ON) G2_SF |= 0x20; else G2_SF &= ~0x20; break;
  case LED_POWER_ON: if(onoff == LED_ON) G5_SF |= 0x10; else G5_SF &= ~0x10; break;
#elif ( MACHINE == HPA_36C )
  case LED_INTENSITY_0: if(onoff == LED_ON) G1_SF |= 0x01; else G1_SF &= ~0x01; break;
  case LED_INTENSITY_1: if(onoff == LED_ON) G1_SF |= 0x02; else G1_SF &= ~0x02; break;
  case LED_INTENSITY_2: if(onoff == LED_ON) G1_SF |= 0x04; else G1_SF &= ~0x04; break;
  case LED_INTENSITY_3: if(onoff == LED_ON) G1_SF |= 0x08; else G1_SF &= ~0x08; break;
  
  case LED_BATTERY_GREEN: if(onoff == LED_ON) G1_SF |= 0x10; else G1_SF &= ~0x10; break;
  case LED_BATTERY_RED: if(onoff == LED_ON) G1_SF |= 0x20; else G1_SF &= ~0x20; break;
  
//  case LED_POWER_SIG1:  if(onoff == LED_ON) G2_SF |= 0x01; else G2_SF &= ~0x01; break;

//  case LED_BAT_GRN1: if(onoff == LED_ON) G2_SF |= 0x02; else G2_SF &= ~0x02; break;

  case LED_BAT_RED2: if(onoff == LED_ON) G2_SF |= 0x04; else G2_SF &= ~0x04; break;
  case LED_BAT_GRN1: if(onoff == LED_ON) G2_SF |= 0x08; else G2_SF &= ~0x08; break;  
  case LED_BAT_GRN2: if(onoff == LED_ON) G2_SF |= 0x10; else G2_SF &= ~0x10; break;
  case LED_BAT_RED1: if(onoff == LED_ON) G2_SF |= 0x20; else G2_SF &= ~0x20; break;
//  case LED_POWER_ON: if(onoff == LED_ON) G5_SF |= 0x10; else G5_SF &= ~0x10; break;
#endif  
  case LED_PLASMA_ON: if(onoff == LED_ON) G5_SF |= 0x01; else G5_SF &= ~0x01; break;
  case LED_DIS_ON: if(onoff == LED_ON) G5_SF |= 0x02; else G5_SF &= ~0x02; break;
  case LED_ION_ON: if(onoff == LED_ON) G5_SF |= 0x04; else G5_SF &= ~0x04; break;
  case LED_RESERVE_ON: if(onoff == LED_ON) G5_SF |= 0x08; else G5_SF &= ~0x08; break;
  case LED_PID_ON: if(onoff == LED_ON) G5_SF |= 0x20; else G5_SF &= ~0x20; break;
  
  }
}

void segmentAlphaControl(unsigned char ch1, unsigned char ch2)
{
  G3_SF = 0;
  G4_SF = 0;
  
  switch(ch1)
  {
  case 'O':
      G4_SF |= 1 << 0; //A
      G4_SF |= 1 << 1; //B
      G4_SF |= 1 << 2; //C
      G4_SF |= 1 << 3; //D
      G4_SF |= 1 << 4; //E
      G4_SF |= 1 << 5; //F
      G4_SF |= 0 << 6; //G
    break;
  case 'U':
      G4_SF |= 0 << 0; //A
      G4_SF |= 1 << 1; //B
      G4_SF |= 1 << 2; //C
      G4_SF |= 1 << 3; //D
      G4_SF |= 1 << 4; //E
      G4_SF |= 1 << 5; //F
      G4_SF |= 0 << 6; //G
    break;
  case 'A':
      G4_SF |= 1 << 0; //A
      G4_SF |= 1 << 1; //B
      G4_SF |= 1 << 2; //C
      G4_SF |= 0 << 3; //D
      G4_SF |= 1 << 4; //E
      G4_SF |= 1 << 5; //F
      G4_SF |= 1 << 6; //G
    break;
  case 'B':
      G4_SF |= 0 << 0; //A
      G4_SF |= 0 << 1; //B
      G4_SF |= 1 << 2; //C
      G4_SF |= 1 << 3; //D
      G4_SF |= 1 << 4; //E
      G4_SF |= 1 << 5; //F
      G4_SF |= 1 << 6; //G
    break;
  case 'C':
      G4_SF |= 1 << 0; //A
      G4_SF |= 0 << 1; //B
      G4_SF |= 0 << 2; //C
      G4_SF |= 1 << 3; //D
      G4_SF |= 1 << 4; //E
      G4_SF |= 1 << 5; //F
      G4_SF |= 0 << 6; //G
    break;
  case 'D':
  case 'd':
      G4_SF |= 0 << 0; //A
      G4_SF |= 1 << 1; //B
      G4_SF |= 1 << 2; //C
      G4_SF |= 1 << 3; //D
      G4_SF |= 1 << 4; //E
      G4_SF |= 0 << 5; //F
      G4_SF |= 1 << 6; //G
    break;
  case 'E':
      G4_SF |= 1 << 0; //A
      G4_SF |= 0 << 1; //B
      G4_SF |= 0 << 2; //C
      G4_SF |= 1 << 3; //D
      G4_SF |= 1 << 4; //E
      G4_SF |= 1 << 5; //F
      G4_SF |= 1 << 6; //G
    break;
  case 'F':
      G4_SF |= 1 << 0; //A
      G4_SF |= 0 << 1; //B
      G4_SF |= 0 << 2; //C
      G4_SF |= 0 << 3; //D
      G4_SF |= 1 << 4; //E
      G4_SF |= 1 << 5; //F
      G4_SF |= 1 << 6; //G
    break;
#ifdef  MULTI_LANGUAGE
  case 'L':
      G4_SF |= 0 << 0; //A
      G4_SF |= 0 << 1; //B
      G4_SF |= 0 << 2; //C
      G4_SF |= 1 << 3; //D
      G4_SF |= 1 << 4; //E
      G4_SF |= 1 << 5; //F
      G4_SF |= 0 << 6; //G
    break;
#endif
  case 'n':
      G4_SF |= 0 << 0; //A
      G4_SF |= 0 << 1; //B
      G4_SF |= 1 << 2; //C
      G4_SF |= 0 << 3; //D
      G4_SF |= 1 << 4; //E
      G4_SF |= 0 << 5; //F
      G4_SF |= 1 << 6; //G
    break;
  case 'o':
      G4_SF |= 0 << 0; //A
      G4_SF |= 0 << 1; //B
      G4_SF |= 1 << 2; //C
      G4_SF |= 1 << 3; //D
      G4_SF |= 1 << 4; //E
      G4_SF |= 0 << 5; //F
      G4_SF |= 1 << 6; //G
    break;
  case 'P':
      G4_SF |= 1 << 0; //A
      G4_SF |= 1 << 1; //B
      G4_SF |= 0 << 2; //C
      G4_SF |= 0 << 3; //D
      G4_SF |= 1 << 4; //E
      G4_SF |= 1 << 5; //F
      G4_SF |= 1 << 6; //G
    break;
  case 'r':
      G4_SF |= 0 << 0; //A
      G4_SF |= 0 << 1; //B
      G4_SF |= 0 << 2; //C
      G4_SF |= 0 << 3; //D
      G4_SF |= 1 << 4; //E
      G4_SF |= 0 << 5; //F
      G4_SF |= 1 << 6; //G
    break;
  case '-':
      G4_SF |= 0 << 0; //A
      G4_SF |= 0 << 1; //B
      G4_SF |= 0 << 2; //C
      G4_SF |= 0 << 3; //D
      G4_SF |= 0 << 4; //E
      G4_SF |= 0 << 5; //F
      G4_SF |= 1 << 6; //G
    break;    
   case '0': G4_SF |= 1 << 0; //A
           G4_SF |= 1 << 1; //B
           G4_SF |= 1 << 2; //C
           G4_SF |= 1 << 3; //D
           G4_SF |= 1 << 4; //E
           G4_SF |= 1 << 5; //F
           G4_SF |= 0 << 6; //G
   break;
   case '1': G4_SF |= 0 << 0; //A
           G4_SF |= 1 << 1; //B
           G4_SF |= 1 << 2; //C
           G4_SF |= 0 << 3; //D
           G4_SF |= 0 << 4; //E
           G4_SF |= 0 << 5; //F
           G4_SF |= 0 << 6; //G
   break;
  case '2': G4_SF |= 1 << 0; //A
          G4_SF |= 1 << 1; //B
          G4_SF |= 0 << 2; //C
          G4_SF |= 1 << 3; //D
          G4_SF |= 1 << 4; //E
          G4_SF |= 0 << 5; //F
          G4_SF |= 1 << 6; //G
  break;
  case '3': G4_SF |= 1 << 0; //A
          G4_SF |= 1 << 1; //B
          G4_SF |= 1 << 2; //C
          G4_SF |= 1 << 3; //D
          G4_SF |= 0 << 4; //E
          G4_SF |= 0 << 5; //F
          G4_SF |= 1 << 6; //G
  break;
  case '4': G4_SF |= 0 << 0; //A
          G4_SF |= 1 << 1; //B
          G4_SF |= 1 << 2; //C
          G4_SF |= 0 << 3; //D
          G4_SF |= 0 << 4; //E
          G4_SF |= 1 << 5; //F
          G4_SF |= 1 << 6; //G
  break;
  case '5': G4_SF |= 1 << 0; //A
          G4_SF |= 0 << 1; //B
          G4_SF |= 1 << 2; //C
          G4_SF |= 1 << 3; //D
          G4_SF |= 0 << 4; //E
          G4_SF |= 1 << 5; //F
          G4_SF |= 1 << 6; //G
  break;
  case '6': G4_SF |= 0 << 0; //A
          G4_SF |= 0 << 1; //B
          G4_SF |= 1 << 2; //C
          G4_SF |= 1 << 3; //D
          G4_SF |= 1 << 4; //E
          G4_SF |= 1 << 5; //F
          G4_SF |= 1 << 6; //G
  break;
  case '7': G4_SF |= 1 << 0; //A
          G4_SF |= 1 << 1; //B
          G4_SF |= 1 << 2; //C
          G4_SF |= 0 << 3; //D
          G4_SF |= 0 << 4; //E
          G4_SF |= 1 << 5; //F
          G4_SF |= 0 << 6; //G
  break;
  case '8': G4_SF |= 1 << 0; //A
          G4_SF |= 1 << 1; //B
          G4_SF |= 1 << 2; //C
          G4_SF |= 1 << 3; //D
          G4_SF |= 1 << 4; //E
          G4_SF |= 1 << 5; //F
          G4_SF |= 1 << 6; //G
  break;
  case '9': G4_SF |= 1 << 0; //A
          G4_SF |= 1 << 1; //B
          G4_SF |= 1 << 2; //C
          G4_SF |= 0 << 3; //D
          G4_SF |= 0 << 4; //E
          G4_SF |= 1 << 5; //F
          G4_SF |= 1 << 6; //G
  break;  
  }
  
  switch(ch2)
  {
  case 'O':
      G3_SF |= 1 << 0; //A
      G3_SF |= 1 << 1; //B
      G3_SF |= 1 << 2; //C
      G3_SF |= 1 << 3; //D
      G3_SF |= 1 << 4; //E
      G3_SF |= 1 << 5; //F
      G3_SF |= 0 << 6; //G
    break;
  case 'U':
      G3_SF |= 0 << 0; //A
      G3_SF |= 1 << 1; //B
      G3_SF |= 1 << 2; //C
      G3_SF |= 1 << 3; //D
      G3_SF |= 1 << 4; //E
      G3_SF |= 1 << 5; //F
      G3_SF |= 0 << 6; //G
    break;
  case 'A':
      G3_SF |= 1 << 0; //A
      G3_SF |= 1 << 1; //B
      G3_SF |= 1 << 2; //C
      G3_SF |= 0 << 3; //D
      G3_SF |= 1 << 4; //E
      G3_SF |= 1 << 5; //F
      G3_SF |= 1 << 6; //G
    break;
  case 'B':
      G3_SF |= 0 << 0; //A
      G3_SF |= 0 << 1; //B
      G3_SF |= 1 << 2; //C
      G3_SF |= 1 << 3; //D
      G3_SF |= 1 << 4; //E
      G3_SF |= 1 << 5; //F
      G3_SF |= 1 << 6; //G
    break;
  case 'C':
      G3_SF |= 1 << 0; //A
      G3_SF |= 0 << 1; //B
      G3_SF |= 0 << 2; //C
      G3_SF |= 1 << 3; //D
      G3_SF |= 1 << 4; //E
      G3_SF |= 1 << 5; //F
      G3_SF |= 0 << 6; //G
    break;
  case 'D':
      G3_SF |= 0 << 0; //A
      G3_SF |= 1 << 1; //B
      G3_SF |= 1 << 2; //C
      G3_SF |= 1 << 3; //D
      G3_SF |= 1 << 4; //E
      G3_SF |= 0 << 5; //F
      G3_SF |= 1 << 6; //G
    break;
  case 'E':
      G3_SF |= 1 << 0; //A
      G3_SF |= 0 << 1; //B
      G3_SF |= 0 << 2; //C
      G3_SF |= 1 << 3; //D
      G3_SF |= 1 << 4; //E
      G3_SF |= 1 << 5; //F
      G3_SF |= 1 << 6; //G
    break;
  case 'F':
      G3_SF |= 1 << 0; //A
      G3_SF |= 0 << 1; //B
      G3_SF |= 0 << 2; //C
      G3_SF |= 0 << 3; //D
      G3_SF |= 1 << 4; //E
      G3_SF |= 1 << 5; //F
      G3_SF |= 1 << 6; //G
    break;
  case 'H':
      G3_SF |= 0 << 0; //A
      G3_SF |= 1 << 1; //B
      G3_SF |= 1 << 2; //C
      G3_SF |= 0 << 3; //D
      G3_SF |= 1 << 4; //E
      G3_SF |= 1 << 5; //F
      G3_SF |= 1 << 6; //G
    break;
  case 'n':
      G3_SF |= 0 << 0; //A
      G3_SF |= 0 << 1; //B
      G3_SF |= 1 << 2; //C
      G3_SF |= 0 << 3; //D
      G3_SF |= 1 << 4; //E
      G3_SF |= 0 << 5; //F
      G3_SF |= 1 << 6; //G
    break;
  case 'o':
      G3_SF |= 0 << 0; //A
      G3_SF |= 0 << 1; //B
      G3_SF |= 1 << 2; //C
      G3_SF |= 1 << 3; //D
      G3_SF |= 1 << 4; //E
      G3_SF |= 0 << 5; //F
      G3_SF |= 1 << 6; //G
    break;
  case 'r':
      G3_SF |= 0 << 0; //A
      G3_SF |= 0 << 1; //B
      G3_SF |= 0 << 2; //C
      G3_SF |= 0 << 3; //D
      G3_SF |= 1 << 4; //E
      G3_SF |= 0 << 5; //F
      G3_SF |= 1 << 6; //G
    break;
  case '-':
      G3_SF |= 0 << 0; //A
      G3_SF |= 0 << 1; //B
      G3_SF |= 0 << 2; //C
      G3_SF |= 0 << 3; //D
      G3_SF |= 0 << 4; //E
      G3_SF |= 0 << 5; //F
      G3_SF |= 1 << 6; //G
    break;
   case '0': G3_SF |= 1 << 0; //A
           G3_SF |= 1 << 1; //B
           G3_SF |= 1 << 2; //C
           G3_SF |= 1 << 3; //D
           G3_SF |= 1 << 4; //E
           G3_SF |= 1 << 5; //F
           G3_SF |= 0 << 6; //G
   break;
   case '1': G3_SF |= 0 << 0; //A
           G3_SF |= 1 << 1; //B
           G3_SF |= 1 << 2; //C
           G3_SF |= 0 << 3; //D
           G3_SF |= 0 << 4; //E
           G3_SF |= 0 << 5; //F
           G3_SF |= 0 << 6; //G
   break;
  case '2': G3_SF |= 1 << 0; //A
          G3_SF |= 1 << 1; //B
          G3_SF |= 0 << 2; //C
          G3_SF |= 1 << 3; //D
          G3_SF |= 1 << 4; //E
          G3_SF |= 0 << 5; //F
          G3_SF |= 1 << 6; //G
  break;
  case '3': G3_SF |= 1 << 0; //A
          G3_SF |= 1 << 1; //B
          G3_SF |= 1 << 2; //C
          G3_SF |= 1 << 3; //D
          G3_SF |= 0 << 4; //E
          G3_SF |= 0 << 5; //F
          G3_SF |= 1 << 6; //G
  break;
  case '4': G3_SF |= 0 << 0; //A
          G3_SF |= 1 << 1; //B
          G3_SF |= 1 << 2; //C
          G3_SF |= 0 << 3; //D
          G3_SF |= 0 << 4; //E
          G3_SF |= 1 << 5; //F
          G3_SF |= 1 << 6; //G
  break;
  case '5': G3_SF |= 1 << 0; //A
          G3_SF |= 0 << 1; //B
          G3_SF |= 1 << 2; //C
          G3_SF |= 1 << 3; //D
          G3_SF |= 0 << 4; //E
          G3_SF |= 1 << 5; //F
          G3_SF |= 1 << 6; //G
  break;
  case '6': G3_SF |= 0 << 0; //A
          G3_SF |= 0 << 1; //B
          G3_SF |= 1 << 2; //C
          G3_SF |= 1 << 3; //D
          G3_SF |= 1 << 4; //E
          G3_SF |= 1 << 5; //F
          G3_SF |= 1 << 6; //G
  break;
  case '7': G3_SF |= 1 << 0; //A
          G3_SF |= 1 << 1; //B
          G3_SF |= 1 << 2; //C
          G3_SF |= 0 << 3; //D
          G3_SF |= 0 << 4; //E
          G3_SF |= 1 << 5; //F
          G3_SF |= 0 << 6; //G
  break;
  case '8': G3_SF |= 1 << 0; //A
          G3_SF |= 1 << 1; //B
          G3_SF |= 1 << 2; //C
          G3_SF |= 1 << 3; //D
          G3_SF |= 1 << 4; //E
          G3_SF |= 1 << 5; //F
          G3_SF |= 1 << 6; //G
  break;
  case '9': G3_SF |= 1 << 0; //A
          G3_SF |= 1 << 1; //B
          G3_SF |= 1 << 2; //C
          G3_SF |= 0 << 3; //D
          G3_SF |= 0 << 4; //E
          G3_SF |= 1 << 5; //F
          G3_SF |= 1 << 6; //G
  break;  
  }
}


void segmentControl(unsigned int num)
{
  unsigned int ten = num / 10;
  unsigned int one = num % 10;
  G3_SF = 0;
  G4_SF = 0;
  switch(one)
  {
   case 0: G3_SF |= 1 << 0; //A
           G3_SF |= 1 << 1; //B
           G3_SF |= 1 << 2; //C
           G3_SF |= 1 << 3; //D
           G3_SF |= 1 << 4; //E
           G3_SF |= 1 << 5; //F
           G3_SF |= 0 << 6; //G
   break;
   case 1: G3_SF |= 0 << 0; //A
           G3_SF |= 1 << 1; //B
           G3_SF |= 1 << 2; //C
           G3_SF |= 0 << 3; //D
           G3_SF |= 0 << 4; //E
           G3_SF |= 0 << 5; //F
           G3_SF |= 0 << 6; //G
   break;
  case 2: G3_SF |= 1 << 0; //A
          G3_SF |= 1 << 1; //B
          G3_SF |= 0 << 2; //C
          G3_SF |= 1 << 3; //D
          G3_SF |= 1 << 4; //E
          G3_SF |= 0 << 5; //F
          G3_SF |= 1 << 6; //G
  break;
  case 3: G3_SF |= 1 << 0; //A
          G3_SF |= 1 << 1; //B
          G3_SF |= 1 << 2; //C
          G3_SF |= 1 << 3; //D
          G3_SF |= 0 << 4; //E
          G3_SF |= 0 << 5; //F
          G3_SF |= 1 << 6; //G
  break;
  case 4: G3_SF |= 0 << 0; //A
          G3_SF |= 1 << 1; //B
          G3_SF |= 1 << 2; //C
          G3_SF |= 0 << 3; //D
          G3_SF |= 0 << 4; //E
          G3_SF |= 1 << 5; //F
          G3_SF |= 1 << 6; //G
  break;
  case 5: G3_SF |= 1 << 0; //A
          G3_SF |= 0 << 1; //B
          G3_SF |= 1 << 2; //C
          G3_SF |= 1 << 3; //D
          G3_SF |= 0 << 4; //E
          G3_SF |= 1 << 5; //F
          G3_SF |= 1 << 6; //G
  break;
  case 6: G3_SF |= 0 << 0; //A
          G3_SF |= 0 << 1; //B
          G3_SF |= 1 << 2; //C
          G3_SF |= 1 << 3; //D
          G3_SF |= 1 << 4; //E
          G3_SF |= 1 << 5; //F
          G3_SF |= 1 << 6; //G
  break;
  case 7: G3_SF |= 1 << 0; //A
          G3_SF |= 1 << 1; //B
          G3_SF |= 1 << 2; //C
          G3_SF |= 0 << 3; //D
          G3_SF |= 0 << 4; //E
          G3_SF |= 1 << 5; //F
          G3_SF |= 0 << 6; //G
  break;
  case 8: G3_SF |= 1 << 0; //A
          G3_SF |= 1 << 1; //B
          G3_SF |= 1 << 2; //C
          G3_SF |= 1 << 3; //D
          G3_SF |= 1 << 4; //E
          G3_SF |= 1 << 5; //F
          G3_SF |= 1 << 6; //G
  break;
  case 9: G3_SF |= 1 << 0; //A
          G3_SF |= 1 << 1; //B
          G3_SF |= 1 << 2; //C
          G3_SF |= 0 << 3; //D
          G3_SF |= 0 << 4; //E
          G3_SF |= 1 << 5; //F
          G3_SF |= 1 << 6; //G
  break;  
  }
  
  switch(ten)
  {
   case 0: G4_SF |= 1 << 0; //A
           G4_SF |= 1 << 1; //B
           G4_SF |= 1 << 2; //C
           G4_SF |= 1 << 3; //D
           G4_SF |= 1 << 4; //E
           G4_SF |= 1 << 5; //F
           G4_SF |= 0 << 6; //G
   break;
   case 1: G4_SF |= 0 << 0; //A
           G4_SF |= 1 << 1; //B
           G4_SF |= 1 << 2; //C
           G4_SF |= 0 << 3; //D
           G4_SF |= 0 << 4; //E
           G4_SF |= 0 << 5; //F
           G4_SF |= 0 << 6; //G
   break;
  case 2: G4_SF |= 1 << 0; //A
          G4_SF |= 1 << 1; //B
          G4_SF |= 0 << 2; //C
          G4_SF |= 1 << 3; //D
          G4_SF |= 1 << 4; //E
          G4_SF |= 0 << 5; //F
          G4_SF |= 1 << 6; //G
  break;
  case 3: G4_SF |= 1 << 0; //A
          G4_SF |= 1 << 1; //B
          G4_SF |= 1 << 2; //C
          G4_SF |= 1 << 3; //D
          G4_SF |= 0 << 4; //E
          G4_SF |= 0 << 5; //F
          G4_SF |= 1 << 6; //G
  break;
  case 4: G4_SF |= 0 << 0; //A
          G4_SF |= 1 << 1; //B
          G4_SF |= 1 << 2; //C
          G4_SF |= 0 << 3; //D
          G4_SF |= 0 << 4; //E
          G4_SF |= 1 << 5; //F
          G4_SF |= 1 << 6; //G
  break;
  case 5: G4_SF |= 1 << 0; //A
          G4_SF |= 0 << 1; //B
          G4_SF |= 1 << 2; //C
          G4_SF |= 1 << 3; //D
          G4_SF |= 0 << 4; //E
          G4_SF |= 1 << 5; //F
          G4_SF |= 1 << 6; //G
  break;
  case 6: G4_SF |= 0 << 0; //A
          G4_SF |= 0 << 1; //B
          G4_SF |= 1 << 2; //C
          G4_SF |= 1 << 3; //D
          G4_SF |= 1 << 4; //E
          G4_SF |= 1 << 5; //F
          G4_SF |= 1 << 6; //G
  break;
  case 7: G4_SF |= 1 << 0; //A
          G4_SF |= 1 << 1; //B
          G4_SF |= 1 << 2; //C
          G4_SF |= 0 << 3; //D
          G4_SF |= 0 << 4; //E
          G4_SF |= 1 << 5; //F
          G4_SF |= 0 << 6; //G
  break;
  case 8: G4_SF |= 1 << 0; //A
          G4_SF |= 1 << 1; //B
          G4_SF |= 1 << 2; //C
          G4_SF |= 1 << 3; //D
          G4_SF |= 1 << 4; //E
          G4_SF |= 1 << 5; //F
          G4_SF |= 1 << 6; //G
  break;
  case 9: G4_SF |= 1 << 0; //A
          G4_SF |= 1 << 1; //B
          G4_SF |= 1 << 2; //C
          G4_SF |= 0 << 3; //D
          G4_SF |= 0 << 4; //E
          G4_SF |= 1 << 5; //F
          G4_SF |= 1 << 6; //G
  break;  
  }
}

void segmentControlIntensity(unsigned char intensity)
{
  switch (intensity) {
    case 1 : segmentAlphaControl('P','1'); break;
    case 2 : segmentAlphaControl('P','2'); break;
    case 3 : segmentAlphaControl('P','3'); break;
    default : segmentAlphaControl('P','E'); break;
  } 
}

void ledComControl(unsigned char num, unsigned char onoff)
{
  if(num == 0)
  {
    if(onoff == LED_ON) GPIOE->BSRRL = GPIO_Pin_1; else GPIOE->BSRRH = GPIO_Pin_1;
  }
  if(num == 1)
  {
    if(onoff == LED_ON) GPIOE->BSRRL = GPIO_Pin_0; else GPIOE->BSRRH = GPIO_Pin_0;
  }
  if(num == 2)
  {
    if(onoff == LED_ON) GPIOC->BSRRL = GPIO_Pin_11; else GPIOC->BSRRH = GPIO_Pin_11;
  }
  if(num == 3)
  {
    if(onoff == LED_ON) GPIOC->BSRRL = GPIO_Pin_10; else GPIOC->BSRRH = GPIO_Pin_10;
  }
  if(num == 4)
  {
    if(onoff == LED_ON) GPIOC->BSRRL = GPIO_Pin_9; else GPIOC->BSRRH = GPIO_Pin_9;
  }
  if(num == 5)
  {
    if(onoff == LED_ON) GPIOC->BSRRL = GPIO_Pin_8; else GPIOC->BSRRH = GPIO_Pin_8;
  }
  if(num == 6)
  {
    if(onoff == LED_ON) GPIOC->BSRRL = GPIO_Pin_7; else GPIOC->BSRRH = GPIO_Pin_7;
  }
  if(num == 7)
  {
    if(onoff == LED_ON) GPIOC->BSRRL = GPIO_Pin_6; else GPIOC->BSRRH = GPIO_Pin_6;
  }
}

void timerLed()
{
  GPIOB->BSRRH = GPIO_Pin_9 | GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15;
#ifdef  ENABLE_DP
//  GPIOC->BSRRH = GPIO_Pin_3 | GPIO_Pin_6 | GPIO_Pin_7 | GPIO_Pin_8 | GPIO_Pin_9 | GPIO_Pin_10 | GPIO_Pin_11;
  GPIOC->BSRRH = GPIO_Pin_7 | GPIO_Pin_8 | GPIO_Pin_9 | GPIO_Pin_10 | GPIO_Pin_11;
#else
  GPIOC->BSRRH = GPIO_Pin_6 | GPIO_Pin_7 | GPIO_Pin_8 | GPIO_Pin_9 | GPIO_Pin_10 | GPIO_Pin_11;
#endif
  GPIOE->BSRRH = GPIO_Pin_0 | GPIO_Pin_1;
 
//#ifndef MONEY_STERILIZER
#if ( MACHINE != MONEY_STERILIZER )
  if(g_60hz == 0)
  {
    GPIOB->BSRRL = GPIO_Pin_12; //Group 1 On
    if(G1_SF & 0x01)  ledComControl(0, LED_ON);
    if(G1_SF & 0x02)  ledComControl(1, LED_ON);
    if(G1_SF & 0x04)  ledComControl(2, LED_ON);
    if(G1_SF & 0x08)  ledComControl(3, LED_ON);
    if(G1_SF & 0x10)  ledComControl(4, LED_ON);
    if(G1_SF & 0x20)  ledComControl(5, LED_ON);
    if(G1_SF & 0x40)  ledComControl(6, LED_ON);
    if(G1_SF & 0x80)  ledComControl(7, LED_ON);
  }
  else if(g_60hz == 1)
  {
    GPIOB->BSRRL = GPIO_Pin_13; //Group 2 On
    if(G2_SF & 0x01)  ledComControl(0, LED_ON);
    if(G2_SF & 0x02)  ledComControl(1, LED_ON);
    if(G2_SF & 0x04)  ledComControl(2, LED_ON);
    if(G2_SF & 0x08)  ledComControl(3, LED_ON);
    if(G2_SF & 0x10)  ledComControl(4, LED_ON);
    if(G2_SF & 0x20)  ledComControl(5, LED_ON);
    if(G2_SF & 0x40)  ledComControl(6, LED_ON);
    if(G2_SF & 0x80)  ledComControl(7, LED_ON);

  }
  else
#endif  // MONEY_STERILIZER
    if(g_60hz == 2)
  {
    GPIOB->BSRRL = GPIO_Pin_14; // LED_2ND = SEG2
    if(G3_SF & 0x01)  ledComControl(0, LED_ON);
    if(G3_SF & 0x02)  ledComControl(1, LED_ON);
    if(G3_SF & 0x04)  ledComControl(2, LED_ON);
    if(G3_SF & 0x08)  ledComControl(3, LED_ON);
    if(G3_SF & 0x10)  ledComControl(4, LED_ON);
    if(G3_SF & 0x20)  ledComControl(5, LED_ON);
    if(G3_SF & 0x40)  ledComControl(6, LED_ON);
    if(G3_SF & 0x80)  ledComControl(7, LED_ON);
#ifdef  ENABLE_DP
//    GPIOC->BSRRL = GPIO_Pin_3;
#endif
  }
  else if(g_60hz == 3)
  {
    GPIOB->BSRRL = GPIO_Pin_15; // LED_1ST = SEG1
    if(G4_SF & 0x01)  ledComControl(0, LED_ON);
    if(G4_SF & 0x02)  ledComControl(1, LED_ON);
    if(G4_SF & 0x04)  ledComControl(2, LED_ON);
    if(G4_SF & 0x08)  ledComControl(3, LED_ON);
    if(G4_SF & 0x10)  ledComControl(4, LED_ON);
    if(G4_SF & 0x20)  ledComControl(5, LED_ON);
    if(G4_SF & 0x40)  ledComControl(6, LED_ON);
    if(G4_SF & 0x80)  ledComControl(7, LED_ON);
#ifdef  ENABLE_DP
//    GPIOC->BSRRL = GPIO_Pin_6;
#endif
  }
//#ifndef MONEY_STERILIZER
#if ( MACHINE != MONEY_STERILIZER )
  else
  {
    GPIOB->BSRRL = GPIO_Pin_9; //Group 2 On
    if(G5_SF & 0x01)  ledComControl(0, LED_ON);
    if(G5_SF & 0x02)  ledComControl(1, LED_ON);
    if(G5_SF & 0x04)  ledComControl(2, LED_ON);
    if(G5_SF & 0x08)  ledComControl(3, LED_ON);
    if(G5_SF & 0x10)  ledComControl(4, LED_ON);
    if(G5_SF & 0x20)  ledComControl(5, LED_ON);
    if(G5_SF & 0x40)  ledComControl(6, LED_ON);
    if(G5_SF & 0x80)  ledComControl(7, LED_ON);
  }
#endif
}

void PIRLedTimer()
{
//  GPIOD->BSRRH = GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7;

  //if(pidLEDOnFlag == TRUE)
  if(pidDetectedStart == TRUE) {
    if(g_60hz == 0) {               // SJM 190809 RED ON?
      GPIOD->BSRRH = GPIO_Pin_5;
      GPIOD->BSRRH = GPIO_Pin_6;
      GPIOD->BSRRL = GPIO_Pin_7;
    }
    else {                          // SJM 190809 ALL OFF?
      GPIOD->BSRRH = GPIO_Pin_5;
      GPIOD->BSRRH = GPIO_Pin_6;
      GPIOD->BSRRH = GPIO_Pin_7;    
    }
  }
  else {
    if(g_60hz == 0) {               // SJM 190809 White ON? 
      GPIOD->BSRRL = GPIO_Pin_5;
      GPIOD->BSRRL = GPIO_Pin_6;
      GPIOD->BSRRH = GPIO_Pin_7;
    }
    else {                          // SJM 190809 ALL OFF?
      GPIOD->BSRRH = GPIO_Pin_5;
      GPIOD->BSRRH = GPIO_Pin_6;
      GPIOD->BSRRH = GPIO_Pin_7;    
    }
  }
}

void REDLed()
{
  if(g_60hz == 0) {                 // SJM 190809 RED ON?
    GPIOD->BSRRH = GPIO_Pin_5;
    GPIOD->BSRRH = GPIO_Pin_6;
    GPIOD->BSRRL = GPIO_Pin_7;
  }
  else {                            // SJM 190809 ALL OFF?
    GPIOD->BSRRH = GPIO_Pin_5;
    GPIOD->BSRRH = GPIO_Pin_6;
    GPIOD->BSRRH = GPIO_Pin_7;    
  }
/*
  GPIOD->BSRRL = GPIO_Pin_7;// | GPIO_Pin_6;
  Delay(100);
  GPIOD->BSRRH = GPIO_Pin_7;  
*/
}

void WhiteLed()
{
  if(g_60hz == 0) {                 // SJM 190809 White ON?
    GPIOD->BSRRL = GPIO_Pin_5;
    GPIOD->BSRRL = GPIO_Pin_6;
    GPIOD->BSRRH = GPIO_Pin_7;
  }
  else {                            // SJM 190809 ALL OFF?
    GPIOD->BSRRH = GPIO_Pin_5;
    GPIOD->BSRRH = GPIO_Pin_6;
    GPIOD->BSRRH = GPIO_Pin_7;    
  }
/*
  GPIOD->BSRRL = GPIO_Pin_7;// | GPIO_Pin_6;
  Delay(100);
  GPIOD->BSRRH = GPIO_Pin_7;  
*/
}
