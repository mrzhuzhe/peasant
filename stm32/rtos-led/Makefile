######################################################################
#  Project
######################################################################

BINARY		= rtos-template
SRCFILES	= main.c 
SRCFILES += ugui.c     
SRCFILES += meter.c
SRCFILES += getline.c 
SRCFILES += intelhex.c
SRCFILES += mcuio.c
SRCFILES += miniprintf.c
SRCFILES += uartlib.c
SRCFILES += usbcdc.c     
SRCFILES += libs/freertos/Source/portable/MemMang/heap_4.c 
SRCFILES += libs/freertos/Source/list.c 
SRCFILES += libs/freertos/Source/portable/GCC/ARM_CM3/port.c 
SRCFILES += libs/freertos/Source/tasks.c 
SRCFILES += libs/freertos/Source/queue.c

LDLIBS		+= -lm
ugui.o:		CFLAGS += -Wno-parentheses

include ./Makefile.incl
include ./Makefile.rtos
# End