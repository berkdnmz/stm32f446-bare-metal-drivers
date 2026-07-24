/*
 * stm32f446xx.h
 *
 *  Created on: May 7, 2026
 *      Author: berkd
 */

#ifndef INC_STM32F446XX_H_
#define INC_STM32F446XX_H_

#define __vo volatile

#include <stdint.h>

/*****************************************START:Processor Specific Details*****************************************/
/*
 * ARM Cortex M4 Processor NVIC ISERx register Addresses
 */
#define NVIC_ISER0						((__vo uint32_t*)0xE000E100UL)
#define NVIC_ISER1						((__vo uint32_t*)0xE000E104UL)
#define NVIC_ISER2						((__vo uint32_t*)0xE000E108UL)
#define NVIC_ISER3						((__vo uint32_t*)0xE000E10CUL)
#define NVIC_ISER4						((__vo uint32_t*)0xE000E110UL)
#define NVIC_ISER5						((__vo uint32_t*)0xE000E114UL)
#define NVIC_ISER6						((__vo uint32_t*)0xE000E118UL)
#define NVIC_ISER7						((__vo uint32_t*)0xE000E11CUL)

/*
 * ARM Cortex M4 Processor NVIC ICERx register Addresses
 */
#define NVIC_ICER0						((__vo uint32_t*)0xE000E180UL)
#define NVIC_ICER1						((__vo uint32_t*)0xE000E184UL)
#define NVIC_ICER2						((__vo uint32_t*)0xE000E188UL)
#define NVIC_ICER3						((__vo uint32_t*)0xE000E18CUL)
#define NVIC_ICER4						((__vo uint32_t*)0xE000E190UL)
#define NVIC_ICER5						((__vo uint32_t*)0xE000E194UL)
#define NVIC_ICER6						((__vo uint32_t*)0xE000E198UL)
#define NVIC_ICER7						((__vo uint32_t*)0xE000E19CUL)

/*
 * ARM Cortex M4 Processor Priority Register Address Calculation
 */
#define NVIC_PR_BASE_ADDR				((__vo uint32_t*)0xE000E400UL)

/*
 * ARM Cortex M4 Processor number of priority bits implemented in Priority Register
 */
#define NO_PR_BITS_IMPLEMENTED			4

/*
 * base addresses of Flash and SRAM memories
 */

#define FLASH_BASEADDR					0x08000000UL
#define SRAM1_BASEADDR					0X20000000UL
#define SRAM2_BASEADDR					0x2001C000UL
#define ROM								0x1FFF0000UL
#define SRAM							SRAM1_BASEADDR


/*
 * AHBx and APBx Peripheral base addresses
 */

#define	PERIPH_BASE						0x40000000UL
#define APB1PERIPH_BASE					PERIPH_BASE
#define APB2PERIPH_BASE					0x40010000UL
#define AHB1PERIPH_BASE					0x40020000UL
#define AHB2PERIPH_BASE					0x50000000UL
#define AHB3PERIPH_BASE					0x60000000UL


/*
 * Base addresses of peripherals which are hanging on AHB2 bus
 */

#define OTG_FS_BASEADDR					(AHB2PERIPH_BASE + 0x0000UL)
#define DCMI_BASEADDR					(AHB2PERIPH_BASE + 0x50000UL)

/*
 * Base addresses of peripherals which are hanging on AHB1 bus
 */

#define GPIOA_BASEADDR					(AHB1PERIPH_BASE + 0x0000UL)
#define GPIOB_BASEADDR					(AHB1PERIPH_BASE + 0x0400UL)
#define GPIOC_BASEADDR					(AHB1PERIPH_BASE + 0x0800UL)
#define GPIOD_BASEADDR					(AHB1PERIPH_BASE + 0x0C00UL)
#define GPIOE_BASEADDR					(AHB1PERIPH_BASE + 0x1000UL)
#define GPIOF_BASEADDR					(AHB1PERIPH_BASE + 0x1400UL)
#define GPIOG_BASEADDR					(AHB1PERIPH_BASE + 0x1800UL)
#define GPIOH_BASEADDR					(AHB1PERIPH_BASE + 0x1C00UL)
#define CRC_BASEADDR 					(AHB1PERIPH_BASE + 0x3000UL)
#define RCC_BASEADDR					(AHB1PERIPH_BASE + 0x3800UL)
#define FLASH_INTERFACE_REG_BASEADDR	(AHB1PERIPH_BASE + 0x3C00UL)
#define BKPSRAM_BASEADDR				(AHB1PERIPH_BASE + 0x4000UL)
#define DMA1_BASEADDR					(AHB1PERIPH_BASE + 0x6000UL)
#define DMA2_BASEADDR					(AHB1PERIPH_BASE + 0x6400UL)
#define OTG_HS_BASEADDR					(AHB1PERIPH_BASE + 0x20000UL)

/*
 * Base addresses of peripherals which are hanging on APB2 bus
 */

#define EXTI_BASEADDR					(APB2PERIPH_BASE + 0x3C00UL)
#define SYSCFG_BASEADDR					(APB2PERIPH_BASE + 0x3800UL)
#define SPI4_BASEADDR					(APB2PERIPH_BASE + 0x3400UL)
#define SPI1_BASEADDR					(APB2PERIPH_BASE + 0x3000UL)
#define USART6_BASEADDR					(APB2PERIPH_BASE + 0x1400UL)
#define USART1_BASEADDR					(APB2PERIPH_BASE + 0x1000UL)

/*
 * Base addresses of peripherals which are hanging on APB1 bus
 */

#define I2C3_BASEADDR					(APB1PERIPH_BASE + 0x5C00UL)
#define I2C2_BASEADDR					(APB1PERIPH_BASE + 0x5800UL)
#define I2C1_BASEADDR					(APB1PERIPH_BASE + 0x5400UL)
#define UART5_BASEADDR					(APB1PERIPH_BASE + 0x5000UL)
#define UART4_BASEADDR					(APB1PERIPH_BASE + 0x4C00UL)
#define USART3_BASEADDR					(APB1PERIPH_BASE + 0x4800UL)
#define USART2_BASEADDR					(APB1PERIPH_BASE + 0x4400UL)
#define SPI3_BASEADDR					(APB1PERIPH_BASE + 0x3C00UL)
#define SPI2_BASEADDR					(APB1PERIPH_BASE + 0x3800UL)



/*
 * Peripheral register definition structures
 */

typedef struct
{
	__vo uint32_t MODER;
	__vo uint32_t OTYPER;
	__vo uint32_t OSPEEDR;
	__vo uint32_t PUPDR;
	__vo uint32_t IDR;
	__vo uint32_t ODR;
	__vo uint32_t BSRR;
	__vo uint32_t LCKR;
	__vo uint32_t AFR[2];

}GPIO_RegDef_t;

typedef struct
{
	__vo uint32_t CR;
	__vo uint32_t PLLCFGR;
	__vo uint32_t CFGR;
	__vo uint32_t CIR;
	__vo uint32_t AHB1RSTR;
	__vo uint32_t AHB2RSTR;
	__vo uint32_t AHB3RSTR;
	uint32_t	  RESERVED0;
	__vo uint32_t APB1RSTR;
	__vo uint32_t APB2RSTR;
	uint32_t	  RESERVED1[2];
	__vo uint32_t AHB1ENR;
	__vo uint32_t AHB2ENR;
	__vo uint32_t AHB3ENR;
	uint32_t	  RESERVED2;
	__vo uint32_t APB1ENR;
	__vo uint32_t APB2ENR;
	uint32_t	  RESERVED3[2];
	__vo uint32_t AHB1LPENR;
	__vo uint32_t AHB2LPENR;
	__vo uint32_t AHB3LPENR;
	uint32_t	  RESERVED4;
	__vo uint32_t APB1LPENR;
	__vo uint32_t APB2LPENR;
	uint32_t      RESERVED5[2];
	__vo uint32_t BDCR;
	__vo uint32_t CSR;
	uint32_t      RESERVED6[2];
	__vo uint32_t SSCGR;
	__vo uint32_t PLLI2SCFGR;
	__vo uint32_t PLLSAICFGR;
	__vo uint32_t DCKCFGR;
	__vo uint32_t CKGATENR;
	__vo uint32_t DCKCFGR2;
}RCC_RegDef_t;


/*
 * Peripheral register definition structure for EXTI
 */
typedef struct
{
	__vo uint32_t IMR;
	__vo uint32_t EMR;
	__vo uint32_t RTSR;
	__vo uint32_t FTSR;
	__vo uint32_t SWIER;
	__vo uint32_t PR;
}EXTI_RegDef_t;

/*
 * Peripheral register definition structure for SPI
 */
typedef struct
{
	__vo uint32_t CR1;
	__vo uint32_t CR2;
	__vo uint32_t SR;
	__vo uint32_t DR;
	__vo uint32_t CRCPR;
	__vo uint32_t RXCRCR;
	__vo uint32_t TXCRCR;
	__vo uint32_t I2SCFGR;
	__vo uint32_t I2SPR;
}SPI_RegDef_t;

/*
 * Peripheral register definition structure for SYSCFG
 */
typedef struct
{
	__vo uint32_t MEMRMP;
	__vo uint32_t PMC;
	__vo uint32_t EXTICR[4];
	uint32_t RESERVED1[2];
	__vo uint32_t CMPCR;
	uint32_t RESERVED2[2];
	__vo uint32_t CFGR;
}SYSCFG_RegDef_t;

/*
 * peripheral definitions
 */

#define GPIOA 							((GPIO_RegDef_t*)GPIOA_BASEADDR)
#define GPIOB 							((GPIO_RegDef_t*)GPIOB_BASEADDR)
#define GPIOC 							((GPIO_RegDef_t*)GPIOC_BASEADDR)
#define GPIOD 							((GPIO_RegDef_t*)GPIOD_BASEADDR)
#define GPIOE 							((GPIO_RegDef_t*)GPIOE_BASEADDR)
#define GPIOF 							((GPIO_RegDef_t*)GPIOF_BASEADDR)
#define GPIOG 							((GPIO_RegDef_t*)GPIOG_BASEADDR)
#define GPIOH 							((GPIO_RegDef_t*)GPIOH_BASEADDR)

#define RCC 							((RCC_RegDef_t*)RCC_BASEADDR)

#define EXTI							((EXTI_RegDef_t*)EXTI_BASEADDR)

#define SYSCFG							((SYSCFG_RegDef_t*)SYSCFG_BASEADDR)

#define SPI1							((SPI_RegDef_t*)SPI1_BASEADDR)
#define SPI2							((SPI_RegDef_t*)SPI2_BASEADDR)
#define SPI3							((SPI_RegDef_t*)SPI3_BASEADDR)
#define SPI4							((SPI_RegDef_t*)SPI4_BASEADDR)
/*
 * Clock Enable Macros for GPIOx peripherals
 */

#define GPIOA_PCLK_EN()					( RCC->AHB1ENR |= ( 1 << 0 ) )
#define GPIOB_PCLK_EN()				 	( RCC->AHB1ENR |= ( 1 << 1 ) )
#define GPIOC_PCLK_EN()					( RCC->AHB1ENR |= ( 1 << 2 ) )
#define GPIOD_PCLK_EN()					( RCC->AHB1ENR |= ( 1 << 3 ) )
#define GPIOE_PCLK_EN()					( RCC->AHB1ENR |= ( 1 << 4 ) )
#define GPIOF_PCLK_EN()		 			( RCC->AHB1ENR |= ( 1 << 5 ) )
#define GPIOG_PCLK_EN()		 			( RCC->AHB1ENR |= ( 1 << 6 ) )
#define GPIOH_PCLK_EN()		 			( RCC->AHB1ENR |= ( 1 << 7 ) )

/*
 * Clock Enable Macros for I2Cx peripherals
 */

#define I2C1_PCLK_EN()			 		( RCC->APB1ENR |= ( 1 << 21 ))
#define I2C2_PCLK_EN()			 		( RCC->APB1ENR |= ( 1 << 22 ))
#define I2C3_PCLK_EN()			 		( RCC->APB1ENR |= ( 1 << 23 ))

/*
 * Clock Enable Macros for SPIx peripherals
 */

#define SPI1_PCLK_EN()		 	 		( RCC->APB2ENR |= ( 1 << 12 ) )
#define SPI2_PCLK_EN()		 	 		( RCC->APB1ENR |= ( 1 << 14 ) )
#define SPI3_PCLK_EN()		 	 		( RCC->APB1ENR |= ( 1 << 15 ) )
#define SPI4_PCLK_EN()		 	 		( RCC->APB2ENR |= ( 1 << 13 ) )

/*
 * Clock Enable Macros for USARTx peripherals
 */

#define USART1_PCLK_EN()		 	 	( RCC->APB2ENR |= ( 1 << 4 ) )
#define USART2_PCLK_EN()		 	 	( RCC->APB1ENR |= ( 1 << 17 ) )
#define USART3_PCLK_EN()		 	 	( RCC->APB1ENR |= ( 1 << 18 ) )
#define UART4_PCLK_EN()		 	 		( RCC->APB1ENR |= ( 1 << 19 ) )
#define UART5_PCLK_EN()		 	 		( RCC->APB1ENR |= ( 1 << 20 ) )
#define USART6_PCLK_EN()		 	 	( RCC->APB2ENR |= ( 1 << 5 ) )

/*
 * Clock Enable Macros for SYSCFG peripherals
 */

#define SYSCFG_PCLK_EN()		 	 	( RCC->APB2ENR |= ( 1 << 14 ) )

/*
 * Clock Disable Macros for GPIOx peripherals
 */

#define GPIOA_PCLK_DI()		 			( RCC->AHB1ENR &= ~( 1 << 0 ) )
#define GPIOB_PCLK_DI()		 			( RCC->AHB1ENR &= ~( 1 << 1 ) )
#define GPIOC_PCLK_DI()		 			( RCC->AHB1ENR &= ~( 1 << 2 ) )
#define GPIOD_PCLK_DI()		 			( RCC->AHB1ENR &= ~( 1 << 3 ) )
#define GPIOE_PCLK_DI()		 			( RCC->AHB1ENR &= ~( 1 << 4 ) )
#define GPIOF_PCLK_DI()		 			( RCC->AHB1ENR &= ~( 1 << 5 ) )
#define GPIOG_PCLK_DI()		 			( RCC->AHB1ENR &= ~( 1 << 6 ) )
#define GPIOH_PCLK_DI()		 			( RCC->AHB1ENR &= ~( 1 << 7 ) )

/*
 * Clock Disable Macros for I2Cx peripherals
 */

#define I2C1_PCLK_DI()			 		( RCC->APB1ENR &= ~( 1 << 21 ))
#define I2C2_PCLK_DI()			 		( RCC->APB1ENR &= ~( 1 << 22 ))
#define I2C3_PCLK_DI()			 		( RCC->APB1ENR &= ~( 1 << 23 ))

/*
 * Clock Disable Macros for SPIx peripherals
 */

#define SPI1_PCLK_DI()		 	 		( RCC->APB2ENR &= ~( 1 << 12 ) )
#define SPI2_PCLK_DI()		 	 		( RCC->APB1ENR &= ~( 1 << 14 ) )
#define SPI3_PCLK_DI()		 	 		( RCC->APB1ENR &= ~( 1 << 15 ) )
#define SPI4_PCLK_DI()		 	 		( RCC->APB2ENR &= ~( 1 << 13 ) )

/*
 * Clock Disable Macros for USARTx peripherals
 */

#define USART1_PCLK_DI()		 	 	( RCC->APB2ENR &= ~( 1 << 4 ) )
#define USART2_PCLK_DI()		 	 	( RCC->APB1ENR &= ~( 1 << 17 ) )
#define USART3_PCLK_DI()		 	 	( RCC->APB1ENR &= ~( 1 << 18 ) )
#define UART4_PCLK_DI()		 	 		( RCC->APB1ENR &= ~( 1 << 19 ) )
#define UART5_PCLK_DI()		 	 		( RCC->APB1ENR &= ~( 1 << 20 ) )
#define USART6_PCLK_DI()		 	 	( RCC->APB2ENR &= ~( 1 << 5 ) )

/*
 * Clock Disable Macros for SYSCFG peripherals
 */

#define SYSCFG_PCLK_DI()		 	 	( RCC->APB2ENR &= ~( 1 << 14 ) )

/*
 * Macros to reset GPIOx peripherals
 */
#define GPIOA_REG_RESET()				do{ ( RCC->AHB1RSTR |= ( 1 << 0 ) ); (RCC->AHB1RSTR &= ~( 1 << 0)); }while(0)
#define GPIOB_REG_RESET()				do{ ( RCC->AHB1RSTR |= ( 1 << 1 ) ); (RCC->AHB1RSTR &= ~( 1 << 1)); }while(0)
#define GPIOC_REG_RESET()				do{ ( RCC->AHB1RSTR |= ( 1 << 2 ) ); (RCC->AHB1RSTR &= ~( 1 << 2)); }while(0)
#define GPIOD_REG_RESET()				do{ ( RCC->AHB1RSTR |= ( 1 << 3 ) ); (RCC->AHB1RSTR &= ~( 1 << 3)); }while(0)
#define GPIOE_REG_RESET()				do{ ( RCC->AHB1RSTR |= ( 1 << 4 ) ); (RCC->AHB1RSTR &= ~( 1 << 4)); }while(0)
#define GPIOF_REG_RESET()				do{ ( RCC->AHB1RSTR |= ( 1 << 5 ) ); (RCC->AHB1RSTR &= ~( 1 << 5)); }while(0)
#define GPIOG_REG_RESET()				do{ ( RCC->AHB1RSTR |= ( 1 << 6 ) ); (RCC->AHB1RSTR &= ~( 1 << 6)); }while(0)
#define GPIOH_REG_RESET()				do{ ( RCC->AHB1RSTR |= ( 1 << 7 ) ); (RCC->AHB1RSTR &= ~( 1 << 7)); }while(0)

/*
 * Macros to reset SPIx peripherals
 */
#define SPI1_REG_RESET()				do{ ( RCC->APB2RSTR |= ( 1 << 12 ) ); (RCC->APB2RSTR &= ~( 1 << 12)); }while(0)
#define SPI2_REG_RESET()				do{ ( RCC->APB1RSTR |= ( 1 << 14 ) ); (RCC->APB1RSTR &= ~( 1 << 14)); }while(0)
#define SPI3_REG_RESET()				do{ ( RCC->APB1RSTR |= ( 1 << 15 ) ); (RCC->APB1RSTR &= ~( 1 << 15)); }while(0)
#define SPI4_REG_RESET()				do{ ( RCC->APB2RSTR |= ( 1 << 13 ) ); (RCC->APB2RSTR &= ~( 1 << 13)); }while(0)

/*
 * TODO: Refactor to static inline function
 * Returns port code for given GPIOx base address
 */
#define GPIO_BASEADDR_TO_CODE(x)		( (x == GPIOA)?0:\
										  (x == GPIOB)?1:\
										  (x == GPIOC)?2:\
										  (x == GPIOD)?3:\
										  (x == GPIOE)?4:\
										  (x == GPIOF)?5:\
										  (x == GPIOG)?6:0)
// TODO: add GPIOH


/*
 * IRQ(Interrupt Request) Numbers of STM32F446x MCU
 */
#define IRQ_NO_EXTI0					6
#define IRQ_NO_EXTI1					7
#define IRQ_NO_EXTI2					8
#define IRQ_NO_EXTI3					9
#define IRQ_NO_EXTI4					10
#define IRQ_NO_EXTI9_5					23
#define IRQ_NO_EXTI15_10				40

/*
 * Macros for NVIC Interrupt Priority Levels
 * (0 = Highest Priority, 15 = Lowest Priority for STM32)
 */
#define NVIC_IRQ_PRIO0					0
#define NVIC_IRQ_PRIO1					1
#define NVIC_IRQ_PRIO2					2
#define NVIC_IRQ_PRIO3					3
#define NVIC_IRQ_PRIO4					4
#define NVIC_IRQ_PRIO5					5
#define NVIC_IRQ_PRIO6					6
#define NVIC_IRQ_PRIO7					7
#define NVIC_IRQ_PRIO8					8
#define NVIC_IRQ_PRIO9					9
#define NVIC_IRQ_PRIO10					10
#define NVIC_IRQ_PRIO11					11
#define NVIC_IRQ_PRIO12					12
#define NVIC_IRQ_PRIO13					13
#define NVIC_IRQ_PRIO14					14
#define NVIC_IRQ_PRIO15					15

/*
 * some generic macros
 */
#define ENABLE 							1
#define DISABLE 						0
#define SET 							ENABLE
#define RESET 							DISABLE
#define GPIO_PIN_SET					SET
#define GPIO_PIN_RESET					RESET
#define FLAG_SET						SET
#define FLAG_RESET						RESET


/***************************************************************************
 * 				Bit position definitions of SPI peripheral
 ***************************************************************************/
/*
 * Bit position definitions SPI_CR1
 */
#define SPI_CR1_CPHA      				0
#define SPI_CR1_CPOL			        1
#define SPI_CR1_MSTR    			    2
#define SPI_CR1_BR       			   	3
#define SPI_CR1_SPE      			   	6
#define SPI_CR1_LSBFIRST    			7
#define SPI_CR1_SSI         			8
#define SPI_CR1_SSM         			9
#define SPI_CR1_RXONLY      			10
#define SPI_CR1_DFF         			11
#define SPI_CR1_CRCNEXT     			12
#define SPI_CR1_CRCEN       			13
#define SPI_CR1_BIDIOE      			14
#define SPI_CR1_BIDIMODE    			15

/*
 * Bit position definitions SPI_CR2
 */
#define SPI_CR2_RXDMAEN    				0
#define SPI_CR2_TXDMAEN    				1
#define SPI_CR2_SSOE       				2
#define SPI_CR2_FRF        				4
#define SPI_CR2_ERRIE      				5
#define SPI_CR2_RXNEIE     				6
#define SPI_CR2_TXEIE      				7

/*
 * Bit position definitions SPI_SR
 */
#define SPI_SR_RXNE       				0
#define SPI_SR_TXE        				1
#define SPI_SR_CHSIDE     				2
#define SPI_SR_UDR        				3
#define SPI_SR_CRCERR     				4
#define SPI_SR_MODF       				5
#define SPI_SR_OVR        				6
#define SPI_SR_BSY        				7
#define SPI_SR_FRE        				8


#include "stm32f446xx_gpio_driver.h"
#include "stm32f446xx_spi_driver.h"

#endif /* INC_STM32F446XX_H_ */
