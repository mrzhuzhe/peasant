/* Simple LED task demo:
 *
 * The LED on PC13 is toggled in task1.
 */
#include "FreeRTOS.h"
#include "task.h"

#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/gpio.h>
#include "opencm3.h"

extern void vApplicationStackOverflowHook(TaskHandle_t pxTask, portCHAR *pcTaskName);

void
vApplicationStackOverflowHook(TaskHandle_t pxTask, portCHAR *pcTaskName) {
	(void)pxTask;
	(void)pcTaskName;
	for(;;);
}

static void
task1(void *args) {
	int i;

	(void)args;

	for (;;) {
		gpio_clear(GPIOC,GPIO14);
		for (i = 0; i < 5e5; i++)
			__asm__("nop");
		gpio_set(GPIOC,GPIO14);
	}
}

int
main(void) {

	rcc_clock_setup_in_hse_8mhz_out_72mhz();	// Use this for "blue pill"
	rcc_periph_clock_enable(RCC_GPIOC);
	gpio_set_mode(GPIOC,GPIO_MODE_OUTPUT_2_MHZ,GPIO_CNF_OUTPUT_PUSHPULL,GPIO14);

	xTaskCreate(task1,"LED",100,NULL,configMAX_PRIORITIES-1,NULL);
	vTaskStartScheduler();
	for (;;);

	return 0;
}

// End