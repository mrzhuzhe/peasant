#include "main.h"
#include <FreeRTOS.h>
#include <task.h>
#include <timers.h>

#if defined STM32F1
    #include <stm32f1xx_hal.h>

    // STM32VL-Discovery green led - PC9
    #define LED_PORT                GPIOC
    #define LED_PIN                 GPIO_PIN_13
    // STM32VL-Discovery blue led - PC8
    //#define LED_PIN                 GPIO_PIN_8
    #define LED_PORT_CLK_ENABLE     __HAL_RCC_GPIOC_CLK_ENABLE
#endif

static void blinky_blinkTask(void *arg)
{
    for(;;)
    {
        vTaskDelay(500);
        HAL_GPIO_TogglePin(LED_PORT, LED_PIN);
    }
}

void blinky_init()
{
    GPIO_InitTypeDef GPIO_Config;

    GPIO_Config.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_Config.Pull = GPIO_NOPULL;
    GPIO_Config.Speed = GPIO_SPEED_FREQ_HIGH;

    GPIO_Config.Pin = LED_PIN;

    LED_PORT_CLK_ENABLE();
    HAL_GPIO_Init(LED_PORT, &GPIO_Config);
}

int main(void)
{
    SystemInit();
    blinky_init();
    
    xTaskCreate(blinky_blinkTask, "blinky", configMINIMAL_STACK_SIZE, NULL, tskIDLE_PRIORITY + 1, NULL);
    
    vTaskStartScheduler();
    for (;;);
    
    return 0;
}

void vApplicationTickHook(void)
{
}

void vApplicationIdleHook(void)
{
}

void vApplicationMallocFailedHook(void)
{
    taskDISABLE_INTERRUPTS();
    for(;;);
}

void vApplicationStackOverflowHook(TaskHandle_t pxTask, char *pcTaskName)
{
    (void) pcTaskName;
    (void) pxTask;

    taskDISABLE_INTERRUPTS();
    for(;;);
}
