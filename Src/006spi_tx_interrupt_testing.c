/*
 * test1.c
 *
 *  Created on: Jul 27, 2026
 *      Author: berkd
 */

/*
 * PB15 --> SPI2_MOSI
 * PB14 --> SPI2_MISO
 * PB13 --> SPI2_SCLK
 * PB12 --> SPI2_NSS
 * ALT function mode : 5
 */

#include "stm32f446xx.h"
#include <string.h>

SPI_Handle_t SPI2Handle;

char user_data[] = "Berk Pasha";

void SPI2_GPIOInits(void)
{
	GPIO_Handle_t SPIPins;

	SPIPins.pGPIOx = GPIOB;

	SPIPins.GPIO_PinConfig.GPIO_PinMode = GPIO_MODE_ALTFN;
	SPIPins.GPIO_PinConfig.GPIO_PinAltFunMode = 5;
	SPIPins.GPIO_PinConfig.GPIO_PinOPType = GPIO_OP_TYPE_PP;
	SPIPins.GPIO_PinConfig.GPIO_PinPuPdControl = GPIO_NO_PUPD;
	SPIPins.GPIO_PinConfig.GPIO_PinSpeed = GPIO_SPEED_FAST;

	//SCLK
	SPIPins.GPIO_PinConfig.GPIO_PinNumber = GPIO_PIN_NO_13;
	GPIO_Init(&SPIPins);

	//MOSI
	SPIPins.GPIO_PinConfig.GPIO_PinNumber = GPIO_PIN_NO_15;
	GPIO_Init(&SPIPins);

	//MISO
	SPIPins.GPIO_PinConfig.GPIO_PinNumber = GPIO_PIN_NO_14;
	GPIO_Init(&SPIPins);

	//NSS
	SPIPins.GPIO_PinConfig.GPIO_PinNumber = GPIO_PIN_NO_12;
	GPIO_Init(&SPIPins);
}

void SPI2_Inits(void)
{

	SPI2Handle.pSPIx = SPI2;

	SPI2Handle.SPIConfig.SPI_DeviceMode = SPI_DEVICE_MODE_MASTER;
	SPI2Handle.SPIConfig.SPI_BusConfig = SPI_BUS_CONFIG_FD;
	SPI2Handle.SPIConfig.SPI_SclkSpeed = SPI_SCLK_SPEED_DIV8;
	SPI2Handle.SPIConfig.SPI_DFF = SPI_DFF_8BITS;
	SPI2Handle.SPIConfig.SPI_CPOL = SPI_CPOL_LOW;
	SPI2Handle.SPIConfig.SPI_CPHA = SPI_CPHA_LOW;
	SPI2Handle.SPIConfig.SPI_SSM = SPI_SSM_DI;

	SPI_Init(&SPI2Handle);

	SPI_SSOEConfig(SPI2Handle.pSPIx, ENABLE);

}

void SPI2_IRQHandler(void)
{
	SPI_IRQHandling(&SPI2Handle);
}

void SPI_ApplicationEventCallback(SPI_Handle_t *pSPIHandle, uint8_t AppEv)
{
	if(AppEv == SPI_EVENT_TX_CMPLT)
	{

	}
}

int main(void)
{
	SPI2_GPIOInits();

	SPI2_Inits();

	SPI_IRQInterruptConfig(IRQ_NO_SPI2, ENABLE);
	SPI_IRQPriorityConfig(IRQ_NO_SPI2, NVIC_IRQ_PRIO15);

	SPI_PeripheralControl(SPI2Handle.pSPIx, ENABLE);

	SPI_SendDataIT(&SPI2Handle, (uint8_t*)user_data, strlen(user_data));

	while(1);
	return 0;
}

