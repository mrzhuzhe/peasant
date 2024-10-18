#include <string.h>
#include <ctype.h>

#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "OLED_Font.h"

#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/gpio.h>
#include <libopencm3/stm32/adc.h>
#include <libopencm3/stm32/dma.h>
#include "opencm3.h"

/*
https://github.com/coderwhq/learning-STM32/blob/main/8-2_DMA%E9%85%8D%E5%90%88AD%E5%A4%9A%E9%80%9A%E9%81%93/Hardware/AD.c
https://github.com/coderwhq/learning-STM32/blob/main/7-2_AD%E5%A4%9A%E9%80%9A%E9%81%93/Hardware/AD.c
https://github.com/ve3wwg/stm32f103c8t6/blob/master/rtos/oled_dma/main.c
https://github.com/libopencm3/libopencm3-examples/blob/master/examples/stm32/f1/other/dma_mem2mem/dma.c
*/

/*引脚初始化*/
void OLED_I2C_Init(void)
{
		rcc_periph_clock_enable(RCC_GPIOB);

	// LED on PC13
	gpio_set_mode(GPIOB,
		GPIO_MODE_OUTPUT_50_MHZ,
		GPIO_CNF_OUTPUT_OPENDRAIN,
		GPIO8|GPIO9);
		
	gpio_set(GPIOB, GPIO8); //	OLED_W_SCL (1);
	gpio_set(GPIOB, GPIO9);	//	OLED_W_SDA (1);
}

void OLED_I2C_Start(void)
{
	gpio_set(GPIOB, GPIO9);
	gpio_set(GPIOB, GPIO8);
	gpio_clear(GPIOB, GPIO9);
	gpio_clear(GPIOB, GPIO8);
}

/**
  * @brief  I2C停止
  * @param  无
  * @retval 无
  */
void OLED_I2C_Stop(void)
{
	gpio_clear(GPIOB, GPIO9);
	gpio_set(GPIOB, GPIO8);
	gpio_set(GPIOB, GPIO9);
}

/**
  * @brief  I2C发送一个字节
  * @param  Byte 要发送的一个字节
  * @retval 无
  */
void OLED_I2C_SendByte(uint8_t Byte)
{
	uint8_t i;
	for (i = 0; i < 8; i++)
	{
		//OLED_W_SDA(Byte & (0x80 >> i));
		if (Byte & (0x80 >> i)) {
			gpio_set(GPIOB, GPIO9);	
		} else {
			gpio_clear(GPIOB, GPIO9);	
		}
		gpio_set(GPIOB, GPIO8);
		gpio_clear(GPIOB, GPIO8);
	}
	gpio_set(GPIOB, GPIO8);	//额外的一个时钟，不处理应答信号
	gpio_clear(GPIOB, GPIO8);
}

/**
  * @brief  OLED写命令
  * @param  Command 要写入的命令
  * @retval 无
  */
void OLED_WriteCommand(uint8_t Command)
{
	OLED_I2C_Start();
	OLED_I2C_SendByte(0x78);		//从机地址
	OLED_I2C_SendByte(0x00);		//写命令
	OLED_I2C_SendByte(Command); 
	OLED_I2C_Stop();
}

/**
  * @brief  OLED写数据
  * @param  Data 要写入的数据
  * @retval 无
  */
void OLED_WriteData(uint8_t Data)
{
	OLED_I2C_Start();
	OLED_I2C_SendByte(0x78);		//从机地址
	OLED_I2C_SendByte(0x40);		//写数据
	OLED_I2C_SendByte(Data);
	OLED_I2C_Stop();
}

/**
  * @brief  OLED设置光标位置
  * @param  Y 以左上角为原点，向下方向的坐标，范围：0~7
  * @param  X 以左上角为原点，向右方向的坐标，范围：0~127
  * @retval 无
  */
void OLED_SetCursor(uint8_t Y, uint8_t X)
{
	OLED_WriteCommand(0xB0 | Y);					//设置Y位置
	OLED_WriteCommand(0x10 | ((X & 0xF0) >> 4));	//设置X位置高4位
	OLED_WriteCommand(0x00 | (X & 0x0F));			//设置X位置低4位
}

/**
  * @brief  OLED清屏
  * @param  无
  * @retval 无
  */
void OLED_Clear(void)
{  
	uint8_t i, j;
	for (j = 0; j < 8; j++)
	{
		OLED_SetCursor(j, 0);
		for(i = 0; i < 128; i++)
		{
			OLED_WriteData(0x00);
		}
	}
}

/**
  * @brief  OLED显示一个字符
  * @param  Line 行位置，范围：1~4
  * @param  Column 列位置，范围：1~16
  * @param  Char 要显示的一个字符，范围：ASCII可见字符
  * @retval 无
  */
void OLED_ShowChar(uint8_t Line, uint8_t Column, char Char)
{      	
	uint8_t i;
	OLED_SetCursor((Line - 1) * 2, (Column - 1) * 8);		//设置光标位置在上半部分
	for (i = 0; i < 8; i++)
	{
		OLED_WriteData(OLED_F8x16[Char - ' '][i]);			//显示上半部分内容
	}
	OLED_SetCursor((Line - 1) * 2 + 1, (Column - 1) * 8);	//设置光标位置在下半部分
	for (i = 0; i < 8; i++)
	{
		OLED_WriteData(OLED_F8x16[Char - ' '][i + 8]);		//显示下半部分内容
	}
}

/**
  * @brief  OLED显示字符串
  * @param  Line 起始行位置，范围：1~4
  * @param  Column 起始列位置，范围：1~16
  * @param  String 要显示的字符串，范围：ASCII可见字符
  * @retval 无
  */
void OLED_ShowString(uint8_t Line, uint8_t Column, char *String)
{
	uint8_t i;
	for (i = 0; String[i] != '\0'; i++)
	{
		OLED_ShowChar(Line, Column + i, String[i]);
	}
}

/**
  * @brief  OLED次方函数
  * @retval 返回值等于X的Y次方
  */
uint32_t OLED_Pow(uint32_t X, uint32_t Y)
{
	uint32_t Result = 1;
	while (Y--)
	{
		Result *= X;
	}
	return Result;
}

/**
  * @brief  OLED显示数字（十进制，正数）
  * @param  Line 起始行位置，范围：1~4
  * @param  Column 起始列位置，范围：1~16
  * @param  Number 要显示的数字，范围：0~4294967295
  * @param  Length 要显示数字的长度，范围：1~10
  * @retval 无
  */
void OLED_ShowNum(uint8_t Line, uint8_t Column, uint32_t Number, uint8_t Length)
{
	uint8_t i;
	for (i = 0; i < Length; i++)							
	{
		OLED_ShowChar(Line, Column + i, Number / OLED_Pow(10, Length - i - 1) % 10 + '0');
	}
}

/**
  * @brief  OLED显示数字（十进制，带符号数）
  * @param  Line 起始行位置，范围：1~4
  * @param  Column 起始列位置，范围：1~16
  * @param  Number 要显示的数字，范围：-2147483648~2147483647
  * @param  Length 要显示数字的长度，范围：1~10
  * @retval 无
  */
void OLED_ShowSignedNum(uint8_t Line, uint8_t Column, int32_t Number, uint8_t Length)
{
	uint8_t i;
	uint32_t Number1;
	if (Number >= 0)
	{
		OLED_ShowChar(Line, Column, '+');
		Number1 = Number;
	}
	else
	{
		OLED_ShowChar(Line, Column, '-');
		Number1 = -Number;
	}
	for (i = 0; i < Length; i++)							
	{
		OLED_ShowChar(Line, Column + i + 1, Number1 / OLED_Pow(10, Length - i - 1) % 10 + '0');
	}
}

/**
  * @brief  OLED显示数字（十六进制，正数）
  * @param  Line 起始行位置，范围：1~4
  * @param  Column 起始列位置，范围：1~16
  * @param  Number 要显示的数字，范围：0~0xFFFFFFFF
  * @param  Length 要显示数字的长度，范围：1~8
  * @retval 无
  */
void OLED_ShowHexNum(uint8_t Line, uint8_t Column, uint32_t Number, uint8_t Length)
{
	uint8_t i, SingleNumber;
	for (i = 0; i < Length; i++)							
	{
		SingleNumber = Number / OLED_Pow(16, Length - i - 1) % 16;
		if (SingleNumber < 10)
		{
			OLED_ShowChar(Line, Column + i, SingleNumber + '0');
		}
		else
		{
			OLED_ShowChar(Line, Column + i, SingleNumber - 10 + 'A');
		}
	}
}

/**
  * @brief  OLED显示数字（二进制，正数）
  * @param  Line 起始行位置，范围：1~4
  * @param  Column 起始列位置，范围：1~16
  * @param  Number 要显示的数字，范围：0~1111 1111 1111 1111
  * @param  Length 要显示数字的长度，范围：1~16
  * @retval 无
  */
void OLED_ShowBinNum(uint8_t Line, uint8_t Column, uint32_t Number, uint8_t Length)
{
	uint8_t i;
	for (i = 0; i < Length; i++)							
	{
		OLED_ShowChar(Line, Column + i, Number / OLED_Pow(2, Length - i - 1) % 2 + '0');
	}
}

/**
  * @brief  OLED初始化
  * @param  无
  * @retval 无
  */
void OLED_Init(void)
{
	uint32_t i, j;
	
	for (i = 0; i < 1000; i++)			//上电延时
	{
		for (j = 0; j < 1000; j++);
	}
	
	OLED_I2C_Init();			//端口初始化
	
	OLED_WriteCommand(0xAE);	//关闭显示
	
	OLED_WriteCommand(0xD5);	//设置显示时钟分频比/振荡器频率
	OLED_WriteCommand(0x80);
	
	OLED_WriteCommand(0xA8);	//设置多路复用率
	OLED_WriteCommand(0x3F);
	
	OLED_WriteCommand(0xD3);	//设置显示偏移
	OLED_WriteCommand(0x00);
	
	OLED_WriteCommand(0x40);	//设置显示开始行
	
	OLED_WriteCommand(0xA1);	//设置左右方向，0xA1正常 0xA0左右反置
	
	OLED_WriteCommand(0xC8);	//设置上下方向，0xC8正常 0xC0上下反置

	OLED_WriteCommand(0xDA);	//设置COM引脚硬件配置
	OLED_WriteCommand(0x12);
	
	OLED_WriteCommand(0x81);	//设置对比度控制
	OLED_WriteCommand(0xCF);

	OLED_WriteCommand(0xD9);	//设置预充电周期
	OLED_WriteCommand(0xF1);

	OLED_WriteCommand(0xDB);	//设置VCOMH取消选择级别
	OLED_WriteCommand(0x30);

	OLED_WriteCommand(0xA4);	//设置整个显示打开/关闭

	OLED_WriteCommand(0xA6);	//设置正常/倒转显示

	OLED_WriteCommand(0x8D);	//设置充电泵
	OLED_WriteCommand(0x14);

	OLED_WriteCommand(0xAF);	//开启显示
		
	OLED_Clear();				//OLED清屏
}

static uint16_t
read_adc(uint8_t channel) {

	adc_set_sample_time(ADC1,channel,ADC_SMPR_SMP_55DOT5CYC);
	adc_set_regular_sequence(ADC1,1,&channel);
	adc_start_conversion_direct(ADC1);
	while ( !adc_eoc(ADC1) )
		taskYIELD();
	return adc_read_regular(ADC1);
}

/*********************************************************************
 * Demo Task:
 *********************************************************************/
static void
demo_task(void *arg __attribute((unused))) {
	int adc3;

	for (;;) {
		vTaskDelay(pdMS_TO_TICKS(1500));
		OLED_ShowNum(2, 1, 123, 3);
		//adc3 = read_adc(3);

		//OLED_ShowNum(2, 1, adc3, 3);
		//OLED_ShowNum(2, 1, 123, 3);
	}
}



enum { CH2, CH3  };

// Static allocations
const uint8_t channel_lenght = 2;
uint16_t channels[2];
uint8_t chanlist[2] = { 2, 3 };
// volatile bool flag;

// // ISR
// void dma1_channel1_isr()
// {
// 	flag = true;

// 	if (dma_get_interrupt_flag(DMA1, DMA_CHANNEL1, DMA_TCIF))
// 		dma_clear_interrupt_flags(DMA1, DMA_CHANNEL1, DMA_TCIF);
// }

void clock_setup()
{
	// high-speed external oscillator (HSE) at 8MHz.
	rcc_clock_setup_pll(&rcc_hse_configs[RCC_CLOCK_HSE8_72MHZ]);
	rcc_periph_clock_enable(RCC_GPIOA); // PA6
}

void dma_setup()
{
	// enable IRQ
	// Without this the timer interrupt routine will never be called.
	nvic_enable_irq(NVIC_DMA1_CHANNEL1_IRQ);

	// configure the DMA
	rcc_periph_clock_enable(RCC_DMA1);

	// The channel is disabled and configuration registers are cleared.
	dma_channel_reset(DMA1, DMA_CHANNEL1);

	// Priority.
	//dma_set_priority(DMA1, DMA_CHANNEL1, DMA_CCR_PL_VERY_HIGH);
	dma_set_priority(DMA1, DMA_CHANNEL1, DMA_CCR_PL_LOW);

	/* 16Bit wide transfer for source and destination. */
	// dma_set_memory_size(DMA1, DMA_CHANNEL1, DMA_CCR_MSIZE_32BIT);
	// dma_set_peripheral_size(DMA1, DMA_CHANNEL1, DMA_CCR_PSIZE_32BIT);
	dma_set_memory_size(DMA1, DMA_CHANNEL1, DMA_CCR_MSIZE_16BIT);
	dma_set_peripheral_size(DMA1, DMA_CHANNEL1, DMA_CCR_PSIZE_16BIT);

	/*
	 * After every 32bits we have to increase the address because
	 * we use RAM.
	 */
	// dma_enable_peripheral_increment_mode(DMA1, DMA_CHANNEL1);
	//dma_disable_peripheral_increment_mode(DMA1, DMA_CHANNEL1); // should be default
	dma_enable_memory_increment_mode(DMA1, DMA_CHANNEL1);
	dma_enable_circular_mode(DMA1, DMA_CHANNEL1);

	/* We define the source as peripheral. */
	dma_set_read_from_peripheral(DMA1, DMA_CHANNEL1);

	/* We want to transfer ADC Data Register. */
	dma_set_peripheral_address(DMA1, DMA_CHANNEL1, (uint32_t)&ADC1_DR);

	/* Destination should be the channels array. */
	dma_set_memory_address(DMA1, DMA_CHANNEL1, (uint32_t)channels);

	/*
	 * Set number of DATA to transfer.
	 * Remember that this means not necessary bytes but MSIZE or PSIZE
	 * depending on your source device.
	 */
	dma_set_number_of_data(DMA1, DMA_CHANNEL1, channel_lenght);

	//dma_enable_transfer_complete_interrupt(DMA1, DMA_CHANNEL1);

	/* Start DMA transfer. */
	dma_enable_channel(DMA1, DMA_CHANNEL1);
}

/**
 * Initialize the ADC1.
 *
 * @warning: rcc_periph_clock_enable(RCC_GPIOA);
 */
void adc_setup()
{
	gpio_set_mode(GPIOA,
			GPIO_MODE_INPUT,
			GPIO_CNF_INPUT_ANALOG,
			GPIO2 | GPIO3);

	rcc_periph_clock_enable(RCC_ADC1);
	adc_power_off(ADC1); // Make sure the ADC doesn't run during config.

	dma_setup();

	/* rcc_peripheral_reset + rcc_peripheral_clear_reset */
	rcc_periph_reset_pulse(RST_ADC1);

	/* Prescaler */
	rcc_set_adcpre(RCC_CFGR_ADCPRE_PCLK2_DIV6);

	/* Independent mode */
	// adc_set_dual_mode(ADC_CR1_DUALMOD_IND);

	/* Scan mode */
	// adc_disable_scan_mode(ADC1); // convert only 1 channel
	adc_enable_scan_mode(ADC1); // scan channels mode (DMA required)

	/* configure for one single conversion. */
	adc_set_single_conversion_mode(ADC1);

	// F1 software trigger mandatory
	adc_enable_external_trigger_regular(ADC1, ADC_CR2_EXTSEL_SWSTART);	// Todo this is the key
	// adc_disable_external_trigger_regular(ADC1);

	/* alignment */
	adc_set_right_aligned(ADC1);

	// adc_set_sample_time(ADC1, ADC_CHANNEL6, ADC_SMPR_SMP_239DOT5CYC);
	adc_set_sample_time_on_all_channels(ADC1, ADC_SMPR_SMP_55DOT5CYC);

	adc_set_regular_sequence(ADC1, channel_lenght, (uint8_t*)chanlist);

	adc_enable_dma(ADC1);
	adc_power_on(ADC1);
	adc_reset_calibration(ADC1);
	adc_calibrate(ADC1);
}


int
main(void) {
	
	nvic_enable_irq(NVIC_DMA1_CHANNEL1_IRQ);


	OLED_Init();
	clock_setup();
	adc_setup();
	

	

	OLED_ShowChar(1, 1, 'A');
	OLED_ShowString(1, 3, "Fuck you!");
	OLED_ShowString(2, 1, "adc2");
	OLED_ShowString(3, 1, "adc3");
	
	int adc2, adc3;
	int i;
	for (;;) {
		for (i = 0; i < 5e5; i++)	/* Wait a bit. */
            __asm__("nop");
		// adc2 = read_adc(2);
		// adc3 = read_adc(3);
		// OLED_ShowNum(2, 6, adc2, 5);
		// OLED_ShowNum(3, 6, adc3, 5);
		adc_start_conversion_regular(ADC1);
		OLED_ShowNum(2, 6, channels[CH2], 5);
		OLED_ShowNum(3, 6, channels[CH3], 5);
	}

	// rtos cannot go with OLED
	// xTaskCreate(demo_task,"monitor",500,NULL,1,NULL);
	// vTaskStartScheduler();
	for (;;);
	return 0;
}

// End