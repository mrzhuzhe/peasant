######################################################################
#  Project
######################################################################

BINARY		= miniblink
SRCFILES	= main.c
SRCFILES	+= Hal/Src/stm32f1xx_hal.c
SRCFILES	+= Hal/Src/stm32f1xx_hal_gpio.c
SRCFILES	+= Hal/Src/stm32f1xx_hal_rcc.c
SRCFILES	+= Hal/Src/stm32f1xx_hal_flash.c
SRCFILES	+= Hal/Src/stm32f1xx_hal_cortex.c
SRCFILES	+= Cmiss/Device/ST/STM32F1xx/Source/system_stm32f1xx.c
SRCFILES	+= Cmiss/Device/ST/STM32F1xx/Source/gcc/startup_stm32f103xb.s


all: elf bin

include ./Makefile.incl

DEFS		+= -DSTM32F103xB

# End