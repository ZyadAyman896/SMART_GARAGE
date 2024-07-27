/******************************************************************************
 *
 * Module: PWM
 *
 * File Name: pwm.h
 *
 * Description: Header file for the ATmega32 PWM driver
 *
 * Author: Zyad Montaser
 *
 *******************************************************************************/
#ifndef PWM_H_
#define PWM_H_

#include "std_types.h"

/*******************************************************************************
 *                                Definitions                                  *
 *******************************************************************************/
#define pwm_PORT_ID PORTB_ID

#define pwm_PIN_ID  PIN3_ID
/************************************************************************/

/*                       MACROS TO BE USED                               */
#define  PIN_NO_0 0
#define  PIN_NO_1 1
#define  PIN_NO_2 2
#define  PIN_NO_3 3
#define  PIN_NO_4 4
#define  PIN_NO_5 5
#define  PIN_NO_6 6
#define  PIN_NO_7 7

/*						Macros for Directions of pins					*/
#define OUTPUT 1
#define INPUT 0
/************************************************************************/

/*******************************************************************************
 *                              Functions Prototypes                           *
 *******************************************************************************/
/* function responsible for generate pwm signal using timer0 with different duty cycles*/
void PWM_Timer0_Generate(uint8 duty_cycle);

/*Function responsible for initializing the timer1 for pwm in fast mode */
void PWM_vInit_T1_FastMode();

/*Function responsible for assining a value in ICR1 REG*/
void PWM_vSetValue_ICR1_T1_Manual(uint16 copy_u16Value);

/*Function responsible for assining a value in OCR1A REG */
void PWM_vSetValue_OCR1A_T1_Manual(uint16 copy_u16Value);


#endif /* PWM_H_ */
