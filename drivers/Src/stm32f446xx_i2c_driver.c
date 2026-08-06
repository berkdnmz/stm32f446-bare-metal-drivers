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
static void I2C_GenerateStartCondition(I2C_RegDef_t *pI2Cx);
static void I2C_ExecuteAddressPhaseWrite(I2C_RegDef_t *pI2Cx, uint8_t SlaveAddr);
static void I2C_ExecuteAddressPhaseRead(I2C_RegDef_t *pI2Cx, uint8_t SlaveAddr);
static void I2C_ClearADDRFlag(I2C_RegDef_t *pI2Cx);
static void I2C_GenerateStopCondition(I2C_RegDef_t *pI2Cx);

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

/*************************************************************************************************************************
 * @fn 					- I2C_MasterSendData
 *
 * @brief				- Sends data to a slave device in blocking mode.
 *
 * @param[in]			- pI2CHandle : Pointer to I2C_Handle_t structure.
 * @param[in]			- pTxBuffer  : Pointer to user Tx buffer.
 * @param[in]			- Len        : Number of bytes to send.
 * @param[in]			- SlaveAddr  : Target 7-bit slave address.
 *
 * @return				- None.
 *
 *************************************************************************************************************************/
void I2C_MasterSendData(I2C_Handle_t *pI2CHandle, uint8_t *pTxBuffer, uint32_t Len, uint8_t SlaveAddr)
{
	//1. Generate the START condition
	I2C_GenerateStartCondition(pI2CHandle->pI2Cx);

	//2. confirm that start generation is completed by checking the SB flag in the SR1
	//	Note: Until SB is cleared SCL will be stretched (pulled to LOW)
	while( ! I2C_GetFlagStatus(pI2CHandle->pI2Cx, I2C_FLAG_SB)  );

	//3. Send the address of the slave with r/nw bit set to w(0) (total 8 bits)
	I2C_ExecuteAddressPhaseWrite(pI2CHandle->pI2Cx, SlaveAddr);

	//4. Confirm that address phase is completed by checking the ADDR flag in the SR1
	while( ! I2C_GetFlagStatus(pI2CHandle->pI2Cx, I2C_FLAG_ADDR)  );

	//5. Clear the ADDR flag according to its software sequence
	//	Note: Until ADDR is cleared SCL will be stretched (pulled to LOW)
	I2C_ClearADDRFlag(pI2CHandle->pI2Cx);

	//6. send the data until len becomes 0

	while(Len > 0)
	{
		while( ! I2C_GetFlagStatus(pI2CHandle->pI2Cx, I2C_FLAG_TXE) ); //Wait till TXE is set
		pI2CHandle->pI2Cx->DR = *pTxBuffer;
		pTxBuffer++;
		Len--;
	}

	//7. when Len becomes zero wait for TXE=1 and BTF=1 before generating the STOP condition
	//	 Note: TXE=1, BTF=1, means that both SR and DR are empty and next transmission should begin
	//	 when BTF=1 SCL will be stretched (pulled to LOW)

	while( ! I2C_GetFlagStatus(pI2CHandle->pI2Cx, I2C_FLAG_TXE) );

	while( ! I2C_GetFlagStatus(pI2CHandle->pI2Cx, I2C_FLAG_BTF) );


	//8. Generate STOP condition and master need not to wait for the completion of stop condition.
	//	 Note: generating STOp, automatically clears the BTF
	I2C_GenerateStopCondition(pI2CHandle->pI2Cx);
}

/*************************************************************************************************************************
 * @fn 					- I2C_MasterReceiveData
 *
 * @brief				- Receives data from a slave device in blocking mode.
 *
 * @param[in]			- pI2CHandle : Pointer to I2C_Handle_t structure.
 * @param[in]			- pRxBuffer  : Pointer to user Rx buffer.
 * @param[in]			- Len        : Number of bytes to receive.
 * @param[in]			- SlaveAddr  : Target 7-bit slave address.
 *
 * @return				- None.
 *
 *************************************************************************************************************************/
void I2C_MasterReceiveData(I2C_Handle_t *pI2CHandle, uint8_t *pRxBuffer, uint32_t Len, uint8_t SlaveAddr)
{
	//1. Generate the START condition
	I2C_GenerateStartCondition(pI2CHandle->pI2Cx);

	//2. confirm that start generation is completed by checking the SB flag in the SR1
	//	 Note: Until SB is cleared SCL will be stretched (pulled to LOW)
	while( ! I2C_GetFlagStatus(pI2CHandle->pI2Cx, I2C_FLAG_SB)  );

	//3. Sent the address of the slave with r/nw bit set to R(1) (total 8 bits)
	I2C_ExecuteAddressPhaseRead(pI2CHandle->pI2Cx, SlaveAddr);

	//4. wait until address phase is completed by checking the ADDR flag in the SR1
	while( ! I2C_GetFlagStatus(pI2CHandle->pI2Cx, I2C_FLAG_ADDR)  );


	//procedure to read only 1 byte from slave
	if(Len == 1)
	{
		//Disable Acking
		I2C_ManageAcking(pI2CHandle->pI2Cx, I2C_ACK_DISABLE);

		//clear the ADDR flag
		I2C_ClearADDRFlag(pI2CHandle->pI2Cx);

		//generate STOP condition
		I2C_GenerateStopCondition(pI2CHandle->pI2Cx);

		//wait until RXNE becomes 1
		while( ! I2C_GetFlagStatus(pI2CHandle->pI2Cx, I2C_FLAG_RXNE) );

		//read data in to buffer
		*pRxBuffer = pI2CHandle->pI2Cx->DR;

		return;
	}

	//procedure to read data from slave when Len > 1
	if(Len > 1)
	{
		//clear the ADDR flag
		I2C_ClearADDRFlag(pI2CHandle->pI2Cx);

		//read the data until Len becomes zero
		for(uint32_t i = Len; i > 0; i--)
		{
			//wait until RXNE becomes 1
			while( ! I2C_GetFlagStatus(pI2CHandle->pI2Cx, I2C_FLAG_RXNE) );

			if(i == 2) //if last 2 bytes are remaining
			{
				//Disable Acking
				I2C_ManageAcking(pI2CHandle->pI2Cx, I2C_ACK_DISABLE);

				//generate STOP condition
				I2C_GenerateStopCondition(pI2CHandle->pI2Cx);
			}

			//read the data from data register in to buffer
			*pRxBuffer = pI2CHandle->pI2Cx->DR;

			//increment the buffer address
			pRxBuffer++;
		}
	}

	//re-enable ACKing
	if(pI2CHandle->I2C_Config.I2C_ACKControl == I2C_ACK_ENABLE)
	{
		I2C_ManageAcking(pI2CHandle->pI2Cx, I2C_ACK_ENABLE);
	}
}


/*
 * Other Peripheral Control APIs
 */
uint8_t I2C_GetFlagStatus(I2C_RegDef_t *pI2Cx, uint32_t FlagName)
{
	if(pI2Cx->SR1 & FlagName)
	{
		return FLAG_SET;
	}
	return FLAG_RESET;
}

/*************************************************************************************************************************
 * @fn 					- I2C_PeripheralControl
 *
 * @brief				- Enables or disables the given I2C peripheral by toggling the PE bit in CR1.
 *
 * @param[in]			- pI2Cx  : Base address of the I2C peripheral (e.g., I2C1, I2C2, I2C3).
 * @param[in]			- EnorDi : ENABLE or DISABLE macro.
 *
 * @return				- None.
 *
 *************************************************************************************************************************/
void I2C_PeripheralControl(I2C_RegDef_t *pI2Cx, uint8_t EnorDi)
{
	if(EnorDi == ENABLE)
	{
		pI2Cx->CR1 |= (1 << I2C_CR1_PE);
	}
	else
	{
		pI2Cx->CR1 &= ~(1 << I2C_CR1_PE);
	}
}

/*************************************************************************************************************************
 * @fn 					- I2C_ManageAcking
 *
 * @brief				- Enables or disables automatic ACKing for the specified I2C peripheral.
 *
 * @param[in]			- pI2Cx  : Base address of the I2C peripheral (e.g., I2C1, I2C2, I2C3).
 * @param[in]			- EnorDi : I2C_ACK_ENABLE or I2C_ACK_DISABLE (or ENABLE/DISABLE).
 *
 * @return				- None.
 *
 * @note				- Toggles the ACK bit in the I2C_CR1 register.
 *************************************************************************************************************************/
void I2C_ManageAcking(I2C_RegDef_t *pI2Cx, uint8_t EnorDi)
{
	if(EnorDi == I2C_ACK_ENABLE)
	{
		//enable the ack
		pI2Cx->CR1 |= (1 << I2C_CR1_ACK);
	}else
	{
		//disable the ack
		pI2Cx->CR1 &= ~(1 << I2C_CR1_ACK);
	}
}

/*
 * Some helper function implementations
 */
uint32_t RCC_GetPLLOutputClock(void)
{
	//TODO
	return 0;
}

/*************************************************************************************************************************
 * @fn 					- RCC_GetPLCK1Value
 *
 * @brief				- Calculates and returns the APB1 bus clock frequency in Hz.
 *
 * @return				- PCLK1 clock frequency in Hz.
 *
 *************************************************************************************************************************/
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

/*************************************************************************************************************************
 * @fn 					- I2C_GenerateStartCondition
 *
 * @brief				- Generates START condition on the I2C bus.
 *
 * @param[in]			- pI2Cx : Base address of the I2C peripheral.
 *
 * @return				- None.
 *
 *************************************************************************************************************************/
static void I2C_GenerateStartCondition(I2C_RegDef_t *pI2Cx)
{
	pI2Cx->CR1 |= ( 1 << I2C_CR1_START);
}

/*************************************************************************************************************************
 * @fn 					- I2C_ExecuteAddressPhaseWrite
 *
 * @brief				- Sends target slave address with Read/Write bit set to Write (0).
 *
 * @param[in]			- pI2Cx     : Base address of the I2C peripheral.
 * @param[in]			- SlaveAddr : 7-bit slave address.
 *
 * @return				- None.
 *
 *************************************************************************************************************************/
static void I2C_ExecuteAddressPhaseWrite(I2C_RegDef_t *pI2Cx, uint8_t SlaveAddr)
{
	SlaveAddr = SlaveAddr << 1;
	SlaveAddr &= ~( 1 << 0);	//SlaveAddr is Slave address + r/nw bit=0;
	pI2Cx->DR = SlaveAddr;
}

/*************************************************************************************************************************
 * @fn 					- I2C_ExecuteAddressPhaseRead
 *
 * @brief				- Sends target slave address with Read/Write bit set to Read (1).
 *
 * @param[in]			- pI2Cx     : Base address of the I2C peripheral.
 * @param[in]			- SlaveAddr : 7-bit slave address.
 *
 * @return				- None.
 *
 *************************************************************************************************************************/
static void I2C_ExecuteAddressPhaseRead(I2C_RegDef_t *pI2Cx, uint8_t SlaveAddr)
{
	SlaveAddr = SlaveAddr << 1;
	SlaveAddr |= (1 << 0);	//SlaveAddr is Slave address + r/nw bit=1;
	pI2Cx->DR = SlaveAddr;
}

/*************************************************************************************************************************
 * @fn 					- I2C_ClearADDRFlag
 *
 * @brief				- Clears ADDR flag by reading SR1 followed by SR2 register.
 *
 * @param[in]			- pI2Cx : Base address of the I2C peripheral.
 *
 * @return				- None.
 *
 *************************************************************************************************************************/
static void I2C_ClearADDRFlag(I2C_RegDef_t *pI2Cx)
{
	uint32_t dummyRead = pI2Cx->SR1;
	dummyRead = pI2Cx->SR2;
	(void)dummyRead;
}

/*************************************************************************************************************************
 * @fn 					- I2C_GenerateStopCondition
 *
 * @brief				- Generates STOP condition on the I2C bus.
 *
 * @param[in]			- pI2Cx : Base address of the I2C peripheral.
 *
 * @return				- None.
 *
 *************************************************************************************************************************/
static void I2C_GenerateStopCondition(I2C_RegDef_t *pI2Cx)
{
	pI2Cx->CR1 |= ( 1 << I2C_CR1_STOP);
}
