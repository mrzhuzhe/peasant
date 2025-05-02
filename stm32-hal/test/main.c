#include "stm32f1xx_hal.h"
#include <stdio.h>

static void
gpio_setup(void) {
	printf("test\n");
}

int
main(void) {
	
    gpio_setup();

	return 0;
}