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
		struct s_temp100 temp;
	} *msgp = (union u_msg *)msg->data;

	OLED_ShowNum(1, 1, (unsigned)msg->msgid, 5);
	OLED_ShowSignedNum(2, 1, msg->fifo, 5);
	OLED_ShowNum(3, 1, (unsigned)msg->fmi, 5);
	OLED_ShowString(4, 1, (msg->rtrf ? "R" : "D"));
	OLED_ShowBinNum(5, 1, msg->data[0], 16);
	// if ( show_rx ) {
	// 	std_printf("[%4u(%d/%u):%c,$%02X]\n",
	// 		(unsigned)msg->msgid,
	// 		msg->fifo,(unsigned)msg->fmi,
	// 		msg->rtrf ? 'R' : 'D',
	// 		msg->data[0]);
	// }

	// if ( !msg->rtrf ) {
	// 	switch ( msg->msgid ) {
	// 	case ID_Temp:
	// 		display_temp(msgp->temp.celciusx100);
	// 		break;
	// 	default:
	// 		break;
	// 	}
	// }
}

struct s_lamp_status lamp_status = { 0, 0, 0, 0, 0, 0 };

void
monitor_task(void *arg) {

	for (;;) {
		OLED_ShowChar(1, 1, 'A');
		OLED_ShowString(1, 3, "Fuck you!");
				
		// gpio_toggle(GPIOC,GPIO14);	
		struct s_lamp_en msg;
		msg.enable = 1;
		msg.reserved = 0;
		can_xmit(ID_LeftEn,false,false,sizeof msg,&msg);

		vTaskDelay(pdMS_TO_TICKS(500));
	}
}

int
main(void) {

	rcc_clock_setup_pll(&rcc_hse_configs[RCC_CLOCK_HSE8_72MHZ]);


	OLED_Init();

	// rcc_periph_clock_enable(RCC_GPIOC);
	// gpio_set_mode(GPIOC,GPIO_MODE_OUTPUT_2_MHZ,GPIO_CNF_OUTPUT_PUSHPULL,GPIO14);
	// gpio_set(GPIOC,GPIO14);
	
	initialize_can(false,true, false);		// !nart, locked, altcfg=fasle PA11/PA12

	xTaskCreate(monitor_task,"monitor",100,NULL,configMAX_PRIORITIES-1,NULL);
	vTaskStartScheduler();
	
	for (;;);

	return 0;
}

// End