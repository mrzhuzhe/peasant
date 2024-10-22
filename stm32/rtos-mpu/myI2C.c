#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/gpio.h>
#include "myI2C.h"
//#define SCL_PORT 			GPIOB
//#define SCL_PIN 			GPIO_Pin_10
//#define SDA_PIN 			GPIO_Pin_11

// #define MyI2C_W_SCL(x)		GPIO_WriteBit(GPIOB, GPIO_Pin_10, (BitAction)(x))
// #define MyI2C_W_SDA(x)		GPIO_WriteBit(GPIOB, GPIO_Pin_11, (BitAction)(x))

// 方便移植，，，写SCL，参数给1或0 就可以释放或拉低SCL
// void MyI2C_W_SCL(uint8_t BitValue)
// {
// 	GPIO_WriteBit(GPIOB, GPIO_Pin_10, (BitAction)BitValue);
// 	Delay_us(10);
// }

// // 写SDA
// void MyI2C_W_SDA(uint8_t BitValue)
// {
// 	// 只要输入值非0，那么就是1
// 	GPIO_WriteBit(GPIOB, GPIO_Pin_11, (BitAction)BitValue);
// 	Delay_us(10);
// }

// 读SDA
uint8_t MyI2C_R_SDA(void)
{
	uint8_t BitValue;
	BitValue = gpio_get(GPIOB, GPIO11);;
	return BitValue;
}

void MyI2C_Init(void)
{
	rcc_periph_clock_enable(RCC_GPIOB);
    gpio_set_mode(GPIOB,
		GPIO_MODE_OUTPUT_50_MHZ,
		GPIO_CNF_OUTPUT_OPENDRAIN,
		GPIO10|GPIO11);
		
	gpio_set(GPIOB, GPIO10); //	OLED_W_SCL (1);
	gpio_set(GPIOB, GPIO11);	//	OLED_W_SDA (1);

}


/* 
	在软件模拟操作时，需要确保每一次操作（除了终止条件）的结尾SCL永远是以0结束，
	这样的话每一次操作的开始SCL永远是以0为开始
	SCL全部交由主机控制
*/ 

void MyI2C_Start(void)
{
	/*
		这里先置高电平是因为在重复起始条件触发前，SCL一定为0，SDA不确定
		所以趁SCL为低电平时先SDA置1，然后再SCL置1，回到空闲状态，顺序不能反，不然可能会触发终止条件
		然后先拉低SDA在拉低SCL就可以触发起始条件
	*/
	gpio_set(GPIOB, GPIO11);
	gpio_set(GPIOB, GPIO10);
	
	gpio_clear(GPIOB, GPIO11);
	gpio_set(GPIOB, GPIO10);
	
}


void MyI2C_Stop(void)
{
	/*
		终止条件触发前，SCL一定为0，SDA根据应答的不同可能为1可能为0，所以为了保证要在SCL高电平期间改变SDA
		需要先将SDA置0，然后再将SCL置1，最后再将SDA置1，这样就做到了在SCL高电平期间改变SDA
		如果在SCL高电平期间改变了SDA的电平，则是触发了起始或终止条件

	*/
	gpio_clear(GPIOB, GPIO11);
	
	gpio_set(GPIOB, GPIO10);
	gpio_set(GPIOB, GPIO11);
	
}

// 发送字节 高位先行
void MyI2C_SendByte(uint8_t Byte)
{
	uint8_t i;
	
	for(i = 0; i < 8; i ++)
	{
		// 因为有了47行注释先前约定好的规则，所以在操作开始时SCL为0，直接修改SDA即可
		if((Byte & (0x80 >> i)) == 0)
		{	
			gpio_clear(GPIOB, GPIO11);
		}
		else
		{
			gpio_set(GPIOB, GPIO11);
		}
		
		// 修改SDA后 SCL置1
		gpio_set(GPIOB, GPIO10);
		// 此时从设备已经在SCL为高电平时读取数据，结尾SCL置0
		gpio_clear(GPIOB, GPIO10);
	}
	
}


uint8_t MyI2C_ReceiveByte(void)
{
	
	uint8_t i, Byte = 0x00;
	
	// 主机释放SDA，切换为从机输入模式
	gpio_set(GPIOB, GPIO11);
	
	/****
		此时从机将数据放到SDA上
	****/

	for(i = 0; i < 8; i ++)
	{

		// 从机设备发送一个bit后，最后SCL是低电平，而我们主机需要再SCL为高电平时读取数据，所以这里置1
		gpio_set(GPIOB, GPIO10);
		// 读取数据
		if(MyI2C_R_SDA() == 1)
		{
			Byte |= (0x80 >> i);
		}
		else
		{
			// 所以如果读出来为0，则就相当于写入0了，Byte默认就为0x00，
		}
			
		// 结尾SCL置0，让从机在SDA上放置下一位数据
		gpio_clear(GPIOB, GPIO10);
	}
	return Byte;
}




// 发送应答
void MyI2C_SendAck(uint8_t Ackbit)
{
	if(Ackbit){
        gpio_set(GPIOB, GPIO11);	//	OLED_W_SDA (1);
    } else {
        gpio_clear(GPIOB, GPIO11);
    }

	gpio_set(GPIOB, GPIO10); // 高电平，从机读取应答
	gpio_clear(GPIOB, GPIO10);
}


uint8_t MyI2C_ReceiveAck(void)
{
	
	uint8_t AckByte;
	// 释放SDA总线的控制权，从机发送应答位
	gpio_set(GPIOB, GPIO11);
	// 置高电平，主机在高电平读取应答位
	gpio_set(GPIOB, GPIO10);
	AckByte = MyI2C_R_SDA();
	gpio_clear(GPIOB, GPIO10);

	return AckByte;
}