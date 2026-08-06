/*
 * 008i2c_master_tx_testing.c
 *
 *  Created on: Aug 3, 2026
 *      Author: berkd
 */

/*
 * Pin Configurations:
 * PB6  --> I2C1_SCL (Alternate Function Mode: AF4)
 * PB7  --> I2C1_SDA (Alternate Function Mode: AF4)
 * PC13 --> B1 USER BUTTON (GPIO Input Mode)
 */

#include "stm32f446xx.h"
#include <string.h>

#define MY_ADDR 		0x61
#define SLAVE_ADDR		0x68

I2C_Handle_t I2C1Handle;

//test data
uint8_t some_data[] = "Berk Pasha";

void delay(void)
{
	for(uint32_t i = 0; i < 500000 / 2; i++);
}

//Function to configure GPIOB pins for I2C1
void I2C1_GPIOInits(void)
{
	GPIO_Handle_t I2CPins;

	I2CPins.pGPIOx = GPIOB;
	I2CPins.GPIO_PinConfig.GPIO_PinMode = GPIO_MODE_ALTFN;
	I2CPins.GPIO_PinConfig.GPIO_PinAltFunMode = 4;
	I2CPins.GPIO_PinConfig.GPIO_PinOPType = GPIO_OP_TYPE_OD;
	I2CPins.GPIO_PinConfig.GPIO_PinPuPdControl = GPIO_NO_PUPD;
	I2CPins.GPIO_PinConfig.GPIO_PinSpeed = GPIO_SPEED_LOW;

	// SCL pin set
	I2CPins.GPIO_PinConfig.GPIO_PinNumber = GPIO_PIN_NO_6;
	GPIO_Init(&I2CPins);

	// SDA pin set
	I2CPins.GPIO_PinConfig.GPIO_PinNumber = GPIO_PIN_NO_7;
	GPIO_Init(&I2CPins);
}

//Function to configure User Button
void GPIO_ButtonInit(void)
{
	GPIO_Handle_t GPIOBtn;

	GPIOBtn.pGPIOx = GPIOC;
	GPIOBtn.GPIO_PinConfig.GPIO_PinMode = GPIO_MODE_IN;
	GPIOBtn.GPIO_PinConfig.GPIO_PinSpeed = GPIO_SPEED_LOW;
	GPIOBtn.GPIO_PinConfig.GPIO_PinPuPdControl = GPIO_NO_PUPD;
	GPIOBtn.GPIO_PinConfig.GPIO_PinNumber = GPIO_PIN_NO_13;

	GPIO_Init(&GPIOBtn);
}

//Function to configure I2C1 peripheral settings
void I2C1_Inits(void)
{
	I2C1Handle.pI2Cx = I2C1;
	I2C1Handle.I2C_Config.I2C_ACKControl = I2C_ACK_ENABLE;
	I2C1Handle.I2C_Config.I2C_DeviceAddress = MY_ADDR;
	I2C1Handle.I2C_Config.I2C_SCLSpeed = I2C_SCL_SPEED_SM;

	I2C_Init(&I2C1Handle);
}

int main(void)
{
	//1. configure GPIO pins and button
	GPIO_ButtonInit();
	I2C1_GPIOInits();

	//2. configure I2C1 peripheral
	I2C1_Inits();

	//3. enable I2C peripheral (PE)
	I2C_PeripheralControl(I2C1, ENABLE);

	while(1)
	{
		//wait until button is pressed
		while( GPIO_ReadFromInputPin(GPIOC, GPIO_PIN_NO_13) == GPIO_PIN_SET);

		delay();

		//send data over I2C
		I2C_MasterSendData(&I2C1Handle, some_data, strlen((char*)some_data), SLAVE_ADDR);
	}


	return 0;
}
