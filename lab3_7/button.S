/*
 * button.S - user button support in assembly on the STM32F4-Discovery Board
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

	.equ ONE_SECOND, 3600000000 // one second in clock cycles
	.equ GPIOA_IDR,			 		0x40020010
	.equ GPIOA_PIN0_BUTTON_MASK,	0x00000001
	.equ DB_CONST, 					100000// one million ticks

	.global button_init
	.global button_scan

    .text

/* ============================================== *
/* *********** STRING DECLARATIONS ************** *
/* ============================================== */
MESSAGE:
	.ascii "Waiting on Button Press >>\n\r\0"
	.align 2

 /* Enable the GPIOA user button peripheral */
button_init:
	push {r0-r3,lr}     /* Save r0-r3 so we can use them in this routine */
	/* Enable GPOIA peripheral clock See: [1]-6.3.12 */
	.equ RCC_AHB1ENR, 0x40023830
	.equ RCC_AHB1ENR_GPIOAEN, 0x00000001
	ldr r0, =RCC_AHB1ENR
	ldr r1, =RCC_AHB1ENR_GPIOAEN
	ldr r2, [r0]     	/* read-modify-write */
	orr r2, r2, r1
	str r2, [r0]
	pop {r0-r3,pc}
/* END button_init */


/* Routine that sits in a tight loop and waits for button press, then debounce*/
button_scan:
	push {r0-r5,lr}
	ldr r2, =GPIOA_IDR
	ldr r3, =GPIOA_PIN0_BUTTON_MASK
	ldr r5, =DB_CONST
	ldr r4, [r2] // initialize LAST value to IDR
	and r4, r3 // mask LAST value
	ldr r0, =MESSAGE
	bl TXword
scanloop: // tight loop to wait for button press
	mov r0, #0 // clear counter
	// LOAD CURRENT VALUE
	ldr r1, [r2] // initialize CURRENT value to IDR
	and r1, r3 // mask CURRENT value
	// CHECK IF CURRENT==LAST
	cmp r1, r4  // check if value changed
	// IF NO CHANGE
	itt eq
	moveq r4,r1 // LAST <= CURRENT
	beq scanloop // keep sampling
	// ELSE IF CHANGE
	mov r4, r1 // TARGET <= CURRENT
debounce:
	ldr r1, [r2] // LOAD CURRENT
	and r1, r3
	// IF COUNTER < MAX
	cmp r0, r5
	blt ctr_incr // go to increment
	// ELSE SIGNAL IS GOOD
	mov r0, r1 // RETURN CURRENT VALUE
	b valid
ctr_incr:
	cmp r1, r4
	ittt eq // if CURRENT== LAST
	addeq r0, #1 // CTR ++
	moveq r4, r1 // LAST <= CURRENT
	beq debounce // check again
	// ELSE CURRENT != LAST, NOT A VALID SIGNAL
	mov r4, r1 // LAST <= CURRENT
	b scanloop
valid: // r0 holds actual value
	cmp r0, #1
	it eq
	// Branch here to whatever subroutine handles button presses
	bleq button_handler

	b scanloop // Scan again
	pop {r0-r5,pc}


/* Kicks off systick up counter on button press */
button_handler:
	push {r0-r3, lr}
	bl lock_mutex // check mutex
	cmp r0, #0
	bne bh_fail // do nothing if locked
	bl ADC_scan // if unlocked, then scan ADC
	bl unlock_mutex // unlock
bh_fail:
	pop {r0-r3, pc} // return





	.end
