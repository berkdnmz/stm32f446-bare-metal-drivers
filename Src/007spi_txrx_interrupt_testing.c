/*
 * 007spi_txrx_interrupt_testing.c
 *
 *  Created on: Jul 28, 2026
 *      Author: berkd
 */

/*
 * Pin Configurations:
 * PB15 --> SPI2_MOSI
 * PB14 --> SPI2_MISO
 * PB13 --> SPI2_SCLK
 * PB12 --> SPI2_NSS
 * Alternate Function Mode: AF5
 */

#include "stm32f446xx.h"
#include <string.h>

// Global handle structure for SPI2
SPI_Handle_t SPI2Handle;

// Transmit buffer with message
char tx_buffer[] = "Berk Pasha";

// Receive buffer to store incoming data from loopback
char rx_buffer[50];

// Application flags to track transfer status
uint8_t tx_complete = 0;
uint8_t rx_complete = 0;

// Function to configure GPIOB pins for SPI2
void SPI2_GPIOInits(void)
{
	GPIO_Handle_t SPIPins;

	SPIPins.pGPIOx = GPIOB;
	SPIPins.GPIO_PinConfig.GPIO_PinMode = GPIO_MODE_ALTFN;
	SPIPins.GPIO_PinConfig.GPIO_PinAltFunMode = 5;
	SPIPins.GPIO_PinConfig.GPIO_PinOPType = GPIO_OP_TYPE_PP;
	SPIPins.GPIO_PinConfig.GPIO_PinPuPdControl = GPIO_NO_PUPD;
	SPIPins.GPIO_PinConfig.GPIO_PinSpeed = GPIO_SPEED_FAST;

	// Clock Pin (SCLK)
	SPIPins.GPIO_PinConfig.GPIO_PinNumber = GPIO_PIN_NO_13;
	GPIO_Init(&SPIPins);

	// MOSI Pin
	SPIPins.GPIO_PinConfig.GPIO_PinNumber = GPIO_PIN_NO_15;
	GPIO_Init(&SPIPins);

	// MISO Pin
	SPIPins.GPIO_PinConfig.GPIO_PinNumber = GPIO_PIN_NO_14;
	GPIO_Init(&SPIPins);

	// NSS Pin
	SPIPins.GPIO_PinConfig.GPIO_PinNumber = GPIO_PIN_NO_12;
	GPIO_Init(&SPIPins);
}

// Function to configure SPI2 peripheral settings
void SPI2_Inits(void)
{
	SPI2Handle.pSPIx = SPI2;
	SPI2Handle.SPIConfig.SPI_DeviceMode = SPI_DEVICE_MODE_MASTER;
	SPI2Handle.SPIConfig.SPI_BusConfig = SPI_BUS_CONFIG_FD;
	SPI2Handle.SPIConfig.SPI_SclkSpeed = SPI_SCLK_SPEED_DIV8;
	SPI2Handle.SPIConfig.SPI_DFF = SPI_DFF_8BITS;
	SPI2Handle.SPIConfig.SPI_CPOL = SPI_CPOL_LOW;
	SPI2Handle.SPIConfig.SPI_CPHA = SPI_CPHA_LOW;
	SPI2Handle.SPIConfig.SPI_SSM = SPI_SSM_DI; // Hardware slave management

	// Initialize SPI2 hardware
	SPI_Init(&SPI2Handle);

	// Enable SSOE for automatic NSS pin control by hardware
	SPI_SSOEConfig(SPI2Handle.pSPIx, ENABLE);
}

// Interrupt Service Routine (ISR) for SPI2
void SPI2_IRQHandler(void)
{
	// Call driver interrupt handler to process TXE or RXNE
	SPI_IRQHandling(&SPI2Handle);
}

// Callback function called by driver when events happen
void SPI_ApplicationEventCallback(SPI_Handle_t *pSPIHandle, uint8_t AppEv)
{
	if(AppEv == SPI_EVENT_TX_CMPLT)
	{
		tx_complete = 1; // Mark transmission as finished
	}
	else if(AppEv == SPI_EVENT_RX_CMPLT)
	{
		rx_complete = 1; // Mark reception as finished
	}
}

int main(void)
{
	uint32_t data_len = strlen(tx_buffer);

	// 1. Initialize GPIO pins and SPI2 peripheral
	SPI2_GPIOInits();
	SPI2_Inits();

	// 2. Enable SPI2 interrupt in NVIC controller
	SPI_IRQPriorityConfig(IRQ_NO_SPI2, NVIC_IRQ_PRIO15);
	SPI_IRQInterruptConfig(IRQ_NO_SPI2, ENABLE);

	// 3. Enable SPI2 peripheral hardware
	SPI_PeripheralControl(SPI2, ENABLE);

	// 4. Start RX interrupt first (Listen)
	SPI_ReceiveDataIT(&SPI2Handle, (uint8_t*)rx_buffer, data_len);

	// 5. Start TX interrupt (Transmit)
	SPI_SendDataIT(&SPI2Handle, (uint8_t*)tx_buffer, data_len);

	// 6. Wait until both TX and RX complete in background
	while(!tx_complete || !rx_complete);


	while(1);

	return 0;
}
