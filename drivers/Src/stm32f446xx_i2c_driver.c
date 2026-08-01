/*
 * stm32f446xx_i2c_driver.c
 *
 *  Created on: Jul 29, 2026
 *      Author: berkd
 */

#include"stm32f446xx_i2c_driver.h"

uint16_t AHB_PreScaler[8] = {2, 4, 8, 16, 64, 128, 256, 512};
uint16_t APB1_PreScaler[4] = {2, 4, 8, 16};

static uint32_t RCC_GetPLLOutputClock(void);
static uint32_t RCC_GetPLCK1Value(void);

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

/*************************************************************************************************************************
 * @fn 					- I2C_Init
 *
 * @brief				- This function initializes the given I2C peripheral with the user settings.
 *
 * @param[in]			- pI2CHandle : Pointer to the I2C_Handle_t structure containing configuration parameters.
 *
 * @return				- None.
 *
 * @Note				- Enables the I2C peripheral clock automatically inside the function.
 *
 *************************************************************************************************************************/
void I2C_Init(I2C_Handle_t *pI2CHandle)
{
	//peripheral clock enable
	I2C_PeriClockControl(pI2CHandle->pI2Cx, ENABLE);

	uint32_t tempreg = 0;

	//ACK control bit
	tempreg |= (pI2CHandle->I2C_Config.I2C_ACKControl << I2C_CR1_ACK);
	pI2CHandle->pI2Cx->CR1 &= ~(1 << I2C_CR1_ACK);
	pI2CHandle->pI2Cx->CR1 |= tempreg;

	//configure the FREQ field of CR2
	tempreg = 0;
	tempreg = RCC_GetPLCK1Value() / 1000000U;
	pI2CHandle->pI2Cx->CR2 &= ~(0x3F);
	pI2CHandle->pI2Cx->CR2 |= (tempreg & 0x3F);

	//program the device own address
	tempreg = 0;
	tempreg = pI2CHandle->I2C_Config.I2C_DeviceAddress << 1;
	tempreg |= (1 << 14);
	pI2CHandle->pI2Cx->OAR1 = tempreg;

	//CCR calculations
	uint16_t ccr_value = 0;
	tempreg = 0;
	if(pI2CHandle->I2C_Config.I2C_SCLSpeed <= I2C_SCL_SPEED_SM)
	{
		//mode is standard mode
		ccr_value = RCC_GetPLCK1Value() / ( 2 * pI2CHandle->I2C_Config.I2C_SCLSpeed);
		tempreg |= (ccr_value & 0xFFFF);
	}else
	{
		//mode is fast mode
		tempreg |= ( 1 << 15 );
		tempreg |= (pI2CHandle->I2C_Config.I2C_FMDutyCycle << 14);
		if(pI2CHandle->I2C_Config.I2C_FMDutyCycle == I2C_FM_DUTY_2)
		{
			ccr_value = RCC_GetPLCK1Value() / ( 3 * pI2CHandle->I2C_Config.I2C_SCLSpeed);
		}else
		{
			ccr_value = RCC_GetPLCK1Value() / ( 25 * pI2CHandle->I2C_Config.I2C_SCLSpeed);
		}
		tempreg |= (ccr_value & 0xFFF);
	}
	pI2CHandle->pI2Cx->CCR |= tempreg;

	//TRISE Configuration
	tempreg = 0;
	if(pI2CHandle->I2C_Config.I2C_SCLSpeed <= I2C_SCL_SPEED_SM)
	{
		// Standard mode: TRISE = (FREQ + 1)
		tempreg = (RCC_GetPLCK1Value() / 1000000U) + 1;
	}else
	{
		// Fast mode: TRISE = ((PCLK1 * 300ns) / 1s) + 1
		tempreg = ((RCC_GetPLCK1Value() * 300U) / 1000000000U) + 1;
	}
	pI2CHandle->pI2Cx->TRISE = (tempreg & 0x3F);
}

/*************************************************************************************************************************
 * @fn 					- I2C_DeInit
 *
 * @brief				- Resets all the registers of the given I2C peripheral to their default state.
 *
 * @param[in]			- pI2Cx : Base address of the I2C peripheral (e.g., I2C1, I2C2, I2C3, I2C4).
 *
 * @return				- None.
 *
 * @Note				- Uses RCC peripheral reset macros to reset the hardware logic.
 *
 *************************************************************************************************************************/
void I2C_DeInit(I2C_RegDef_t *pI2Cx)
{
	if(pI2Cx == I2C1)
	{
		I2C1_REG_RESET();
	}
	else if(pI2Cx == I2C2)
	{
		I2C2_REG_RESET();
	}
	else if(pI2Cx == I2C3)
	{
		I2C3_REG_RESET();
	}
}

/*
 * Helper function to calculate APB1 Clock frequency
 */
uint32_t RCC_GetPLLOutputClock(void)
{
	//TODO
	return 0;
}

uint32_t RCC_GetPLCK1Value(void)
{
	uint32_t pclk1, SystemClk = 0;
	uint8_t clksrc, temp, ahbp, apb1p;

	clksrc = ((RCC->CFGR >> 2) & 0x3);

	if(clksrc == 0)      { SystemClk = 16000000; } // HSI
	else if(clksrc == 1) { SystemClk = 8000000;  } // HSE
	else if(clksrc == 2) { SystemClk = RCC_GetPLLOutputClock(); }

	// AHB Prescaler
	temp = ((RCC->CFGR >> 4) & 0xF);
	if(temp < 8) { ahbp = 1; }
	else         { ahbp = AHB_PreScaler[temp - 8]; }

	// APB1 Prescaler
	temp = ((RCC->CFGR >> 10) & 0x7);
	if(temp < 4) { apb1p = 1; }
	else         { apb1p = APB1_PreScaler[temp - 4]; }

	pclk1 = (SystemClk / ahbp) / apb1p;

	return pclk1;
}
