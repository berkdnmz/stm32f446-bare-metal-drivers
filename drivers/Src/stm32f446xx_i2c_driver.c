/*
 * stm32f446xx_i2c_driver.c
 *
 *  Created on: Jul 29, 2026
 *      Author: berkd
 */

#include"stm32f446xx_i2c_driver.h"

/*************************************************************************************************************************
 * @fn 					- I2C_PeriClockControl
 *
 * @brief				- This function enables or disables the peripheral clock for the given I2C peripheral.
 *
 * @param[in]			- pI2Cx : Base address of the I2C peripheral (e.g., I2C1, I2C2, I2C3).
 * @param[in]			- EnorDi : ENABLE or DISABLE macro to control the clock.
 *
 * @return				- None.
 *
 * @Note				- None.
 *
 *************************************************************************************************************************/
void I2C_PeriClockControl(I2C_RegDef_t *pI2Cx, uint8_t EnorDi)
{
	if(EnorDi == ENABLE)
	{
		if(pI2Cx == I2C1)
		{
			I2C1_PCLK_EN();
		}else if(pI2Cx == I2C2)
		{
			I2C2_PCLK_EN();
		}else if(pI2Cx == I2C3)
		{
			I2C3_PCLK_EN();
		}
	}
	else
	{
		if (pI2Cx == I2C1)
		{
			I2C1_PCLK_DI();
		}
		else if (pI2Cx == I2C2)
		{
			I2C2_PCLK_DI();
		}
		else if (pI2Cx == I2C3)
		{
			I2C3_PCLK_DI();
		}
	}
}
