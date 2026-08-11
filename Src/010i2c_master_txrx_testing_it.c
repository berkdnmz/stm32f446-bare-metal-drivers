/*
 * 010i2c_master_txrx_testing_it.c
 *
 *  Created on: Aug 11, 2026
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

#define MY_ADDR             	0x61
#define MPU6050_SLAVE_ADDR  	0x68
#define MPU6050_WHO_AM_I_REG 	0x75

I2C_Handle_t I2C1Handle;
uint8_t RxBuffer[1];
volatile uint8_t rxComplt = DISABLE;
volatile uint8_t txComplt = DISABLE;

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

// I2C1 Event Interrupt Handler
void I2C1_EV_IRQHandler(void)
{
	I2C_EV_IRQHandling(&I2C1Handle);
}

// I2C1 Error Interrupt Handler
void I2C1_ER_IRQHandler(void)
{
	I2C_ER_IRQHandling(&I2C1Handle);
}

// Application Callback Implementation
void I2C_ApplicationEventCallback(I2C_Handle_t *pI2CHandle, uint8_t AppEv)
{
	if(AppEv == I2C_EV_RX_CMPLT)
	{
		// Reception completed successfully
		rxComplt = ENABLE;
	}
	else if(AppEv == I2C_EV_TX_CMPLT)
	{
		// Transmission completed successfully
		txComplt = ENABLE;
	}
}

int main(void)
{
	uint8_t command_code = MPU6050_WHO_AM_I_REG;

	//1. Configure GPIO pins and user button
	GPIO_ButtonInit();
	I2C1_GPIOInits();

	//2. Configure I2C1 peripheral settings
	I2C1_Inits();

	//3. Configure NVIC for I2C Event and Error IRQs
	I2C_IRQInterruptConfig(IRQ_NO_I2C1_EV, ENABLE);
	I2C_IRQInterruptConfig(IRQ_NO_I2C1_ER, ENABLE);

	// 4. Enable I2C peripheral
	I2C_PeripheralControl(I2C1, ENABLE);
	I2C_ManageAcking(I2C1, I2C_ACK_ENABLE);

	while(1)
	{
		// Wait until the button is pressed (0 means pressed)
		while( GPIO_ReadFromInputPin(GPIOC, GPIO_PIN_NO_13) == GPIO_PIN_SET );

		// Wait a little bit to prevent button bouncing
		delay();

		// Step 1: Send the register address we want to read.
		// We use Repeated Start (I2C_ENABLE_SR) because we will read data right after this.
		I2C_MasterSendDataIT(&I2C1Handle, &command_code, 1, MPU6050_SLAVE_ADDR, I2C_ENABLE_SR);

		// Wait here until sending is finished in the background
		while(txComplt != ENABLE);

		// Step 2: Read 1 byte of data from that register.
		// We don't need Repeated Start anymore, so we disable it (I2C_DISABLE_SR).
		I2C_MasterReceiveDataIT(&I2C1Handle, RxBuffer, 1, MPU6050_SLAVE_ADDR, I2C_DISABLE_SR);

		// Wait here until reading is finished in the background
		while(rxComplt != ENABLE);

		// TEST RESULT:
		// Put a breakpoint on this line!
		// If RxBuffer[0] is 0x68, your driver and sensor work perfectly!
	}

	return 0;
}










