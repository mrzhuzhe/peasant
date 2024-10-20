#include "FreeRTOS.h"
#include "portmacro.h"
#include "task.h"
#include "queue.h"
#include "oled.h"
#include "canmsgs.h"

#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/gpio.h>
#include "opencm3.h"

/*********************************************************************
 * CAN Receive Callback
 *********************************************************************/
void
can_recv(struct s_canmsg *msg) {

	union u_msg {
		struct s_lamp_en temp;
	} *msgp = (union u_msg *)msg->data;

	OLED_ShowNum(1, 1, (unsigned)msg->msgid, 4);
	OLED_ShowNum(1, 6, (unsigned)msg->fifo, 4);
	OLED_ShowNum(2, 1, (unsigned)msg->fmi, 4);
	OLED_ShowString(2, 6, (msg->rtrf ? "R" : "D"));
	// OLED_ShowBinNum(3, 1, msg->data[0], 4);
	// OLED_ShowBinNum(3, 6, msg->data[1], 4);	// seems little endian

	OLED_ShowBinNum(3, 1, msgp->temp.enable, 4);
	OLED_ShowNum(3, 6, (unsigned)msgp->temp.reserved, 4);

}

struct s_lamp_status lamp_status = { 0, 0, 0, 0, 0, 0 };

void
monitor_task(void *arg) {
	uint8_t count = 0;
	for (;;) {
		// OLED_ShowChar(1, 1, 'A');
		// OLED_ShowString(1, 3, "Fuck you!");
				
		// gpio_toggle(GPIOC,GPIO14);	
		struct s_lamp_en msg;
		msg.enable = 1;
		msg.reserved = count;
		//can_xmit(ID_LeftEn,false,false,sizeof msg,&msg);
		can_xmit(ID_ParkEn,false,false,sizeof msg,&msg);
		//OLED_ShowNum(2, 1, (unsigned)count, 5);
		vTaskDelay(pdMS_TO_TICKS(500));
		count++;
	}
}

int
main(void) {

	rcc_clock_setup_pll(&rcc_hse_configs[RCC_CLOCK_HSE8_72MHZ]);


	OLED_Init();

	// rcc_periph_clock_enable(RCC_GPIOC);
	// gpio_set_mode(GPIOC,GPIO_MODE_OUTPUT_2_MHZ,GPIO_CNF_OUTPUT_PUSHPULL,GPIO14);
	// gpio_set(GPIOC,GPIO14);
	
	initialize_can(false, true, false);		// !nart, locked, altcfg=fasle PA11/PA12

	xTaskCreate(monitor_task,"monitor",100,NULL,configMAX_PRIORITIES-1,NULL);
	vTaskStartScheduler();
	
	for (;;);

	return 0;
}

// End