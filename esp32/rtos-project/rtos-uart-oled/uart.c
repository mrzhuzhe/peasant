


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

/*********************************************************************
 * Receive buffers
 *********************************************************************/

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

static struct s_uart_info uarts[3] = {
	{ USART1, RCC_USART1, NVIC_USART1_IRQ, uart1_getc, uart1_putc },
	{ USART2, RCC_USART2, NVIC_USART2_IRQ, uart2_getc, uart2_putc },
	{ USART3, RCC_USART3, NVIC_USART3_IRQ, uart3_getc, uart3_putc }
};

static struct s_uart *uart_data[3] = { NULL, NULL, NULL };

/*********************************************************************
 * Receive data for USART
 *********************************************************************/

static void
uart_common_isr(unsigned ux) {
	struct s_uart *uartp = uart_data[ux];			/* Access USART's buffer */
	uint32_t uart = uarts[ux].usart;			/* Lookup USART address */
	uint32_t ntail;						/* Next tail index */
	char ch;						/* Read data byte */

	if ( !uartp )
		return;						/* Not open for ISR receiving! */

	while ( USART_SR(uart) & USART_SR_RXNE ) {		/* Read status */
		ch = USART_DR(uart);				/* Read data */
		ntail = (uartp->tail + 1) % USART_BUF_DEPTH;	/* Calc next tail index */

		/* Save data if the buffer is not full */
		if ( ntail != uartp->head ) {			/* Not full? */
			uartp->buf[uartp->tail] = ch;		/* No, stow into buffer */
			uartp->tail = ntail;			/* Advance tail index */
		}
	}
}

/*********************************************************************
 * USART1 ISR
 *********************************************************************/

void
usart1_isr(void) {
	uart_common_isr(0);
}

/*********************************************************************
 * USART2 ISR
 *********************************************************************/

void
usart2_isr(void) {
	uart_common_isr(1);
}

/*********************************************************************
 * USART3 ISR
 *********************************************************************/

void
usart3_isr(void) {
	uart_common_isr(2);
}

/*********************************************************************
 * Open the UART for I/O:
 *
 * ARGUMENTS:
 *	1.	uartno		1 == USART1, ... 3 == USART3
 *	2.	baud		Baud rate, eg. 38400
 *	3.	cfg		Config: eg. "8N1"
 *	4.	mode		"r", "rw" or just "w"
 *	5.	rts		When True: Use RTS
 *	6.	cts		When True: Use CTS
 *
 * RETURNS:
 *	0	Success
 *	-1	Fail: Bad uartno
 *	-2	Fail: Bad parity config
 *	-3	Fail: Bad mode config (r/w)
 *	-4	Fail: Bad stop bits config
 *
 * EXAMPLES:
 * 	open_uart(1,38400,"8N1","w",0,0);	UART1, TX, No RTS/CTS
 * 	open_uart(2,19200,"7E1","rw",0,0);	UART2, RX+TX, No RTS/CTS
 * 	open_uart(3,115200,"8N1","rw",1,1);	UART3, RX+TX, RTS/CTS
 *********************************************************************/

int
open_uart(uint32_t uartno,uint32_t baud,const char *cfg,const char *mode,int rts,int cts) {
	uint32_t uart, ux, stopb, iomode, parity, fc;
	struct s_uart_info *infop;
	bool rxintf = false;

	if ( uartno < 1 || uartno > 3 )
		return -1;			/* Invalid UART ref */

	infop = &uarts[ux = uartno-1];		/* USART parameters */
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
		if ( uart_data[ux] == NULL ){
			uart_data[ux] = malloc(sizeof(struct s_uart));
		}
		//uart_data[ux]->tail = (uint16_t)0;
		uart_data[ux]->head = (uint16_t)0;	// this line cause dead
	}
	
	return 0;
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

/*********************************************************************
 * Put one character to device, non-blocking
 *
 * RETURNS:
 *	0	Sent char
 *	-1	Device busy
 *********************************************************************/
int
putc_uart_nb(uint32_t uartno,char ch) {
	uint32_t uart = uarts[uartno-1].usart;

	if ( (USART_SR(uart) & USART_SR_TXE) == 0 )
		return -1;	/* Busy */
	usart_send_blocking(uart,ch);
	return 0;		/* Success */
}

/*********************************************************************
 * Put one character to device, block (yield) until TX ready (blocking)
 *********************************************************************/
void
putc_uart(uint32_t uartno,char ch) {
	uint32_t uart = uarts[uartno-1].usart;

	while ( (USART_SR(uart) & USART_SR_TXE) == 0 )
		taskYIELD();	
	usart_send_blocking(uart,ch);
}

/*********************************************************************
 * Write size bytes to TX, yielding until all is sent (blocking)
 *********************************************************************/

void
write_uart(uint32_t uartno,const char *buf,uint32_t size) {
	uint32_t uart = uarts[uartno-1].usart;

	for ( ; size > 0; --size ) {
		while ( (USART_SR(uart) & USART_SR_TXE) == 0 )
			taskYIELD();	
		usart_send_blocking(uart,*buf++);
	}
}

/*********************************************************************
 * Send Null terminated string, yeilding (blocking)
 *********************************************************************/

void
puts_uart(uint32_t uartno,const char *buf) {
	uint32_t uart = uarts[uartno-1].usart;

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
	struct s_uart *uptr = uart_data[uartno-1];

	if ( !uptr )
		return -1;	// No known uart
	return get_char(uptr);
}

/*********************************************************************
 * Receive a byte, blocking
 *********************************************************************/

char
getc_uart(uint32_t uartno) {
	struct s_uart *uptr = uart_data[uartno-1];
	int rch;

	if ( !uptr )
		return -1;	// No known uart
	while ( (rch = get_char(uptr)) == -1 )
		taskYIELD();
	return (char)rch;
}

/*********************************************************************
 * Get cooked input line
 *********************************************************************/

int
getline_uart(uint32_t uartno,char *buf,uint32_t bufsiz) {
	struct s_uart_info *uart = &uarts[uartno-1];

	return getline(buf,bufsiz,uart->getc,uart->putc);
}

/*********************************************************************
 * Close USART (frees RAM)
 *********************************************************************/

void
close_uart(uint32_t uartno) {
	uint32_t ux = uartno - 1;
	struct s_uart *uptr = uart_data[ux];

	usart_disable_rx_interrupt(uarts[ux].usart);

	if ( uptr && uart_data[ux] ) {
		free(uart_data[ux]);
		uart_data[ux] = 0;
	}
}

/*********************************************************************
 * Optional use routines for UART1
 *********************************************************************/

void
uart1_putc(char ch) {
	if ( ch == '\n' )
		putc_uart(1,'\r');
	putc_uart(1,ch);
}

void
uart1_puts(const char *buf) {
	puts_uart(1,buf);
}

int
uart1_getc(void) {
	return getc_uart(1);
}

int
uart1_peek(void) {
	return getc_uart_nb(1);
}

int
uart1_gets(char *buf,unsigned bufsiz) {
	return getline_uart(1,buf,bufsiz);
}

void
uart1_write(const char *buf,unsigned bytes) {
	write_uart(1,buf,bytes);
}

int
uart1_getline(char *buf,unsigned bufsiz) {
	return getline_uart(1,buf,bufsiz);
}

/*********************************************************************
 * Optional use routines for UART2
 *********************************************************************/

void
uart2_putc(char ch) {
	if ( ch == '\n' )
		putc_uart(2,'\r');
	putc_uart(2,ch);
}

void
uart2_puts(const char *buf) {
	puts_uart(2,buf);
}

int
uart2_getc(void) {
	return getc_uart(2);
}

int
uart2_peek(void) {
	return getc_uart_nb(2);
}

int
uart2_gets(char *buf,unsigned bufsiz) {
	return getline_uart(2,buf,bufsiz);
}

void
uart2_write(const char *buf,unsigned bytes) {
	write_uart(2,buf,bytes);
}

int
uart2_getline(char *buf,unsigned bufsiz) {
	return getline_uart(2,buf,bufsiz);
}

/*********************************************************************
 * Optional use routines for UART3
 *********************************************************************/

void
uart3_putc(char ch) {
	if ( ch == '\n' )
		putc_uart(3,'\r');
	putc_uart(3,ch);
}

void
uart3_puts(const char *buf) {
	puts_uart(3,buf);
}

int
uart3_getc(void) {
	return getc_uart(3);
}

int
uart3_peek(void) {
	return getc_uart_nb(3);
}

int
uart3_gets(char *buf,unsigned bufsiz) {
	return getline_uart(3,buf,bufsiz);
}

int
uart3_getline(char *buf,unsigned bufsiz) {
	return getline_uart(3,buf,bufsiz);
}

void
uart3_write(const char *buf,unsigned bytes) {
	write_uart(3,buf,bytes);
}

static QueueHandle_t uart_txq;				// TX queue for UART

void
init_usart(void) {
	rcc_clock_setup_pll(&rcc_hse_configs[RCC_CLOCK_HSE8_72MHZ]);
	rcc_periph_clock_enable(RCC_GPIOA);
	gpio_set_mode(GPIOA,GPIO_MODE_OUTPUT_50_MHZ,GPIO_CNF_OUTPUT_ALTFN_PUSHPULL,GPIO_USART1_TX);
	gpio_set_mode(GPIOA,GPIO_MODE_INPUT,GPIO_CNF_INPUT_FLOAT,GPIO_USART1_RX);

	open_uart(1,38400,"8N1","rw",1,1);
	uart_txq = xQueueCreate(256,sizeof(char));
}


void
uart_task(void *args) {
	int gc;
	char kbuf[256], ch;

	// puts_uart(1,"\n\ruart_task() has begun:\n\r");

	// for (;;) {
	// 	if ( (gc = getc_uart_nb(1)) != -1 ) {
	// 		puts_uart(1,"\r\n\nENTER INPUT: ");

	// 		ch = (char)gc;
	// 		if ( ch != '\r' && ch != '\n' ) {
	// 			/* Already received first character */
	// 			kbuf[0] = ch;
	// 			putc_uart(1,ch);
	// 			getline_uart(1,kbuf+1,sizeof kbuf-1);
	// 		} else	{
	// 			/* Read the entire line */
	// 			getline_uart(1,kbuf,sizeof kbuf);
	// 		}

	// 		//puts_uart(1,"\r\nReceived input '");
	// 		//puts_uart(1,kbuf);
	// 		if (strncmp(kbuf, "12345", 5) == 0) {
	// 			OLED_ShowString(2, 6, "Bingo");
	// 		} else {
	// 			OLED_ShowString(2, 6, kbuf);
	// 		}
	// 		//puts_uart(1,"'\n\r\nResuming prints...\n\r");
	// 	}

	// 	/* Receive char to be TX */
	// 	if ( xQueueReceive(uart_txq,&ch,10) == pdPASS )
	// 		putc_uart(1,ch);
	// 	/* Toggle LED to show signs of life */
	// 	gpio_toggle(GPIOC,GPIO13);
	// }
}

/* End uartlib.c */
/*
	GPIOA10 GPIO_USART1_TX
	GPIOA9 GPIO_USART1_RX
	// 38400
*/

// void itoa(int num, char* str, int Length){
// 	for (int i = 0; i < Length; i++)							
// 	{
// 		str[i] = (num / OLED_Pow(10, Length - i - 1) % 10 + '0');
// 	}
// }

// #define BUFFER_LEN 5
// char buff[BUFFER_LEN];
// uint8_t buffer_index = 0;
// void handle_buff(uint8_t data, uint8_t len){
// 	buff[buffer_index] = data;
// 	buffer_index+=1;
// 	if (data == '\n' || buffer_index >= len) {
// 		for (uint8_t i = buffer_index; i < len; i++){
// 			buff[i] = ' ';
// 		}
// 		buffer_index = 0;
// 		if (strncmp(buff, "12345", 5) == 0) {
// 			OLED_ShowString(2, 6, "Bingo");
// 		} else {
// 			OLED_ShowString(2, 6, buff);
// 		}
// 		// read adc 2
// 		int adc2 = read_adc(2);
// 		const int len = 5;
// 		char snum[len];
// 		itoa(adc2, snum, len);
// 		uart_put_str(snum, len);
// 		OLED_ShowNum(3, 6, adc2, len);
// 	}
// }