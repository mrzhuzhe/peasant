/* Demo program for OLED 128x64 SSD1306 controller
 * Warren Gay   Sun Dec 17 22:49:14 2017
 *
 * Important!  	You must have a pullup resistor on the NSS
 * 	       	line in order that the NSS (/CS) SPI output
 *		functions correctly as a chip select. The
 *		SPI peripheral configures NSS pin as an
 *		open drain output.
 *
 * OLED		4-Wire SPI
 *
 * PINS:
 *	PC13	LED
 *	PA15	/CS (NSS, with 10k pullup)
 *	PB3	SCK
 *	PB5	MOSI (MISO not used)
 *	PB10	D/C
 *	PB11	/Reset
 */
#include <string.h>
#include <ctype.h>

#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"

#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/gpio.h>
#include <libopencm3/stm32/adc.h>
#include "opencm3.h"
#include "oled.h"
// #define OLED_W_SCL(x)		GPIO_WriteBit(GPIOB, GPIO_Pin_8, (BitAction)(x))
// #define OLED_W_SDA(x)		GPIO_WriteBit(GPIOB, GPIO_Pin_9, (BitAction)(x))

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

int
main(void) {

	rcc_clock_setup_pll(&rcc_hse_configs[RCC_CLOCK_HSE8_72MHZ]);


	OLED_Init();

	rcc_periph_clock_enable(RCC_GPIOA);		// Enable GPIOA for ADC
	gpio_set_mode(GPIOA,
		GPIO_MODE_INPUT,
		GPIO_CNF_INPUT_ANALOG,
		GPIO3|GPIO2);				// PA0 & PA1

	// Initialize ADC:
	rcc_peripheral_enable_clock(&RCC_APB2ENR,RCC_APB2ENR_ADC1EN);
	adc_power_off(ADC1);
	rcc_peripheral_reset(&RCC_APB2RSTR,RCC_APB2RSTR_ADC1RST);
	rcc_peripheral_clear_reset(&RCC_APB2RSTR,RCC_APB2RSTR_ADC1RST);

	adc_set_dual_mode(ADC_CR1_DUALMOD_IND);		// Independent mode
	adc_enable_scan_mode(ADC1);
	adc_set_right_aligned(ADC1);
	adc_set_single_conversion_mode(ADC1);

	adc_power_on(ADC1);
	adc_reset_calibration(ADC1);
	adc_calibrate_async(ADC1);
	while ( adc_is_calibrating(ADC1) );

	OLED_ShowChar(1, 1, 'A');
	OLED_ShowString(1, 3, "Fuck you!");
	OLED_ShowString(2, 1, "adc2");
	OLED_ShowString(3, 1, "adc3");
	
	int adc2=0, adc3=0;
	int i;
	for (;;) {
		for (i = 0; i < 5e5; i++)	/* Wait a bit. */
            __asm__("nop");
		adc2 = read_adc(2);
		//adc3 = read_adc(3);
		OLED_ShowNum(2, 6, adc2, 5);
		OLED_ShowNum(3, 6, adc3, 5);
	}

	// rtos cannot go with OLED
	// xTaskCreate(demo_task,"monitor",500,NULL,1,NULL);
	// vTaskStartScheduler();
	for (;;);
	return 0;
}

// End