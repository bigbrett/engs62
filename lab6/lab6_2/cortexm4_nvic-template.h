#pragma once

/*
 * The following definitions are for the "Interrupt Set/Clear Enable" registers
 * in the Nested Vector Interrupt Controller - see the Programming Manual
 * section 4.3.2.  This register functions like the BSRR register used
 * to set/reset the GPIOs in lab 1.  Writing 1 to a bit in the ISER sets
 * the interrupt enable bit, but writing 0 to a bit does not alter the enable bit.
 */
/* Set register addresses */
#define NVIC_ISER1_ADDR (0xE000E100)
#define NVIC_ISER2_ADDR (0xE000E104)
#define NVIC_ISER3_ADDR (0xE000E108)
/* Clear register addresses */
#define NVIC_ICER1_ADDR (0xE000E180)
#define NVIC_ICER2_ADDR (0xE000E184)
#define NVIC_ICER3_ADDR (0xE000E188)

/* modify the value in an ISER register indirectly off its address */
#define NVIC_ISER1 (*((uint32_t *)(NVIC_ISER1_ADDR)))
#define NVIC_ISER2 (*((uint32_t *)(NVIC_ISER2_ADDR)))
#define NVIC_ISER3 (*((uint32_t *)(NVIC_ISER3_ADDR)))

/* similar for clearing the register via its address */
#define NVIC_ISER1 (*((uint32_t *)(NVIC_ISER1_ADDR)))
#define NVIC_ISER2 (*((uint32_t *)(NVIC_ISER2_ADDR)))
#define NVIC_ISER3 (*((uint32_t *)(NVIC_ISER3_ADDR)))

/* EXTI0 is position 6, priorithy 13 */
#define NVIC_INTERRUPT_EXTI_0_ENABLE()        (NVIC_ISER1 = 0x00000040)
#define NVIC_INTERRUPT_EXTI_0_DISABLE()       (NVIC_ICER1 = 0x00000040)

/* USART2 is position 38, priority 45 -- set/reset*/
/* From table 43 in Reference manual, USART2 is mapped to interrrupt position 38.
 * ISER1/ICER1 maps positions 0-31. ISER2/ICER2 maps positions 32 to 63.
 * Position 38 is therefore bit 6 of ISER2/ICER2 -- setting this register
 * to 0x40 will set bit 6.
 */
#define NVIC_INTERRUPT_USART_2_ENABLE()       (NVIC_ISER2 |= 0x00000040)
#define NVIC_INTERRUPT_USART_2_DISABLE()      (NVIC_ICER2 |= 0x00000040)
#define NVIC_INTERRUPT_USART_3_ENABLE()       (NVIC_ISER2 |= 0x00000080)
#define NVIC_INTERRUPT_USART_3_DISABLE()      (NVIC_ICER2 |= 0x00000080)
#define NVIC_INTERRUPT_TIM7_ENABLE()		  (NVIC_ISER2 |= 0x00800000)
#define NVIC_INTERRUPT_TIM6_ENABLE()		  (NVIC_ISER2 |= 0x00400000)

/* ADC is position 18, priority 45 -- set/reset*/
/* From table 43 in Reference manual, ADC is mapped to interrrupt position 18.
 * ISER1/ICER1 maps positions 0-31. ISER2/ICER2 maps positions 32 to 63.
 * Position 18 is therefore bit 18 of ISER1/ICER1 -- setting this register
 * to 0x400000 will set bit 18.  0100 0000 0000 0000 0000
 * */
#define NVIC_INTERRUPT_ADC_ENABLE()			(NVIC_ISER1 |= 0x00040000)
#define NVIC_INTERRUPT_ADC_DISABLE()		(NVIC_ICER1 |= 0x00040000)
