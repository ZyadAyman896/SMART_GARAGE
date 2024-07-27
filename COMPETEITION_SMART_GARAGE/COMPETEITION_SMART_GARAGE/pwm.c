/******************************************************************************
 *
 * Module: PWM
 *
 * File Name: pwm.c
 *
 * Description: Source file for the ATmega32 PWM driver
 *
 * Author: Zyad Montaser
 *
 *******************************************************************************/

#include <avr/io.h> /* To use PWM/Timer0 Registers */
#include "pwm.h"
#include "gpio.h" /* to use GPIO_setupPinDirection function*/

/* function responsible for generate pwm signal using timer0 with different duty cycles*/
void PWM_Timer0_Generate(uint8 duty_cycle)
{

	TCNT0 = 0; // Set Timer Initial Value to 0

	if(duty_cycle <100)
		OCR0  = (uint8)( ((uint32)duty_cycle*256)/100 ); //Set Compare value
	else if (duty_cycle ==100)
		OCR0=255; /*to avoid the overflow (256 exceed the range of uint8)*/

	GPIO_setupPinDirection(pwm_PORT_ID,pwm_PIN_ID,PIN_OUTPUT); // Configure PB3/OC0 as output pin --> pin where the PWM signal is generated from MC

	/* Configure timer control register
	 * 1. Fast PWM mode FOC0=0
	 * 2. Fast PWM Mode WGM01=1 & WGM00=1
	 * 3. Clear OC0 when match occurs (non inverted mode) COM00=0 & COM01=1
	 * 4. clock = F_CPU/8 CS00=0 CS01=1 CS02=0
	 */
	TCCR0 = (1<<WGM00) | (1<<WGM01) | (1<<COM01) | (1<<CS01);
}


/*Function responsible for initializing the timer1 for pwm in fast mode */
void PWM_vInit_T1_FastMode()
{
	
		//we made it CLR on compare to get the desired correct signal for servo
		// Make OC1A pin as output
		GPIO_setupPinDirection(PORTD_ID, PIN_NO_5,OUTPUT);
		//checked  //Set timer1 count zero
		TCNT1 = 0 ;			
		//for frequency 50Hz  // Set TOP count for timer1 in ICR1 register
		ICR1 = 2499 ; 
		//Set Fast PWM, TOP in ICR1, Clr OC1A on compare match, Clk/64
		TCCR1A = (1<<PIN_NO_7) |  (1<<PIN_NO_1) ;
		TCCR1B = (1<<PIN_NO_3) | (1<<PIN_NO_4) | (1<<PIN_NO_0) | (1<<PIN_NO_1) ;

	
}

/*Function responsible for assining a value in ICR1 REG*/
void PWM_vSetValue_ICR1_T1_Manual(uint16 copy_u16Value)
{
	
	ICR1 = copy_u16Value ;
	
}

/*Function responsible for assining a value in OCR1A REG */
void PWM_vSetValue_OCR1A_T1_Manual(uint16 copy_u16Value)
{
		OCR1A = copy_u16Value ;
}