/* Simple LED task demo:
 *
 * The LED on PC13 is toggled in task1.
 */
#include "FreeRTOS.h"
#include "portmacro.h"
#include "task.h"
#include "list.h"
#include "opencm3.h"

#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/gpio.h>
#include "oled.h"
#include "uart.h"
#include "adc.h"
// extern void vApplicationStackOverflowHook(TaskHandle_t pxTask, portCHAR *pcTaskName);

void
vApplicationStackOverflowHook(TaskHandle_t xTask, portCHAR *pcTaskName) {
	(void)xTask;
	(void)pcTaskName;
	for(;;){
		// gpio_toggle(GPIOC,GPIO14);
		// vTaskDelay(pdMS_TO_TICKS(500));
	};
}

int
main(void) {

	//rcc_clock_setup_in_hse_8mhz_out_72mhz();	// Use this for "blue pill"
	rcc_clock_setup_pll(&rcc_hse_configs[RCC_CLOCK_HSE8_72MHZ]);

	rcc_periph_clock_enable(RCC_GPIOC);
	gpio_set_mode(GPIOC,GPIO_MODE_OUTPUT_2_MHZ,GPIO_CNF_OUTPUT_PUSHPULL,GPIO14);
	gpio_set(GPIOC,GPIO14);

	OLED_Init();
	
	init_adc();

	init_usart();


	OLED_ShowChar(1, 1, 'A');
	OLED_ShowString(1, 3, "Fuck you!");	
	OLED_ShowString(2, 1, "q2");
	OLED_ShowString(3, 1, "adc2");

	//xTaskCreate(uart_task,"UART",100,NULL,configMAX_PRIORITIES-1,NULL);

	vTaskStartScheduler();
	for (;;);

	return 0;
}

// End