
#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/gpio.h>
#include <libopencm3/stm32/usart.h>
#include <libopencm3/cm3/nvic.h>
#include "oled.h"
#include <string.h>
#include "adc.h"

void
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
	usart_enable(USART1);
}

static inline void uart_putc(char ch) {
    usart_send_blocking(USART1, ch);
}

static inline void uart_put_str(char* str, uint16_t len) {
    for (uint16_t i =0;i <len;i++){
		uart_putc(str[i]);
	}
}

uint16_t uart_getc() {
    return usart_recv_blocking(USART1);
}

void itoa(int num, char* str, int Length){
	for (int i = 0; i < Length; i++)							
	{
		str[i] = (num / OLED_Pow(10, Length - i - 1) % 10 + '0');
	}
}

#define BUFFER_LEN 5
char buff[BUFFER_LEN];
uint8_t buffer_index = 0;
void handle_buff(uint8_t data, uint8_t len){
	buff[buffer_index] = data;
	buffer_index+=1;
	if (data == '\n' || buffer_index >= len) {
		for (uint8_t i = buffer_index; i < len; i++){
			buff[i] = ' ';
		}
		buffer_index = 0;
		if (strncmp(buff, "12345", 5) == 0) {
			OLED_ShowString(2, 6, "Bingo");
		} else {
			OLED_ShowString(2, 6, buff);
		}
		// read adc 2
		int adc2 = read_adc(2);
		const int len = 5;
		char snum[len];
		itoa(adc2, snum, len);
		uart_put_str(snum, len);
		OLED_ShowNum(3, 6, adc2, len);
	}
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
		//	 accumulate and handle cmd
		handle_buff(data, BUFFER_LEN);

		/* Enable transmit interrupt so it sends back the data. */
		USART_CR1(USART1) |= USART_CR1_TXEIE;
		
		gpio_set(GPIOC,GPIO13);
	}

	/* Check if we were called because of TXE. */	
	// if (((USART_CR1(USART1) & USART_CR1_TXEIE) != 0) &&
	// 	((USART_SR(USART1) & USART_SR_TXE) != 0)) {
	// 	/* Indicate that we are sending out data. */
	// 	gpio_clear(GPIOC, GPIO13);        

	// 	/* Put data into the transmit register. */
	// 	uart_putc(data);		

	// 	/* Disable the TXE interrupt as we don't need it anymore. */
	// 	USART_CR1(USART1) &= ~USART_CR1_TXEIE;
		
	// 	gpio_set(GPIOC,GPIO13);
	// }	
	
}