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
		pGPIOHandle->pGPIOx->MODER = temp;
		temp = 0;
	}else
	{
		//this part will code later . ( interrupt mode )
	}
}




void GPIO_DeInit(GPIO_RegDef_t *pGPIOx);

/*
 * Data read and write
 */
uint8_t GPIO_ReadFromInputPin(GPIO_RegDef_t *pGPIOx, uint8_t PinNumber);
uint16_t GPIO_ReadFromInputPort(GPIO_RegDef_t *pGPIOx);
void GPIO_WriteToOutputPin(GPIO_RegDef_t *pGPIOx, uint8_t PinNumber, uint8_t Value);
void GPIO_WriteToOutputPort(GPIO_RegDef_t *pGPIOx, uint16_t Value);
void GPIO_ToggleOutputPin(GPIO_RegDef_t *pGPIOx, uint8_t PinNumber);

/*
 * IRQ Configuration and ISR handling
 */
void GPIO_IRQConfig(uint8_t IRQNumber, uint8_t IRQPriority, uint8_t EnorDi);
void GPIO_IRQHandling(uint8_t PinNumber);

