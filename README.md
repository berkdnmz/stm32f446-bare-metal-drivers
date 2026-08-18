# STM32F446xx Bare-Metal Peripheral Driver Development

**English | [🇹🇷 Türkçe](#-türkçe)**

![C](https://img.shields.io/badge/C-Embedded-blue)
![ARM Cortex-M4](https://img.shields.io/badge/Core-ARM_Cortex--M4-green)
![STM32F446RE](https://img.shields.io/badge/MCU-STM32F446RE-success)
![Bare Metal](https://img.shields.io/badge/Bare--Metal-Register_Level-orange)

A comprehensive, register-level bare-metal hardware driver library developed from scratch in Embedded C for the STM32F446RE (ARM Cortex-M4) microcontroller.

This project implements complete peripheral driver architectures (MCU Peripheral Layer, Driver Header/Source API Layer, and Application/Test Layer) without using STM32Cube HAL or LL libraries. All implementations strictly follow the official STM32F446xx Reference Manual (RM0390) and ARM Cortex-M4 documentation.

---

## Table of Contents

- [Key Features](#key-features)
- [Driver Architecture and Peripherals](#driver-architecture-and-peripherals)
- [Repository Structure](#repository-structure)
- [Build and Toolchain](#build-and-toolchain)
- [References and Acknowledgments](#references-and-acknowledgments)
- [🇹🇷 Türkçe](#-türkçe)

---

## Key Features

- **Zero High-Level Abstraction:** Written purely using register-level bit manipulations, bitwise operations, and atomic register access.
- **Interrupt-Driven Architecture:** Non-blocking asynchronous I/O powered by custom state machines and ARM Cortex-M4 NVIC configuration.
- **Custom Clock Tree Calculations:** Internal fixed-point arithmetic for system clock prescalers (AHB/APB), I2C timing (CCR/TRISE), and USART fractional baud rates.
- **Hardware Protocol Coverage:** Fully featured drivers for GPIO, SPI, I2C, and USART/UART.
- **Validated Applications:** 11+ structured application test cases covering polling, interrupt handlers, master/slave interactions, and edge triggers.

---

## Driver Architecture and Peripherals

### 1. Device Peripheral Base (`stm32f446xx.h`)

- Memory map base addresses (Flash, SRAM1, SRAM2, System ROM, Peripheral buses).
- Register definition structures for memory-mapped I/O peripherals (`RCC`, `GPIO`, `SPI`, `I2C`, `USART`, `EXTI`, `SYSCFG`, `NVIC`).
- Clock management macros (Clock Enable/Disable, Peripheral Soft Reset via `RCC_APBxRSTR` using `do { } while(0)` idiom).
- NVIC interrupt positions (`IRQ_NO_x`), priority configurations, and shift amounts.

### 2. GPIO Driver (`stm32f446xx_gpio_driver`)

- Modes: Input, Output (Push-Pull/Open-Drain), Alternate Function (AF0–AF15), Analog.
- Speeds & Pull-Up/Down: Low, Medium, Fast, High speed with Pull-up, Pull-down, or Floating.
- Interrupts: Rising, Falling, and Both-edge detection using EXTI and SYSCFG.

### 3. SPI Driver (`stm32f446xx_spi_driver`)

- Master/Slave operation.
- Full-Duplex, Half-Duplex, and Simplex communication.
- 8-bit and 16-bit frame formats.
- CPOL/CPHA support (Modes 0–3).
- Software and Hardware NSS management.
- Blocking polling APIs and interrupt-driven APIs with application callbacks.

### 4. I2C Driver (`stm32f446xx_i2c_driver`)

- Master/Slave transmitter and receiver modes.
- Standard Mode (100 kHz) and Fast Mode (400 kHz).
- Dynamic CCR/TRISE calculations based on APB1 frequency.
- 7-bit addressing and Repeated Start support.
- Interrupt-driven state machine handling `TXE`, `RXNE`, `SB`, `ADDR`, `BTF`, `BERR`, `ARLO`, `AF`, and `OVR` events.

### 5. USART/UART Driver (`stm32f446xx_usart_driver`)

- TX, RX, and Full-Duplex modes.
- 8-bit and 9-bit word lengths.
- Even, Odd, and No parity.
- Multiple stop-bit configurations.
- RTS/CTS hardware flow control.
- BRR calculation with 8x/16x oversampling.
- Interrupt handling for `TXE`, `TC`, `RXNE`, `IDLE`, `CTS`, `FE`, `NE`, and `ORE` flags.

---

## Repository Structure

```text
.
|-- drivers/
|   |-- Inc/
|   |   |-- stm32f446xx.h
|   |   |-- stm32f446xx_gpio_driver.h
|   |   |-- stm32f446xx_spi_driver.h
|   |   |-- stm32f446xx_i2c_driver.h
|   |   `-- stm32f446xx_usart_driver.h
|   `-- Src/
|       |-- stm32f446xx_gpio_driver.c
|       |-- stm32f446xx_spi_driver.c
|       |-- stm32f446xx_i2c_driver.c
|       `-- stm32f446xx_usart_driver.c
|-- Src/
|   |-- 001led_toggle.c
|   |-- 002led_button.c
|   |-- 003led_button_ext.c
|   |-- 004button_interrupt.c
|   |-- 005spi_tx_testing.c
|   |-- 006spi_tx_interrupt_testing.c
|   |-- 007spi_txrx_interrupt_testing.c
|   |-- 008i2c_master_tx_testing.c
|   |-- 009i2c_master_rx_testing.c
|   |-- 010i2c_master_txrx_testing_it.c
|   |-- 011i2c_master_tx_slave_rx_it.c
|   |-- syscalls.c
|   `-- sysmem.c
|-- Startup/
|   `-- startup_stm32f446retx.s
|-- STM32F446RETX_FLASH.ld
|-- STM32F446RETX_RAM.ld
`-- README.md
```
---

## Build and Toolchain

| Item | Value |
|:---|:---|
| **Target MCU** | STM32F446RE |
| **Core** | ARM Cortex-M4 with FPU |
| **Flash** | 512 KB |
| **SRAM** | 128 KB |
| **Board** | NUCLEO-F446RE |
| **IDE** | STM32CubeIDE |
| **Compiler** | arm-none-eabi-gcc (C99, `-O0`) |
| **Debugger** | ST-LINK/V2-1 (SWD) |

### Build Instructions

1. Clone this repository.
2. Open it using **STM32CubeIDE**.
3. Select a single test application in `Src/`.
4. Exclude other test files containing `main()` from the build.
5. Build (`Ctrl+B`).
6. Flash using `F11`.

---

## References and Acknowledgments

### STMicroelectronics
- STM32F446xx Reference Manual (RM0390)
- STM32F446xE Datasheet
- NUCLEO-F446RE User Manual (UM1724)

### ARM
- ARM Cortex-M4 Generic User Guide

### Sensor Documentation
- MPU-6050 Product Specification / Datasheet

### Learning Resource
- Inspired by FastBit Embedded Brain Academy — MCU1 Course

---

# 🇹🇷 Türkçe

**[🇬🇧 English](#stm32f446xx-bare-metal-peripheral-driver-development) | Türkçe**

STM32F446RE (ARM Cortex-M4) mikrodenetleyicisi için **Embedded C** kullanılarak sıfırdan geliştirilmiş, register seviyesinde çalışan kapsamlı bir bare-metal donanım sürücü kütüphanesi.

Bu projede STM32Cube HAL veya LL kütüphaneleri kullanılmadan; **MCU Peripheral Layer**, **Driver API Layer** ve **Application/Test Layer** mimarileri resmi **STM32F446xx Reference Manual (RM0390)** ve **ARM Cortex-M4** dokümantasyonu esas alınarak geliştirilmiştir.

---

## İçindekiler

- [Temel Özellikler](#temel-özellikler)
- [Sürücü Mimarisi ve Çevre Birimleri](#sürücü-mimarisi-ve-çevre-birimleri)
- [Repository Yapısı](#repository-yapısı)
- [Derleme ve Geliştirme Ortamı](#derleme-ve-geliştirme-ortamı)
- [Kaynaklar ve Teşekkür](#kaynaklar-ve-teşekkür)

---

## Temel Özellikler

- **HAL veya LL kullanılmadan** tamamen register seviyesinde geliştirilmiştir.
- **Kesme (Interrupt) Tabanlı Mimari:** Asenkron I/O yönetimi ve ARM Cortex-M4 NVIC yapılandırması.
- **Dinamik Saat Hesaplamaları:** AHB/APB prescaler, I2C CCR/TRISE ve USART BRR hesaplamaları.
- **Donanım Protokolleri:** GPIO, SPI, I2C ve USART/UART sürücüleri.
- **Test Senaryoları:** Polling ve interrupt durumlarını kapsayan 11'den fazla test uygulaması.

---

## Sürücü Mimarisi ve Çevre Birimleri

### 1. Device Peripheral Base (`stm32f446xx.h`)
- Bellek haritaları ve peripheral base address tanımlamaları.
- RCC, GPIO, SPI, I2C, USART, EXTI, SYSCFG ve NVIC register yapıları.
- Clock Enable/Disable ve peripheral soft reset işlemleri (`do { } while(0)` makroları).
- NVIC interrupt numaraları ve priority yapılandırmaları.

### 2. GPIO Driver (`stm32f446xx_gpio_driver`)
- Input, Output, Alternate Function ve Analog modları.
- Push-Pull ve Open-Drain çıkış seçenekleri.
- Pull-up, Pull-down ve Floating yapılandırmaları.
- EXTI ve SYSCFG üzerinden Rising, Falling ve Both-edge interrupt desteği.

### 3. SPI Driver (`stm32f446xx_spi_driver`)
- Master ve Slave çalışma modları.
- Full-Duplex, Half-Duplex ve Simplex iletişim.
- 8-bit ve 16-bit veri çerçeveleri.
- CPOL/CPHA desteği (SPI Modes 0–3).
- Software ve Hardware NSS yönetimi.
- Polling ve interrupt-driven API'ler.

### 4. I2C Driver (`stm32f446xx_i2c_driver`)
- Master ve Slave transmitter/receiver modları.
- Standard Mode (100 kHz) ve Fast Mode (400 kHz).
- APB1 frekansına göre dinamik CCR/TRISE hesaplamaları.
- 7-bit addressing ve Repeated Start desteği.
- `TXE`, `RXNE`, `SB`, `ADDR`, `BTF` ve hata durumları için interrupt-driven state machine.

### 5. USART/UART Driver (`stm32f446xx_usart_driver`)
- TX, RX ve Full-Duplex çalışma modları.
- 8-bit ve 9-bit word length.
- Even, Odd ve No Parity seçenekleri.
- Farklı stop-bit yapılandırmaları.
- RTS/CTS hardware flow control.
- 8x/16x oversampling destekli BRR hesaplaması.
- `TXE`, `TC`, `RXNE`, `IDLE`, `CTS` ve hardware error interrupt yönetimi.

---

## Repository Yapısı

```text
.
|-- drivers/
|   |-- Inc/
|   |   |-- stm32f446xx.h
|   |   |-- stm32f446xx_gpio_driver.h
|   |   |-- stm32f446xx_spi_driver.h
|   |   |-- stm32f446xx_i2c_driver.h
|   |   `-- stm32f446xx_usart_driver.h
|   `-- Src/
|       |-- stm32f446xx_gpio_driver.c
|       |-- stm32f446xx_spi_driver.c
|       |-- stm32f446xx_i2c_driver.c
|       `-- stm32f446xx_usart_driver.c
|-- Src/
|   |-- 001led_toggle.c
|   |-- 002led_button.c
|   |-- 003led_button_ext.c
|   |-- 004button_interrupt.c
|   |-- 005spi_tx_testing.c
|   |-- 006spi_tx_interrupt_testing.c
|   |-- 007spi_txrx_interrupt_testing.c
|   |-- 008i2c_master_tx_testing.c
|   |-- 009i2c_master_rx_testing.c
|   |-- 010i2c_master_txrx_testing_it.c
|   |-- 011i2c_master_tx_slave_rx_it.c
|   |-- syscalls.c
|   `-- sysmem.c
|-- Startup/
|   `-- startup_stm32f446retx.s
|-- STM32F446RETX_FLASH.ld
|-- STM32F446RETX_RAM.ld
`-- README.md
```

---

## Derleme ve Geliştirme Ortamı

| Özellik | Değer |
|:---|:---|
| **Mikrodenetleyici** | STM32F446RE |
| **Çekirdek** | ARM Cortex-M4 with FPU |
| **Flash** | 512 KB |
| **SRAM** | 128 KB |
| **Geliştirme Kartı** | NUCLEO-F446RE |
| **IDE** | STM32CubeIDE |
| **Derleyici** | arm-none-eabi-gcc (C99, `-O0`) |
| **Debugger** | ST-LINK/V2-1 (SWD) |

### Derleme Adımları

1. Repoyu klonlayın.
2. Projeyi **STM32CubeIDE** ile açın.
3. `Src/` klasöründen tek bir test uygulamasını aktif bırakın.
4. Diğer `main()` içeren test dosyalarını derleme dışı bırakın.
5. `Ctrl+B` ile projeyi derleyin.
6. `F11` ile karta yükleyin.

---

## Kaynaklar ve Teşekkür

### STMicroelectronics
- STM32F446xx Reference Manual (RM0390)
- STM32F446xE Datasheet
- NUCLEO-F446RE User Manual (UM1724)

### ARM
- ARM Cortex-M4 Generic User Guide

### Sensör Dokümantasyonu
- MPU-6050 Product Specification / Datasheet

### Öğrenme Kaynağı
- FastBit Embedded Brain Academy — MCU1 Course