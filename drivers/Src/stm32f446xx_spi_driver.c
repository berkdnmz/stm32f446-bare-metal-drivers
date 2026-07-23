/*
 * stm32f446xx_spi_driver.c
 *
 *  Created on: Jul 22, 2026
 *      Author: berkd
 */


#include "stm32f446xx_spi_driver.h"


/*************************************************************************************************************************
 * @fn 					- SPI_PeriClockControl
 *
 * @brief				- This function enables or disables the peripheral clock for the given SPI peripheral.
 *
 * @param[in]			- pSPIx : Base address of the SPI peripheral (e.g., SPI1, SPI2, SPI3, SPI4).
 * @param[in]			- EnorDi : ENABLE or DISABLE macro to control the clock.
 *
 * @return				- None.
 *
 * @Note				- The peripheral clock must be enabled before configuring SPI registers or calling SPI_Init().
 *
 *************************************************************************************************************************/
void SPI_PeriClockControl(SPI_RegDef_t *pSPIx, uint8_t EnorDi)
{
	if(EnorDi == ENABLE)
	{
		if(pSPIx == SPI1)
		{
			SPI1_PCLK_EN();
		}else if(pSPIx == SPI2)
		{
			SPI2_PCLK_EN();
		}else if(pSPIx == SPI3)
		{
			SPI3_PCLK_EN();
		}else if(pSPIx == SPI4)
		{
			SPI4_PCLK_EN();
		}
	}
	else
	{
		//TODO
	}
}

/*************************************************************************************************************************
 * @fn 					- SPI_Init
 *
 * @brief				- This function initializes the given SPI peripheral with the user settings.
 *
 * @param[in]			- pSPIHandle : Pointer to the SPI_Handle_t structure containing configuration parameters.
 *
 * @return				- None.
 *
 * @Note				- None.
 *
 *************************************************************************************************************************/
void SPI_Init(SPI_Handle_t *pSPIHandle)
{
	 //first lets configure the SPI_CR1 register

	uint32_t tempreg = 0;

	//1. configure the device mode
	tempreg |= pSPIHandle->SPIConfig.SPI_DeviceMode << SPI_CR1_MSTR;

	//2. configure the bus config
	if(pSPIHandle->SPIConfig.SPI_BusConfig == SPI_BUS_CONFIG_FD)
	{
		//BIDI mode should be cleared
		tempreg &= ~( 1 << SPI_CR1_BIDIMODE );
	}else if(pSPIHandle->SPIConfig.SPI_BusConfig == SPI_BUS_CONFIG_HD)
	{
		//BIDI mode should be set
		tempreg |= ( 1 << SPI_CR1_BIDIMODE );
	}else if(pSPIHandle->SPIConfig.SPI_BusConfig == SPI_BUS_CONFIG_SIMPLEX_RXONLY)
	{
		//BIDI mode should be cleared
		tempreg &= ~( 1 << SPI_CR1_BIDIMODE );
		//RXONLY bit must be set
		tempreg |= ( 1 << SPI_CR1_RXONLY );
	}

	//3. configure the SPI serial clock speed (baud rate)
	tempreg |= pSPIHandle->SPIConfig.SPI_SclkSpeed << SPI_CR1_BR;

	//4. configure the DFF
	tempreg |= pSPIHandle->SPIConfig.SPI_DFF << SPI_CR1_DFF;

	//5. configure the CPOL
	tempreg |= pSPIHandle->SPIConfig.SPI_CPOL << SPI_CR1_CPOL;

	//6. configure the CPHA
	tempreg |= pSPIHandle->SPIConfig.SPI_CPHA << SPI_CR1_CPHA;

	//7. configure the SSM
	tempreg |= pSPIHandle->SPIConfig.SPI_SSM << SPI_CR1_SSM;

	pSPIHandle->pSPIx->CR1 = tempreg;

}

/*************************************************************************************************************************
 * @fn 					- SPI_DeInit
 *
 * @brief				- Resets all the registers of the given SPI peripheral to their default state.
 *
 * @param[in]			- pSPIx : Base address of the SPI peripheral (e.g., SPI1, SPI2, SPI3, SPI4).
 *
 * @return				- None.
 *
 * @Note				- Uses RCC peripheral reset macros to reset the hardware logic.
 *
 *************************************************************************************************************************/
void SPI_DeInit(SPI_RegDef_t *pSPIx)
{
	if(pSPIx == SPI1)
	{
		SPI1_REG_RESET();
	}
	else if(pSPIx == SPI2)
	{
		SPI2_REG_RESET();
	}
	else if(pSPIx == SPI3)
	{
		SPI3_REG_RESET();
	}
	else if(pSPIx == SPI4)
	{
		SPI4_REG_RESET();
	}
}

