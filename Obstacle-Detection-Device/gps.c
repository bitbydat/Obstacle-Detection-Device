#include "gps.h"


//char GPS_buf[100] = "$GNRMC,084852.99,A,2236.9453,N,11408.4790,E,0.53,292.44,1412";
char GPS_buf[100] = {0};

void GPS_UART_Init(u8 uart, u32 baudrate)
{
	UART_init(uart,baudrate);
}

void GPS_process_data(char *buf, rmc_t *rmc)
{
	strncpy(rmc->id, buf+1, 5);
	rmc->id[5] = '\0';
	
	char str[100] = {0};
	double raw_deg;
	strncpy(str, buf, sizeof(str));
	char *token = strtok(str, ",");
	uint8_t field = 0;

	while(token != NULL)
	{
		switch(field)
		{
			case 1:	//Time field
					 strncpy(rmc->time_chr, token, sizeof(rmc->time_chr));
					 rmc->time_chr[sizeof(rmc->time_chr)-1] = '\0';
					 
					 sscanf(token, "%2hhu%2hhu%2hhu.%2hhu", 
					 									 &rmc->time.hour,
					 									 &rmc->time.minute,
					 									 &rmc->time.second,
					 									 &rmc->time.millisecond);
					 break;
			case 2: // Data valid
           rmc->data_valid = token[0];
           break;
			case 3: // Latitude field
           strncpy(rmc->lat_chr, token, sizeof(rmc->lat_chr));
           rmc->lat_chr[sizeof(rmc->lat_chr)-1] = '\0';
					 
           sscanf(token, "%2hhu%lf", &rmc->lat.ddd, &rmc->lat.decimal_deg);
           rmc->lat.mm = (uint8_t)rmc->lat.decimal_deg;
           rmc->lat.mmmm = (uint16_t)((rmc->lat.decimal_deg - rmc->lat.mm) * 10000);
           raw_deg = rmc->lat.ddd + (rmc->lat.decimal_deg / 60.0);
					 rmc->lat.decimal_deg = ((uint32_t)(raw_deg*1000000+0.5))/1000000.0;
           break;

       case 4: // N/S
           rmc->lat_dir = token[0];
           if (rmc->lat_dir == 'S') rmc->lat.decimal_deg *= -1;
           break;

       case 5: // Longitude field
           strncpy(rmc->lon_chr, token, sizeof(rmc->lon_chr));
           rmc->lon_chr[sizeof(rmc->lon_chr)-1] = '\0';

           sscanf(token, "%3hhu%lf", &rmc->lon.ddd, &rmc->lon.decimal_deg);
           rmc->lon.mm = (uint8_t)rmc->lon.decimal_deg;
           rmc->lon.mmmm = (uint16_t)((rmc->lon.decimal_deg - rmc->lon.mm) * 10000);
           raw_deg = rmc->lon.ddd + (rmc->lon.decimal_deg / 60.0);
					 rmc->lon.decimal_deg = ((uint32_t)(raw_deg * 1000000 + 0.5)) / 1000000.0;
           break;

       case 6: // E/W
           rmc->lon_dir = token[0];
           if (rmc->lon_dir == 'W') rmc->lon.decimal_deg *= -1;
           break;
			
			default:
					 break;
		}
		token = strtok(NULL, ",");
        field++;
	}
	
	
	
}

void convert_nmea_to_decimal(char *str, uint8_t is_latitude)
{
	char deg_str[4] = {0};
	int  deg_len = is_latitude ? 2 : 3;
	
	strncpy(deg_str, str, deg_len);
	int degree = atoi(deg_str);
	
	double minutes = atof(str + deg_len);
	
	double decimal = (double)degree + (minutes/60.0f);
	sprintf(str, "%.6f", decimal);
}

