/*****************************************************************************
 * | File        :   DEV_Config.h
 * | Author      :   Waveshare team (ported to RP2350)
 * | Function    :   Hardware underlying interface - RP2350 Pico SDK
 * | Info        :
 *----------------
 * | This version:   V1.0 (RP2350 Port)
 * | Date        :   2026-03-12
 * | Info        :   Pico SDK HAL for 2-CH RS485 HAT
 ******************************************************************************/
#ifndef _DEV_CONFIG_H_
#define _DEV_CONFIG_H_

#include <stdint.h>

/*---------- Data type aliases (kept for compatibility) ----------*/
#define UBYTE uint8_t
#define UWORD uint16_t
#define UDOUBLE uint32_t

/*---------- SPI Configuration ----------*/
#define SPI_PORT spi0 // We will use PIO to emulate SPI on the required pins
#define SPI_SCK 21    // Header Pin 40 -> GP21
#define SPI_MOSI 20   // Header Pin 38 -> GP20
#define SPI_MISO 19   // Header Pin 35 -> GP19
#define SPI_CS 18     // Header Pin 12 -> GP18

#define SPI_SPEED_HZ 1000000 // 1 MHz

/*---------- GPIO Pin Definitions ----------*/
#define IRQ_PIN 24 // Header Pin 18 -> GP24
#define TXDEN_1 27 // Header Pin 13 -> GP27
#define TXDEN_2 22 // Header Pin 15 -> GP22



/*---------- Module Init / Exit ----------*/
uint8_t DEV_ModuleInit(void);
void DEV_ModuleExit(void);

/*---------- GPIO ----------*/
void DEV_GPIO_Mode(UWORD Pin, UWORD Mode);
void DEV_Digital_Write(UWORD Pin, uint8_t Value);
uint8_t DEV_Digital_Read(UWORD Pin);

/*---------- Delay ----------*/
void DEV_Delay_ms(UDOUBLE xms);

/*---------- SPI ----------*/
void DEV_SPI_Init(void);
void DEV_SPI_WriteByte(uint8_t Value);
void DEV_SPI_Write_nByte(uint8_t *pData, uint32_t Len);
void DEV_SPI_transmission_nByte(uint8_t *pData, uint32_t Len);

#endif
