#ifndef __DFPLAYER_DRIVE_H
#define __DFPLAYER_DRIVE_H

#include <allheader.h>

#define DF_PORT						2
#define DF_BAUDRATE				9600

#define START_BYTE 				0x7E
#define VERSION    				0xFF
#define CMD_LENG    			0x06
#define FEEDBACK	  			0x00
#define SOURCE_TF_CARD  	0x02
#define END_BYTE   				0xEF
#define INIT_CMD					0x3F

//DFplayer send command 
#define NEXT 					0x01
#define PREVIOUS			0x02	
#define	SELECT_TRACK	0x03
#define INCREASE_VOL	0x04
#define DECREASE_VOL	0x05
#define SELECT_VOL		0x06
#define RESET					0x0C
#define PAUSE					0x0E
#define RESUME				0x0D

//DFplayer audio id
#define Left2m 				01
#define Left1m 				02
#define Left50cm		  03
#define Front2m 			04
#define Front1m 			05
#define Front50cm 		06
#define Right2m 			07
#define Right1m 			08
#define Right50cm 		09
#define Stop 					10
#define GPS_sent 			11 
#define No_GPS 				12



void DF_UART_Init(u8 uart, u32 baudrate);
void DF_Send_CMD(u8 cmd, u8 para1, u8 para2);
void DF_Init(void);
void DF_Play(u8 audio_number);
void DF_Set_Volume(u8 audio_volume);
void DF_Increase_Volume(void);
void DF_Decrease_Volume(void);
void DF_Pause(void);
void DF_Resume(void);
void DF_Reset(void);
void Alert_Handle(uint32_t *distance);
int get_alert_level(uint16_t distance);

//#define Buzzer_On 	GPIOA->ODR |= GPIO_ODR_ODR11
//#define Buzzer_Off 	GPIOA->ODR &= ~GPIO_ODR_ODR11

#define Buzzer_On 	GPIOB->ODR |= GPIO_ODR_ODR9
#define Buzzer_Off 	GPIOB->ODR &= ~GPIO_ODR_ODR9;

#endif


