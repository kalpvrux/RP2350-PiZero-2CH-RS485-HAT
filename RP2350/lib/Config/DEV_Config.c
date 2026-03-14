/*****************************************************************************
 * | File        :   DEV_Config.c
 * | Author      :   Waveshare team (ported to RP2350)
 * | Function    :   Hardware underlying interface - RP2350 Pico SDK
 * | Info        :
 *----------------
 * | This version:   V1.0 (RP2350 Port)
 * | Date        :   2026-03-12
 * | Info        :   Pico SDK HAL implementation for 2-CH RS485 HAT
 ******************************************************************************/
#include "DEV_Config.h"
#include "hardware/gpio.h"
#include "pico/stdio.h"
#include "pico/time.h"
#include <stdio.h>

/******************************************************************************
 * GPIO Functions
 ******************************************************************************/
void DEV_GPIO_Mode(UWORD Pin, UWORD Mode) {
  gpio_init(Pin);
  if (Mode == 0) {
    gpio_set_dir(Pin, GPIO_IN);
    gpio_pull_up(Pin); // IRQ is active-low, needs pull-up
  } else {
    gpio_set_dir(Pin, GPIO_OUT);
  }
}

void DEV_Digital_Write(UWORD Pin, uint8_t Value) { gpio_put(Pin, Value); }

uint8_t DEV_Digital_Read(UWORD Pin) { return gpio_get(Pin) ? 1 : 0; }

/******************************************************************************
 * Delay
 ******************************************************************************/
void DEV_Delay_ms(UDOUBLE xms) { sleep_ms(xms); }

/******************************************************************************
 * SPI Functions
 ******************************************************************************/
void DEV_SPI_Init(void) {
  printf("RP2350 Bit-bang SPI Init (GP18-21)\r\n");

  // Initialize pins for bit-banging
  gpio_init(SPI_SCK);
  gpio_set_dir(SPI_SCK, GPIO_OUT);
  gpio_put(SPI_SCK, 0); // SCLK idle low (Mode 0)

  gpio_init(SPI_MOSI);
  gpio_set_dir(SPI_MOSI, GPIO_OUT);
  gpio_put(SPI_MOSI, 0);

  gpio_init(SPI_MISO);
  gpio_set_dir(SPI_MISO, GPIO_IN);

  // CS pin: manual control
  gpio_init(SPI_CS);
  gpio_set_dir(SPI_CS, GPIO_OUT);
  gpio_put(SPI_CS, 1); // CS idle high
}

/**
 * Bit-bang one byte (Full Duplex)
 */
static uint8_t SPI_TransferByte(uint8_t byte) {
  uint8_t received = 0;
  for (int i = 7; i >= 0; i--) {
    // Set MOSI
    gpio_put(SPI_MOSI, (byte >> i) & 0x01);

    // Small delay for timing (approx 1MHz)
    busy_wait_us_32(1);

    // SCLK High
    gpio_put(SPI_SCK, 1);

    // Sample MISO
    if (gpio_get(SPI_MISO)) {
      received |= (1 << i);
    }

    busy_wait_us_32(1);

    // SCLK Low
    gpio_put(SPI_SCK, 0);
  }
  return received;
}

void DEV_SPI_WriteByte(uint8_t Value) {
  gpio_put(SPI_CS, 0);
  SPI_TransferByte(Value);
  gpio_put(SPI_CS, 1);
}

void DEV_SPI_Write_nByte(uint8_t *pData, uint32_t Len) {
  gpio_put(SPI_CS, 0);
  for (uint32_t i = 0; i < Len; i++) {
    SPI_TransferByte(pData[i]);
  }
  gpio_put(SPI_CS, 1);
}

void DEV_SPI_transmission_nByte(uint8_t *pData, uint32_t Len) {
  gpio_put(SPI_CS, 0);
  for (uint32_t i = 0; i < Len; i++) {
    pData[i] = SPI_TransferByte(pData[i]);
  }
  gpio_put(SPI_CS, 1);
}

/******************************************************************************
 * GPIO Config — set up TXDEN and IRQ pins
 ******************************************************************************/
static void GPIO_Config(void) {
  DEV_GPIO_Mode(TXDEN_1, 1); // Output: RS485 CH1 direction
  DEV_GPIO_Mode(TXDEN_2, 1); // Output: RS485 CH2 direction
  DEV_GPIO_Mode(IRQ_PIN, 0); // Input:  SC16IS752 interrupt

  // Default: both channels in receive mode (TXDEN high = RX)
  DEV_Digital_Write(TXDEN_1, 1);
  DEV_Digital_Write(TXDEN_2, 1);
}

/******************************************************************************
 * Module Init / Exit
 ******************************************************************************/
uint8_t DEV_ModuleInit(void) {
  // Initialize Pico SDK stdio (USB serial for printf)
  stdio_init_all();

  // Brief delay for USB enumeration
  sleep_ms(2000);

  printf("2-CH RS485 HAT — RP2350 PiZero\r\n");

  GPIO_Config();

  return 0;
}

void DEV_ModuleExit(void) { printf("Module Exit\r\n"); }
