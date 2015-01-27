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
    .global waitctr

	.equ BUSYWAIT_1_SECOND,	32000000
	.equ BUSYWAIT_CTR, 		16000000

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



/* increments r0 every second */
waitctr:
	push {r0-r3,lr}
	mov r0, #5 // initialize outside loop
countloop:
	cmp r0, #0
	beq countend
	ldr r3, =BUSYWAIT_CTR // initialize inside loop
waitctrloop:
    cmp r3, #0
    beq waitctrend
    sub r3, #1
    b waitctrloop
waitctrend:
	bl USART2_send // echo count in r0
	sub r0, #1
	b countloop
countend:
	pop {r0-r3,pc}



.end
