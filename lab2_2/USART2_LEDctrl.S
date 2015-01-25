/*
 * USART2_LEDctrl.S - UART2 interface in assembly on the STM32F4-Discovery Board
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

	/* Generic code section */
	.text

	.global RX_LED // USART LED Control w/ echo



	/* GPIO port D Input Data Register -- status of pins */
	.equ GPIOD_IDR,			 0x40020C10

 	/* Bit pattern for LED colors */
	.equ GPIOD_BSRR,          0x40020C18  /* ([1]-2.3, [1]-7.4.7) */
	.equ GPIOD_BSRR_SET_12,   0x0000F000
	.equ GPIOD_BSRR_SET_12_GRN,   0x00001000  /* ([1]-7.4.7 */
	.equ GPIOD_BSRR_SET_12_ORG,   0x00002000
	.equ GPIOD_BSRR_SET_12_BLU,   0x00008000
	.equ GPIOD_BSRR_SET_12_RED,   0x00004000
	.equ GPIOD_BSRR_RESET_12, 		0xF0000000
	.equ GPIOD_BSRR_RESET_12_GRN,  	0x10000000
	.equ GPIOD_BSRR_RESET_12_ORG,	0x20000000
	.equ GPIOD_BSRR_RESET_12_BLU,	0x80000000
	.equ GPIOD_BSRR_RESET_12_RED,	0x40000000



/* String declarations */
GRNstring:
	.ascii "GREEN\ \0"
	.align 2
ORGstring:
	.ascii "ORANGE\ \0"
	.align 2
BLUstring:
	.ascii "BLUE\ \0"
	.align 2
REDstring:
	.ascii "RED\ \0"
	.align 2
ALLstring:
	.ascii "ALL\ TOGGLE\0"
	.align 2
ONstring:
	.ascii "\ ON\0"
	.align 2
OFFstring:
	.ascii "\ OFF\0"
	.align 2

/* Serially transmits a word over UART
		 r0 MUST point to start of word */
TXword:
	push {r1,r2,lr}
TXloop:
	ldrb r1,[r0] // load target byte into r1
	cmp r1,#0 // check if end of string
	beq return // if end of string, return to handle
	mov r2,r0 // store r0
	mov r0, r1 // pass bit as argument in r0
	bl USART2_send // send bit
	mov r0,r2 // restore address in r0
	add r0,1 // increment pointer towards next bit
	b TXloop
return:
	pop {r1,r2,pc} // else return to handler
/* end TXword*/


/* Serial recieves character, stores in r0, then echoes back to screen */
RXecho:
	push {lr}
	bl USART2_recv // tell USART to begin receiving
	bl TXnewline // send carriage return
	bl USART2_send // send bit (stored in r0)
	pop {pc}
/* end RXecho */


/* serial transmit newline over UART */
TXnewline:
	push {r0,lr}
	mov r0, #'\n'
	bl USART2_send
	mov r0, #'\r'
	bl USART2_send
	pop {r0,pc}
/* end TXnewline */


/* receives serial messages to control LEDs */
RX_LED:
	push {r0,r2,lr}
/* Scans for user serial input */
RX_LED_scan:
	bl USART2_recv // r0<=RX_data
	bl TXnewline
	cmp r0,#'g'
	it eq
	bleq toggle_grn // green LED
	cmp r0,#'o'
	it eq
	bleq toggle_org // orange LED
	cmp r0,#'b'
	it eq
	bleq toggle_blu // blue LED
	cmp r0,#'a'
	it eq
	bleq toggle_all // all LEDs
	cmp r0,#'r'
	it eq
	bleq toggle_red // all LEDs
	bl USART2_send // else echo input
	//b RX_LED_scan
	pop {r0,r2,pc}
/* END RX_LED */


toggle_grn:
	push {r0,lr}
	ldr r0, =GRNstring // r0-->"BLUE"
	bl TXword
	ldr r0, =LED_grn // pass LED address
	bl LED_toggle // toggle LED in r0
	bl LED_status // returns r0=0 if off
	cmp r0,#0
	ite eq
	ldreq r0, =ONstring
	ldrne r0, =OFFstring
	bl TXword // report LED Status
	pop {r0,pc}


toggle_org:
	push {r0,lr}
	ldr r0, =ORGstring // r0-->"ORANGE"
	bl TXword
	ldr r0, =LED_org // pass orange LED address
	bl LED_toggle // toggle LED in r0
	bl LED_status // returns r0=0 if off
	cmp r0,#0
	ite eq
	ldreq r0, =ONstring
	ldrne r0, =OFFstring
	bl TXword // report LED Status
	pop {r0,pc}

toggle_blu:
	push {r0,lr}
	ldr r0, =BLUstring // r0-->"BLUE"
	bl TXword
	ldr r0, =LED_blu // pass LED address
	bl LED_toggle // toggle LED in r0
	bl LED_status // returns r0=0 if off
	cmp r0,#0
	ite eq
	ldreq r0, =ONstring
	ldrne r0, =OFFstring
	bl TXword // report LED Status
	pop {r0,pc}


toggle_red:
	push {r0,lr}
	ldr r0, =REDstring // r0-->"RED"
	bl TXword
	ldr r0, =LED_red // pass LED address
	bl LED_toggle // toggle LED in r0
	bl LED_status // returns r0=0 if off
	cmp r0,#0
	ite eq
	ldreq r0, =ONstring
	ldrne r0, =OFFstring
	bl TXword // report LED Status
	pop {r0,pc}


toggle_all:
	push {r0,lr}
	ldr r0, =ALLstring // r0-->"ALL TOGGLE"
	bl TXword // report LED
	bl toggle_grn
	bl toggle_blu
	bl toggle_org
	bl toggle_red
	pop {r0,pc}


	.end


