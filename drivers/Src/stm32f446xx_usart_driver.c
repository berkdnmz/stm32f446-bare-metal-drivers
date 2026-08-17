/*
 * stm32f446xx_usart_driver.c
 *
 *  Created on: Aug 17, 2026
 *      Author: berkd
 */

#include "stm32f446xx_usart_driver.h"

#include "stm32f446xx_usart_driver.h"

/*************************************************************************************************************************
 * @fn                  - USART_PeriClockControl
 *
 * @brief               - This function enables or disables the peripheral clock for the given USART/UART peripheral.
 *
 * @param[in]           - pUSARTx : Base address of the USART/UART peripheral (e.g., USART1, USART2, USART3, UART4, UART5, USART6).
 * @param[in]           - EnorDi  : ENABLE or DISABLE macro to control the clock.
 *
 * @return              - None.
 *
 * @Note                - None.
 *
 *************************************************************************************************************************/
void USART_PeriClockControl(USART_RegDef_t *pUSARTx, uint8_t EnorDi)
{
    if(EnorDi == ENABLE)
    {
        if(pUSARTx == USART1)
        {
            USART1_PCLK_EN();
        }
        else if(pUSARTx == USART2)
        {
            USART2_PCLK_EN();
        }
        else if(pUSARTx == USART3)
        {
            USART3_PCLK_EN();
        }
        else if(pUSARTx == UART4)
        {
            UART4_PCLK_EN();
        }
        else if(pUSARTx == UART5)
        {
            UART5_PCLK_EN();
        }
        else if(pUSARTx == USART6)
        {
            USART6_PCLK_EN();
        }
    }
    else
    {
        if(pUSARTx == USART1)
        {
            USART1_PCLK_DI();
        }
        else if(pUSARTx == USART2)
        {
            USART2_PCLK_DI();
        }
        else if(pUSARTx == USART3)
        {
            USART3_PCLK_DI();
        }
        else if(pUSARTx == UART4)
        {
            UART4_PCLK_DI();
        }
        else if(pUSARTx == UART5)
        {
            UART5_PCLK_DI();
        }
        else if(pUSARTx == USART6)
        {
            USART6_PCLK_DI();
        }
    }
}
