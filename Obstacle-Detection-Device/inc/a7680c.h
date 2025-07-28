#ifndef __A7680C_DRIVE_H
#define __A7680C_DRIVE_H


#include "allheader.h"



#define A7680C_PORT 3
#define A7680C_BAUDRATE 115200


void A7680C_UART_Init(u8 uart, u32 baudrate);
void A7680C_send_msg(void);
char A7680C_get_LBS(void);
void A7680C_process_data(char *lat, char *lon, uint8_t sz_lat, uint8_t sz_lon);



extern char A7680C_buf[40];

#endif


