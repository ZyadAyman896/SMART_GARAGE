/*
 * Smart_Garage_System.h
 *
 *  Created on: Jul 6, 2024
 *      Author: Zyad Montaser
 */

#ifndef SMART_GARAGE_SYSTEM_H_
#define SMART_GARAGE_SYSTEM_H_
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include "lcd.h"
#include "keypad.h"
#include "gpio.h"
#include "SERVO_MOTOR.h"
#include "std_types.h"
#include "adc.h"
#include "common_macros.h"
/*******************************************************************************
 *                                Definitions                                  *
 *******************************************************************************/
#define Max_CARS_NUMBER 3
#define ADC_THRESHOLD 511 /* 1023/2=511 */
#define ADC_CHANNEL 0
#define EQUALBUTTON '='
#define PASSWORD_SIZE 2
#define UNMATCHED_PASSWORDS    10 /*any random number */
#define MATCHED_PASSWORDS      20/*any random number */

#define INPUT 0
#define INT_0 0
#define INT_1 1
#define FALLING_EDGE 2
/*******************************************************************************
 *                         Types Declarations                                   *
 *******************************************************************************/
typedef enum
{
	/*Enumeration to define the garage states*/
	Busy,Available
}Garage_State;

typedef enum
{
	/*Enumeration to define the garage state*/
	Untriggered,triggered
}Flag_State;

/*******************************************************************************
 *                      Global Variables                                   *
 *******************************************************************************/
uint8 Cars_Number=0;
uint8 INT0_IR1_FLAG=Untriggered;
uint8 INT1_IR2_FLAG=Untriggered;
uint16 ADC_result=0;


/*******************************************************************************
 *                      Functions Prototypes                                   *
 *******************************************************************************/

//
void changeToAvailableOnLCD(void);

//
void changeToBusyOnLCD(void);

//
void changeCarsNumberOnLCD(void);

//
void enterTheCar(void);

//
void EXTI_vEnable_I_Bit();

//
void Exit_TheCar(void);

//
void Checking_LDR_to_Change_Light_Status(void);

//**************************************************************\\
					PASSWORD FUNCTIONS PROTOTYPES

//
uint8 compareArrays(uint8 *arr1,uint8 *arr2,uint8 size);

//
void getPassword(uint8 *pass,uint8 size,uint8 firstPlace);

//
void CreatePassword(void);







//
void EXTI_vConfig_(uint8 copy_u8Int_No , uint8 copy_u8Dirn_Of_Pin , uint8 copy_u8Sense_Opt);



#endif /* SMART_GARAGE_SYSTEM_H_ */
