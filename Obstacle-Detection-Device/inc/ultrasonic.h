#ifndef __ULTRASONIC_DRIVE_H
#define __ULTRASONIC_DRIVE_H


#include "allheader.h"

#define OFFSET_RIGHT 	2220
#define OFFSET_LEFT 	200
#define OFFSET_FRONT 	210

#define LOWER_LIMIT 2U
#define UPPER_LIMIT 200U

#define RIGHT 0
#define FRONT 1
#define LEFT 	2

#define INPUT_NUMS 3


void Ultrasonic_Init(void);
void Ultrasonic_Measure(uint32_t *distance);
int Ignore_Distance_Zone(uint32_t *distance);
double KF(uint8_t id, double U);


#endif

