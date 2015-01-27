/*
 * LED.S - LED support in assembly on the STM32F4-Discovery Board
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
    .global LED_init
    .global LED_on
    .global LED_off
    .global LED_toggle_grn
    .global LED_blink

    /* Constants visible to the world ( due to ".global" below ) */
    .equ LED_grn, 0x1
    .equ LED_org, 0x2
    .equ LED_red, 0x4
    .equ LED_blu, 0x8
    .equ LED_all, 0xF

    .global LED_grn
    .global LED_red
    .global LED_org
    .global LED_blu
    .global LED_all

	/* GPIO PORT status */
	.equ GPIOD_IDR,          0x40020C10
	.equ GPIOD_BSRR,          0x40020C18

	.text

/*
 * LED_init
 *
 * Initialze the GPIOD port to drive the LEDs attached to PD12-PD15
 */
LED_init:
	push {r0-r2, lr}
	/* Enable GPIOD Peripheral Clock (using read-modify-write) */
  	.equ RCC_AHB1ENR,          0x40023830  /* ([1]-2.3 Table 2) */
  	.equ RCC_AHB1ENR_GPIODEN,  0x08        /* ([1]-6.3.12) */
  	ldr r0, =RCC_AHB1ENR          /* r0=addr of GPIO Clock Enable Register */
  	ldr r1, =RCC_AHB1ENR_GPIODEN  /* r1=Port D Enable Bit */
  	ldr r2, [r0]
  	orr r2, r2, r1
  	str r2, [r0]			      /* Store #0x08 into address 0x40023830 */

  	/* Configure GPIOs connected to LEDs as output, push-pull */
   	.equ GPIOD_MODER,          0x40020C00   /* ([1]-2.3, [1]-7.4.1) */
  	.equ GPIOD_MODER_LEDS_OUT, 0x55000000   /* ([1]-7.4.1) */
  	.equ GPIOD_MODER_LEDS_MASK, 0x00fffffff /* Reset bits 23-31 */
  	ldr r0, =GPIOD_MODER
  	ldr r1, =GPIOD_MODER_LEDS_MASK
  	ldr r2, [r0]
  	and r2, r2, r1
  	ldr r1, =GPIOD_MODER_LEDS_OUT
  	orr r2, r2, r1
  	str r2, [r0]

  	/* Return */
  	pop {r0-r2, pc}


/*
 * LED_on:
 *
 * Turn on the LEDs flagged in register r0
 */
LED_on:
	push {r0,r1,lr}
	ldr r1, =GPIOD_BSRR // address of LED port
	lsl r0,#12
	str r0, [r1] // turn on LED with given color
	pop {r0,r1,pc}


/*
 * LED_off:
 *
 * Turn off the LEDs flagged in register r0
 */
LED_off:
	push {r0,r1,lr}
	ldr r1, =GPIOD_BSRR
	lsl r0, #28 // shift 16 bits to reset value
	str r0, [r1] // turn off LED
	pop {r0,r1,pc}


/*
 * LED_toggle_grn:
 *
 * Toggle the state of the green LED
 */
LED_toggle_grn:
	push {r0,lr}
	ldr r0,=LED_grn // pass to LED_status
	bl LED_status // check status of LED
	cmp r0,#0 // if r0=0 LED is off
	it eq
	beq toggle_on
toggle_off: // turn off LED
	ldr r0,=LED_grn
	bl LED_off
	b toggle_grn_END
toggle_on: // turn on LED
	ldr r0,=LED_grn
	bl LED_on
toggle_grn_END:
	pop {r0,pc}

/* Infinite LED blink loop
 * LED passed in r0 */
LED_blink:
    mov r2, #1 // set duration to 1 second
    bl LED_on // turn on leds
    bl waitn // wait duration (r2)
    bl LED_off // turn off leds
    bl waitn
    b LED_blink

/* Checks LEDs flagged in r0
 *	returns r0=1 if ON, r0=0 if OFF */
LED_status:
	push {r1,r2,lr}
	ldr r1, =GPIOD_IDR // address of data reg
	ldr r2, [r1] // contents of IDR
	lsl r0, #12
	and r0,r2 // check status
	cmp r0,#0 // evaluates to 0 if LED off
	ite eq
	moveq r0,#0 // if equal, r0<=0
	movne r0,#1 // else not equal r0<=1
	pop {r1,r2,pc}

	.end



