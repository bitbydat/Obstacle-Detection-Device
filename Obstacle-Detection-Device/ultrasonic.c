#include "ultrasonic.h"





void Ultrasonic_Init(void)
{
	RCC->APB2ENR |= RCC_APB2ENR_AFIOEN;
	AFIO->MAPR |= AFIO_MAPR_TIM3_REMAP_1;
	
	timer_pwm_micro(PA,0,20000,1);
	timer_capture_micro(PA, 1, 20000, FALL);
	DelayMs(60);
//	timer_pwm_micro(PB,6,20000,1);
//	timer_capture_micro(PB, 7, 20000, FALL);
//	DelayMs(60);
//	timer_pwm_micro(PB,4,20000,1);
//	timer_capture_micro(PB, 5, 20000, FALL);
//	DelayMs(60);	
}

void Ultrasonic_Measure(uint32_t *distance)
{
	distance[RIGHT] 	=(timer_get_capture_micro(PA, 1)- OFFSET_RIGHT)/58;
	if(Ignore_Distance_Zone(distance))
	{
		KF(RIGHT, distance[RIGHT]);
	}
//	distance[LEFT] 	=(timer_get_capture_micro(PB, 5) - OFFSET_LEFT)/58;
//	if(Ignore_Distance_Zone(distance+1))
//	{
//		KF(LEFT, distance[LEFT]);
//	}
//	distance[FRONT] 	=(timer_get_capture_micro(PB, 7) - OFFSET_FRONT)/58;
//	if(Ignore_Distance_Zone(distance+2))
//	{
//		KF(FRONT, distance[FRONT]);
//	}
}

int Ignore_Distance_Zone(uint32_t *distance)
{
	*distance = ((*distance > 5U && *distance < 200U) ? *distance : 0U);
	return *distance;
}

double KF(uint8_t id, double U)
{
  static const double R[INPUT_NUMS] = {10, 10, 10};
//  static const double H[INPUT_NUMS] = {1.00, 1.00, 1.00};
  static double Q[INPUT_NUMS] = {0.5, 0.5, 0.5};
  static double P[INPUT_NUMS] = {0};
  static double U_hat[INPUT_NUMS] = {0};
  static double K[INPUT_NUMS] = {0};
	
	if (id >= INPUT_NUMS) return 0; 
  
	K[id] = P[id]/(P[id]+R[id]);
  U_hat[id] += + K[id]*(U - U_hat[id]);
  P[id] = (1-K[id])*(P[id]+Q[id]);
  return U_hat[id];
}


