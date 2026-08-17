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

/*********************************************************************
 * @fn                - USART_Init
 *
 * @brief             - Initializes the given USART/UART peripheral according to the specified parameters in USART_Config.
 *
 * @param[in]         - pUSARTHandle : Pointer to the USART_Handle_t structure containing configuration info.
 *
 * @return            - None.
 *
 * @Note              - Baud rate calculation will be implemented in the next step.
 *********************************************************************/
void USART_Init(USART_Handle_t *pUSARTHandle)
{
    // Temporary variable
    uint32_t tempreg = 0;

    /******************************** Configuration of CR1 ******************************************/

    // 1. Enable the Clock for given USART peripheral
    USART_PeriClockControl(pUSARTHandle->pUSARTx, ENABLE);

    // 2. Enable USART Tx and Rx engines according to the USART_Mode configuration item
    if (pUSARTHandle->USART_Config.USART_Mode == USART_MODE_ONLY_RX)
    {
        tempreg |= (1 << USART_CR1_RE);
    }
    else if (pUSARTHandle->USART_Config.USART_Mode == USART_MODE_ONLY_TX)
    {
        tempreg |= (1 << USART_CR1_TE);
    }
    else if (pUSARTHandle->USART_Config.USART_Mode == USART_MODE_TXRX)
    {
        tempreg |= ((1 << USART_CR1_RE) | (1 << USART_CR1_TE));
    }

    // 3. Configure the Word length configuration item
    tempreg |= (pUSARTHandle->USART_Config.USART_WordLength << USART_CR1_M);

    // 4. Configuration of parity control bit fields
    if (pUSARTHandle->USART_Config.USART_ParityControl == USART_PARITY_EN_EVEN)
    {
        tempreg |= (1 << USART_CR1_PCE);
        // By default, PS bit is 0 (EVEN parity)
    }
    else if (pUSARTHandle->USART_Config.USART_ParityControl == USART_PARITY_EN_ODD)
    {
        tempreg |= (1 << USART_CR1_PCE);
        tempreg |= (1 << USART_CR1_PS);
    }

    // Program the CR1 register
    pUSARTHandle->pUSARTx->CR1 = tempreg;

    /******************************** Configuration of CR2 ******************************************/

    tempreg = 0;

    // Configure the number of stop bits inserted during USART frame transmission
    tempreg |= (pUSARTHandle->USART_Config.USART_NoOfStopBits << USART_CR2_STOP);

    // Program the CR2 register
    pUSARTHandle->pUSARTx->CR2 = tempreg;

    /******************************** Configuration of CR3 ******************************************/

    tempreg = 0;

    // Configuration of USART hardware flow control
    if (pUSARTHandle->USART_Config.USART_HWFlowControl == USART_HW_FLOW_CTRL_CTS)
    {
        tempreg |= (1 << USART_CR3_CTSE);
    }
    else if (pUSARTHandle->USART_Config.USART_HWFlowControl == USART_HW_FLOW_CTRL_RTS)
    {
        tempreg |= (1 << USART_CR3_RTSE);
    }
    else if (pUSARTHandle->USART_Config.USART_HWFlowControl == USART_HW_FLOW_CTRL_CTS_RTS)
    {
        tempreg |= ((1 << USART_CR3_CTSE) | (1 << USART_CR3_RTSE));
    }

    // Program the CR3 register
    pUSARTHandle->pUSARTx->CR3 = tempreg;

    /******************************** Configuration of BRR (Baudrate register) **************************/

}
