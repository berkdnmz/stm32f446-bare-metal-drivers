/*
 * stm32f446xx_spi_driver.c
 *
 *  Created on: Jul 22, 2026
 *      Author: berkd
 */


#include "stm32f446xx_spi_driver.h"


/**********************************************************************************************
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
 ***********************************************************************************************/
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

