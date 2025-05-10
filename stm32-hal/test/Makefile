######################################################################
#  Project
######################################################################

BINARY		= miniblink
SRCFILES	= main.c
SRCFILES	+= stm32f1xx_hal_gpio.c
SRCFILES	+= stm32f1xx_hal_rcc.c
SRCFILES	+= stm32f1xx_hal_flash.c
SRCFILES	+= stm32f1xx_hal_cortex.c


all: elf bin

include ./Makefile.incl

DEFS		+= -DSTM32F103xB

# End