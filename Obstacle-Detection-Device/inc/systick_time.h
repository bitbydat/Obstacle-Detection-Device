#ifndef __SYSTICK_TIME_H
#define __SYSTICK_TIME_H


#include "stm32f10x.h" 

void systick_init(void);
void DelayMs(unsigned long t);
void systick_int(unsigned short uart_1_mgr[],unsigned short uart_2_mgr[],unsigned short uart_3_mgr[]);
void systick_isr_init(void);
void Delaymicro(void);
void DelayUs(unsigned long t);


#endif

