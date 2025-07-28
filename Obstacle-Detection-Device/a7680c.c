#include "a7680c.h"


//char A7680C_buf[40] = "+CLBS: 0,21.013981,105.836349,550\r\n";
char A7680C_buf[40] = {0};

char init_cmd[6][60] = {"AT\r\n",
											 "AT+CPIN?\r\n",
											 "AT+CSQ\r\n",
											 "AT+CGREG?\r\n",
											 "AT+CMGF=1\r\n",
											 "AT+CNMI=0,0,0,0,0\r\n"
											};

char sms_cmd[5][60] = {"AT\r\n",
											 "AT+CMGF=1\r\n",
											 "AT+CMGS=\"+84344921037\"\r\n",};

void A7680C_UART_Init(u8 uart, u32 baudrate)
{
	UART_init(uart,baudrate);
	
	for(int i =0; i<6; i++)
	{
		UART_SEND(3, init_cmd[i]);
		DelayMs(100);
	}
}

void A7680C_send_msg(void)
{ 
	char ctrz[4] = {0x1A,'\r','\n'};
	strcpy(sms_cmd[3], location_msg);
	strcpy(sms_cmd[4], ctrz);
	
	for(int i =0; i<5; i++)
	{
		UART_SEND(2, sms_cmd[i]);
		vTaskDelay(pdMS_TO_TICKS(100));
	}
}

char A7680C_get_LBS(void)
{
	UART_SEND(3, "AT\r\n");
	vTaskDelay(pdMS_TO_TICKS(100));
	UART_SEND(3, "AT+CLBS=1,1\r\n");
	vTaskDelay(pdMS_TO_TICKS(100));
	
	return A7680C_buf[7];
}

void A7680C_process_data(char *lat, char *lon, uint8_t sz_lat, uint8_t sz_lon)
{	//"+CLBS: 0,21.013981,105.836349,550\r\n"
	char buf[40];
	strncpy(buf, A7680C_buf, sizeof(buf));
	char *token = strtok(buf, ":"); 
    token = strtok(NULL, ",");     

    token = strtok(NULL, ",");       // Latitude
    if (token) strncpy(lat, token, sz_lat);

    token = strtok(NULL, ",");       // Longitude
    if (token) strncpy(lon, token, sz_lon);
}


void USART3_IRQHandler(void)
{
	static uint8_t i = 0, flag_lbs = 0, not_end = 0;
	uint8_t tmp = UART_RX(3);
	UART_TX(2, tmp);
	
	if(tmp == '+' || not_end == 1)
	{
		not_end = 1;
		if(i < 5 || flag_lbs == 1)
		{
			A7680C_buf[i] = tmp;
			i++;
		}
		else if(i == 5 && A7680C_buf[2] == 'L' && tmp == ':')
		{
				A7680C_buf[i] = tmp;
				i++;
				flag_lbs = 1;
	  }
		if(tmp == '\n') 
		{
			flag_lbs = 0;
			i=0;
			not_end = 0;
		}
	}
}


