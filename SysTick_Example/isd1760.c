/***********************************************************************************************************************
 * Name:            ISD1760.C
 * 
 * Description      음성칩 ISD1760 을 사용하기 위한 프로그램 함수들
 *
 * Date:            2015-06-05
 *
 * Author:          김상범 
 **********************************************************************************************************************/

#ifndef     _ISD1760_C_
#define     _ISD1760_C_
             
#include    <isd1760.h>

unsigned char g_volume = 15;      // SJM 190716 added by SJM. originally it was 15(constant)

unsigned char spi(unsigned char data)
{

  unsigned char rValue;
  while(SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) == RESET);
  SPI_I2S_SendData(SPI1, data);
  while(SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_RXNE) == RESET);
  rValue = SPI_I2S_ReceiveData(SPI1);    
  return rValue;
}

/***********************************************************************************************************************
 * ISD1760을 초기화 한다
 *
 * @author      김상범
 * @date        2015-06-04
 **********************************************************************************************************************/
void ISD1760_init (void)
{
    //
    // SPI 를 초기화 한다
    // SPI Enable; Data Order: LSB first; SCK: high when idle; Setup: rising edge of SCK
    //
    //SPCR = 0x7C; 

        SPI_InitTypeDef  SPI_InitStructure;
        GPIO_InitTypeDef GPIO_InitStructure;
        
        //Uart_printf1("\rLTC2492_init\n");

	//SPI2 Periph clock enable 
        RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1, ENABLE);
        RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
        //Configure SPI1 pins: SCK, MISO and MOSI 
        GPIO_PinAFConfig(GPIOA, GPIO_PinSource5, GPIO_AF_SPI1);
        GPIO_PinAFConfig(GPIOA, GPIO_PinSource6, GPIO_AF_SPI1);
        GPIO_PinAFConfig(GPIOA, GPIO_PinSource7, GPIO_AF_SPI1);
	
        GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
        GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
        GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
        GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_NOPULL;
        
        /* SPI SCK pin configuration */
        GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;
        GPIO_Init(GPIOA, &GPIO_InitStructure);
        
        /* SPI  MISO pin configuration */
        GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_6;
        GPIO_Init(GPIOA, &GPIO_InitStructure);  
        
        /* SPI  MOSI pin configuration */
        GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_7;
        GPIO_Init(GPIOA, &GPIO_InitStructure);

        //Configure PA8 : ISD1760 CS pin 
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;
        GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
        GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
        GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
        GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
        GPIO_Init(GPIOA, &GPIO_InitStructure);

        /* SPI configuration -------------------------------------------------------*/
        SPI_I2S_DeInit(SPI1);
        
	SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
	SPI_InitStructure.SPI_Mode = SPI_Mode_Master;
	SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;
	SPI_InitStructure.SPI_CPOL = SPI_CPOL_High;
	SPI_InitStructure.SPI_CPHA = SPI_CPHA_2Edge;
	SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;
	SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_256;
	SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_LSB;
	SPI_InitStructure.SPI_CRCPolynomial = 7;
	
        
        /* Configure the Priority Group to 1 bit */                
        //NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
        
        /* Configure the SPI interrupt priority */
        /*NVIC_InitStructure.NVIC_IRQChannel = SPI1_IRQn;
        NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
        NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
        NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
        NVIC_Init(&NVIC_InitStructure);*/
        
        SPI_Init(SPI1, &SPI_InitStructure);
        
	SPI_Cmd(SPI1, ENABLE);
        //SPI_I2S_ITConfig(SPI1, SPI_I2S_IT_RXNE, ENABLE);

        
        
	return;	
}

/***********************************************************************************************************************
 * ISD1760 에 대해서 PowerUp 을 수행한다
 *
 * @author      김상범
 * @return      contents of SR0 register. See ISD1700 datasheet for more information
 * @date        2015-06-04
 **********************************************************************************************************************/
unsigned short ISD1760_powerUp (void)
{
    unsigned short rValue = 0;

    GPIO_ResetBits(GPIOA, GPIO_Pin_8);
    
    rValue = spi(0x11);
    rValue <<= 8;
    rValue |= spi(0x00);

    Delay(1);
    GPIO_SetBits(GPIOA, GPIO_Pin_8);

    return rValue;
}

/***********************************************************************************************************************
 * ISD1760 에 대해서 PowerDown 을 수행한다
 *
 * @author      김상범
 * @return      contents of SR0 register. See ISD1700 datasheet for more information 
 * @date        2009-01-28
 **********************************************************************************************************************/
unsigned short ISD1760_powerDown (void)
{
    unsigned short rValue = 0;
    unsigned long status;
    unsigned short count;

    // Stop 보내기
    count = 0;
    do
    {
        ISD1760_stop();
        status = ISD1760_readStatus();
        count++;
    } while((status & CHECK_CMDERR) && count < ISD1760_TIMEOUT);
    //Uart_printf1("\r\nstop count = %d", count);
        
    // Interrupt 기다린다
    count = 0;
    do
    {
        status = ISD1760_readStatus();
        count++;
    } while(!(status & CHECK_INT) && count < 10);
    //Uart_printf1("\r\nint count = %d", count);

    // ClearINT 수행
    count = 0;
    do
    {
        ISD1760_clearInt();
        status = ISD1760_readStatus();
        count++;
    } while ((status & CHECK_CMDERR) && count < ISD1760_TIMEOUT);
    //Uart_printf1("clear count = %d\n", count);
     
    GPIO_ResetBits(GPIOA, GPIO_Pin_8);
    rValue = spi(COMMAND_PD);
    rValue <<= 8;
    rValue |= spi(0x00);
    Delay(1);
    GPIO_SetBits(GPIOA, GPIO_Pin_8);

    return rValue;
}

/***********************************************************************************************************************
 * ISD1760 에 대해서 STOP을 수행한다. 하고 있던 동작을 중단한다
 *
 * @author      김상범
 * @return      contents of SR0 register. See ISD1700 datasheet for more information
 * @date        2009-01-28
 **********************************************************************************************************************/
unsigned short ISD1760_stop (void)
{
    unsigned short rValue = 0;
    
    GPIO_ResetBits(GPIOA, GPIO_Pin_8);
    rValue = spi(COMMAND_STOP);
    rValue <<= 8;
    rValue |= spi(0x00);
    Delay(1);
    GPIO_SetBits(GPIOA, GPIO_Pin_8);
   
    return rValue;
}

/***********************************************************************************************************************
 * ISD1760 에 대해서 인터럽트를 해지한다. 이렇게 함으로써 다음 명령을 수행할 수 있다
 *
 * @author      김상범
 * @return      contents of SR0 register. See ISD1700 datasheet for more information
 * @date        2009-01-28
 **********************************************************************************************************************/
unsigned short ISD1760_clearInt (void)
{
    unsigned long rValue = 0;
    GPIO_ResetBits(GPIOA, GPIO_Pin_8);
    
    rValue = spi(COMMAND_CLR_INT);
    rValue <<= 8;
    rValue |= spi(0x00);

    Delay(1);
    GPIO_SetBits(GPIOA, GPIO_Pin_8);

    return rValue;
}

/***********************************************************************************************************************
 * ISD1760 에 대해서 상태를 읽는다. ISD1760 이 새 명령을 받아들일 준비가 되어있는지 확인한다
 *
 * @author      김상범
 * @return      contents of SR0 register and SR1 register. See ISD1700 datasheet for more information
 * @date        2009-01-28
 **********************************************************************************************************************/
unsigned long ISD1760_readStatus (void)
{
    unsigned long rValue = 0;

    GPIO_ResetBits(GPIOA, GPIO_Pin_8);
    
    rValue = spi(COMMAND_RD_STATUS);

    rValue <<= 8;
    rValue |= spi(0x00);
    rValue <<=8;
    rValue |= spi(0x00);
    
    Delay(5);
    GPIO_SetBits(GPIOA, GPIO_Pin_8);
    
    //printf("rValue : %x\r\n", rValue);
    return rValue;
}

/***********************************************************************************************************************
 * ISD1760 에 대해서 Reset 을 수행한다. 만약 어떤 명령어가 수행되고 있다면 멈추가 Power Down 상태로 간다
 *
 * @author      김상범
 * @return      contents of SR0 register. See ISD1700 datasheet for more information
 * @date        2009-01-28
 **********************************************************************************************************************/
unsigned short ISD1760_reset (void)
{
    unsigned short rValue = 0;

    GPIO_ResetBits(GPIOA, GPIO_Pin_8);
    rValue = spi(COMMAND_RESET);
    rValue <<= 8;
    rValue |= spi(0);
    Delay(1);
    GPIO_SetBits(GPIOA, GPIO_Pin_8);

    return rValue;
}

/***********************************************************************************************************************
 * ISD1760 에 대해서 RD_APC 를 수행한다.
 *
 * @author      김상범
 * @return      contents of SR0 and APC register
 * @date        2009-01-28
 **********************************************************************************************************************/
unsigned long ISD1760_readAPC (void)
{
    unsigned long rValue = 0;

    GPIO_ResetBits(GPIOA, GPIO_Pin_8);
    rValue = spi(COMMAND_RD_APC);
    rValue <<= 8;
    rValue |= spi(0x00);
    rValue <<= 8;
    rValue |= spi(0x00);
    rValue <<= 8;
    rValue |= spi(0x00);
    Delay(1);
    GPIO_SetBits(GPIOA, GPIO_Pin_8);

    return rValue;
}

/***********************************************************************************************************************
 * ISD1760 에 대해서 WR_NVCFG 를 수행한다.
 *
 * @author      김상범
 * @return      contents of SR0
 * @date        2009-01-28
 **********************************************************************************************************************/
unsigned short ISD1760_writeNVCFG (void)
{
    unsigned short rValue = 0;

    GPIO_ResetBits(GPIOA, GPIO_Pin_8);
    rValue = spi(COMMAND_WR_NVCFG);
    rValue <<= 8;
    rValue |= spi(0x00);
    Delay(1);
    GPIO_SetBits(GPIOA, GPIO_Pin_8);

    return rValue;
}
#if 0     // SJM 190625 NoUse
/***********************************************************************************************************************
 * ISD1760 에 대해서 LD_NVCFG 를 수행한다.
 *
 * @author      김상범
 * @return      contents of SR0
 * @date        2009-01-28
 **********************************************************************************************************************/
unsigned short ISD1760_loadNVCFG  (void)
{
    unsigned short rValue = 0;

    GPIO_ResetBits(GPIOA, GPIO_Pin_8);
    rValue = spi(COMMAND_LD_NVCFG);
    rValue <<= 8;
    rValue |= spi(0x00);
    Delay(1);    
    GPIO_SetBits(GPIOA, GPIO_Pin_8);

    return rValue;
}
#endif    // if 0 SJM 190625 NoUse
/***********************************************************************************************************************
 * ISD1760 에 대해서 WR_APC2 를 수행한다. 주로 Volume control 을 위해서 쓰인다
 *
 * @author      김상범
 * @param       data, [in] data to write on APC register
 * @return      contents of SR0 register and 1st byte of SR0. See ISD1700 datasheet for more information
 * @date        2009-01-28
 **********************************************************************************************************************/
unsigned long ISD1760_writeAPC2 (unsigned short data)
{
    unsigned long rValue = 0;

    GPIO_ResetBits(GPIOA, GPIO_Pin_8);
    rValue = spi(COMMAND_WR_APC2);
    rValue <<= 8;
    rValue |= spi(data & 0xFF);
    rValue <<= 8;
    rValue |= spi(data >> 8);
    Delay(1);    
    GPIO_SetBits(GPIOA, GPIO_Pin_8);

    return rValue;
}
#if 0     // SJM 190625 NoUse
/***********************************************************************************************************************
 * ISD1760 에 대해서 G_ERASE 를 수행한다.
 *
 * @author      김상범
 * @return      contents of SR0
 * @date        2009-01-28
 **********************************************************************************************************************/
unsigned short ISD1760_globalErase (void)
{
    unsigned short rValue = 0;

    GPIO_ResetBits(GPIOA, GPIO_Pin_8);
    rValue = spi(COMMAND_G_ERASE);
    rValue <<= 8;
    rValue |= spi(0x00);
    GPIO_SetBits(GPIOA, GPIO_Pin_8);

    return rValue;
}

/***********************************************************************************************************************
 * 지정된 시작주소와 끝 주소에 대해서 녹음을 수행한다. 
 * 항상 setErase 를 처음에 호출해야 한다
 * 순서
 * 1. ISD1760_setErase 를 호출한다
 * 2. SetRec 를 수행하고 명령이 끝났는지 확인한다
 * 3. Power Down 을 수행한다
 *
 * @author      김상범
 * @param       startAddr, [in] start address
 * @param       endAddr, [in] end address
 * @date        2009-01-28
 **********************************************************************************************************************/
void ISD1760_setRecord (unsigned short startAddr, unsigned short endAddr)
{
    unsigned long status;

    // Reset 수행
    do
    {
        status = ISD1760_reset();
        status = ISD1760_readStatus();
    } while(status & CHECK_PU); 

    // Power up 을 하고 CMD_ERR 확인
    //printf("\r\nSetRecord : PowerUp");
    do
    {
        ISD1760_powerUp();
        Delay(DELAY_TPUD);
        status = ISD1760_readStatus();
    } while(!(status & CHECK_PU || status & CMD_ERR));
    
    // Clear Int 수행하고 CHECK_RDY 확인
    //printf("\r\nSetRecord : ClearInt()");
    status = ISD1760_clearInt();
    do
    {
        status = ISD1760_readStatus();
    } while(!(status & CHECK_RDY));
           
///////////////////////////////////////////
    // Write APC2
    //printf("\r\nSetRecord : APC2()");    
    ISD1760_writeAPC2(0x400);
    do
    {
        status = ISD1760_readStatus();
    } while(!(status & CHECK_RDY));
    
    //printf("\r\nSetRecord : NVCFG()");    
    ISD1760_writeNVCFG();
    do
    {
        status = ISD1760_readStatus();
    } while(!(status & CHECK_RDY));
    
    // Clear Int 수행하고 CHECK_RDY 확인
    //printf("\r\nSetRecord : ClearInt()");    
    status = ISD1760_clearInt();
    do
    {
        status = ISD1760_readStatus();
    } while(!(status & CHECK_RDY));
////////////////////////////////////////////
    //printf("\r\nSetRecord : SET_REC ...");    
    GPIO_ResetBits(GPIOA, GPIO_Pin_8);
    spi(COMMAND_SET_REC);
    spi(0x00);
    spi(startAddr & 0xFF);
    spi(startAddr >> 8);
    spi(endAddr & 0xFF);
    spi(endAddr >> 8);
    spi(0x00);
    Delay(1);
    GPIO_SetBits(GPIOA, GPIO_Pin_8);
                      
    //mdelay(3);
    //status = ISD1760_readStatus();
    
    // 명령어가 끝났는지 확인
    //printf("\r\nSetRecord : Wait INT()");    
    do
    {
        status = ISD1760_readStatus();
    } while(!(status & CHECK_INT));
    
    // Clear Int 수행하고 CHECK_RDY 확인
    //printf("\r\nSetRecord : ClearInt()");       
    status = ISD1760_clearInt();    
    do
    {
        status = ISD1760_readStatus();
    } while(!(status & CHECK_RDY));
    
    //printf("\r\nSetRecord : PowerDown()");   
    do
    {
        status = ISD1760_powerDown();
    } while(status & CHECK_PU);
}
#endif    // if 0 SJM 190625 NoUse

/***********************************************************************************************************************
 * 지정된 시작주소와 끝 주소에 대해서 재생을 수행한다
 * 순서
 * 1. Reset을 수행한다. 이전에 Play 가 있었다면 멈춘다
 * 2. Power Up을 하고 CMD_ERR 를 확인한다 그리고 Tpud 만큼 기다린다
 * 3. Clear Int 를 수행한 후 CHECK_RDY 를 확인한다
 * 4. Set Play 를 수행한다
 * 이 함수는 Play 가 다 끝났는지 확인하지 않는다
 *
 * @author      김상범
 * @param       startAddr, [in] start address
 * @param       endAddr, [in] end address
 * @date        2009-06-05
 * @history
 *  2009-06-05  무한루프 빠지는 경우 해제
 **********************************************************************************************************************/
void ISD1760_setPlay (unsigned short startAddr, unsigned short endAddr, unsigned char volume)
{
    unsigned long status;
    unsigned short count;
    
    // 만약 이미 Play 중이라면 Stop 한다
    status = ISD1760_readStatus();

    if(status & CHECK_PLAYING)
    {
        // Stop 보내기
        count = 0;
        do
        {
            ISD1760_stop();
            status = ISD1760_readStatus();
            count++;
        } while((status & CHECK_CMDERR) && count < ISD1760_TIMEOUT );
        
        // Interrupt 기다린다
        count = 0;
        do
        {
            status = ISD1760_readStatus();
            count++;
        } while(!(status & CHECK_INT) && count < ISD1760_INT_TIMEOUT);
        
        // ClearINT 수행
        count = 0;
        do
        {
            ISD1760_clearInt();
            status = ISD1760_readStatus();
            count++;
        } while ((status & CHECK_CMDERR) && count < ISD1760_TIMEOUT);
    }
    
    // 만약 PU 가 아니라면 Power up 수행
    count = 0;
    ISD1760_clearInt();
    do
    {
        status = ISD1760_readStatus();
        count++;
    } while(!(status & CHECK_RDY) && count < ISD1760_TIMEOUT);

    
    // 현재 전원이 켜져있지 않다면 Power up 수행
    if(!(status & CHECK_PU))
    {
        count = 0;
        do
        {
            ISD1760_powerUp();
            Delay(DELAY_TPUD);
            status = ISD1760_readStatus();
            count++;
        
            // Timeout
            if(count >= ISD1760_TPUD_TIMEOUT) break;
            
        } while( !(status & CHECK_PU) || (status & CMD_ERR) ); 
    }
    
    // Set Volume
    ISD1760_setVolume(volume);
    //ISD1760_writeAPC2(volume);
    // RDY bit 수행
    count = 0;
    ISD1760_clearInt();
    do
    {
        status = ISD1760_readStatus();
        count++;
    } while(!(status & CHECK_RDY) && count < ISD1760_TIMEOUT);
    

    // Set Play
    GPIO_ResetBits(GPIOA, GPIO_Pin_8);
    spi(COMMAND_SET_PLAY);
    spi(0x00);
    spi(startAddr & 0xFF);
    spi(startAddr >> 8);
    spi(endAddr & 0xFF);
    spi(endAddr >> 8);
    spi(0x00);
    Delay(1);
    GPIO_SetBits(GPIOA, GPIO_Pin_8);
    Delay(3);
    status = ISD1760_readStatus();
}
#if 0     // SJM 190625 NoUse
/***********************************************************************************************************************
 * 지정된 시작주소와 끝 주소에 대해서 삭제를 수행한다
 * 순서
 * 1. Power Up 수행 후 CMD_ERR 확인하고 Tpud 동안 기다린다
 * 2. Clear Int 수행하고 CHECK_RDY 한다
 * 3. SetErase 하고 CHECK_INT 해서 명령어가 끝났는지 확인한다.
 * 4. Clear Int 수행하고 CHECK_RDY 한다.
 * 따라서 이 명령을 수행하면 항상 ISD1760 을 Power up 상태에서 새로운 명령을 받아들일 준비가 된다
 *
 * @author      김상범
 * @param       startAddr, [in] start address
 * @param       endAddr, [in] end address
 * @date        2009-01-23
 **********************************************************************************************************************/
void ISD1760_setErase (unsigned short startAddr, unsigned short endAddr)
{
    unsigned long status;

    // Reset 수행
    do
    {
        status = ISD1760_reset();
        status = ISD1760_readStatus();
    } while(status & CHECK_PU); 
    
    // Power up 을 하고 CMD_ERR 확인
    do
    {
      
        ISD1760_powerUp();
        Delay(DELAY_TPUD);
        status = ISD1760_readStatus();
    } while(!(status & CHECK_PU || status & CMD_ERR));
    
    // Clear Int 수행하고 CHECK_RDY 확인
    status = ISD1760_clearInt();
    do
    {
      
        status = ISD1760_readStatus();
    } while(!(status & CHECK_RDY));

    // Set erase
    GPIO_ResetBits(GPIOA, GPIO_Pin_8);
    spi(COMMAND_SET_ERASE);
    spi(0x00);
    spi(startAddr & 0xFF);
    spi(startAddr >> 8);
    spi(endAddr & 0xFF);
    spi(endAddr >> 8);
    spi(0x00);
    Delay(1);
    GPIO_SetBits(GPIOA, GPIO_Pin_8);
                 
    Delay(1);
    status = ISD1760_readStatus();

    // 명령어가 끝났는지 확인
    do
    {
      
        status = ISD1760_readStatus();
    } while(!(status & CHECK_INT));
    
    // Clear Int 수행하고 CHECK_RDY 확인
    status = ISD1760_clearInt();    
    do
    {
      
        status = ISD1760_readStatus();
    } while(!(status & CHECK_RDY));
    
    do
    {
      
        status = ISD1760_powerDown();
    } while(status & CHECK_PU);
}
#endif    // if 0 SJM 190625 NoUse

/***********************************************************************************************************************
 * Volume 을 설정한다
 * 순서
 * 1. Power down 을 해서 이전 명령이 실행된 것 이 있다면 완료하고 수행하도록 한다
 * 2. Power up 을 해서 Idle 상태로 만든다. 후에 Tpud 만큼 기다린다
 * 3. Clear Int 를 수행한다.
 * 4. WR_APC2 명령을 수행한다. 완료하면 CHECK_RDY 를 해서 명령어가 끝나고 준비상태가 되도록 한다
 *
 * @author      김상범
 * @param       volume, [in] 원하는 volume
 * @date        2009-01-28
 **********************************************************************************************************************/
void ISD1760_setVolume (unsigned char volume)
{
    unsigned long status;
    unsigned short count;

    //printf("v1\r\n");
    // Check RDY bit?
    count = 0;
    ISD1760_clearInt();
    do
    {
        status = ISD1760_readStatus();
        count++;
    } while(!(status & CHECK_RDY) && count < ISD1760_TIMEOUT_COUNTER);
    //printf("v2\r\n");
    // 현재 전원이 켜져있지 않다면 Power up 수행
    if(!(status & CHECK_PU))
    {
        count = 0;
        do
        {
            ISD1760_powerUp();
            Delay(DELAY_TPUD);
            status = ISD1760_readStatus();
            count++;
            //printf("status : 0x%x\r\n", status);
            // Timeout
            if(count >= ISD1760_TIMEOUT_COUNTER) break;
            
        } while( !(status & CHECK_PU) || (status & CMD_ERR) );
    }
    //printf("v3\r\n");
    status = ISD1760_readAPC();
    status = status & 0xFFFF;
    //printf("v4\r\n");
    // Check volume setting
    status >>= 8;
    if((status & 0x07) != (7 - volume))
    {
        // Write APC2
        count = 0;
        do
        {
            status = ISD1760_readStatus();
            count++;
        } while(!(status & CHECK_RDY) && count < ISD1760_TIMEOUT_COUNTER);
        
        ISD1760_writeAPC2(DEFAULT_APC | ((7 - volume) & 0x07));
        //printf("\r\n calc : %d", DEFAULT_APC | ((7 - volume) & 0x07));
        count = 0;
        do
        {
            status = ISD1760_readStatus();
            count++;
        } while(!(status & CHECK_RDY) && count < ISD1760_TIMEOUT_COUNTER);
        
        ISD1760_writeNVCFG();
        count = 0;
        do
        {
            status = ISD1760_readStatus();
            count++;
        } while(!(status & CHECK_RDY) && count < ISD1760_TIMEOUT_COUNTER);
    }
    //printf("v5\r\n");
}
#if 0     // SJM 190625 NoUse
/***********************************************************************************************************************
 * Test Program
 **********************************************************************************************************************/
void TestISD1760 (void)
{
    unsigned long status;
    unsigned short startAddr = 0x10, endAddr = 0x2F;
    //printf("\r\nTest Start");    
    do
    {
        status = ISD1760_powerUp();
        Delay(DELAY_TPUD);
        status = ISD1760_readStatus();
    } while(!(status & CHECK_PU) || status & CMD_ERR);

    //printf("\r\nPower up End!!");

    ISD1760_clearInt();
    do
    {
        status = ISD1760_readStatus();
    } while(!(status & CHECK_RDY));

    //printf("\r\nSet erase Start!!");    
    // Set erase
    GPIO_ResetBits(GPIOA, GPIO_Pin_8);
    spi(COMMAND_SET_ERASE);
    spi(0x00);
    spi(startAddr & 0xFF);
    spi(startAddr >> 8);
    spi(endAddr & 0xFF);
    spi(endAddr >> 8);
    spi(0x00);
    Delay(1);
    GPIO_SetBits(GPIOA, GPIO_Pin_8);
    
    //printf("\r\nSet erase End!!");

    do
    {
        status = ISD1760_readStatus();
    } while(!(status & CHECK_INT));
                 
    //printf("\r\nSet Clear Start!!");

    ISD1760_clearInt();
    //printf("\r\nSet Clear End!!");

    do
    {
        status = ISD1760_readStatus();
    } while(!(status & CHECK_RDY));

    //printf("\r\nSet set Record!!");
             
    // Set Record
    GPIO_ResetBits(GPIOA, GPIO_Pin_8);
    spi(COMMAND_SET_REC);
    spi(0x00);
    spi(startAddr & 0xFF);
    spi(startAddr >> 8);
    spi(endAddr & 0xFF);
    spi(endAddr >> 8);
    spi(0x00);
    Delay(1);
    GPIO_SetBits(GPIOA, GPIO_Pin_8);
    
    do {
        status = ISD1760_readStatus();
    } while(!(status & CHECK_INT));
    
    ISD1760_clearInt();
    do
    {
        status = ISD1760_readStatus();
    } while(!(status & CHECK_RDY));
    
    //ISD1760_writeAPC2(10);
    //printf("\r\nSet Play!!");
    // Play
    GPIO_ResetBits(GPIOA, GPIO_Pin_8);
    spi(COMMAND_SET_PLAY);
    spi(0x00);
    spi(startAddr & 0xFF);
    spi(startAddr >> 8);
    spi(endAddr & 0xFF);
    spi(endAddr >> 8);
    spi(0x00);
    Delay(1);
    GPIO_SetBits(GPIOA, GPIO_Pin_8);
    
    do
    {
        status = ISD1760_readStatus();
    } while(!(status & CHECK_INT));
    
    // Power down 을 하고 CMD_ERR 확인. 이전 명령 실행된 것이 있다면 완료하고 Power 다운 상태로 간다
    //printf("\r\nPower Down!!");
    do
    {
        status = ISD1760_powerDown();
    } while(status & CMD_ERR);
    
    //printf("\r\nFunction End!!!");
}

/***********************************************************************************************************************
 * Analog Path 를 설정한다
 *
 * @author      김상범
 * @date        2009-06-05
 **********************************************************************************************************************/
void ISD1760_setAnalogPath (void)
{
    unsigned long status;
    unsigned short count;
    //printf("\r\nISD1760_setAnalogPath Start!!!");
    // Check RDY bit?
    count = 0;
    ISD1760_clearInt();
    do
    {
        status = ISD1760_readStatus();
        count++;
    } while(!(status & CHECK_RDY) && count < ISD1760_TIMEOUT_COUNTER);
    
    // 현재 전원이 켜져있지 않다면 Power up 수행
    if(!(status & CHECK_PU))
    {
        count = 0;
        do
        {
            ISD1760_powerUp();
            Delay(DELAY_TPUD);
            status = ISD1760_readStatus();
            count++;
        
            // Timeout
            if(count >= ISD1760_TIMEOUT_COUNTER) break;
            
        } while( !(status & CHECK_PU) || (status & CMD_ERR) );
    }
    
    status = ISD1760_readAPC();
    status = status & 0xFFFF;
    if(status != 0x4004)
    {
        // Write APC2
        ISD1760_writeAPC2(0x440);
        count = 0;
        do
        {
            status = ISD1760_readStatus();
            count++;
        } while(!(status & CHECK_RDY) && count < ISD1760_TIMEOUT_COUNTER);
        
        ISD1760_writeNVCFG();
        count = 0;
        do
        {
            status = ISD1760_readStatus();
            count++;
        } while(!(status & CHECK_RDY) && count < ISD1760_TIMEOUT_COUNTER);
    }
    
   //printf("\r\nISD1760_setAnalogPath End!!!");
}
#endif // if 0 SJM 190625 NoUse
#endif      /* _ISD1760_C_ */