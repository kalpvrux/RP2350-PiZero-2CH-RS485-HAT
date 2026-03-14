#include <stdio.h>

#include "DEV_Config.h"
#include "RS485.h"

int main() {
  // Initialize RP2350 (stdio, GPIO, etc.)
  if (DEV_ModuleInit() != 0) {
    printf("Module init failed!\r\n");
    return 1;
  }

  // Initialize SPI bus
  DEV_SPI_Init();

  // Initialize RS485 Channel 1 at 115200 baud
  RS485_CH1_begin(115200);
  DEV_Delay_ms(10);

  // Send welcome message
  RS485_CH1_WriteString("Waveshare 2-CH RS485 HAT on RP2350\r\n");

  printf("RS485 echo started. Waiting for data...\r\n");

  // Echo loop
  UBYTE pData[100];
  int i = 0;

  while (1) {
    pData[i] = RS485_CH1_ReadByte();

    if (pData[i] == '\n') {
      // Print received data to USB serial (debug)
      for (int j = 0; j < i + 1; j++)
        printf("%c", pData[j]);

      // Echo back via RS485
      RS485_CH1_Write(pData, i + 1);
      i = 0;
    } else if (i > 98) {
      // Buffer overflow protection
      i = 0;
    } else {
      i++;
    }
  }

  DEV_ModuleExit();
  return 0;
}
