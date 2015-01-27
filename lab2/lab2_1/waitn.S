/*
 * waitn.S - waitn support in assembly on the STM32F4-Discovery Board
 *
 * Target: STM32F4-Discovery w/ STM32F407VGT6 processor,
 *         ROM = 1M   @ 0x08000000
 *         RAM = 192k @ 0x20000000
 *
 * REFERENCES:
 *
 * All documentation references written as "[#]-<section>" where:
 *
 * [1] = RM0090 STM32F407 Reference Manual.pdf
 * [2] = STM32F4 Discovery Schematic.pdf
 * [3] = STM32F407xx Data Sheet.pdf
 *
 */
	.syntax unified   			/* Use unified style assembly syntax */
	.thumb            			/* Cortex-M3 only supports Thumb code */

    /* Make these routines visible to other modules */
    .global waitn

	.equ BUSYWAIT_1_SECOND,		3200000

    .text

/*
 * waitn
 * r3 must equal clk_period=320000 and r2=duration (s) */
waitn:
    push {r3,lr}
	ldr r3, =BUSYWAIT_1_SECOND
    b waitloop
waitloop:
    cmp r3, #0
    beq waitend
    sub r3, #1
    b waitloop
waitend:
    pop {r3,pc}
/* end waitn */


