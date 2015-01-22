/*
 * lab1.S - LEDs and USART2 in assembly on the STM32F4-Discovery Board
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
 *
 */
	.syntax unified   			/* Use unified style assembly syntax */
	.thumb            			/* Cortex-M3 only supports Thumb code */

    /*
     * On power-up the Cortex-M family processors expect a few pieces
     * of information to be pre-configured at a few pre-defined addresses.
     * Specifically, a table of "vectors" placed at the start of
     * memory (address 0x00000000) contains, in order:
     * 0x00000000 : Address of stack top in RAM
     * 0x00000004 : Address of code to run on start-up/reset
     * 0x00000008 : Address of a hardware fault handler routine
     *
     * The assembler directive ".type <func name>, %function" must
     * be provided for each routine listed in the vector table
     * to hint the linker to provide the correct target address for
     * routines written using "thumb" instructions
     *
     * In order to place the vector table where it belongs in memory
     * it is given the special section name "vectors". This is done
     * using the ".section" directive below so the linker knows which
     * chunk of code is the vector table and can place it where
     * required.  (See the STM32F407VG.ld linker script).
     */
	.type reset_handler, %function
	.type NMI_handler, %function
	.type fault_handler, %function
	.equ STACK_TOP, 0x20000100     /* 256 bytes of stack in RAM region */

	.section vectors
	.word STACK_TOP
	.word reset_handler    /* What to do on power-on */
	.word NMI_handler      /* Non-maskable interrupt handler */
	.word fault_handler    /* Hardware fault handler */

	/* Generic code section */
	.text

/* In the event of a non-maskable interrupt, loop here forever */
NMI_handler:
	b NMI_handler

/* In the event of a fault, loop here forever */
fault_handler:
	ldr r1, =GPIOD_BSRR_SET_12_RED // set leds to red
	mov r2, #1 // set duration to 1 second
	bl led_SR // turn on leds
	bl waitn // wait duration (r2)
	ldr r1, =GPIOD_BSRR_RESET_12 // set leds to off
	bl led_SR // turn off leds
	bl waitn
	b fault_handler


/* wait routine
* r3 must equal clk_period=320000 and r2=duration (s) */
waitn:
	push {r3,lr}
	mul r3, r2 /* 3200000 cycles/second * x seconds */
	b waitloop
waitloop:
	cmp r3, #0
	beq waitend
	sub r3, #1
	b waitloop
waitend:
	pop {r3,pc}
/* end waitn */


/* blink routine lab 1 */
blink:
	push {r1,lr}
	mov r2, #1 // set wait duration
	ldr r1, =GPIOD_BSRR_SET_12_GRN // set leds to green
	bl led_SR // turn on leds
	bl waitn // wait duration (r2)
	ldr r1, =GPIOD_BSRR_RESET_12 // set leds to off
	bl led_SR // turn off leds
	bl waitn
	ldr r1, =GPIOD_BSRR_SET_12_ORG // set leds to orange
	bl led_SR // turn on leds
	bl waitn // wait duration (r2)
	ldr r1, =GPIOD_BSRR_RESET_12 // set leds to off
	bl led_SR // turn off leds
	bl waitn
	ldr r1, =GPIOD_BSRR_SET_12_BLU // set leds to blue
	bl led_SR // turn on leds
	bl waitn // wait duration (r2)
	ldr r1, =GPIOD_BSRR_RESET_12 // set leds to off
	bl led_SR // turn off leds
	bl waitn
	ldr r1, =GPIOD_BSRR_SET_12 // set leds to all
	mov r2, #3
	bl led_SR // turn on leds
	bl waitn
	pop {r1,pc}
	/* light selection passed in r1, duration (sec) passed in r2 */
led_SR:
	push {lr}
	str r1, [r0] // turn leds on
	pop {pc}
/* end blink*/


/* Serially transmits a word over UART */
Hellostring:
	.ascii "hello!\0"
	.align 2
	/* loop to send bits via USART2 */
TXword:
	push {r0,r1,r2,lr}
	ldr r0, =Hellostring // point to start of word
	bl USART2_init // initialize USART
TXloop:
	ldrb r1,[r0] // load target byte into r1
	cmp r1,#0 // check if end of string
	beq return // if end of string, return to handler

	mov r2,r0 // store r0
	mov r0, r1 // pass bit as argument in r0
	bl USART2_send // send bit
	mov r0,r2 // restore address in r0
	add r0,1 // increment pointer towards next bit
	b TXloop
return:
	bl TXnewline
	pop {r0,r1,r2,pc} // else return to handler
/* end Hellostring */


/* Serial recieves character and echoes back to screen */
RXecho:
	push {lr}
	bl USART2_init // initialize USART
	bl USART2_recv // tell USART to begin receiving
	bl TXnewline // send carriage return
	bl USART2_send // send bit (stored in r0)
	pop {pc}
/* end RXecho */


/* serial sends newline over UART */
TXnewline:
	push {r0,lr}
	mov r0, #'\n'
	bl USART2_send
	mov r0, #'\r'
	bl USART2_send
	pop {r0,pc}
/* end TXnewline */


	/* This is the code we want to run out of reset/on start-up */
reset_handler:
	cpsid i					/* disable interupts - power-on default */

  	/* STEP 1: Enable GPIOD Peripheral Clock */
  	.equ RCC_AHB1ENR,          0x40023830  /* ([1]-2.3 Table 2) */
  	.equ RCC_AHB1ENR_GPIODEN,  0x08        /* ([1]-6.3.12) */

  	ldr r0, =RCC_AHB1ENR          /* r0=addr of GPIO Clock Enable Register */
  	ldr r1, =RCC_AHB1ENR_GPIODEN  /* r1=Port D Enable Bit */
  	str r1, [r0]			      /* Store #0x08 into address 0x40023830 */

  	/* STEP2: Configure GPIO connected to LED as output, push-pull */
   	.equ GPIOD_MODER,        0x40020C00  /* ([1]-2.3, [1]-7.4.1) */
  	.equ GPIOD_MODER_GP,     0x55000000  /* ([1]-7.4.1) */
  	ldr r0, =GPIOD_MODER
  	ldr r1, =GPIOD_MODER_GP
  	str r1, [r0]

 	/* Bit pattern for LED colors */
	.equ GPIOD_BSRR,          0x40020C18  /* ([1]-2.3, [1]-7.4.7) */
	.equ GPIOD_BSRR_SET_12,   0x0000F000
	.equ GPIOD_BSRR_SET_12_GRN,   0x00001000  /* ([1]-7.4.7 */
	.equ GPIOD_BSRR_SET_12_ORG,   0x00002000
	.equ GPIOD_BSRR_SET_12_BLU,   0x00008000
	.equ GPIOD_BSRR_SET_12_RED,   0x00004000
	.equ GPIOD_BSRR_RESET_12, 0xF0000000

	/* duration constant */
	.equ BUSYWAIT_1_SECOND, 3200000 /* one second constant */

	/* r0 points to LED port */
	ldr r0, =GPIOD_BSRR

	/* r3 holds duration constant */
	ldr r3, =BUSYWAIT_1_SECOND

	/* lights off */
	ldr r1, =GPIOD_BSRR_RESET_12
	str r1, [r0]

	/* Blink routine */
	//bl blink
	//bl TXword
foo:
	bl RXecho
	b foo

endloop:
	b endloop

	.end


