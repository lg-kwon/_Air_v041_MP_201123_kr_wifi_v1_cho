#ifndef __REMOTE_H_
#define __REMOTE_H_

#include "main.h"

#ifdef  USE_TNY_1311S_REMOTE
//  #define TNY_1311S_CODE_FIRST                  0x56
//  #define TNY_1311S_CODE_SECOND                 0xa9
  /*********REMOTE KEY CODE*******/
  #define TNY_1311S_POWER                       0x20
  #define TNY_1311S_MODE                        0x21
  #define TNY_1311S_LEFT                        0x22
  #define TNY_1311S_OK                          0x23
  #define TNY_1311S_RIGHT                       0x24
  #define TNY_1311S_UP                          0x5B
  #define TNY_1311S_DOWN                        0x46
  #define TNY_1311S_TIMER                       0x25
  #define TNY_1311S_HUMAN                       0x26
  #define TNY_1311S_SETUP                       0x27
  #define TNY_1311S_HOME                        0x28
#endif  // USE_TNY_1311S_REMOTE

#ifdef  USE_119SP_REMOTE
  #define OKC_119SP_CODE_FIRST                  0x9A
  #define OKC_119SP_CODE_SECOND                 0x65
  /*********REMOTE KEY CODE*******/
  #define OKC_119SP_POWER                       0x40
  #define OKC_119SP_STERILIZATION               0x5A
  #define OKC_119SP_O3                          0x5F
  #define OKC_119SP_TIMER                       0x5E
  #define OKC_119SP_WIND                        0x0F
  #define OKC_119SP_ION                         0x4F
  #define OKC_119SP_PLASMA                      0x4E
  #define OKC_119SP_UP                          0x5B
  #define OKC_119SP_MENU                        0x1B
  #define OKC_119SP_OK                          0x5D
  #define OKC_119SP_DOWN                        0x46
  #define OKC_119SP_RESERVED                    0x51
#endif  // USE_119SP_REMOTE

/**********REMOTE CODE**********/
#define REMOTE_CODE_FIRST       0x3C
#define REMOTE_CODE_SECOND      0xC3

/*********REMOTE KEY CODE*******/
#define REMOTE_POWER                        0x00
#define REMOTE_STERILIZATION                0x02
#define REMOTE_O3                           0x03
#define REMOTE_TIMER                        0x05
#define REMOTE_WIND                         0x06
#define REMOTE_ION                          0x07
#define REMOTE_PLASMA                       0x08
#define REMOTE_UP                           0x0B
#define REMOTE_MENU                         0x0D
#define REMOTE_OK                           0x0F
#define REMOTE_DOWN                         0x11
#define REMOTE_RESERVE                      0x12

/*********REMOTE KEY CODE ADDED*******/
#define REMOTE_OZONE_1                      0x01
#define REMOTE_OZONE_2                      0x04
#define REMOTE_OZONE_3                      0x0A
#define REMOTE_OZONE_4                      0x0C
#define REMOTE_OZONE_5                      0x0E
#define REMOTE_OZONE_6                      0x10
#define REMOTE_OZONE_7                      0x13
#define REMOTE_OZONE_8                      0x14
//#define REMOTE_OZONE_9                      0x15
//#define REMOTE_OZONE_10                     0x16
/*********REMOTE KEY FLAG*******/
#define REMOTE_POWER_FLAG                             0x00000001
#define REMOTE_STERILIZATION_FLAG                     0x00000002
#define REMOTE_O3_FLAG                                0x00000004
#define REMOTE_TIMER_FLAG                             0x00000008
#define REMOTE_WIND_FLAG                              0x00000010
#define REMOTE_ION_FLAG                               0x00000020
#define REMOTE_PLASMA_FLAG                            0x00000040
#define REMOTE_UP_FLAG                                0x00000080
#define REMOTE_MENU_FLAG                              0x00000100
#define REMOTE_OK_FLAG                                0x00000200
#define REMOTE_DOWN_FLAG                              0x00000400
#define REMOTE_RESERVE_FLAG                           0x00000800

// SJM 190704 New definition for RemoteController Button accordingly
#define BUTTON_POWER                                  0x00000001
#define BUTTON_PLASMA                                 0x00000002
#define BUTTON_DISINFECT                              0x00000004
#define BUTTON_ANION                                  0x00000008
#define BUTTON_TIME                                   0x00000010
#define BUTTON_INTENSITY                              0x00000020
#define BUTTON_HUMAN                                  0x00000040
#define BUTTON_UP                                     0x00000080
#define BUTTON_CONFIRM                                0x00000100
#define BUTTON_RIGHT                                  0x00000200
#define BUTTON_DOWN                                   0x00000400
#define BUTTON_PERIOD                                 0x00000800

#define BUTTON_POWER_LONG                             0x00010000
#define BUTTON_PLASMA_LONG                            0x00020000
#define BUTTON_DISINFECT_LONG                         0x00040000
#define BUTTON_ANION_LONG                             0x00080000
#define BUTTON_TIME_LONG                              0x00100000
#define BUTTON_INTENSITY_LONG                         0x00200000
#define BUTTON_HUMAN_LONG                             0x00400000
#define BUTTON_UP_LONG                                0x00800000
#define BUTTON_CONFIRM_LONG                           0x01000000
#define BUTTON_RIGHT_LONG                             0x02000000
#define BUTTON_DOWN_LONG                              0x04000000
#define BUTTON_PERIOD_LONG                            0x08000000

#define REMOTE_OZ1_FLAG                               0x00001000
#define REMOTE_OZ2_FLAG                               0x00002000
#define REMOTE_OZ3_FLAG                               0x00004000
#define REMOTE_OZ4_FLAG                               0x00008000
#define REMOTE_OZ5_FLAG                               0x10000000
#define REMOTE_OZ6_FLAG                               0x20000000
#define REMOTE_OZ7_FLAG                               0x40000000
#define REMOTE_OZ8_FLAG                               0x80000000
//  #define REMOTE_OZ9_FLAG                             0x00001000
//  #define REMOTE_OZ10_FLAG                            0x00001000
#define REMOTE_OZ_LOW                                 0x1000f000
#define REMOTE_OZ_HIGH                                0xe0000000

#define REMOTE_POWER_LONG_FLAG                        0x00010000
#define REMOTE_STERILIZATION_LONG_FLAG                0x00020000
#define REMOTE_O3_LONG_FLAG                           0x00040000
#define REMOTE_TIMER_LONG_FLAG                        0x00080000
#define REMOTE_WIND_LONG_FLAG                         0x00100000
#define REMOTE_ION_LONG_FLAG                          0x00200000
#define REMOTE_PLASMA_LONG_FLAG                       0x00400000
#define REMOTE_UP_LONG_FLAG                           0x00800000
#define REMOTE_MENU_LONG_FLAG                         0x01000000
#define REMOTE_OK_LONG_FLAG                           0x02000000
#define REMOTE_DOWN_LONG_FLAG                         0x04000000
#define REMOTE_RESERVE_LONG_FLAG                      0x08000000

#ifdef USE_TNY_1311S_REMOTE
  #define TNY_POWER_FLAG                              0x10000001
  #define TNY_MODE_FLAG                               0x10000002
  #define TNY_LEFT_FLAG                               0x10000004
  #define TNY_OK_FLAG                                 0x10000008
  #define TNY_RIGHT_FLAG                              0x10000010
  #define TNY_TIMER_FLAG                              0x10000020
  #define TNY_HUMAN_FLAG                              0x10000040
  #define TNY_SETUP_FLAG                              0x10000080
  #define TNY_HOME_FLAG                               0x10000100

  #define TNY_POWER_LONG_FLAG                         0x10001000
  #define TNY_MODE_LONG_FLAG                          0x10002000
  #define TNY_LEFT_LONG_FLAG                          0x10004000
  #define TNY_OK_LONG_FLAG                            0x10008000
  #define TNY_RIGHT_LONG_FLAG                         0x10010000
  #define TNY_TIMER_LONG_FLAG                         0x10020000
  #define TNY_HUMAN_LONG_FLAG                         0x10040000
  #define TNY_SETUP_LONG_FLAG                         0x10080000
  #define TNY_HOME_LONG_FLAG                          0x10100000
#endif
/**********REMOTE TIME**********/
#define REMOTE_NORMAL_FLAG      2
#define REMOTE_LONG_FLAG        1

/**********REMOTE STATE**********/
#define REMOTE_IDLE             0x00
#define REMOTE_READY_LOW        0x01
#define REMOTE_READY_HIGH       0x02
#define REMOTE_CUSTOM_CODE_LOW  0x03
#define REMOTE_DATA_HIGH        0x04
#define REMOTE_DATA_LOW         0x05

/**********REMOTE TIME**********/
#define REMOTE_MARGIN           6        //6 * 50(timer interrupt) = 300us
#define REMOTE_SAMPLING_TIME    50       //timer interrupt -> 50us

/**********REMOTE STATE**********/
#define TIME_READY_LOW            9000 / REMOTE_SAMPLING_TIME     //9ms
#define TIME_READY_HIGH           4500 / REMOTE_SAMPLING_TIME     //4.5ms
#define TIME_DATA_LOW             560 / REMOTE_SAMPLING_TIME      //0.56ms
#define TIME_DATA_BIT_LOW         560 / REMOTE_SAMPLING_TIME      //0.56ms
#define TIME_DATA_BIT_HIGH        1680 / REMOTE_SAMPLING_TIME      //1.68ms
#define TIME_REPEAT               2250 / REMOTE_SAMPLING_TIME      //2.25ms
#define REMOTE_ERROR              300  //350ms
#define TIME_FLAME_INTERVAL       120000 / REMOTE_SAMPLING_TIME    //108ms + 12ms

/*****function ******/
void remoteInit();
void remoteTimerInit();
void remoteCheck();
void remoteFlag(unsigned char *data, unsigned char type);

#endif