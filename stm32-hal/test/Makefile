######################################################################
#  Project
######################################################################

BINARY		= miniblink
SRCFILES	= main.c
SRCFILES	+= stm32f1xx_hal.c
SRCFILES	+= stm32f1xx_hal_gpio.c
SRCFILES	+= stm32f1xx_hal_rcc.c
SRCFILES	+= stm32f1xx_hal_flash.c
SRCFILES	+= stm32f1xx_hal_cortex.c
SRCFILES	+= cmiss/Device/ST/STM32F1xx/Source/system_stm32f1xx.c
SRCFILES	+= cmiss/Device/ST/STM32F1xx/Source/gcc/startup_stm32f103xb.s


all: elf bin

include ./Makefile.incl

DEFS		+= -DSTM32F103xB

# End