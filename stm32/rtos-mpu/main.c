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

static void
task1(void *args) {
	int i;

	(void)args;

	for (;;) {
		// gpio_clear(GPIOC,GPIO14);
		// for (i = 0; i < 5e5; i++)
		// 	__asm__("nop");		
		// //vTaskDelay(pdMS_TO_TICKS(500));
		// gpio_set(GPIOC,GPIO14);
		// for (i = 0; i < 5e5; i++)
		// 	__asm__("nop");			
		gpio_toggle(GPIOC,GPIO14);
		vTaskDelay(pdMS_TO_TICKS(500));
		
		OLED_ShowChar(1, 1, 'A');
		OLED_ShowString(1, 3, "Fuck you!");
		OLED_ShowNum(2, 1, 12345, 5);
		OLED_ShowSignedNum(2, 7, -66, 4);
		OLED_ShowHexNum(3, 1, 0xAA55, 4);
		OLED_ShowBinNum(4, 1, 0xAA55, 16);
	
	}
}

int
main(void) {

	//rcc_clock_setup_in_hse_8mhz_out_72mhz();	// Use this for "blue pill"
	rcc_clock_setup_pll(&rcc_hse_configs[RCC_CLOCK_HSE8_72MHZ]);

	rcc_periph_clock_enable(RCC_GPIOC);
	gpio_set_mode(GPIOC,GPIO_MODE_OUTPUT_2_MHZ,GPIO_CNF_OUTPUT_PUSHPULL,GPIO14);
	gpio_set(GPIOC,GPIO14);

	OLED_Init();

	xTaskCreate(task1,"LED",100,NULL,configMAX_PRIORITIES-1,NULL);
	vTaskStartScheduler();
	for (;;);

	return 0;
}

// End