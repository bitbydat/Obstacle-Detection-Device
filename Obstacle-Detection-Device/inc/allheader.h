#ifndef __ALLHEADER_H
#define __ALLHEADER_H


#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include "string.h"
#include "stm32f10x.h"
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

#define TRUE 1
#define FALSE 0

void vTaskUltrasonicMeasure(void *ptr);
void vTaskAlert(void *ptr);
void vTaskLocationHandler(void *pvParameter);
void vTaskLedBlue(void *ptr);

extern char location_msg[60];


#endif



