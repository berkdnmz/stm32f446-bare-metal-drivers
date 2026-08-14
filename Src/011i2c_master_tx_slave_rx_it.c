/*
 * 011i2c_master_tx_slave_rx_it.c
 *
 *  Created on: Aug 13, 2026
 *      Author: berkd
 */

/*
 * Pin Configurations:
 * PB6  --> I2C1_SCL (Alternate Function Mode: AF4)  Master
 * PB7  --> I2C1_SDA (Alternate Function Mode: AF4)  Master
 * PB10 --> I2C2_SCL (Alternate Function Mode: AF4)  Slave
 * PB3  --> I2C2_SDA (Alternate Function Mode: AF4)  Slave
 * PC13 --> B1 USER BUTTON (GPIO Input Mode)
 */

#include "stm32f446xx.h"
#include <string.h>

#define SLAVE_ADDR      0x68

I2C_Handle_t I2C1Handle; // Handle structure for Master (I2C1)
I2C_Handle_t I2C2Handle; // Handle structure for Slave (I2C2)

// Data to be sent by the Master
uint8_t tx_buffer[] = "STM32 Slave Test";

// Buffer to store data received by the Slave
uint8_t rx_buffer[32];
uint8_t rx_index = 0;

// Configures the GPIO pins used by I2C1 (Master)
void I2C1_GPIOInits(void)
{
    GPIO_Handle_t I2CPins;
    I2CPins.pGPIOx = GPIOB;
    I2CPins.GPIO_PinConfig.GPIO_PinMode = GPIO_MODE_ALTFN;
    I2CPins.GPIO_PinConfig.GPIO_PinAltFunMode = 4;
    I2CPins.GPIO_PinConfig.GPIO_PinOPType = GPIO_OP_TYPE_OD; // Open Drain is required for I2C
    I2CPins.GPIO_PinConfig.GPIO_PinPuPdControl = GPIO_NO_PUPD;
    I2CPins.GPIO_PinConfig.GPIO_PinSpeed = GPIO_SPEED_LOW;

    // Initialize SCL pin (PB6)
    I2CPins.GPIO_PinConfig.GPIO_PinNumber = GPIO_PIN_NO_6;
    GPIO_Init(&I2CPins);

    // Initialize SDA pin (PB7)
    I2CPins.GPIO_PinConfig.GPIO_PinNumber = GPIO_PIN_NO_7;
    GPIO_Init(&I2CPins);
}

// Configures the GPIO pins used by I2C2 (Slave)
void I2C2_GPIOInits(void)
{
	GPIO_Handle_t I2CPins;

	I2CPins.pGPIOx = GPIOB;
	I2CPins.GPIO_PinConfig.GPIO_PinMode = GPIO_MODE_ALTFN;
	I2CPins.GPIO_PinConfig.GPIO_PinAltFunMode = 4;
	I2CPins.GPIO_PinConfig.GPIO_PinOPType = GPIO_OP_TYPE_OD;
	I2CPins.GPIO_PinConfig.GPIO_PinPuPdControl = GPIO_NO_PUPD;
	I2CPins.GPIO_PinConfig.GPIO_PinSpeed = GPIO_SPEED_LOW;

	// Initialize SCL pin (PB10)
	I2CPins.GPIO_PinConfig.GPIO_PinNumber = GPIO_PIN_NO_10;
	GPIO_Init(&I2CPins);

	// Initialize SDA pin (PB3)
	I2CPins.GPIO_PinConfig.GPIO_PinNumber = GPIO_PIN_NO_3;
	GPIO_Init(&I2CPins);
}

// Initializes the I2C1 hardware settings (Master)
void I2C1_Inits(void)
{
    I2C1Handle.pI2Cx = I2C1;
    I2C1Handle.I2C_Config.I2C_ACKControl = I2C_ACK_ENABLE;
    I2C1Handle.I2C_Config.I2C_DeviceAddress = 0x61; // Master's own address
    I2C1Handle.I2C_Config.I2C_SCLSpeed = I2C_SCL_SPEED_SM; // Standard Mode (100kHz)

    I2C_Init(&I2C1Handle);
}

// Initializes the I2C2 hardware settings (Slave)
void I2C2_Inits(void)
{
    I2C2Handle.pI2Cx = I2C2;
    I2C2Handle.I2C_Config.I2C_ACKControl = I2C_ACK_ENABLE;
    I2C2Handle.I2C_Config.I2C_SCLSpeed = I2C_SCL_SPEED_SM;
    I2C2Handle.I2C_Config.I2C_DeviceAddress = SLAVE_ADDR; // The address it will respond to

    I2C_Init(&I2C2Handle);
}

// Configures the on-board User Button (PC13)
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

// Interrupt Service Routines (ISRs) for I2C1 (Master)
void I2C1_EV_IRQHandler(void) { I2C_EV_IRQHandling(&I2C1Handle); }
void I2C1_ER_IRQHandler(void) { I2C_ER_IRQHandling(&I2C1Handle); }

// Interrupt Service Routines (ISRs) for I2C2 (Slave)
void I2C2_EV_IRQHandler(void) { I2C_EV_IRQHandling(&I2C2Handle); }
void I2C2_ER_IRQHandler(void) { I2C_ER_IRQHandling(&I2C2Handle); }

// Callback function triggered by the driver when an I2C event happens
void I2C_ApplicationEventCallback(I2C_Handle_t *pI2CHandle, uint8_t AppEv)
{
    // Make sure the event is coming from the Slave device (I2C2)
    if(pI2CHandle->pI2Cx == I2C2)
    {
        if(AppEv == I2C_EV_DATA_RCV)
        {
            // Read the incoming byte from the Master and store it in the buffer
            rx_buffer[rx_index++] = I2C_SlaveReceiveData(pI2CHandle->pI2Cx);
        }
        else if(AppEv == I2C_EV_STOP)
        {
            // Master has stopped sending data. Add a null terminator to make it a valid string.
            rx_buffer[rx_index] = '\0';
        }
    }
}

int main(void)
{
    // 1. Initialize GPIO pins for button and I2C peripherals
    GPIO_ButtonInit();
    I2C1_GPIOInits();
    I2C2_GPIOInits();

    // 2. Initialize the I2C hardware configurations
    I2C1_Inits();
    I2C2_Inits();

    // 3. Enable the IRQ lines in the NVIC (Processor side)
    I2C_IRQInterruptConfig(IRQ_NO_I2C1_EV, ENABLE);
    I2C_IRQInterruptConfig(IRQ_NO_I2C1_ER, ENABLE);
    I2C_IRQInterruptConfig(IRQ_NO_I2C2_EV, ENABLE);
    I2C_IRQInterruptConfig(IRQ_NO_I2C2_ER, ENABLE);

    // 4. Turn on the I2C peripherals
    I2C_PeripheralControl(I2C1, ENABLE);
    I2C_PeripheralControl(I2C2, ENABLE);

    // 5. Setup the Slave device to listen for interrupts and send ACKs
    I2C_SlaveEnableDisableCallbackEvents(I2C2, ENABLE);
    I2C_ManageAcking(I2C2, I2C_ACK_ENABLE);

    while(1)
	{
        // Wait here until the User Button is pressed (ACTIVE LOW)
		while(GPIO_ReadFromInputPin(GPIOC, GPIO_PIN_NO_13) == GPIO_PIN_SET);

        // Simple software delay for button debounce
		for(uint32_t i = 0; i < 250000; i++);

		// Prepare the receive buffer for the new transfer
		rx_index = 0;
		memset(rx_buffer, 0, sizeof(rx_buffer));

        // Start sending data from Master (I2C1) to Slave (I2C2) in Interrupt mode
		I2C_MasterSendDataIT(&I2C1Handle, tx_buffer, strlen((char*)tx_buffer), SLAVE_ADDR, I2C_DISABLE_SR);
	}

    return 0;
}
