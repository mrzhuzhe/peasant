/*
 * This file is part of the libopencm3 project.
 *
 * Copyright (C) 2009 Uwe Hermann <uwe@hermann-uwe.de>
 *
 * This library is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this library.  If not, see <http://www.gnu.org/licenses/>.
 */
#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/gpio.h>

static void
gpio_setup(void) {

	/* Enable GPIOC clock. */
	rcc_periph_clock_enable(RCC_GPIOC);

	/* Set GPIO8 (in GPIO port C) to 'output push-pull'. */
	gpio_set_mode(GPIOC,GPIO_MODE_OUTPUT_2_MHZ,
		      GPIO_CNF_OUTPUT_PUSHPULL,GPIO14);

    gpio_set_mode(GPIOC,GPIO_MODE_INPUT,
		      GPIO_CNF_INPUT_PULL_UPDOWN,GPIO15);
}

int
main(void) {
	int i = 0;
    int j = 0;
    int repeat = 3;
    int delay = 5e5;
	gpio_setup();
    
    gpio_set(GPIOC,GPIO14);		/* LED off */

    uint16_t isGPIO15On = 0;
	for (;;) {

        isGPIO15On = gpio_get(GPIOC,GPIO15);	
        if (isGPIO15On != 0){
            for (j=0; j<repeat;j++){
                gpio_clear(GPIOC,GPIO14);	/* LED on */
                for (i = 0; i < delay*3; i++)	/* Wait a bit. */
                    __asm__("nop");

                gpio_set(GPIOC,GPIO14);		/* LED off */
                for (i = 0; i < delay; i++)	/* Wait a bit. */
                    __asm__("nop");
            }
            
            gpio_clear(GPIOC,GPIO14);	/* LED on */
            for (i = 0; i < delay*30; i++)	/* Wait a bit. */
                __asm__("nop");
            gpio_set(GPIOC,GPIO14);		/* LED off */
            for (i = 0; i < delay*10; i++)	/* Wait a bit. */
            	__asm__("nop");
            isGPIO15On = 0;
        }

	}

	return 0;
}