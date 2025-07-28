#ifndef __ALLHEADER_H
#define __ALLHEADER_H


#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include "stm32f10x.h"
#include "string.h"
#include "systick_time.h"
#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"
#include "gp_drive.h"
#include "timer_drive.h"
#include "uart_drive.h"
#include "button.h"
#include "dfplayer.h"
#include "gps.h"
#include "a7680c.h"
#include "ultrasonic.h"
#include "esp32.h"

#define is_obstacle (obstacle_distance[0] || obstacle_distance[1] || obstacle_distance[2])

void vTaskUltrasonicMeasure(void *ptr);
void vTaskAlert(void *ptr);
void vTaskLocationHandler(void *pvParameter);
void vTaskLedBlue(void *ptr);

void Alert_Handle(uint32_t *distance);
void convert_nmea_to_decimal(char *str, uint8_t is_latitude);
int get_alert_level(uint16_t distance);

extern char location_msg[60];


#endif



