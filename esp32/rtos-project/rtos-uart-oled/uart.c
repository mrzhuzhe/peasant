


#include <stdlib.h>
#include <string.h>
#include <stdarg.h>

#include <FreeRTOS.h>
#include <task.h>
#include <queue.h>

#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/gpio.h>
#include <libopencm3/stm32/usart.h>
#include <libopencm3/cm3/nvic.h>

#include "uart.h"
#include "getline.h"

#include "oled.h"
#include "adc.h"

#define USART_BUF_DEPTH	32

struct s_uart {
	volatile uint16_t head;			/* Buffer head index (pop) */
	volatile uint16_t tail;			/* Buffer tail index (push) */
	uint8_t		buf[USART_BUF_DEPTH];	/* Circular receive buffer */
};

struct s_uart_info {
	uint32_t	usart;			/* USART address */
	uint32_t	rcc;			/* RCC address */
	uint32_t	irq;			/* IRQ number */
	int		(*getc)(void);
	void		(*putc)(char ch);
};

static struct s_uart_info uarts = { USART1, RCC_USART1, NVIC_USART1_IRQ, uart1_getc, uart1_putc };

static struct s_uart uart_data = {
	0,
	0,
	{ 0 }
};

static QueueHandle_t uart_txq;				// TX queue for UART

/*********************************************************************
 * Receive data for USART
 *********************************************************************/
void usart1_isr(void) {

	struct s_uart *uartp = &uart_data;			/* Access USART's buffer */
	uint32_t uart = uarts.usart;			/* Lookup USART address */
	uint32_t ntail;						/* Next tail index */
	char ch;						/* Read data byte */

	if ( !uartp )
		return;						/* Not open for ISR receiving! */

	while ( USART_SR(uart) & USART_SR_RXNE ) {		/* Read status */
		ch = USART_DR(uart);				/* Read data */
		//ch = usart_recv_blocking(uart);
		ntail = (uartp->tail + 1) % USART_BUF_DEPTH;	/* Calc next tail index */
		/* Save data if the buffer is not full */
		if ( ntail != uartp->head ) {			/* Not full? */
			uartp->buf[uartp->tail] = ch;		/* No, stow into buffer */
			uartp->tail = ntail;			/* Advance tail index */
		}
	}
}

int open_uart(uint32_t uartno,uint32_t baud,const char *cfg,const char *mode,int rts,int cts) {
	uint32_t uart, ux, stopb, iomode, parity, fc;
	struct s_uart_info *infop;
	bool rxintf = false;

	if ( uartno < 1 || uartno > 3 )
		return -1;			/* Invalid UART ref */

	infop = &uarts;		/* USART parameters */
	uart = infop->usart;			/* USART address */
	usart_disable_rx_interrupt(uart);
	/*************************************************************
	 * Parity
	 *************************************************************/

	switch ( cfg[1] ) {
	case 'O':
		parity = USART_PARITY_ODD;
		break;
	case 'E':
		parity = USART_PARITY_EVEN;
		break;
	case 'N':
		parity = USART_PARITY_NONE;
		break;
	/* No Mark parity? Use 2-stop bits for that? */
	default:
		return -2;		// Bad parity
	}
	/*************************************************************
	 * Stop bits
	 *************************************************************/

	stopb = USART_STOPBITS_1;

	switch ( cfg[2] ) {
	case '.':
	case '0':
		stopb = USART_STOPBITS_0_5;
		break;
	case '1':
		if ( cfg[3] == '.' )
			stopb = USART_STOPBITS_1_5;
		else	stopb = USART_STOPBITS_1;
		break;
	case '2':
		stopb = USART_STOPBITS_2;
		break;
	default:
		return -4;
	}

	/*************************************************************
	 * Transmit mode: "r", "w" or "rw"
	 *************************************************************/

	if ( mode[0] == 'r' && mode[1] == 'w' ) {
		iomode = USART_MODE_TX_RX;
		rxintf = true;
	} else if ( mode[0] == 'r' ) {
		iomode = USART_MODE_RX;
		rxintf = true;
	} else if ( mode[0] == 'w' )
		iomode =  USART_MODE_TX;
	else	return -3;		/* Mode fail */

	/*************************************************************
	 * Setup RX ISR
	 *************************************************************/

	if ( rxintf ) {
		// if ( uart_data == NULL ){
		// 	uart_data = (struct s_uart*)malloc(sizeof(struct s_uart));
		// }
		// uart_data->head = (uint16_t)0;	// this line cause dead	seem heap malloc will cause memory corruption
		// uart_data->tail = (uint16_t)0;
		uart_data.head = 0;
		uart_data.tail = 0;	
	}
	
	/*************************************************************
	 * Flow control mode:
	 *************************************************************/

	fc = USART_FLOWCONTROL_NONE;
	if ( rts ) {
		if ( cts )
			fc = USART_FLOWCONTROL_RTS_CTS;
		else	fc = USART_FLOWCONTROL_RTS;
	} else if ( cts ) {
		fc = USART_FLOWCONTROL_CTS;
	}
	/*************************************************************
	 * Establish settings:
	 *************************************************************/	
	rcc_periph_clock_enable(infop->rcc);
	usart_set_baudrate(uart,baud);
	usart_set_databits(uart,cfg[0]&0x0F);
	usart_set_stopbits(uart,stopb);
	usart_set_mode(uart,iomode);
	usart_set_parity(uart,parity);
	usart_set_flow_control(uart,fc);

	nvic_enable_irq(infop->irq);
	usart_enable(uart);
	usart_enable_rx_interrupt(uart);

	return 0;		/* Success */
}

void init_usart(void) {

	rcc_periph_clock_enable(RCC_GPIOA);

	// GPIO_USART1_TX/GPIO13 on GPIO port A for tx
	gpio_set_mode(GPIOA,GPIO_MODE_OUTPUT_50_MHZ,GPIO_CNF_OUTPUT_ALTFN_PUSHPULL,GPIO_USART1_TX);	//  GPIOA10

	// interrupt https://github.com/libopencm3/libopencm3-examples/blob/master/examples/stm32/f1/obldc/usart_irq/usart_irq.c
	gpio_set_mode(GPIOA, GPIO_MODE_INPUT,GPIO_CNF_INPUT_FLOAT, GPIO_USART1_RX);	//  GPIOA9

	open_uart(1,38400,"8N1","rw", 0, 0);
	uart_txq = xQueueCreate(256,sizeof(char));
}

/*********************************************************************
 * Get cooked input line
 *********************************************************************/

int
getline_uart(uint32_t uartno,char *buf,uint32_t bufsiz) {
	struct s_uart_info *uart = &uarts;

	return getline(buf,bufsiz,uart->getc,uart->putc);
}

/*********************************************************************
 * Put one character to device, block (yield) until TX ready (blocking)
 *********************************************************************/
void
putc_uart(uint32_t uartno,char ch) {
	uint32_t uart = uarts.usart;

	while ( (USART_SR(uart) & USART_SR_TXE) == 0 )
		taskYIELD();	
	usart_send_blocking(uart,ch);
}

/*********************************************************************
 * Send Null terminated string, yeilding (blocking)
 *********************************************************************/

void
puts_uart(uint32_t uartno,const char *buf) {
	uint32_t uart = uarts.usart;

	while ( *buf ) {
		while ( (USART_SR(uart) & USART_SR_TXE) == 0 )
			taskYIELD();	
		usart_send_blocking(uart,*buf++);
	}
}

/*********************************************************************
 * Internal: Return data given s_uart *
 *********************************************************************/

static int
get_char(struct s_uart *uptr) {
	char rch;

	if ( uptr->head == uptr->tail )
		return -1;	// No data available
	rch = uptr->buf[uptr->head];	
	uptr->head = ( uptr->head + 1 ) % USART_BUF_DEPTH;
	return rch;
}

/*********************************************************************
 * Receive a byte, non-blocking
 *********************************************************************/

int
getc_uart_nb(uint32_t uartno) {
	struct s_uart *uptr = &uart_data;

	if ( !uptr )
		return -1;	// No known uart
	return get_char(uptr);
}

/*********************************************************************
 * Receive a byte, blocking
 *********************************************************************/

char
getc_uart(uint32_t uartno) {
	struct s_uart *uptr = &uart_data;
	int rch;

	if ( !uptr )
		return -1;	// No known uart
	while ( (rch = get_char(uptr)) == -1 )
		taskYIELD();
	return (char)rch;
}

void uart1_putc(char ch) {
	if ( ch == '\n' )
		putc_uart(1,'\r');
	putc_uart(1,ch);
}

int uart1_getc(void) {
	return getc_uart(1);
}

void uart_task(void *args) {
	int gc;
	char kbuf[256], ch;

	//puts_uart(1,"\n\ruart_task() has begun:\n\r");	
	for (;;) {
		//usart_send_blocking(uarts.usart, 'a');
		usart1_isr();
		if ( (gc = getc_uart_nb(1)) != -1 ) {
			//usart_send_blocking(uarts.usart, gc);
			puts_uart(1,"\r\n\nENTER INPUT: ");

			ch = (char)gc;
			if ( ch != '\r' && ch != '\n' ) {
				/* Already received first character */
				kbuf[0] = ch;
				putc_uart(1,ch);
				//getline_uart(1,kbuf+1,sizeof kbuf-1);
			} else	{
				/* Read the entire line */
				//getline_uart(1,kbuf,sizeof kbuf);
			}

			puts_uart(1,"\r\nReceived input '");
			puts_uart(1,kbuf);
			puts_uart(1,"'\n\r\nResuming prints...\n\r");
		}

		/* Receive char to be TX */
		if ( xQueueReceive(uart_txq,&ch,10) == pdPASS )
			putc_uart(1,ch);
	}
}