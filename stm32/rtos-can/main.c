#include "FreeRTOS.h"
#include "portmacro.h"
#include "task.h"
#include "queue.h"
#include "oled.h"
#include "canmsgs.h"

#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/gpio.h>
#include "opencm3.h"


void
monitor_task(void *arg) {

	for (;;) {
		OLED_ShowChar(1, 1, 'A');
		OLED_ShowString(1, 3, "Fuck you!");
		OLED_ShowNum(2, 1, 12345, 5);
		OLED_ShowSignedNum(2, 7, -66, 4);
		OLED_ShowHexNum(3, 1, 0xAA55, 4);
		OLED_ShowBinNum(4, 1, 0xAA55, 16);
		
		// gpio_toggle(GPIOC,GPIO14);
		// vTaskDelay(pdMS_TO_TICKS(500));
	}
}

int
main(void) {

	rcc_clock_setup_pll(&rcc_hse_configs[RCC_CLOCK_HSE8_72MHZ]);


	OLED_Init();

	// rcc_periph_clock_enable(RCC_GPIOC);
	// gpio_set_mode(GPIOC,GPIO_MODE_OUTPUT_2_MHZ,GPIO_CNF_OUTPUT_PUSHPULL,GPIO14);
	// gpio_set(GPIOC,GPIO14);
	
	
	xTaskCreate(monitor_task,"monitor",100,NULL,configMAX_PRIORITIES-1,NULL);
	vTaskStartScheduler();
	
	for (;;);

	return 0;
}

// End