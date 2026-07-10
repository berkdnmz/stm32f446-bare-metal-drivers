/*
 * stm32f446xx_gpio_driver.c
 *
 *  Created on: Jul 6, 2026
 *      Author: berkd
 */

#include "stm32f446xx_gpio_driver.h"


/**********************************************************************************************
 * @fn 					- GPIO_PeriClockControl
 *
 * @brief				- This function controls the peripherals clock for the given GPIO port.
 *
 * @param[in]			- pGPIOx : GPIO port to be configured.
 * @param[in]			- EnorDi : Enable or disable the peripherals clock.
 *
 * @return				- None.
 *
 * @Note				- The peripheral clock must be enabled before calling GPIO_Init()
 *
 ***********************************************************************************************/
void GPIO_PeriClockControl(GPIO_RegDef_t *pGPIOx, uint8_t EnorDi)
{
	if(EnorDi == ENABLE)
	{
		if(pGPIOx == GPIOA)
		{
			GPIOA_PCLK_EN();
		}else if(pGPIOx == GPIOB)
		{
			GPIOB_PCLK_EN();
		}else if(pGPIOx == GPIOC)
		{
			GPIOC_PCLK_EN();
		}else if(pGPIOx == GPIOD)
		{
			GPIOD_PCLK_EN();
		}else if(pGPIOx == GPIOE)
		{
			GPIOE_PCLK_EN();
		}else if(pGPIOx == GPIOF)
		{
			GPIOF_PCLK_EN();
		}else if(pGPIOx == GPIOG)
		{
			GPIOG_PCLK_EN();
		}else if(pGPIOx == GPIOH)
		{
			GPIOH_PCLK_EN();
		}
		else
		{
			//TODO
		}
	}
}

/***********************************************************************************************
 * @fn 					- GPIO_Init
 *
 * @brief				- Configures the selected GPIO pin according to the GPIO handle configuration.
 *
 * @param[in]			- pGPIOHandle : Pointer to GPIO handle structure.
 *
 * @return				- None.
 *
 * @Note				- Enable the peripheral clock before calling this function
 *
 ************************************************************************************************/
void GPIO_Init(GPIO_Handle_t *pGPIOHandle)
{
	uint32_t temp=0; //temp register

	//1. configure the mode of gpio pin

	if(pGPIOHandle->GPIO_PinConfig.GPIO_PinMode <= GPIO_MODE_ANALOG)
	{
		//the non interrupt mode
		temp = (pGPIOHandle->GPIO_PinConfig.GPIO_PinMode << (2 * pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber ) );
		pGPIOHandle->pGPIOx->MODER &= ~( 0x3 << ( 2 * pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber ) ); //clearing
		pGPIOHandle->pGPIOx->MODER |= temp; //setting

	}else
	{
		//this part will code later . ( interrupt mode )
	}

	//2. configure the speed
	temp = (pGPIOHandle->GPIO_PinConfig.GPIO_PinSpeed << ( 2 * pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber ) );
	pGPIOHandle->pGPIOx->OSPEEDR &= ~( 0x3 << ( 2 * pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber ) ); //clearing
	pGPIOHandle->pGPIOx->OSPEEDR |= temp; //setting

	//3. configure the pupd settings
	temp = (pGPIOHandle->GPIO_PinConfig.GPIO_PinPuPdControl << ( 2 * pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber ) );
	pGPIOHandle->pGPIOx->PUPDR &= ~( 0x3 << ( 2 * pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber ) ); //clearing
	pGPIOHandle->pGPIOx->PUPDR |= temp; //setting

	//4. configure the optype
	temp = (pGPIOHandle->GPIO_PinConfig.GPIO_PinOPType << (pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber ) );
	pGPIOHandle->pGPIOx->OTYPER &= ~( 0x1 << (pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber ) ); //clearing
	pGPIOHandle->pGPIOx->OTYPER |= temp; //setting

	//5. configure the alt functionality
	if(pGPIOHandle->GPIO_PinConfig.GPIO_PinMode == GPIO_MODE_ALTFN)
	{
		//configure the alt function registers.
		uint8_t temp1, temp2;

		temp1 = pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber / 8;
		temp2 = pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber % 8;
		pGPIOHandle->pGPIOx->AFR[temp1] &= ~( 0xF << ( 4 * temp2 ) ); //clearing
		pGPIOHandle->pGPIOx->AFR[temp1] |= (pGPIOHandle->GPIO_PinConfig.GPIO_PinAltFunMode << (4 * temp2 ) ); //setting
	}

}

/***********************************************************************************************
 * @fn 					- GPIO_DeInit
 *
 * @brief				- Resets all registers of the given GPIO port to their default state.
 *
 * @param[in]			- pGPIOx 	: Base address of the GPIO peripheral
 *
 * @return				- None.
 *
 * @Note				- This function triggers the peripherals reset using the using the RCC_AHB1RSTR
 *
 ************************************************************************************************/
void GPIO_DeInit(GPIO_RegDef_t *pGPIOx)
{
	if(pGPIOx == GPIOA)
	{
		GPIOA_REG_RESET();
	}else if(pGPIOx == GPIOB)
	{
		GPIOB_REG_RESET();
	}else if(pGPIOx == GPIOC)
	{
		GPIOC_REG_RESET();
	}else if(pGPIOx == GPIOD)
	{
		GPIOD_REG_RESET();
	}else if(pGPIOx == GPIOE)
	{
		GPIOE_REG_RESET();
	}else if(pGPIOx == GPIOF)
	{
		GPIOF_REG_RESET();
	}else if(pGPIOx == GPIOG)
	{
		GPIOG_REG_RESET();
		}else if(pGPIOx == GPIOH)
	{
		GPIOH_REG_RESET();
	}
	else
	{
		//TODO
	}
}

/***********************************************************************************************
 * @fn 					- GPIO_ReadFromInputPin
 *
 * @brief				- Reads the value of a specific pin from the input data register
 *
 * @param[in]			- pGPIOx 	: Base address of the GPIO peripheral
 * @param[in]			- PinNumber : The pin number to be read
 *
 * @return				- The state of the pin (0 or 1)
 *
 * @Note				- None
 *
 ************************************************************************************************/
uint8_t GPIO_ReadFromInputPin(GPIO_RegDef_t *pGPIOx, uint8_t PinNumber)
{
	uint8_t value;

	value = (uint8_t)((pGPIOx->IDR >> PinNumber) & 0x00000001 );

	return value;
}

/***********************************************************************************************
 * @fn 					- GPIO_ReadFromInputPort
 *
 * @brief				- Reads the entire input data register of the given GPIO port
 *
 * @param[in]			- pGPIOx 	: Base address of the GPIO peripheral
 *
 * @return				- The current state of the entire port (16 bits)
 *
 * @Note				- None
 *
 ************************************************************************************************/
uint16_t GPIO_ReadFromInputPort(GPIO_RegDef_t *pGPIOx)
{
	uint16_t value;

	value = (uint16_t)pGPIOx->IDR;

	return value;
}

/***********************************************************************************************
 * @fn 					- GPIO_WriteToOutputPin
 *
 * @brief				- Writes a specific value (SET or RESET) to the given output pin
 *
 * @param[in]			- pGPIOx     : Base address of the GPIO peripheral
 * @param[in]			- PinNumber  : The pin number to be written
 * @param[in]			- Value      : Value to be written (GPIO_PIN_SET or GPIO_PIN_RESET)
 *
 * @return				- None
 *
 * @Note				- Ensure the pin is configured as output before calling this function
 *
 ************************************************************************************************/
void GPIO_WriteToOutputPin(GPIO_RegDef_t *pGPIOx, uint8_t PinNumber, uint8_t Value)
{
	if(Value == GPIO_PIN_SET)
	{
		//write 1 to the output data register at the bit field corresponding to the pin number
		pGPIOx->ODR |= ( 1 << PinNumber);
	}else
	{
		//write 0
		pGPIOx->ODR &= ~( 1 << PinNumber);
	}
}

/***********************************************************************************************
 * @fn 					- GPIO_WriteToOutputPort
 *
 * @brief				- Writes a 16-bit value to the entire output data register of the port
 *
 * @param[in]			- pGPIOx     : Base address of the GPIO peripheral
 * @param[in]			- Value      : 16-bit value to be written to the ODR
 *
 * @return				- None.
 *
 * @Note				- This function overwrites the entire port output data register
 *
 ************************************************************************************************/
void GPIO_WriteToOutputPort(GPIO_RegDef_t *pGPIOx, uint16_t Value)
{
	pGPIOx->ODR = Value;
}

/***********************************************************************************************
 * @fn 					- GPIO_ToggleOutputPin
 *
 * @brief				- Toggles the state of a specific output pin.
 *
 * @param[in]			- pGPIOx     : Base address of the GPIO peripheral.
 * @param[in]			- PinNumber  : The pin number to be toggled.
 *
 * @return				- None.
 *
 * @Note				- This function performs an XOR operation on the output data register bit.
 *
 ************************************************************************************************/
void GPIO_ToggleOutputPin(GPIO_RegDef_t *pGPIOx, uint8_t PinNumber)
{
	pGPIOx->ODR ^= ( 1 << PinNumber);
}

/*
 * IRQ Configuration and ISR handling
 */
void GPIO_IRQConfig(uint8_t IRQNumber, uint8_t IRQPriority, uint8_t EnorDi);
void GPIO_IRQHandling(uint8_t PinNumber);

