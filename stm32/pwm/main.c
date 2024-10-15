#include <string.h>
#include <ctype.h>

#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"

#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/gpio.h>
#include <libopencm3/stm32/timer.h>
#include "opencm3.h"

/*
	OC输出比较 Output Compare 通过比较CNT与CCR(捕获比较寄存器)值的关系，
	来对输出电平进行置1、置0或翻转的操作，用于输出一定频率和占空比的PWM波形

	PWM 脉冲宽度调制，是一个数字信号
	咋具有 **惯性的系统** 中，可以通过对一系列脉冲的宽度进行调制，来等效的获得所需要的模拟参量，常应用于电机控速等领域
	频率= 1/Ts 占空比=Ton/Ts 分辨率 = 占空比变化步距
	占空比越大，等效的模拟电压就越趋近于高电平，占空比越小，等效的模拟电压就越趋近于低电平 


	PWM 频率  	Freq = CK_PSC / (PSC + 1) / (ARR + 1)     和计数器溢出频率一样，每次清0时都代表着一个周期结束
	PWM 占空比  Duty = CCR / (ARR + 1) (高电平占比)
	PWM 频率	Reso = 1 / (ARR + 1)                      这个的含义需要根据结合占空比来看，这个乘上CCR就是占空比
*/
	

void PWM_Init(void)
{
	
	rcc_periph_clock_enable(RCC_GPIOA);	
	rcc_periph_clock_enable(RCC_AFIO);
	rcc_periph_clock_enable(RCC_TIM2);
	
	gpio_set_mode(
		GPIOA,
                GPIO_MODE_OUTPUT_50_MHZ,
        	GPIO_CNF_OUTPUT_ALTFN_PUSHPULL,
                GPIO1
	);
	
	// TIM2:
	timer_disable_counter(TIM2);
	rcc_periph_reset_pulse(RST_TIM2);

	timer_set_mode(TIM2,
		TIM_CR1_CKD_CK_INT,
		TIM_CR1_CMS_EDGE,
		TIM_CR1_DIR_UP);

	timer_set_period(TIM2, 20000-1);	// ARR
	timer_set_prescaler(TIM2, 72-1); // PSC see https://github.com/ve3wwg/stm32f103c8t6/pull/12/
	//timer_set_repetition_counter(TIM2,0);	// Only needed for advanced timers:
	timer_enable_preload(TIM2);
	timer_continuous_mode(TIM2);

	timer_disable_oc_output(TIM2,TIM_OC2);
	timer_set_oc_mode(TIM2,TIM_OC2,TIM_OCM_PWM1);
	timer_set_oc_polarity_high(TIM2,TIM_OC2);						

	timer_enable_oc_output(TIM2,TIM_OC2);
	timer_set_oc_value(TIM2,TIM_OC2, 500);	// CCR
	timer_enable_counter(TIM2);

	/*
		因为舵机要求PWM周期为 20ms  1Hz/20ms = 1Hz / 0.02s = 50Hz
	
		PWM频率 = 50Hz = 72MHz / 72Hz / 20000Hz
	
		因为舵机要求高电平时间 0.5ms--2.5ms
		
		500 / 20000 = 2.5%     占空比
		2.5% * 20ms = 0.5ms    
		2500 / 20000 = 12.5%   占空比
		12.5% * 20ms = 2.5ms
	
	*/
	
}


void PWM_SetCompare2(uint16_t Compare)
{
	//PWM_SetCompare2(TIM2, Compare);
	timer_set_oc_value(TIM2, TIM_OC2, Compare);
	//timer_set_oc_value(TIM2, TIM2_CCR2, Compare);
}


void Servo_Init(void)
{
	PWM_Init();
}
// 这段代码还涉及到一个除法和取整操作，因为角度需要被转换为PWM的占空比。
// 使用浮点数可以更精确地执行这个转换，因为浮点数可以提供小数部分，而整数类型（如 uint16_t）只能表示整数。
void Servo_SetAngle(uint16_t Angle)
{
	// 0     500
	// 180   2500
	//PWM_SetCompare2((uint16_t)((float)Angle / 180.f) * 2000 + 500);
	PWM_SetCompare2(Angle*11 + 500);
}

int
main(void) {

	rcc_clock_setup_pll(&rcc_hse_configs[RCC_CLOCK_HSE8_72MHZ]);

	int i;
	Servo_Init();

	//timer_disable_counter(TIM2);
	Servo_SetAngle(0);
	//timer_enable_counter(TIM2);


	// xTaskCreate(monitor_task,"monitor",500,NULL,1,NULL);
	// vTaskStartScheduler();
	for (;;);
	return 0;
}

// End