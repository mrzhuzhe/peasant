#include <string.h>
#include <ctype.h>

#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "oled.h"

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

static uint16_t
read_adc(uint8_t channel) {

	adc_set_sample_time(ADC1,channel,ADC_SMPR_SMP_55DOT5CYC);
	adc_set_regular_sequence(ADC1,1,&channel);
	adc_start_conversion_direct(ADC1);
	while ( !adc_eoc(ADC1) )
		taskYIELD();
	return adc_read_regular(ADC1);
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

	dma_enable_transfer_complete_interrupt(DMA1, DMA_CHANNEL1);

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

static void
adc_task() {
	
    int adc2=0 , adc3=0;
	int i;
	for (;;) {
		for (i = 0; i < 5e5; i++)	/* Wait a bit. */
            __asm__("nop");
		// adc2 = read_adc(2);
		// adc3 = read_adc(3);
		// OLED_ShowNum(2, 6, adc2, 5);
		// OLED_ShowNum(3, 6, adc3, 5);
		OLED_ShowString(1, 3, "fuck you!");
		adc_start_conversion_regular(ADC1);
		OLED_ShowNum(2, 6, channels[CH2], 5);
		OLED_ShowNum(3, 6, channels[CH3], 5);
		//OLED_ShowNum(2, 6, 123, 5);
	}
}

int
main(void) {
	
	nvic_enable_irq(NVIC_DMA1_CHANNEL1_IRQ);

	OLED_Init();
	clock_setup();
	adc_setup();

	OLED_ShowChar(1, 1, 'A');
	OLED_ShowString(1, 3, "result!");
	OLED_ShowString(2, 1, "adc2");
	OLED_ShowString(3, 1, "adc3");
		
	int adc2=0 , adc3=0;
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

	//	TODO rtos cannot go with DMA OLED 
	// xTaskCreate(adc_task,"adc",300,NULL,1,NULL);
	// vTaskStartScheduler();
	for (;;);
	return 0;
}

// End