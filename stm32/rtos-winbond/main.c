/* Winbond W25Q32 Code
 * Warren Gay Fri Oct 27 23:52:33 2017
 *
 * Important!  	You must have a pullup resistor on the NSS
 * 	       	line in order that the NSS (/CS) SPI output
 *		functions correctly as a chip select. The
 *		SPI peripheral configures NSS pin as an
 *		open drain output.
 *
 * PINS:
 *	PC13	LED
 *	PA4	/CS (NSS, with 10k pullup)
 *	PA5	SCK
 *	PA6	MISO
 *	PA7	MOSI

 // Todo init fail https://github.com/coderwhq/learning-STM32/blob/main/11-1_%E8%BD%AF%E4%BB%B6SPI%E8%AF%BB%E5%86%99W25Q64/Hardware/W25Q64.c
 https://github.com/libopencm3/libopencm3-examples/blob/master/examples/stm32/f1/lisa-m-2/spi/spi.c
 */
#include <string.h>
#include <ctype.h>

#include "mcuio.h"
#include "miniprintf.h"
#include "intelhex.h"

#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"

#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/gpio.h>
#include <libopencm3/stm32/spi.h>
#include "opencm3.h"

#define W25_CMD_MANUF_DEVICE	0x90
#define W25_CMD_JEDEC_ID	0x9F
#define W25_CMD_WRITE_EN	0x06
#define W25_CMD_WRITE_DI	0x04
#define W25_CMD_READ_SR1	0x05
#define W25_CMD_READ_SR2	0x35
#define W25_CMD_CHIP_ERASE	0xC7
#define W25_CMD_READ_DATA	0x03
#define W25_CMD_FAST_READ	0x0B
#define W25_CMD_WRITE_DATA	0x02
#define W25_CMD_READ_UID	0x4B
#define W25_CMD_PWR_ON		0xAB
#define W25_CMD_PWR_OFF		0xB9
#define W25_CMD_ERA_SECTOR	0x20
#define W25_CMD_ERA_32K		0x52
#define W25_CMD_ERA_64K		0xD8

#define DUMMY			0x00

#define W25_SR1_BUSY		0x01
#define W25_SR1_WEL		0x02

static const char *cap[3] = {
	"W25X16",	// 14
	"W25X32",	// 15
	"W25X64"	// 16
};	


void MySPI_W_SS(uint8_t BitValue)
{
	if (BitValue == 1) {
		gpio_set(GPIOA, GPIO4);
	} else {
		gpio_clear(GPIOA, GPIO4);
	}
}

void MySPI_W_SCK(uint8_t BitValue)
{
	if (BitValue == 1) {
		gpio_set(GPIOA, GPIO5);
	} else {
		gpio_clear(GPIOA, GPIO5);
	}
}

void MySPI_W_MOSI(uint8_t BitValue)
{
	if (BitValue == 1) {
		gpio_set(GPIOA, GPIO7);
	} else {
		gpio_clear(GPIOA, GPIO7);
	}
}

uint8_t MySPI_R_MISO(void)
{
	return gpio_get(GPIOA, GPIO6);
}

void MySPI_Start(void)
{
	MySPI_W_SS(0);
}

void MySPI_Stop(void)
{
	MySPI_W_SS(1);
}
// 模式0 交换字节
uint8_t MySPI_SwapByte(uint8_t ByteSend)
{
	uint8_t i;
	for(i = 0; i < 8; i ++)
	{
		// 要保证MySPI_W_MOSI这个函数是非0即1的特征才能这么写，否则要用if判断或者右移到最低位
		MySPI_W_MOSI(ByteSend & 0x80);
		ByteSend <<= 1;
		/****在此时，从机也移出了一个数据位，放在MISO上****/
		MySPI_W_SCK(1);
		/****在此时，从机会把MOSI上的数据读走****/
		ByteSend |= MySPI_R_MISO();
		MySPI_W_SCK(0);
	}
	return ByteSend;
}

static void
spi_setup(void) {
	
	gpio_set_mode(
		GPIOA,
                GPIO_MODE_OUTPUT_50_MHZ,
                GPIO_CNF_INPUT_PULL_UPDOWN,
                GPIO4|GPIO5|GPIO7		// NSS=PA4,SCK=PA5,MOSI=PA7
	);
	
	gpio_set_mode(
		GPIOA,
		GPIO_MODE_INPUT,
		//GPIO_CNF_INPUT_FLOAT,
		GPIO_CNF_INPUT_PULL_UPDOWN,
		GPIO6				// MISO=PA6
	);

	// 初始化的默认电平
	MySPI_W_SS(1);
	MySPI_W_SCK(0);

}

int
main(void) {

	rcc_clock_setup_pll(&rcc_hse_configs[RCC_CLOCK_HSE8_72MHZ]);	// Blue pill

	rcc_periph_clock_enable(RCC_GPIOA);
	rcc_periph_clock_enable(RCC_GPIOC);

	// LED on PC13
	gpio_set_mode(GPIOC,GPIO_MODE_OUTPUT_2_MHZ,GPIO_CNF_OUTPUT_PUSHPULL,GPIO13);

	spi_setup();
	gpio_clear(GPIOC,GPIO13);				// PC13 = on

	//usb_start(1,1);
	//std_set_device(mcu_usb);			// Use USB for std I/O
	gpio_set(GPIOC,GPIO13);			// PC13 = off

	uint8_t MID;
	uint16_t DID;
	MySPI_Start();
	MySPI_SwapByte(W25_CMD_JEDEC_ID);
	MID = MySPI_SwapByte(DUMMY);
	DID = MySPI_SwapByte(DUMMY);
	DID <<= 8;
	DID |= MySPI_SwapByte(DUMMY);
	MySPI_Stop();

	for (;;);
	return 0;
}

// End