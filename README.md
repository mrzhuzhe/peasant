# Peasant
> 听好了乡巴佬，等我当上国王，我要制定新的税法

## Linux kernel 

1. 内存碎片 freelist + buddy算法 + 扁平化堆容器
    有新增有删除，就会产生背包问题，仅仅合并空的内存段也不能很好的解决此问题
2. An OS how to https://github.com/cfenollosa/os-tutorial  
https://www.cs.bham.ac.uk/~exr/lectures/opsys/10_11/lectures/os-dev.pdf 
https://wiki.osdev.org/Main_Page
https://littleosbook.github.io/ https://web.archive.org/web/20160412174753/http://www.jamesmolloy.co.uk/tutorial_html/index.html
3. Master Boot Record (MBR) real mode protected mode
4. STM32F103C8T6 https://github.com/CShark/stm32usb https://stackoverflow.com/questions/73551956/stm32-bare-metal-usb-implementation
    OpenOCD
    libopencm3
    Todo intelx86 usb communicate with stm32 with custom driver
5. rtos https://github.com/ve3wwg/stm32f103c8t6
6. More topics
    
    Cook: Security things in Linux v5.3

    Unexporting the system call table

    Controll-flow integrity for the kernel

    Unexporting kallsyms_lookup_name

    Kernel Proble

    Kernel address space layout randomization
7. task schedule / interupt handler task deny , task wait , task workqueue 
    https://sysprog21.github.io/lkmpg/
    https://github.com/d0u9/Linux-Device-Driver ldd3 code
    
8. gstack gdb strace 
9. amd and nvidia opensource driver
    https://wiki.gentoo.org/wiki/Linux_firmware
    https://github.com/GPUOpen-Drivers/pal
    https://github.com/NVIDIA/open-gpu-kernel-modules

10. gstream
11. usb request block
    firmware hotplugin 
    network dma
12. Clifford Cummings
    hardware software cooperation

13. 硅初始化
    opensil https://github.com/openSIL/openSIL
    AMD Utralscale https://www.amd.com/en/products/adaptive-socs-and-fpgas/soc/zynq-ultrascale-plus-mpsoc.html

14. Neuromophic computing 
    snn



### Detail

    Firmware - OS    
                - boot
                    - magic number 
                    - layout 
                - driver - char device 
                            - protocal 
                            - abi
                - file system 
                - memory management 
                    - DMA
                    - page table 
                    - vitual address
                - workqueue
                - network interface
                - interrupt handle
                - Peripheral device
                    - I2C
                    - SPI 
                    - UART
                    - PCIE
                    - Ananlog to digital
                - Timer Tree 
                    - PWN

    FPGA - RTL - LUT 
               - switch

    Fluid Simulate - discrete - geo
                              - gradient
                   - solver
                   - boundary condition




## Network protocol
1. 流量工程 https://github.com/Kong/kong


## VHDL
1. Static Timing Analysis for Nanometer Designs: A Practical Approach
2. advanced digital design with the verilog hdl


## MCU
1. program and erase flash memory
PRFTBS PRFTBE HLFCYA LATENCG
adress offset 

2. boot configuration
    main flash memory / system memory / embedded SRAM

3. Embedded bootloader

4. CRC 循环冗余位check

