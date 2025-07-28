#include "dfplayer.h"


u16 checksum;

void DF_UART_Init(u8 uart, u32 baudrate)
{
	UART_init(uart, baudrate);
	DelayMs(50);
	DF_Send_CMD(INIT_CMD, 0, SOURCE_TF_CARD);
	DelayMs(50);
	DF_Send_CMD(0x3D, 0, 0x01);
	DelayMs(50);
	DF_Set_Volume(0x0F);
	
}

void DF_Send_CMD(u8 cmd, u8 para1, u8 para2)
{
	checksum = VERSION + CMD_LENG + cmd + FEEDBACK + para1 + para2;
	checksum = 0xFFFF - checksum + 1;
	
	u8 DF_Packet[10] = {START_BYTE, VERSION, CMD_LENG, cmd, FEEDBACK, para1, para2};
	
	UART_SEND_SERIAL(DF_PORT, DF_Packet, 10);
}


void DF_Play(u8 audio_number)
{
	DF_Send_CMD(SELECT_TRACK, 0, audio_number);
}

void DF_Set_Volume(u8 audio_volume)
{
	DF_Send_CMD(SELECT_VOL, 0, audio_volume);
}

void DF_Increase_Volume(void)
{
	DF_Send_CMD(INCREASE_VOL, 0, 0);
}

void DF_Decrease_Volume(void)
{
	DF_Send_CMD(DECREASE_VOL, 0, 0);
}

void DF_Pause(void)
{
	DF_Send_CMD(PAUSE, 0, 0);
}

void DF_Resume(void)
{
	DF_Send_CMD(RESUME, 0, 0);
}

 void DF_Reset(void)
{
	DF_Send_CMD(RESET, 0, 0);
}





