#include "esp32.h"


char ESP32_buf[40] = {0};

void ESP32_SPI_Init(void)
{
 	init_GP(PA, 4, OUT50, O_GP_PP);		//CS Pin  - D5(ESP32)
	init_GP(PA, 5, OUT50, O_AF_PP);		//SCK Pin - D18(ESP32)
	init_GP(PA, 6, IN, I_F);					//MISO Pin - D19(ESP32)
	init_GP(PA, 7, OUT50, O_AF_PP);		//MOSI Pin - D23(ESP32)

	RCC->APB2ENR |= RCC_APB2ENR_SPI1EN;
	SPI1->CR1 &= ~0xFFFF;
	SPI1->CR2 &= ~0xFFFF;
	//Configure STM32 as Master mode; Baud rate = fPCLK/16; Software slave management enabled; Enable SPI
	SPI1->CR1 |= SPI_CR1_MSTR | SPI_CR1_BR_1 | SPI_CR1_BR_0 
								| SPI_CR1_SSM | SPI_CR1_SSI | SPI_CR1_SPE;
}

void ESP32_TxRx(char *txdata, char *rxdata, char size)
{
	for(int i = 0; i<size; i++)
	{
		while(!(SPI1->SR & (SPI_SR_TXE))) {}  // wait for TXE bit to set -> This will indicate that the buffer is empty
	  SPI1->DR = txdata[i];  
	  while(!(SPI1->SR & (SPI_SR_RXNE))){}
		/*Read data from data register*/
		*rxdata++ = (SPI1->DR);
	}
	while (((SPI1->SR)&(SPI_SR_BSY))) {};  // wait for BSY bit to Reset -> This will indicate that SPI is not busy in communication	
	/*Clear OVR flag*/
	(void)SPI1->DR;
	(void)SPI1->SR;
}


void ESP32_Rx(char *rxdata, char size)
{
	for(int i = 0; i<size; i++)
	{
		while (!((SPI1->SR)&(SPI_SR_TXE))) {};  // wait for TXE bit to set -> This will indicate that the buffer is empty
	  SPI1->DR = 0xFF;  // send dummy data to slave
	  while(!(SPI1->SR & (SPI_SR_RXNE))){}
		/*Read data from data register*/
		*rxdata++ = (SPI1->DR);
	}
	while (((SPI1->SR)&(SPI_SR_BSY))) {};  // wait for BSY bit to Reset -> This will indicate that SPI is not busy in communication	
	/*Clear OVR flag*/
	(void)SPI1->DR;
	(void)SPI1->SR;
}

char ESP32_get_LBS(void)
{
	char tx_bufA[16] = {'A'};
	char tx_bufB[16] = {'B'};
	
	vTaskDelay(pdMS_TO_TICKS(100));
	ESP32_select();
	ESP32_TxRx(tx_bufA, ESP32_buf, 16);
	ESP32_unselect();
	
	vTaskDelay(pdMS_TO_TICKS(100));
	ESP32_select();
	ESP32_TxRx(tx_bufB, ESP32_buf+16, 16);
	ESP32_unselect();
	
	return ESP32_buf[0];
}

void ESP32_process_data(char *lat, char *lon, uint8_t sz_lat, uint8_t sz_lon)
{
	//LBS message: "0,21.013463,105.834371,50\r\n"
	char buf[40];
	strncpy(buf, ESP32_buf, sizeof(buf));
	char *token = strtok(buf, ","); 

  token = strtok(NULL, ",");       // Latitude
  if (token) strncpy(lat, token, sz_lat);
  token = strtok(NULL, ",");       // Longitude
  if (token) strncpy(lon, token, sz_lon);
}

