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

/*********************************************************************
 * @fn                 - USART_DeInit
 *
 * @brief              - Resets the specified USART peripheral registers using RCC reset registers
 *
 * @param[in]          - pUSARTx : Base address of the USART peripheral (USART1, USART2, etc.)
 *
 * @return             - None
 *
 * @Note               - Sets and clears the corresponding reset bit in RCC APBxRSTR registers
 */
void USART_DeInit(USART_RegDef_t *pUSARTx)
{
	if(pUSARTx == USART1)
	{
		USART1_REG_RESET();
	}
	else if(pUSARTx == USART2)
	{
		USART2_REG_RESET();
	}
	else if(pUSARTx == USART3)
	{
		USART3_REG_RESET();
	}
	else if(pUSARTx == UART4)
	{
		UART4_REG_RESET();
	}
	else if(pUSARTx == UART5)
	{
		UART5_REG_RESET();
	}
	else if(pUSARTx == USART6)
	{
		USART6_REG_RESET();
	}
}

/*********************************************************************
 * @fn                - USART_SendData
 *
 * @brief             - Sends data over USART in blocking/polling mode
 *
 * @param[in]         - pUSARTHandle : Pointer to USART_Handle_t structure
 * @param[in]         - pTxBuffer    : Pointer to user transmission data buffer
 * @param[in]         - Len          : Number of bytes to transfer
 *
 * @return            - None
 *********************************************************************/
void USART_SendData(USART_Handle_t *pUSARTHandle, uint8_t *pTxBuffer, uint32_t Len)
{
    uint16_t *pdata;

    // Loop over until "Len" number of bytes are transferred
    for(uint32_t i = 0 ; i < Len; i++)
    {
        // 1. Wait until TXE flag is set in the SR
        while(! USART_GetFlagStatus(pUSARTHandle->pUSARTx, USART_FLAG_TXE));

        // 2. Check the USART_WordLength item for 9BIT or 8BIT in a frame
        if(pUSARTHandle->USART_Config.USART_WordLength == USART_WORDLEN_9BITS)
        {
            // If 9BIT, load the DR with 2 bytes masking the bits other than first 9 bits
            pdata = (uint16_t*) pTxBuffer;
            pUSARTHandle->pUSARTx->DR = (*pdata & (uint16_t)0x01FF);

            // Check for USART_ParityControl
            if(pUSARTHandle->USART_Config.USART_ParityControl == USART_PARITY_DISABLE)
            {
                // No parity: 9 bits of user data -> increment pointer twice
                pTxBuffer++;
                pTxBuffer++;
            }
            else
            {
                // Parity used: 8 bits user data + 1 bit HW parity
                pTxBuffer++;
            }
        }
        else
        {
            // This is 8-bit data transfer
            pUSARTHandle->pUSARTx->DR = (*pTxBuffer & (uint8_t)0xFF);

            // Increment the buffer address
            pTxBuffer++;
        }
    }

    // 3. Wait till TC flag is set in the SR before exiting
    while(! USART_GetFlagStatus(pUSARTHandle->pUSARTx, USART_FLAG_TC));
}

/*********************************************************************
 * @fn                - USART_ReceiveData
 *
 * @brief             - Receives data over USART in blocking/polling mode
 *
 * @param[in]         - pUSARTHandle : Pointer to USART_Handle_t structure
 * @param[in]         - pRxBuffer    : Pointer to user reception buffer
 * @param[in]         - Len          : Number of bytes to receive
 *
 * @return            - None
 *********************************************************************/
void USART_ReceiveData(USART_Handle_t *pUSARTHandle, uint8_t *pRxBuffer, uint32_t Len)
{
    // Loop over until "Len" number of bytes are transferred
    for(uint32_t i = 0 ; i < Len; i++)
    {
        // 1. Wait until RXNE flag is set in the SR
        while(! USART_GetFlagStatus(pUSARTHandle->pUSARTx, USART_FLAG_RXNE));

        // 2. Check the USART_WordLength (9BIT or 8BIT)
        if(pUSARTHandle->USART_Config.USART_WordLength == USART_WORDLEN_9BITS)
        {
            // We are going to receive 9bit data in a frame
            if(pUSARTHandle->USART_Config.USART_ParityControl == USART_PARITY_DISABLE)
            {
                // No parity: all 9 bits are user data
                *((uint16_t*) pRxBuffer) = (pUSARTHandle->pUSARTx->DR & (uint16_t)0x01FF);

                // Increment pointer twice
                pRxBuffer++;
                pRxBuffer++;
            }
            else
            {
                // Parity used: 8 bits user data, 1 bit parity
                *pRxBuffer = (pUSARTHandle->pUSARTx->DR & (uint8_t)0xFF);
                pRxBuffer++;
            }
        }
        else
        {
            // We are going to receive 8bit data in a frame
            if(pUSARTHandle->USART_Config.USART_ParityControl == USART_PARITY_DISABLE)
            {
                // No parity: all 8 bits are user data
                *pRxBuffer = (uint8_t)(pUSARTHandle->pUSARTx->DR & (uint8_t)0xFF);
            }
            else
            {
                // Parity used: 7 bits user data + 1 bit parity
                *pRxBuffer = (uint8_t)(pUSARTHandle->pUSARTx->DR & (uint8_t)0x7F);
            }

            // Increment the buffer address
            pRxBuffer++;
        }
    }
}

/*********************************************************************
 * @fn                - USART_SendDataIT
 *
 * @brief             - Initiates non-blocking transmission over USART using interrupts
 *
 * @param[in]         - pUSARTHandle : Pointer to USART_Handle_t structure
 * @param[in]         - pTxBuffer    : Pointer to user transmission data buffer
 * @param[in]         - Len          : Number of bytes to transfer
 *
 * @return            - Current transmission state (USART_READY or USART_BUSY_IN_TX)
 *********************************************************************/
uint8_t USART_SendDataIT(USART_Handle_t *pUSARTHandle, uint8_t *pTxBuffer, uint32_t Len)
{
    uint8_t txstate = pUSARTHandle->TxBusyState;

    if(txstate != USART_BUSY_IN_TX)
    {
        pUSARTHandle->TxLen = Len;
        pUSARTHandle->pTxBuffer = pTxBuffer;
        pUSARTHandle->TxBusyState = USART_BUSY_IN_TX;

        // Implement the code to enable interrupt for TXE (CR1 -> TXEIE)
        pUSARTHandle->pUSARTx->CR1 |= (1 << USART_CR1_TXEIE);

        // Implement the code to enable interrupt for TC (CR1 -> TCIE)
        pUSARTHandle->pUSARTx->CR1 |= (1 << USART_CR1_TCIE);
    }

    return txstate;
}

/*********************************************************************
 * @fn                - USART_ReceiveDataIT
 *
 * @brief             - Initiates non-blocking reception over USART using interrupts
 *
 * @param[in]         - pUSARTHandle : Pointer to USART_Handle_t structure
 * @param[in]         - pRxBuffer    : Pointer to user reception buffer
 * @param[in]         - Len          : Number of bytes to receive
 *
 * @return            - Current reception state (USART_READY or USART_BUSY_IN_RX)
 *********************************************************************/
uint8_t USART_ReceiveDataIT(USART_Handle_t *pUSARTHandle, uint8_t *pRxBuffer, uint32_t Len)
{
    uint8_t rxstate = pUSARTHandle->RxBusyState;

    if(rxstate != USART_BUSY_IN_RX)
    {
        pUSARTHandle->RxLen = Len;
        pUSARTHandle->pRxBuffer = pRxBuffer;
        pUSARTHandle->RxBusyState = USART_BUSY_IN_RX;

        // Implement the code to enable interrupt for RXNE (CR1 -> RXNEIE)
        pUSARTHandle->pUSARTx->CR1 |= (1 << USART_CR1_RXNEIE);
    }

    return rxstate;
}

/*
 * Other Peripheral Control APIs
 */
/*********************************************************************
 * @fn      		  - USART_GetFlagStatus
 *
 * @brief             - Returns the status of the specified USART SR flag
 *
 * @param[in]         - pUSARTx : Base address of the USART peripheral
 * @param[in]         - FlagName : Flag to check (USART_FLAG_TXE, USART_FLAG_RXNE, etc.)
 *
 * @return            - FLAG_SET or FLAG_RESET
 *********************************************************************/
uint8_t USART_GetFlagStatus(USART_RegDef_t *pUSARTx, uint32_t FlagName)
{
	if(pUSARTx->SR & FlagName)
	{
		return FLAG_SET;
	}
	return FLAG_RESET;
}
