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
	b fault_handler

	/* LED OPERATIONS */
blink_grn: // Turns on green LED
	push {r0,r1,lr}
	ldr r1, =GPIOD_BSRR_SET_12_GRN // green
	str r1, [r0]
	//bl waitn
	//bl lightsoff
	pop {r0,r1,pc}

blink_org: // Turns on orange LED
	push {r0,r1,lr}
	ldr r1, =GPIOD_BSRR_SET_12_ORG // orange
	str r1, [r0]
	//bl waitn
	//bl lightsoff
	pop {r0,r1,pc}

blink_blu: // turns on blue LED
	push {r0,r1,lr}
	ldr r1, =GPIOD_BSRR_SET_12_BLU // blue
	str r1, [r0]
	//bl waitn
	//bl lightsoff
	pop {r0,r1,pc}

blink_all: // turns all LEDS on
	push {r0,r1,lr}
	ldr r1, =GPIOD_BSRR_SET_12 // all on
	str r1, [r0]
	//bl waitn
	//bl lightsoff
	pop {r0,r1,pc}

waitn: // r2 must equal clk_period=320000 for 1 second loop
	push {r0,r1,lr}
	mov r0, #1
	mul r0, r2
	b waitloop

waitloop:
	cmp r0, #0
	beq waitend
	sub r0, #1
	b waitloop

waitend:
	pop {r0,r1,pc}

lightsoff:
	/* Turn the LED off */
	push {r0,r1,lr}
	ldr r1, =GPIOD_BSRR_RESET_12
	str r1, [r0]
	pop {r0,r1,pc}

blink:
	push {r0,r1,lr}
	bl blink_grn
	bl waitn
	bl lightsoff

	bl blink_org
	bl waitn
	bl lightsoff

	bl blink_blu
	bl waitn
	bl lightsoff

	bl blink_all
	bl waitn
	bl waitn
	bl waitn
	bl lightsoff
	pop {r0,r1,pc}


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

 	/* Constants for LED on for 1 second, then off */
	.equ GPIOD_BSRR,          0x40020C18  /* ([1]-2.3, [1]-7.4.7) */
	.equ GPIOD_BSRR_SET_12,   0x0000F000
	.equ GPIOD_BSRR_SET_12_GRN,   0x00001000  /* ([1]-7.4.7 */
	.equ GPIOD_BSRR_SET_12_ORG,   0x00002000
	.equ GPIOD_BSRR_SET_12_BLU,   0x00008000
	.equ GPIOD_BSRR_RESET_12, 0xF0000000
	.equ BUSYWAIT_1_SECOND, 3200000 /* one second constant */

	/* load 1 second delay constant */
	ldr r2, =BUSYWAIT_1_SECOND

	/* r0 points to LED port */
	ldr r0, =GPIOD_BSRR

	/* lights off */
	ldr r1, =GPIOD_BSRR_RESET_12
	str r1, [r0]

	/* Blink routines */

	mov r3, #3
thrice:
	cmp r3, #0
	beq endloop

	sub r3, #1
	bl blink
	b thrice

endloop:
	b endloop  /* spin here forever in a tight loop */

	.end


