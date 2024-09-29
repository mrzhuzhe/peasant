
#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/gpio.h>
#include <libopencm3/stm32/usart.h>

static void
init_gpio(void) {
	rcc_clock_setup_pll(&rcc_hse_configs[RCC_CLOCK_HSE8_72MHZ]);
    rcc_periph_clock_enable(RCC_GPIOC);

	/* Set GPIO8 (in GPIO port C) to 'output push-pull'. */
	gpio_set_mode(GPIOC,GPIO_MODE_OUTPUT_2_MHZ,
		      GPIO_CNF_OUTPUT_PUSHPULL,GPIO13);

	
}

static void
init_usart(void) {

	rcc_periph_clock_enable(RCC_GPIOA);
	rcc_periph_clock_enable(RCC_USART1);

	// GPIO_USART1_TX/GPIO13 on GPIO port A for tx
	gpio_set_mode(GPIOA,GPIO_MODE_OUTPUT_50_MHZ,GPIO_CNF_OUTPUT_ALTFN_PUSHPULL,GPIO_USART1_TX);

	usart_set_baudrate(USART1,38400);
	usart_set_databits(USART1,8);
	usart_set_stopbits(USART1,USART_STOPBITS_1);

	usart_set_mode(USART1,USART_MODE_TX);
	usart_set_parity(USART1,USART_PARITY_NONE);
	usart_set_flow_control(USART1,USART_FLOWCONTROL_NONE);
	usart_enable(USART1);
}

static inline void uart_putc(char ch) {
    usart_send_blocking(USART1, ch);
}

int
main(void) {

	init_gpio();
	init_usart();
    int c = '0' -1;
    int delay = 5e5;
    int i = 0;
	for (;;) {		
        gpio_toggle(GPIOC,GPIO13);	// Toggle LED
        for (i = 0; i < delay*10; i++){ /* Wait a bit. */
            	__asm__("nop");
        }
        if (++c >= 'Z') {
            uart_putc(c);
            uart_putc('\r');
            uart_putc('\n');
            c = '0' -1;
        } else {
            uart_putc(c);
        }
        
	}

	return 0;
}

/* End uart.cpp */