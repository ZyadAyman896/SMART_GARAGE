/*
 * SERVO_MOTOR.c
 *
 * Created: 7/6/2024 3:32:47 PM
 *  Author: Zyad Ayman
 */ 

#include "SERVO_MOTOR.h"


//Init of PWM for Servo motor in fast mode ==> Timer1
void SERVO_MOTOR_vInit()
{
		//Initializing PWM of Timer1
		PWM_vInit_T1_FastMode();

	
}


//OUTPUTTING PWM Pulses on OCR1A PIN
void SERVO_MOTOR_vRotate_90ClockWise()
{
	
	//setting OCR1A Value to get 5% duty Cycle
	PWM_vSetValue_OCR1A_T1_Manual(187);
	
	
}

//OUTPUTTING PWM Pulses on OCR1A PIN
void SERVO_MOTOR_vRotate_90Anti_ClockWise()
{
		//setting OCR1A Value to get 7.5% duty Cycle
		PWM_vSetValue_OCR1A_T1_Manual(125);
}