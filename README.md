# Peasant
> 听好了乡巴佬，等我当上国王，我要制定新的税法


## Stm32 test
```
// 开发板型号： stm32f103c8t6 

// GPIO 测试， 仅使用opencm3
/stm32/led_test/

// uart 通信
/stm32/uart_test/

// rtos gpio 测试
/stm32/rtos-template

// pwn 驱动舵机和电机 用的电机控制芯片是 tb6612fng
/stm32/pwm

// 模拟信号转换为数字信号
/stm32/rtos-adc

// can 总线
/stm32/rtos-can

// 用dma搬运adc数据
/stm32/rtos-dma

// 用软件i2c连接oled和mpu6050陀螺仪
/stm32/rtos-mpu

// 用软件i2c连接oled
/stm32/rtos-oled

// usb 连接 , 注意用这个连接时一定要拔掉电源，因为usb本身就是电源
/stm32/rtos-usbcdc

// Warnning Not finish yet dont use this
rtos-winbond 

```

## A Detail of MCU firmware

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

    









