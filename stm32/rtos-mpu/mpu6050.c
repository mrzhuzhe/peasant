#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/gpio.h>
#include "myI2C.h"
#include "mpu6050_reg.h"

#define MPU6050_ADDRESS 	0xD0

// 指定地址写一个字节
void MPU6050_WriteReg(uint8_t RegAddress, uint8_t Data)
{
	MyI2C_Start();
	MyI2C_SendByte(MPU6050_ADDRESS);
	MyI2C_ReceiveAck();
	MyI2C_SendByte(RegAddress);
	MyI2C_ReceiveAck();
	MyI2C_SendByte(Data);
	MyI2C_ReceiveAck();
	MyI2C_Stop();
}

// 指定地址读一个字节
uint8_t MPU6050_ReadReg(uint8_t RegAddress)
{
	uint8_t Data;
	
	MyI2C_Start();
	MyI2C_SendByte(MPU6050_ADDRESS);
	MyI2C_ReceiveAck();
	MyI2C_SendByte(RegAddress);
	MyI2C_ReceiveAck();
	
	MyI2C_Start(); // 重复起始条件
	MyI2C_SendByte(MPU6050_ADDRESS | 0x01); // 修改读写位为1 读
	MyI2C_ReceiveAck();
	Data = MyI2C_ReceiveByte();
	MyI2C_SendAck(1); // 不给从机应答
	MyI2C_Stop();
	
	return Data;
}

void MPU6050_Init(void)
{
	MyI2C_Init();
	
	// 通过写寄存器，配置MPU6050内的寄存器，需要对照MPU6050的寄存器手册查看
	MPU6050_WriteReg(MPU6050_PWR_MGMT_1, 0x01); // 解除睡眠，采用陀螺仪时钟
	MPU6050_WriteReg(MPU6050_PWR_MGMT_2, 0x00); // 6轴均不待机
	MPU6050_WriteReg(MPU6050_SMPLRT_DIV, 0x09); // 采样分频为10
	MPU6050_WriteReg(MPU6050_CONFIG, 0x06); // 滤波参数最大
	MPU6050_WriteReg(MPU6050_GYRO_CONFIG, 0x18); // 陀螺仪最大量程
	MPU6050_WriteReg(MPU6050_ACCEL_CONFIG, 0x18); // 加速度计最大量程
	
}

uint8_t MPU6050_GetID(void)
{
	return MPU6050_ReadReg(MPU6050_WHO_AM_I);
}

void MPU6050_GetData(int16_t *AccX, int16_t	*AccY, int16_t *AccZ,
						int16_t *GyroX, int16_t	*GyroY, int16_t *GyroZ)
{
	uint8_t DataH, DataL;
	
	DataH = MPU6050_ReadReg(MPU6050_ACCEL_XOUT_H);
	DataL = MPU6050_ReadReg(MPU6050_ACCEL_XOUT_L);
	
	*AccX = (DataH << 8) | DataL; // 8位数据进行左移后会自动进行类型转换，移出去的位不会丢失
	
	DataH = MPU6050_ReadReg(MPU6050_ACCEL_YOUT_H);
	DataL = MPU6050_ReadReg(MPU6050_ACCEL_YOUT_L);
	
	*AccY = (DataH << 8) | DataL; 
	
	DataH = MPU6050_ReadReg(MPU6050_ACCEL_ZOUT_H);
	DataL = MPU6050_ReadReg(MPU6050_ACCEL_ZOUT_L);
	
	*AccZ = (DataH << 8) | DataL; 
	
	
	
	DataH = MPU6050_ReadReg(MPU6050_GYRO_XOUT_H);
	DataL = MPU6050_ReadReg(MPU6050_GYRO_XOUT_L);
	
	*GyroX = (DataH << 8) | DataL; 
	
	DataH = MPU6050_ReadReg(MPU6050_GYRO_YOUT_H);
	DataL = MPU6050_ReadReg(MPU6050_GYRO_YOUT_L);
	
	*GyroY = (DataH << 8) | DataL; 
	
	DataH = MPU6050_ReadReg(MPU6050_GYRO_ZOUT_H);
	DataL = MPU6050_ReadReg(MPU6050_GYRO_ZOUT_L);
	
	*GyroZ = (DataH << 8) | DataL; 
	
}