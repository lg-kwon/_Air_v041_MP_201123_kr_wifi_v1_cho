#include "Key.h"

/***********************************************************************************************************************
 * Name:            interrupt.c
 *
 * Description:     Sub
 *
 **********************************************************************************************************************/
						


/***********************************************************************************************************************
 *                                                  MEMBER VARIABLES
 **********************************************************************************************************************/

/**
 * Name             : m_keyTimeCounter
 * Description      : Chattering
 */
signed      int    m_keyTimeCounter = 0;

/**
 * Name             : m_keyDetected, m_keyPressed
 * Description      : Key
 */
static      char    m_keyDetected = 0, m_keyPressed = 0;

/**
 * Name             : m_modeKeyPressed
 * Description      : 
 */
static      char    m_modeKeyCounter = 0;

/**
 * Description      : Key flags
 * See also         : KEY FLAGS DEFINES
 */
static  unsigned    int     m_tempKeyFlag1 = 0, m_tempKeyFlag2 = 0;

unsigned int g_keyFlag;

/***********************************************************************************************************************
 * key Port Init
 *
 **********************************************************************************************************************/
void keyPortInit()
{
  GPIO_InitTypeDef  GPIO_InitStructure;
  
  /* Enable the key Clock */
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE);
  
  /* Configure the Relay pin */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3 | GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_6;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIOE, &GPIO_InitStructure);
}

/***********************************************************************************************************************
 * Key flag
 *
 **********************************************************************************************************************/
unsigned long timer_get_key_flag (void)
{
    unsigned long flag = 0;

    if(GPIO_ReadInputDataBit(GPIOE, GPIO_Pin_4) == RESET)		flag |= KEY_PWR_CHECK;
    if(GPIO_ReadInputDataBit(GPIOE, GPIO_Pin_3) == RESET)		flag |= KEY_STER_CHECK;
    if(GPIO_ReadInputDataBit(GPIOE, GPIO_Pin_6) == RESET)		flag |= KEY_O3_CHECK;
    if(GPIO_ReadInputDataBit(GPIOE, GPIO_Pin_5) == RESET)		flag |= KEY_TIME_CHECK;
    return flag;
}

/***********************************************************************************************************************
 * Flag set
 *
 **********************************************************************************************************************/
void timer_set_key_flag (unsigned long flag, unsigned char type)
{
    if(type == TYPE_NORMAL)
    {
        if((flag & KEY_PWR_CHECK) > 0)             		  g_keyFlag |= KEY_PWR_FLAG;
        if((flag & KEY_STER_CHECK) > 0)	 	 	          g_keyFlag |= KEY_STER_FLAG;
        if((flag & KEY_O3_CHECK) > 0)	 	 	          g_keyFlag |= KEY_O3_FLAG;
        if((flag & KEY_TIME_CHECK) > 0)	 	 	          g_keyFlag |= KEY_TIME_FLAG;
    }
    else if(type == TYPE_LONG)
    {
        if((flag & KEY_PWR_CHECK))             		          g_keyFlag |= KEY_PWR_FLAG;
        if((flag & KEY_STER_CHECK))             		  g_keyFlag |= KEY_STER_FLAG;
        if((flag & KEY_O3_CHECK))	 	 	          g_keyFlag |= KEY_O3_FLAG;
        if((flag & KEY_TIME_CHECK))             		  g_keyFlag |= KEY_TIME_FLAG;

    }/*
    else if(type == TYPE_CONTINUE)
    {
      if((flag & KEY_START_CHECK))             		          g_keyFlag |= KEY_START_CONTINUE_FLAG;
      if((flag & KEY_LEFT_CHECK))             		          g_keyFlag |= KEY_LEFT_CONTINUE_FLAG;
    }*/
    //printf("g_keyFlag : [%x], type[%d]\r\n", g_keyFlag, type);    
}

/***********************************************************************************************************************
 * 
 *
 **********************************************************************************************************************/
void timer_key_check (void)
{
    if((m_keyDetected == FALSE) &&( m_keyPressed == FALSE))
    {    
        m_tempKeyFlag1 = timer_get_key_flag();
        
        if(m_tempKeyFlag1 > 0)                                      // (2)
        {    
            m_keyDetected = TRUE;
            m_keyTimeCounter = 10;
        }
    }
    else if((m_keyDetected == TRUE) &&( m_keyPressed == FALSE))
    {
        if(--m_keyTimeCounter == 0)
        {
            m_tempKeyFlag2 = timer_get_key_flag();
                            
            if(m_tempKeyFlag1 == m_tempKeyFlag2)
            {
                //g_keyFlag = m_tempKeyFlag1;                         // (1)
                                
                if(m_modeKeyCounter == 0)
                {
                    m_modeKeyCounter = 15;       // 50ms
                    m_keyTimeCounter = 10;
                    return;
                }
                else
                {
                    if(--m_modeKeyCounter == 0)
                    {
                        timer_set_key_flag(m_tempKeyFlag1, TYPE_LONG);
                        //printf("g_keyFlag2 : [%x]\r\n", m_tempKeyFlag1);
                        m_keyPressed = TRUE;
                        /////Long
                        //g_continueFlag = TRUE;
                        //g_continueTimer = 25;
                        //g_key = m_tempKeyFlag1;
                    }
                    else
                    {
                        m_keyTimeCounter = 10;
                    }
                    return;
                }
            }
            else   
            {
                if(m_tempKeyFlag1 > 0) 
                {
                    timer_set_key_flag(m_tempKeyFlag1, TYPE_NORMAL);
                }

                m_modeKeyCounter = 0;
                m_tempKeyFlag1 = m_tempKeyFlag2;
                m_keyPressed = TRUE;
            }
        }
        else if(m_keyTimeCounter > 0)
        {
          m_tempKeyFlag2 = timer_get_key_flag();
          if(m_tempKeyFlag2 > 0)
          {
            if(m_tempKeyFlag1 != m_tempKeyFlag2)
            {
                m_tempKeyFlag1 = m_tempKeyFlag2;
            }
          }
        }
    }
    else if(m_keyPressed == TRUE)
    {
        m_tempKeyFlag2 = timer_get_key_flag();
        
        if(timer_get_key_flag() == 0)
        {
          m_keyPressed = FALSE;
          m_keyDetected = FALSE;
        }
    }
}


