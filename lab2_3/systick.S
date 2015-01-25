/*
 * systick.S - systick support in assembly on the STM32F4-Discovery Board
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
    .global systick_init

	.equ SYSTICK_CSR, 0xE000E010 // SYSTICK Control and Status Register
	.equ SYSTICK_RVR, 0xE000E014 // SYSTICK Reload Value Register
	.equ SYSTICK_CVR, 0xE000E018 // SYSTICK Current Value Register
	.equ SYSTICK_ENABLE_MASK, 0x00000007 // orring sets enable to 1

	.equ ONE_SECOND, 3600000000 // one second in clock cycles
    .text

/*
 * Initialize SYSTICK interrupt
 */
systick_init:
	push {r0-r2, lr}
	// Disable Systick
	ldr r0, =SYSTICK_CSR // address of CSR
	mov r1, #0
	str r1, [r0] // disable

	// Write new reload value-->RVR
	ldr r0, =SYSTICK_RVR
	ldr r1, =ONE_SECOND
	str r1, [r0] // 1 second freqency

	// Write to CVR to clear value to 0
	ldr r0, =SYSTICK_CVR
	mov r1, #0
	str r1, [r0] // clear value

	// Write to CSR to start SYSTICK timer
	ldr r0, =SYSTICK_CSR
	ldr r2, =SYSTICK_ENABLE_MASK
	ldr r1, [r0] // read
	orr r1, r2 // modify
	str r1, [r0] // write

  	pop {r0-r2, pc}
/* END SYSTICK_INIT */

	.end
