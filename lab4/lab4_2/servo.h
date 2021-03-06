/*
 * servo.h - Servo Motor support for stm32f4xx board
 *
 *  Created on: Feb 7, 2015
 *  For use with ENGS 62 Lab 4
 *      Author: Brett Nicholas
 */

/* PUBLIC: controls position of servo motor based on value of external potentiometer */
//void servo_potCtrl();

/* rotate servo to 0 degrees */
void servo_gateClose();

/* rotate servo to 90 degrees */
void servo_gateOpen();




