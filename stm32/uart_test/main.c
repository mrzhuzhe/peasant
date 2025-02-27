
#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/gpio.h>
#include <libopencm3/stm32/usart.h>
#include <libopencm3/cm3/nvic.h>

const int delay = 5e5;

static void
init_gpio(void) {
	rcc_clock_setup_pll(&rcc_hse_configs[RCC_CLOCK_HSE8_72MHZ]);
    rcc_periph_clock_enable(RCC_GPIOC);

	/* Set GPIO8 (in GPIO port C) to 'output push-pull'. */
	gpio_set_mode(GPIOC,GPIO_MODE_OUTPUT_2_MHZ,
		      GPIO_CNF_OUTPUT_PUSHPULL,GPIO13);

	gpio_set_mode(GPIOC,GPIO_MODE_OUTPUT_2_MHZ,
		      GPIO_CNF_OUTPUT_PUSHPULL,GPIO14);
			  

	gpio_set(GPIOC,GPIO13);
	gpio_set(GPIOC,GPIO14);

}

static void
init_usart(void) {

	rcc_periph_clock_enable(RCC_GPIOA);
	rcc_periph_clock_enable(RCC_USART1);

	/* Enable the USART1 interrupt. */
	nvic_enable_irq(NVIC_USART1_IRQ);

	// GPIO_USART1_TX/GPIO13 on GPIO port A for tx
	gpio_set_mode(GPIOA,GPIO_MODE_OUTPUT_50_MHZ,GPIO_CNF_OUTPUT_ALTFN_PUSHPULL,GPIO_USART1_TX);	//  GPIOA10

	// interrupt https://github.com/libopencm3/libopencm3-examples/blob/master/examples/stm32/f1/obldc/usart_irq/usart_irq.c
	gpio_set_mode(GPIOA, GPIO_MODE_INPUT,GPIO_CNF_INPUT_FLOAT, GPIO_USART1_RX);	//  GPIOA9

	usart_set_baudrate(USART1,38400);
	usart_set_databits(USART1,8);
	usart_set_stopbits(USART1,USART_STOPBITS_1);

	usart_set_mode(USART1,USART_MODE_TX_RX);
	usart_set_parity(USART1,USART_PARITY_NONE);
	usart_set_flow_control(USART1,USART_FLOWCONTROL_NONE);

	/* Enable USART1 Receive interrupt. */
	USART_CR1(USART1) |= USART_CR1_RXNEIE;
	;
	usart_enable(USART1);
}

static inline void uart_putc(char ch) {
    usart_send_blocking(USART1, ch);
}

uint16_t uart_getc() {
    return usart_recv_blocking(USART1);
}

void usart1_isr(void)
{
	static uint8_t data = 'A';
	int i = 0;
	/* Check if we were called because of RXNE. */
	if (((USART_CR1(USART1) & USART_CR1_RXNEIE) != 0) &&
		((USART_SR(USART1) & USART_SR_RXNE) != 0)) {
		/* Indicate that we got data. */
		gpio_clear(GPIOC, GPIO13);

		/* Retrieve the data from the peripheral. */
		data = uart_getc();

		/* Enable transmit interrupt so it sends back the data. */
		USART_CR1(USART1) |= USART_CR1_TXEIE;
		
		// for (i = 0; i < delay*10; i++){ /* Wait a bit. */
		// 	__asm__("nop");
		// }
		gpio_set(GPIOC,GPIO13);
	}

	/* Check if we were called because of TXE. */
	if (((USART_CR1(USART1) & USART_CR1_TXEIE) != 0) &&
		((USART_SR(USART1) & USART_SR_TXE) != 0)) {
		/* Indicate that we are sending out data. */
		gpio_clear(GPIOC, GPIO14);

		/* Put data into the transmit register. */
		uart_putc(data);

		/* Disable the TXE interrupt as we don't need it anymore. */
		USART_CR1(USART1) &= ~USART_CR1_TXEIE;
		
		// for (i = 0; i < delay*10; i++){ /* Wait a bit. */
		// 	__asm__("nop");
		// }
		gpio_set(GPIOC,GPIO14);
	}
	
}

int
main(void) {

	init_gpio();
	init_usart();
    int c = '0' -1;
    int i = 0;
	for (;;) {		
        // gpio_toggle(GPIOC,GPIO13);	// Toggle LED
        // for (i = 0; i < delay*10; i++){ /* Wait a bit. */
        //     	__asm__("nop");
        // }
        // if (++c >= 'Z') {
        //     uart_putc(c);
        //     uart_putc('\r');
        //     uart_putc('\n');
        //     c = '0' -1;
        // } else {
        //     uart_putc(c);
        // }
		usart1_isr();
        
	}

	return 0;
}

/* End uart.cpp */