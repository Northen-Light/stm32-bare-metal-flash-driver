#include <stdint.h>
#include "spi.h"
#include "flash.h"

#define DATA_BYTES_LENGTH                 4096

const uint32_t page_address = 0x0000E0;
jedec_id_t id;
uint8_t out_buffer[DATA_BYTES_LENGTH];
uint8_t in_buffer[DATA_BYTES_LENGTH];

void main(void) {
  flash_status_t status;

  spi1_init();
  flash_init();

  flash_read_jedec_id(&id);

  flash_read(DATA_BYTES_LENGTH, page_address, out_buffer);

  status = flash_block_64KB_erase(page_address);
  
  if (status == FLASH_STATUS_OK) {
    flash_read(DATA_BYTES_LENGTH, page_address, out_buffer);
  }

  for (uint32_t index = 0; index < DATA_BYTES_LENGTH; index++) {
    in_buffer[index] = (uint8_t)(index % 256);
  }

  status = flash_write(DATA_BYTES_LENGTH, in_buffer, page_address);

  if (status == FLASH_STATUS_OK) {
    flash_read(DATA_BYTES_LENGTH, page_address, out_buffer);
  }

  while(1) {}
}