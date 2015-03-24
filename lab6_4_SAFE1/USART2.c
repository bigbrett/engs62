#include <USART2.h>



/* flag to indicate USART2 has been initialized */
/* toggled in init function */
static uint32_t initialized = 0;

static uint8_t buffer[2];
static uint8_t buff_cnt = 0;



/* module-global pointer to the callback function for rx'd bytes */
/* populated in init function. */
static void(*rx_callback_fn)(uint8_t byte);


/* Rudimentary handler assumes the interrupt is due to a byte rx event */
void __attribute__ ((interrupt)) USART2_handler(void)
{
	uint8_t byte;

	/* must read the USART2_DR to clear the interrupt */
	byte = USART2->DR;

	if (rx_callback_fn)
	{
		rx_callback_fn(byte);
	}
}


void USART2_init(void(*USART2_rx_callback)(uint8_t byte))
{
	rx_callback_fn = USART2_rx_callback;

	/* Enable GPIOD, as USART2 TX is on PD5, and RX is on PD6 */
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIODEN;

	/* Configure PD5 as AF7 (Output, push-pull) */
	GPIOD->AFRL &= GPIOx_AFRL_AFRL5_MASK;
	GPIOD->AFRL |= GPIOx_AFRL_AFRL5_AF7;
	GPIOD->MODER &= GPIOx_MODER_PIN5_MASK;
	GPIOD->MODER |= GPIOx_MODER_PIN5_AF;

	/* Configure PD6 as AF7 (Input) */
	GPIOD->AFRL &= GPIOx_AFRL_AFRL6_MASK;
	GPIOD->AFRL |= GPIOx_AFRL_AFRL6_AF7;
	GPIOD->MODER &= GPIOx_MODER_PIN6_MASK;
	GPIOD->MODER |= GPIOx_MODER_PIN6_AF;

	/* Reset the USART peripheral and enable its clock */
	RCC->APB1ENR &= ~RCC_APB1ENR_USART2;
	RCC->APB1RSTR |= RCC_APB1RSTR_USART2;
	RCC->APB1RSTR &= ~RCC_APB1RSTR_USART2;
	RCC->APB1ENR |= RCC_APB1ENR_USART2;// for part 1
	//			U

	/* Enable the USART peripheral */
	USART2->CR1 |= USARTx_CR1_UE;  /* Enable */

	/* Configure for 8N1, 9600 baud (assuming 16MHz clock) */
	USART2->BRR = 1667;             /* 16MHz/1667 ~= 9600 */

	/* Enable transmit */
	USART2->CR1 |= (USARTx_CR1_TE);

	/*
	 *  If a callback function was registered, enable receive and the
	 *  receive interrupt.
	 */
	if( USART2_rx_callback )
	{
		/* Configure Receive Interrupt */
		NVIC_INTERRUPT_USART_2_ENABLE();
		USART2->CR1 |= USARTx_CR1_RXNEIE;
		USART2->CR1 |= (USARTx_CR1_RE);
	}

	initialized = 1;
}


/* Send a single character out USART2 */
void USART2_putchar(uint8_t byte)
{
	if( initialized )
	{
		/* Wait for the transmit shift register to be free... */
		while( !(USART2->SR & USARTx_SR_TXE) );
		USART2->DR = byte;
	}
}


/* Send a null-terminated string out USART2 */
void USART2_putstr(uint8_t *buffer)
{
	if( initialized )
	{
		while( *buffer != '\0')
		{
			USART2_putchar(*buffer);
			buffer++;
		}
	}
}


/* Takes a 32-bit value and prints on console as hexadecimal number */
void printHex(uint32_t val)
{
	uint32_t nibbles[8]; // array holding each nibble
	uint32_t mask = 0xF; // 1-nibble mask
	int i;

	// foreach nibble, get LSNibble, add offset, then LSR 4 bits to so next nibble is LSN
	for (i=0; i<8; ++i, val>>=4) {
		nibbles[i]= val & mask; // get nibble i

		// add ASCII offsets
		if (nibbles[i]< 10) {
			nibbles[i] += 48;
		}
		else {
			nibbles[i] += 55; }
	}

	// Print array in reverse order
	for (i=7; i>=0; i--) {
		USART2_putchar(nibbles[i]);
	}
	USART2_putchar('\n');
	USART2_putchar('\r');
}


/*
 * adds a byte to USART2 buffer
 */
void USART2_buffer_add(uint8_t byte)
{
	buffer[buff_cnt] = byte;
	buff_cnt++;
}

void USART2_buffer_clr()
{
	buff_cnt = 0;
}

/*
 * Returns number of chars in buffer
 */
uint32_t USART2_buffer_length()
{
	return buff_cnt;
}

/*
 * Returns ASCII representation of buffer as an integer
 */
uint32_t* USART2_buffer_getInt()
{
	return 10*(buffer[0]-48)+(buffer[1]-48);
}

/*
 * returns ASCII representation of buffer as string of characters
 */
uint8_t* USART2_buffer_getStr()
{
	return int2str(USART2_buffer_getInt());
}
