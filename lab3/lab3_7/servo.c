/*
 * servo.c - Servo Motor support for stm32f4xx board
 *
 *  Created on: Feb 7, 2015
 *  For use with ENGS 62 Lab 4
 *      Author: Brett Nicholas
 */

/* PUBLIC: controls position of servo motor based on value of external potentiometer */
#include "timer.h"
#include "ADC.h"
#include "stdint.h"
#include "stm32f4xx.h"

void servo_potCtrl(void)
{
	// get pot value
	uint32_t potval = ADC_scanEcho();

	// scale pot value to duty cycle range (between 6% to 9%)
	uint32_t new_dc = potval/41 + 100; // potval/41 maps [0x0, 0xFFF] to [0, 4095] and 100*0.01ms = 1ms (new CK_CNT rate)

	// update duty cycle
	TIM4->TIMx_CCR1 = new_dc;
	//timer_updateDutyCycle(new_dc);
}

void servo_scanLoop(void)
{
	while(1)
		servo_potCtrl();

}
