/*
 * COMPETEITION_SMART_GARAGE.c
 *
 * Created: 7/6/2024 3:16:08 PM
 * Author : Zyad Ayman
 */ 


#include "main.h"


int main(void)
{
	//Initialization of servo motor PWM Settings
	SERVO_MOTOR_vInit();


	/* Create configuration structure for ADC driver */
	ADC_ConfigType ADC_Configurations = {AVCC,F_CPU_64};

	/*enable I-Bit for external interrupt*/
	EXTI_vEnable_I_Bit();


	/*set the direction of led pin PD0 as output*/
	GPIO_setupPinDirection(PORTD_ID,PIN0_ID,PIN_OUTPUT);

	/*set the direction of the switches pins PD2 & PD3 as input (IR simulators)*/
	GPIO_setupPinDirection(PORTD_ID,PIN2_ID,PIN_INPUT);
	GPIO_setupPinDirection(PORTD_ID,PIN3_ID,PIN_INPUT);
	
	//Activating the internal pullup for pins PD2 & PD3
	GPIO_writePin(PORTD_ID,PIN_NO_2,1);
	GPIO_writePin(PORTD_ID,PIN_NO_3,1);

	/* initialize the led to be turned off*/
	GPIO_writePin(PORTD_ID,PIN0_ID,LOGIC_LOW);

	/*passing the configuration structure to the function by address*/
	ADC_init(&ADC_Configurations);

	LCD_init();/*initialize LCD module*/

	CreatePassword();

	/*Display number of cars inside the garage on LCD*/
	LCD_clearScreen();
	_delay_ms(100);
	LCD_displayString("Cars Number =0");

	/*Display the garage state on LCD*/
	LCD_moveCursor(1,0);
	LCD_displayString("State: Available");

	//INTIALIZING INT0
	EXTI_vConfig_( INT_0 , INPUT , FALLING_EDGE);
	//INTIALIZING INT1
	EXTI_vConfig_(INT_1 , INPUT , FALLING_EDGE);

	while(1)
	{
		/************************************************************************/
		/*                     LDR MODULE                                                 */
		/************************************************************************/		
		Checking_LDR_to_Change_Light_Status();
		
		/************************************************************************/
		/*							ISR1                                                       */

 
}
	}
	
ISR(INT0_vect)
{
	///*trigger the interrupt flag to indicate that INT0 occurs*/
	//INT0_IR1_FLAG=triggered;
	if(Cars_Number == Max_CARS_NUMBER)
	{
		changeToBusyOnLCD();
	}
	else
	{
		enterTheCar();

		if (Cars_Number == (Max_CARS_NUMBER - 1))
		{
			changeToBusyOnLCD();
		}

		Cars_Number++; /*add the entered car to the counter*/
		changeCarsNumberOnLCD();
	}

	SET_BIT(GIFR,6); /*clear the interrupt flag for the next time*/

}

ISR(INT1_vect)
{
	/*trigger the interrupt flag to indicate that INT1 occurs*/
	//INT1_IR2_FLAG=triggered;
	if(Cars_Number == Max_CARS_NUMBER)
	{
		//Opening the gate for car to pass and waiting till pass and closing the gate again
		Exit_TheCar();

		//Displaying Available Word on lcd
		changeToAvailableOnLCD();

		//decrement the cars number by one
		Cars_Number-- ;

		changeCarsNumberOnLCD();
	}
	else
	{
		//Opening the gate for car to pass and waiting till pass and closing the gate again
		Exit_TheCar();

		//decrement the cars number by one
		Cars_Number-- ;

		changeCarsNumberOnLCD();
	}
	SET_BIT(GIFR,7); /*clear the interrupt flag for the next time*/
	
}

void changeToAvailableOnLCD(void)
{
	/*change the garage state to Available*/
	LCD_moveCursor(1,7);
	LCD_displayString("Available");/*display 5 spaces to avoid overwrite of "Available"word*/
}

void changeToBusyOnLCD(void)
{
	/*change the garage state to busy*/
	LCD_moveCursor(1,7);
	LCD_displayString("Busy     ");/*display 5 spaces to avoid overwrite of "Available"word*/
}

void changeCarsNumberOnLCD(void)
{
	LCD_moveCursor(0,13);
	LCD_intgerToString(Cars_Number); /*change number of cars on LCD*/
}

void enterTheCar(void)
{
	/*Rotating the servo motor to open the arm gate*/
	SERVO_MOTOR_vRotate_90ClockWise();

	/*wait the car to pass the arm */
	while (!GPIO_readPin(PORTD_ID,PIN_NO_2));

	/*Rotating the servo motor to close the arm gate*/
	SERVO_MOTOR_vRotate_90Anti_ClockWise();
}


void Exit_TheCar(void)
{
	/*Rotating the servo motor to open the arm gate*/
	SERVO_MOTOR_vRotate_90ClockWise();

	/*wait the car to pass the arm */
	while (!GPIO_readPin(PORTD_ID,PIN_NO_3));

	/*Rotating the servo motor to close the arm gate*/
	SERVO_MOTOR_vRotate_90Anti_ClockWise();
}



void EXTI_vEnable_I_Bit()
{
	SET_BIT(SREG,PIN_NO_7);
}

void Checking_LDR_to_Change_Light_Status(void)
{
	ADC_result=ADC_readChannel(ADC_CHANNEL);

	/* at the night*/
	if(ADC_result >= ADC_THRESHOLD)
	{
		GPIO_writePin(PORTD_ID,PIN0_ID,LOGIC_HIGH);/*turn on the led*/
	}

	/* at the Day light*/
	else if(ADC_result < ADC_THRESHOLD)
	{
		GPIO_writePin(PORTD_ID,PIN0_ID,LOGIC_LOW);/*turn off the led*/
	}
}


/************************************************************************/
/*					PASSWORD FUNCTIONS                                                                     */
/************************************************************************/

uint8 compareArrays(uint8 *arr1,uint8 *arr2,uint8 size)
{
	uint8 i;
	for(i=0;i<size;i++)
	{
		if(arr1[i]!=arr2[i])
		{
			return UNMATCHED_PASSWORDS;
		}
	}
	return MATCHED_PASSWORDS;
}
/************************************************************************/
/*                                                                      */
/************************************************************************/

#define NO_KEY_PRESSED 255 // Define a unique value that represents no key press
uint8 getDebouncedKey(void) {
	uint8 key = KEYPAD_getPressedKey();
	_delay_ms(50);
	if (key == KEYPAD_getPressedKey()) {
		return key;
	}
	return NO_KEY_PRESSED; // Define NO_KEY_PRESSED as a unique value that represents no key press
}


uint8 getStableKey(void) {
	static uint8 lastKey = NO_KEY_PRESSED;
	uint8 currentKey = getDebouncedKey();

	if (currentKey != lastKey) {
		lastKey = currentKey;
		if (currentKey != NO_KEY_PRESSED) {
			return currentKey;
		}
	}
	return NO_KEY_PRESSED;
}

void getPassword(uint8 *pass, uint8 size, uint8 firstPlace) {
	uint8 i = 0, key;
	while (i < size) {
		key = getStableKey();
		if (key != NO_KEY_PRESSED) {
			if (key < 10) { /* if the user enters any number from 0 to 9 store it in pass array */
				pass[i] = key;
				LCD_moveCursor(1, firstPlace + i);
				LCD_displayCharacter('*');
				_delay_ms(900);
				i++;
				} else if (key == 13 && i > 0) { /* handling the delete button */
				i--;
				LCD_moveCursor(1, firstPlace + i);
				LCD_displayCharacter(' ');
				_delay_ms(900);
			}
		}
	}
	/* wait for pressing = (enter) */
	while (getStableKey() != EQUALBUTTON) {}
}

/*function to get the password from the user then store it in an array
 * and display * for each number then go to infinite loop until press = */

// 
// void getPassword(uint8 *pass,uint8 size,uint8 firstPlace)
// {
// 	uint8 i,key;
// 	for(i=0;i<size;i++)
// 	{
// 		key=KEYPAD_getPressedKey();
// 		
// 		_delay_ms(50);
// 		if(key <10 )/*if the user enters any number from 0 to 9 store it in pass array*/
// 		{
// 			pass[i]=key;
// 			LCD_moveCursor(1,firstPlace+i);
// 			LCD_displayCharacter('*');
// 			_delay_ms(900);
// 		}
// 		else if(key ==13)/*handling the delete button */
// 		{
// 			i--;
// 			LCD_moveCursor(1,firstPlace+i);
// 			LCD_displayCharacter(' ');
// 			i--;
// 			_delay_ms(900);
// 		}
// 		else/*if the user enters any other button do nothing and wait a valid number*/
// 		{
// 			while(key >10){key=KEYPAD_getPressedKey();}
// 			_delay_ms(50);
// 		}
// 	}
// 	/*wait for pressing =(enter) */
// 	while(KEYPAD_getPressedKey() !=EQUALBUTTON){}
// }

void CreatePassword(void)
{
	uint8 pass1[PASSWORD_SIZE]={0},pass2[PASSWORD_SIZE]={0},CheckPass;

	LCD_clearScreen();
	LCD_displayString("Enter pass 2 Num");

	/*Get the 1st password from the user then store it in pass1 array
	 * and display '*' for each number entered then wait for pressing =(enter)*/
	getPassword(pass1,PASSWORD_SIZE,0);

	/*clear screen and display the new string*/
	LCD_clearScreen();
	_delay_ms(100);
	LCD_displayString("plz re-enter the");
	LCD_displayStringRowColumn(1,0,"same pass: ");

	/*Get the 2nd password from the user and store it in pass2 array
	 * and display '*' for each number entered and wait for pressing =(enter)*/
	getPassword(pass2,PASSWORD_SIZE,11);

	CheckPass=compareArrays(pass1,pass2,PASSWORD_SIZE);

	/*if 2 different passwords are entered */
	if(CheckPass== UNMATCHED_PASSWORDS)
	{
		LCD_clearScreen();
		LCD_displayString("Passwords differ");
		LCD_displayStringRowColumn(1,0,"Retry");
		_delay_ms(3000);/*delay for vision*/
		CreatePassword();
	}

	/*if 2 matched passwords are entered*/
	else if(CheckPass==MATCHED_PASSWORDS)
		{
			LCD_clearScreen();
			LCD_displayString("Password created");
			LCD_displayStringRowColumn(1,0,"successfully");
			_delay_ms(1500);/*delay for vision*/
		}

}


void EXTI_vConfig_(uint8 copy_u8Int_No , uint8 copy_u8Dirn_Of_Pin , uint8 copy_u8Sense_Opt)
{
	switch(copy_u8Int_No )
	{
		//********

		//Configuring INT0

		//********
		case 0 :
			//enabling INT0
			SET_BIT(GICR ,6);
			//configuring dirn
			if(copy_u8Dirn_Of_Pin == INPUT)
				GPIO_setupPinDirection(PORTD_ID,PIN2_ID,PIN_INPUT);
			else
				GPIO_setupPinDirection(PORTD_ID,PIN2_ID,PIN_OUTPUT);
			//configuring sensing options
			switch(copy_u8Sense_Opt)
			{
				case 0 :
					CLEAR_BIT(MCUCR , 0);
					CLEAR_BIT(MCUCR , 1);
				break;
				case 1 :
					CLEAR_BIT(MCUCR , 1);
					SET_BIT(MCUCR , 0);
				break;
				case 2 :
					CLEAR_BIT(MCUCR , 0);
					SET_BIT(MCUCR , 1);
				break;
				case 3 :
					SET_BIT(MCUCR , 0);
					SET_BIT(MCUCR , 1);
				break;
			}
		break;
		//********

		//Configuring INT1

		//********
		case 1 :
			//enabling INT1

			SET_BIT(GICR ,7);

			//configuring dirn

			if(copy_u8Dirn_Of_Pin == INPUT)
				GPIO_setupPinDirection(PORTD_ID,PIN3_ID,PIN_INPUT);
			else
				GPIO_setupPinDirection(PORTD_ID,PIN3_ID,PIN_OUTPUT);
		//configuring sensing options
			switch(copy_u8Sense_Opt)
			{
				case 0 :
					CLEAR_BIT(MCUCR , 2);
					CLEAR_BIT(MCUCR , 3);
				break;
				case 1 :
					CLEAR_BIT(MCUCR , 3);
					SET_BIT(MCUCR , 2);
				break;
				case 2 :
					CLEAR_BIT(MCUCR , 2);
					SET_BIT(MCUCR , 3);
				break;
				case 3 :
					SET_BIT(MCUCR , 2);
					SET_BIT(MCUCR , 3);
				break;
			}
		break;
	}
}