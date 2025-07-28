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
	
	init_GP(PB, 9, OUT50, O_GP_PP);
	GPIOB->ODR &= ~GPIO_ODR_ODR9;
	
	
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
	DF_Send_CMD(SELECT_TRACK, 01, audio_number);
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
    {1, 2, 3},  // LEFT
    {4, 5, 6},  // FRONT
    {7, 8, 9}   // RIGHT
	};

  int level_right = get_alert_level(distance[RIGHT]);
  int level_front = get_alert_level(distance[FRONT]);
	int level_left 	= get_alert_level(distance[LEFT]);
	
	if (level_left == 4 || level_right == 4 || level_front == 4) 
		{
        Buzzer_On;
        return;
    }
	else Buzzer_Off;

	if (level_left == 0 && level_right == 0 && level_front == 0) 
		{
        return;
    }
	if (level_left == 4 && level_right == 4 && level_front == 4) 
		{
        DF_Play(Stop);
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
      DF_Play(file_id);
		}
}





