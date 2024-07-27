/*
 * SERVO_MOTOR.h
 *
 * Created: 7/6/2024 3:33:03 PM
 *  Author: Zyad Ayman
 */ 


#ifndef SERVO_MOTOR_H_
#define SERVO_MOTOR_H_

#include "pwm.h"


//Init of PWM for Servo motor in fast mode ==> Timer1
void SERVO_MOTOR_vInit();

//OUTPUTTING PWM Pulses on OCR1A PIN
void SERVO_MOTOR_vRotate_90ClockWise();

//OUTPUTTING PWM Pulses on OCR1A PIN
void SERVO_MOTOR_vRotate_90Anti_ClockWise();

#endif /* SERVO_MOTOR_H_ */