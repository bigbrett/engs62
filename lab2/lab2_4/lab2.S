/*
 * lab2.S - Interrupts and Mutual Exclusion
 *
 * Target: STM32F4-Discovery w/ STM32F407VGT6 processor,
 *         ROM = 1M   @ 0x08000000
 *         RAM = 192k @ 0x20000000
 *
 */
	.syntax unified   			/* Use unified style assembly syntax */
	.thumb            			/* Cortex-M3 only supports Thumb code */

    /* Linker hints for vector table functions */
	.type reset_handler, %function
	.type NMI_handler, %function
	.type fault_handler, %function
	.type default_handler, %function
	.type systick_handler, %function
	.type USART2_handler, %function

	.equ STACK_TOP, 0x20000100     /* 256 bytes of stack in RAM region */
	.equ BAD_ADDR, 0xFFFFFFFF  	   /* Dummy Fault */
 	.equ USART2_DR, 0x40004404

	.global CTR_ADDR

	 /* VECTOR TABLE */
	.section vectors
	/* System Handlers */
	.word STACK_TOP
	.word reset_handler    /* What to do on power-on */
	.word NMI_handler      /* Non-maskable interrupt handler */
	.word fault_handler    /* Hardware fault handler */
	.word fault_handler    /* Memory manager handler (disabled) */
	.word fault_handler    /* Bus fault (disabled) */
	.word fault_handler    /* Usage fault (disabled) */
	.word 0                /* Reserved */
	.word 0                /* Reserved */
	.word 0                /* Reserved */
	.word 0                /* Reserved */
	.word default_handler  /* System service call */
	.word default_handler  /* Debug Monitor */
	.word default_handler  /* Reserved */
	.word default_handler  /* Pendable service call */
	.word systick_handler  /* Systick */

	/* Peripheral Handlers */
	.rept 38  /* Repeat this block 38 times */
	.word default_handler  /* ISRs 0-37 */
	.endr
	.word USART2_handler   /* ISR 38, USART2 handler */
	.rept 43
	.word default_handler  /* ISRs 39-81 */
	.endr

	/* Generic code section */
	.text

	/* In the event of a non-maskable interrupt, loop here forever */
NMI_handler:
	b NMI_handler

	/* In the event of a fault, loop here forever */
fault_handler:
	ldr r0, =LED_red
	bl LED_blink
    b fault_handler

	/* Address system tick event */
systick_handler:
	push {r0,r1,lr}
	ldr r1, =CTR_ADDR // address of counter
	ldr r0, [r1] // value of CTR
	bl USART2_send
	add r0, #1 // CTR++
	str r0, [r1]
	pop {r0,r1,pc}

	/* Address USART2 interrupts */
USART2_handler:
	push {lr}
	bl RX_LED // LED control
	bl USART2_clr
	pop {pc}

	/* Default handler for un-handled exceptions/interrupts */
default_handler:
	b default_handler


/*
 * NVIC_init:
 *
 * Set the enable bit for Interrupt #38 (USART2).
 * See the Programming Manual section 4.3.2 page 196.
 *
 * NOTE: the NVIC configuration out of power-on/reset is sufficient
 */
NVIC_init:
	.equ NVIC_ISER1, 0xE000E104      /* Interrupt Set Enable Register 1 */
	.equ NVIC_ISER1_USART2_EN, 0x40  /* Enable USART2 Global Interrupts */
	push {r0,r1,lr}
	ldr r0, =NVIC_ISER1
	ldr r1, =NVIC_ISER1_USART2_EN
	str r1, [r0]
	pop {r0,r1,pc}

/*
 * reset_handler:
 *
 * This is the code we want to run out of reset/on start-up
 */
reset_handler:
	/* Global interrupt disable - power on/reset default */
	cpsid i

	/* Enable LEDs and turn them all on */
  	bl LED_init

  	/* Initialize USART2 9600 baud, 8 data, no parity, 1 stop */
  	bl USART2_init

  	/* Enable peripheral interrupts, see routine above */
  	bl NVIC_init

	bl button_init

  	/* Turn all LEDs off */
  	bl LED_off

	/* Global interrupt enable */
  	cpsie i

	/* Show the USART2 peripheral is configured and connected */
	.lcomm CTR_ADDR 4
	bl unlock_mutex
	b button_scan



endloop:
	b endloop  /* spin here forever in a tight loop */

	.end


