#include <stdint.h>
#include "spi.h"
#include "flash.h"

jedec_id_t id;
uint8_t out_buffer[600];
uint8_t in_buffer[600];

void main(void) {
  spi1_init();
  flash_init();

  flash_read_jedec_id(&id);
  flash_read(600, 0x0000E0, out_buffer);
  flash_sector_erase(0x000E0);
  flash_read(600, 0x0000E0, out_buffer);

  for (uint32_t index = 0; index < 600; index++) {
    in_buffer[index] = (uint8_t)(index % 256);
  }

  flash_write(600, in_buffer, 0x0000E0);
  flash_read(600, 0x0000E0, out_buffer);

  while(1) {}
}