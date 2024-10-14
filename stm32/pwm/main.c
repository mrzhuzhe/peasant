#include <string.h>
#include <ctype.h>

#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"

#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/gpio.h>
#include <libopencm3/stm32/spi.h>
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
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	TIM_OCInitTypeDef TIM_OCInitStructure;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;
	/*
		因为舵机要求PWM周期为 20ms  1Hz/20ms = 1Hz / 0.02s = 50Hz
	
		PWM频率 = 50Hz = 72MHz / 72Hz / 20000Hz
	
		因为舵机要求高电平时间 0.5ms--2.5ms
		
		500 / 20000 = 2.5%     占空比
		2.5% * 20ms = 0.5ms    
		2500 / 20000 = 12.5%   占空比
		12.5% * 20ms = 2.5ms
	
	*/
	TIM_TimeBaseInitStructure.TIM_Period = 20000 - 1; // ARR
	TIM_TimeBaseInitStructure.TIM_Prescaler = 72 - 1; // PSC
	TIM_TimeBaseInitStructure.TIM_RepetitionCounter = 0; 
	
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseInitStructure);
	
	TIM_OCStructInit(&TIM_OCInitStructure); 
	
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
	TIM_OCInitStructure.TIM_Pulse = 500; // CCR
	
	TIM_OC2Init(TIM2, &TIM_OCInitStructure);
	
	TIM_Cmd(TIM2, ENABLE); // 启动定时器
}


void PWM_SetCompare2(uint16_t Compare)
{
	TIM_SetCompare2(TIM2, Compare);
}


void Servo_Init(void)
{
	PWM_Init();
}
// 这段代码还涉及到一个除法和取整操作，因为角度需要被转换为PWM的占空比。
// 使用浮点数可以更精确地执行这个转换，因为浮点数可以提供小数部分，而整数类型（如 uint16_t）只能表示整数。
void Servo_SetAngle(float Angle)
{
	// 0     500
	// 180   2500
	PWM_SetCompare2(Angle / 180 * 2000 + 500);
}

int
main(void) {

	rcc_clock_setup_pll(&rcc_hse_configs[RCC_CLOCK_HSE8_72MHZ]);


	Servo_Init();
	Servo_SetAngle(0);
	
	
	
	// xTaskCreate(monitor_task,"monitor",500,NULL,1,NULL);
	// vTaskStartScheduler();
	for (;;);
	return 0;
}

// End