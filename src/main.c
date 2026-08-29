#include <stdint.h>
#include "spi.h"
#include "flash.h"

jedec_id_t id;
uint8_t out_buffer[64];
uint8_t in_buffer[64];

void main(void) {
  spi1_init();
  flash_init();

  flash_read_jedec_id(&id);
  flash_read_data(16, 0x000000, out_buffer);
  flash_sector_erase(0x000000);
  flash_read_data(16, 0x000000, out_buffer);

  for (uint32_t index = 0; index < 16; index++) {
    in_buffer[index] = (uint8_t)index;
  }

  flash_page_program(16, in_buffer, 0x000000);
  flash_read_data(16, 0x000000, out_buffer);

  while(1) {}
}