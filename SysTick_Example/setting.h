#ifndef __SETTING_H
#define __SETTING_H

#include "main.h"
#include "RTC.h"

#define COMMAND_STR_MAX          100
#define COMMAND_MAX              30

#define SUCCESS                 0
#define FAIL                    1

char digitNum[100];
int digit_idx = 0;

extern unsigned char rx_head;
extern unsigned char rx_tail;
extern unsigned char rx_buffer[MAX_RX_BUFFER];

typedef void (*func_t)(void);
int digit_analysis(char *string);
void settingMainMenu(void);
void dateSetting();
unsigned int getNum(void);
void timeSetting();
void factoryReset();

#endif  // __SETTING_H