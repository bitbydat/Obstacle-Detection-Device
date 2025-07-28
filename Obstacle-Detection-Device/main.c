#include "allheader.h"

uint32_t obstacle_distance[3] = {0};
char location_msg[60] = {0};
uint16_t millis_tick = 0, last_ticks = 0, pre_ticks = 0;

rmc_t gps;
SemaphoreHandle_t xMutex;

static TaskHandle_t pxAlert = NULL;
static TaskHandle_t pxLocationHandler	= NULL;
static TaskHandle_t pxSendSMS	= NULL;
static TaskHandle_t pxLedBlue = NULL;

void sleep(void)
{
	for(int i = 7200; i>0; i--)
		{
			for (int j = 1000; j>0; j--);
		}
}

void vTaskUltrasonicMeasure(void *ptr)
{
	TickType_t xLastWakeTime;
	xLastWakeTime = xTaskGetTickCount();
	while(1)
	{
		if(xSemaphoreTake(xMutex, portMAX_DELAY) == pdTRUE)
        {
					Ultrasonic_Measure(obstacle_distance);
					xSemaphoreGive(xMutex);
				}
	if(is_obstacle) 
	{
		xTaskNotifyGive(pxLedBlue);
//		xTaskNotifyGive(pxAlert);
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
		if (xSemaphoreTake(xMutex, portMAX_DELAY) == pdTRUE)
        {
					memcpy(local_distance, obstacle_distance, sizeof(obstacle_distance));
					xSemaphoreGive(xMutex);
        }
		Alert_Handle(local_distance);
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
		if(GPS_buf[17] == 'A')    //if GPS data is VALID, process GPS data
		{
			GPS_process_data(GPS_buf, &gps); 
			sprintf( msg, "https://www.google.com/maps?q=%.6lf,%.6lf\r\n", gps.lat.decimal_deg  , gps.lon.decimal_deg);
		}
//		else if(A7680C_get_LBS() == '0')	//if LBS data is VALID, process LBS data
//		{
//				A7680C_process_data(lat, lon, sizeof(lat), sizeof(lon));
//				sprintf( msg, "https://www.google.com/maps?q=%s,%s\r\n", lat, lon);
//		} 
		else if(ESP32_get_LBS() == '0')			//if LBS data is VALID, process LBS data
		{
				ESP32_process_data(lat, lon, sizeof(lat), sizeof(lon));
		    sprintf( msg, "https://www.google.com/maps?q=%s,%s\r\n", lat, lon);
		}
		else 
		{
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
	A7680C_send_msg();
	}
}

int main()
{
	__disable_irq();
	systick_init();	
	ButtonLED_Init();
	ESP32_SPI_Init();
	DF_UART_Init(DF_PORT, DF_BAUDRATE);
//	UART_init(2, 115200);
	A7680C_UART_Init(A7680C_PORT, A7680C_BAUDRATE); 
	GPS_UART_Init(GPS_PORT, GPS_BAUDRATE);
	Ultrasonic_Init();
	
	xMutex = xSemaphoreCreateMutex();
	
	if(xMutex != NULL)
	{
		xTaskCreate(vTaskLedBlue,						"LedBlue",    				configMINIMAL_STACK_SIZE, NULL, 								tskIDLE_PRIORITY, 	 &pxLedBlue);
		xTaskCreate(vTaskUltrasonicMeasure, "UltrasonicMeasure",  configMINIMAL_STACK_SIZE, NULL, 								tskIDLE_PRIORITY,	 NULL);
		xTaskCreate(vTaskAlert, 						"Alert", 							configMINIMAL_STACK_SIZE, NULL, 								tskIDLE_PRIORITY+1, &pxAlert);
		xTaskCreate(vTaskLocationHandler, 	"LocationHandler", 		configMINIMAL_STACK_SIZE, (void*)location_msg, 	tskIDLE_PRIORITY,	 &pxLocationHandler);
		xTaskCreate(vTaskSendSMS, 					"SendSMS", 						configMINIMAL_STACK_SIZE, NULL, 								tskIDLE_PRIORITY+2, &pxSendSMS );
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

void USART1_IRQHandler(void)
{
	static uint8_t count = 0;
	uint8_t tmp = UART_RX(1);
	
//	UART_TX(2, tmp);
	if(tmp == '$')
	{ 
		count = 0;
	}
		GPS_buf[count] = tmp;
		count++;
}

void USART2_IRQHandler(void)
{
	UART_TX(3, UART_RX(2));
}


int get_alert_level(uint16_t distance) 
{
    if (distance < 30 && distance > 5) 		return 4;
    if (distance < 60 && distance > 50) 	return 3;
    if (distance < 120 && distance > 100) return 2;
		if (distance < 200U && distance > 180U) return 1;
    return 0;
}

void Alert_Handle(uint32_t *distance)
{
	uint8_t file_id = 0;
	const uint8_t sound_table[3][3] = 
	{
    {1, 2, 3},  // RIGHT
    {4, 5, 6},  // FRONT
    {7, 8, 9}   // LEFT
	};

  int level_right = get_alert_level(distance[RIGHT]);
  int level_front = get_alert_level(distance[FRONT]);
	int level_left 	= get_alert_level(distance[LEFT]);
	
	Buzzer_Off();
	
	if (level_left == 0 && level_right == 0 && level_front == 0) 
		{
        return;
    }
	
	if (level_left == 4 && level_right == 4 && level_front == 4) 
		{
        DF_Play(10);
        return;
    }
	else if (level_left == 4 || level_right == 4 || level_front == 4) 
		{
        Buzzer_On(300);
        return;
    }
		
	int min_level = 4;
  int selected_dir = -1;

  if (level_left > 0 && level_left < min_level) {
      min_level = level_left;
      selected_dir = LEFT;
  }
  if (level_right > 0 && level_right < min_level) {
      min_level = level_right;
      selected_dir = RIGHT;
  }
  if (level_front > 0 && level_front < min_level) {
      min_level = level_front;
      selected_dir = FRONT;
  }

  if (selected_dir != -1) 
		{
      file_id = sound_table[selected_dir][min_level-1];
//      DF_Play(file_id);
		}
}



void vTaskLedBlue(void *ptr)
{	
	init_GP(PB, 9, OUT50, O_GP_PP);
	while(1)
	{
	ulTaskNotifyTake(pdTRUE, portMAX_DELAY);
	GPIOB->ODR &= ~GPIO_ODR_ODR9;
	sleep();
	GPIOB->ODR |= GPIO_ODR_ODR9;
	sleep();
	GPIOB->ODR &= ~GPIO_ODR_ODR9;
	}
	vTaskDelete(NULL);
}



