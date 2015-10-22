/*
 * servo.h - Servo Motor support for stm32f4xx board
 *
 *  Created on: Feb 7, 2015
 *  For use with ENGS 62 Lab 4
 *      Author: Brett Nicholas
 */
#include "stdint.h"
#include "timer.h"
#include "ADC.h"
#include "stm32f4xx_timer.h"
#include "stm32f4xx_gpio.h"

/* PUBLIC: controls position of servo motor based on value of external potentiometer */
void servo_potCtrl();





