/*
 * @file main.c
 *
 * @brief Main source code for the PWM program.
 *
 * This file contains the main entry point and function definitions for the PWM program.
 * This lab demonstrates how to generate PWM signals using the two PWM modules: M0PWM and M1PWM.
 *
 * It interfaces with the following:
 *  - User LED (RGB) Tiva C Series TM4C123G LaunchPad
 *	- PMOD BTN module
 *	-	HS-485HB Servo
 *
 * It configures the pins used by the PMOD BTN module to generate interrupts on rising edges.
 * The PMOD BTN module will be used to update the duty cycle value for the HS-485HB Servo.
 * 
 * In addition, Timer 0A is used to generate periodic interrupts every 1 ms. It will
 * update the duty cycle for the RGB LED (specifically PF2) every 5 ms to demonstrate
 * a fading LED effect.
 *
 * @ Adrian Solorzano 
 */

#include "TM4C123GH6PM.h"
#include "SysTick_Delay.h"
#include "PWM_Clock.h"
#include "PWM1_3.h"

#include "PWM0_0.h"
#include "Timer_0A_Interrupt.h"
#include "PMOD_BTN_Interrupt.h"

void Timer_0A_Periodic_Task(void);

void PMOD_BTN_Handler(uint8_t pmod_btn_status);

static uint32_t Timer_0A_ms_elapsed = 0;

static uint16_t RGB_LED_duty_cycle = 0;

static uint8_t increment_duty_cycle = 1;

int main(void)
{
	SysTick_Delay_Init();
	
	PWM_Clock_Init();
	
	PWM1_3_Init(62500, 3125);
	
	PWM0_0_Init(62500, 1875);
	
	PMOD_BTN_Interrupt_Init(&PMOD_BTN_Handler);
	
	Timer_0A_Interrupt_Init(&Timer_0A_Periodic_Task);
	
	while(1)
	{						

	}
}

void PMOD_BTN_Handler(uint8_t pmod_btn_status)
{
	switch(pmod_btn_status)
	{
		case 0x04:
		{
			PWM0_0_Update_Duty_Cycle(1875);
			break;
		}
		case 0x08:
		{
			PWM0_0_Update_Duty_Cycle(3125); // updates buttons based on table values
			break;
		}
		case 0x10:
		{
			PWM0_0_Update_Duty_Cycle(5000);
			break;
		}
		
		case 0x20:
		{
			PWM0_0_Update_Duty_Cycle(7187);
			break;
		}
		
		default:
		{
			break;
		}
	}
}


void Timer_0A_Periodic_Task(void)
{
	Timer_0A_ms_elapsed++;
	
	if ((Timer_0A_ms_elapsed % 5) == 0)
	{
		if (increment_duty_cycle == 1)
		{
			RGB_LED_duty_cycle = RGB_LED_duty_cycle + 50;
			if (RGB_LED_duty_cycle >= 56250)
			{
			increment_duty_cycle = 0;
			}
	  }
	
	else
	{
		RGB_LED_duty_cycle = RGB_LED_duty_cycle - 50; // decrements duty cycle
		if (RGB_LED_duty_cycle == 0) // checks to see if duty cycle is zero
		{
			increment_duty_cycle = 1;  // resets duty cycle
		}
	}	
	 PWM1_3_Update_Duty_Cycle(RGB_LED_duty_cycle); // updates duty cycle
}
	}
