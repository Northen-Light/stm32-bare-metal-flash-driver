#include <stdint.h>
#include "spi.h"

void main(void) {
  spi1_init();

  while(1) {
    uint8_t byte = spi1_transfer(0xAB);
  }
}