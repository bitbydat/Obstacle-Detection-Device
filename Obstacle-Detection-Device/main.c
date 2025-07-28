#include "allheader.h"

uint32_t obstacle_distance[3] = {0};
char location_msg[60] = {0};
uint16_t millis_tick = 0, last_ticks = 0, pre_ticks = 0;
uint8_t ultrasonic_stop_flag = 0;
uint8_t is_located_flag = 0;

rmc_t gps;
SemaphoreHandle_t xMutex;

static TaskHandle_t pxAlert = NULL;
static TaskHandle_t pxLocationHandler	= NULL;
static TaskHandle_t pxSendSMS	= NULL;

void vTaskUltrasonicMeasure(void *ptr)
{
	TickType_t xLastWakeTime;
	xLastWakeTime = xTaskGetTickCount();
	while(1)
	{
		if(!ultrasonic_stop_flag)
		{
			if(xSemaphoreTake(xMutex, portMAX_DELAY) == pdTRUE)
			{
				Ultrasonic_Measure(obstacle_distance);
				xSemaphoreGive(xMutex);
				if(is_obstacle) 
				{
					xTaskNotifyGive(pxAlert);
				}
			}
		}
		vTaskDelayUntil(&xLastWakeTime, pdMS_TO_TICKS(500));
	}
}


void vTaskAlert(void *ptr)
{
	uint32_t local_distance[3] = {0}; 
	while(1)
	{
		ulTaskNotifyTake(pdTRUE, portMAX_DELAY);
		if (xSemaphoreTake(xMutex, 0) == pdTRUE)
        {
					memcpy(local_distance, obstacle_distance, sizeof(obstacle_distance));
					xSemaphoreGive(xMutex);
					Alert_Handle(local_distance);
        }
	}
}

void vTaskLocationHandler(void *pvParameter)
{
	char *msg = (char *)pvParameter;
		
	char lat[20] = {0};
	char lon[20] = {0};
	TickType_t xLastWakeTime;
	xLastWakeTime = xTaskGetTickCount();
	
	while(1)
	{
		if(GPS_buf[17] == 'A')    //if GPS data is AVAILABLE, process GPS data
		{
			is_located_flag = TRUE;
			GPS_process_data(GPS_buf, &gps); 
			sprintf( msg, "https://www.google.com/maps?q=%.6lf,%.6lf\r\n", gps.lat.decimal_deg  , gps.lon.decimal_deg);
		}
		else if(A7680C_get_LBS() == '0')	//if 4G-based LBS data is AVAILABLE, process LBS data
		{
			is_located_flag = TRUE;
			A7680C_process_data(lat, lon, sizeof(lat), sizeof(lon));
			sprintf( msg, "https://www.google.com/maps?q=%s,%s\r\n", lat, lon);
		} 
		else if(ESP32_get_LBS() == '0')			//if WiFi-based LBS data is AVAILABLE, process LBS data
		{
			is_located_flag = TRUE;			
			ESP32_process_data(lat, lon, sizeof(lat), sizeof(lon));
		  sprintf( msg, "https://www.google.com/maps?q=%s,%s\r\n", lat, lon);
		}
		else 
		{
			is_located_flag = FALSE;
			strcpy(msg, "Cant get location data\r\n");
		}
		vTaskDelayUntil(&xLastWakeTime, pdMS_TO_TICKS(5000));
	}
}


void vTaskSendSMS(void *ptr)
{
	while(1)
	{
		ulTaskNotifyTake(pdTRUE, portMAX_DELAY);
		if(is_located_flag == TRUE)
		{
			A7680C_send_msg();
			DF_Play(GPS_sent);
		}
		else
		{
			DF_Play(No_GPS);
		}
	}
}

int main()
{
	__disable_irq();
	systick_init();	
	Button_Init();
	ESP32_SPI_Init();
	DF_UART_Init(DF_PORT, DF_BAUDRATE);
	A7680C_UART_Init(A7680C_PORT, A7680C_BAUDRATE); 
	GPS_UART_Init(GPS_PORT, GPS_BAUDRATE);
	Ultrasonic_Init();
	
	xMutex = xSemaphoreCreateMutex();
	
	if(xMutex != NULL)
	{
		xTaskCreate(vTaskUltrasonicMeasure, "UltrasonicMeasure",  configMINIMAL_STACK_SIZE, NULL, 								tskIDLE_PRIORITY,	 	 NULL);
		xTaskCreate(vTaskAlert, 						"Alert", 							configMINIMAL_STACK_SIZE, NULL, 								tskIDLE_PRIORITY+1,	 &pxAlert);
		xTaskCreate(vTaskLocationHandler, 	"LocationHandler", 		configMINIMAL_STACK_SIZE, (void*)location_msg, 	tskIDLE_PRIORITY,	 	 &pxLocationHandler);
		xTaskCreate(vTaskSendSMS, 					"SendSMS", 						configMINIMAL_STACK_SIZE, NULL, 								tskIDLE_PRIORITY+2,  &pxSendSMS);
	}
	__enable_irq();
		
	vTaskStartScheduler();
	
	while(1){}
	return 0;
}

void EXTI0_IRQHandler()
{
	BaseType_t xHigherPriorityTaskWoken = pdFALSE;
	if(EXTI->PR & EXTI_PR_PR0)
	{
		EXTI->PR |= EXTI_PR_PR0;
		vTaskNotifyGiveFromISR(pxSendSMS, &xHigherPriorityTaskWoken); 
	}
	portYIELD_FROM_ISR( xHigherPriorityTaskWoken );
}


void EXTI15_10_IRQHandler(void)
{
	if(EXTI->PR & EXTI_PR_PR12)
	{
		EXTI->PR |= EXTI_PR_PR12;
		ultrasonic_stop_flag ^= 1;
		Buzzer_Off;
		
	}
	

}






