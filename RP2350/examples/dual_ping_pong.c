/*****************************************************************************
 * | File        :   dual_ping_pong.c
 * | Author      :   Waveshare team
 * | Function    :   2-CH RS485 HAT example — Dual Channel Ping-Pong
 * | Info        :   Sends data from CH1 to CH2, then CH2 to CH1.
 *                   Requires physical connection: CH1(A,B) connected to
 * CH2(A,B).
 ******************************************************************************/
#include "DEV_Config.h"
#include "RS485.h"
#include <stdio.h>

int main() {
  // Initialize RP2350 (stdio, GPIO, etc.)
  if (DEV_ModuleInit() != 0) {
    printf("Module init failed!\r\n");
    return 1;
  }

  // Initialize SPI bus
  DEV_SPI_Init();

  // Initialize both RS485 channels at 115200 baud
  printf("Initializing RS485 Channel 1 and 2...\r\n");
  RS485_CH1_begin(115200);
  RS485_CH2_begin(115200);
  DEV_Delay_ms(100);

  printf("\n2-CH RS485 Dual Channel Ping-Pong Test started.\r\n");
  printf("Hardware Setup: Connect CH1 A -> CH2 A and CH1 B -> CH2 B\r\n\n");

  char rx_char;
  int cycle = 1;

  while (1) {
    printf("--- Cycle %d ---\r\n", cycle++);

    // 1. Send from CH1 to CH2
    printf("[CH1 -> CH2] Sending: 'Hello from CH1\\n'\r\n");
    RS485_CH1_WriteString("Hello from CH1\n");

    printf("[CH2] Received: ");
    // Read until newline - Note: RS485_CH2_ReadByte is blocking
    while (1) {
      rx_char = RS485_CH2_ReadByte();
      printf("%c", rx_char);
      if (rx_char == '\n')
        break;
    }

    DEV_Delay_ms(500); // Small pause

    // 2. Send from CH2 to CH1
    printf("[CH2 -> CH1] Sending: 'Reply from CH2\\n'\r\n");
    RS485_CH2_WriteString("Reply from CH2\n");

    printf("[CH1] Received: ");
    // Read until newline
    while (1) {
      rx_char = RS485_CH1_ReadByte();
      printf("%c", rx_char);
      if (rx_char == '\n')
        break;
    }

    printf("-----------------------------------\r\n\n");
    DEV_Delay_ms(2000); // Wait before next cycle
  }

  DEV_ModuleExit();
  return 0;
}
