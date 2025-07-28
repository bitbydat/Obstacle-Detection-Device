#ifndef __ESP32_H
#define __ESP32_H


#include "allheader.h"



#define ESP32_select() 		GPIOA->ODR &= ~GPIO_ODR_ODR4
#define ESP32_unselect() 	GPIOA->ODR |= GPIO_ODR_ODR4

extern char ESP32_buf[40];

void ESP32_SPI_Init(void);
void ESP32_TxRx(char *txdata, char *rxdata, char size);
void ESP32_Rx(char *rxdata, char size);
char ESP32_get_LBS(void);
void ESP32_process_data(char *lat, char *lon, uint8_t sz_lat, uint8_t sz_lon);



#endif

