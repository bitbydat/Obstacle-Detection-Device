#ifndef __GPS_DRIVE_H
#define __GPS_DRIVE_H


#include "allheader.h"


#define GPS_PORT				1
#define GPS_BAUDRATE 	9600


typedef struct
{
		uint8_t hour; 					// UTC hour (0–23)
    uint8_t minute;					// UTC minute (0–59)
    uint8_t second;					// UTC second (0–59)
    uint8_t millisecond;		// Optional subsecond (0–99)
} time_t;

typedef struct 
{
		uint8_t ddd;
		uint8_t mm;
		uint16_t mmmm;
		double decimal_deg;
} location_t;


typedef struct
{
		char id[6];
		time_t time;					// UTC time
		char time_chr[10];		// "hhmmss.ss"
		char data_valid;			// 'A' = valid, 'V' = invalid
		location_t lat;				
		char lat_chr[10];			// "ddmm.mmmm"
		char lat_dir;					// 'N' or 'S'
		location_t lon;
		char lon_chr[11];			// "dddmm.mmmm"
		char lon_dir;					// 'E' or 'W'
} rmc_t;


extern char GPS_buf[100];

void GPS_UART_Init(u8 uart, u32 baudrate);
void GPS_process_data(char *buf, rmc_t *rmc);




#endif



