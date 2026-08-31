#include <stdint.h>
#include "spi.h"
#include "flash.h"

#define DATA_BYTES_LENGTH                 4096

typedef enum {
  FLASH_VERIFY_NOT_RUN,
  FLASH_VERIFY_OK,
  FLASH_VERIFY_FAILED
} flash_verify_status_t;

const uint32_t page_address = 0x0000E0;
jedec_id_t id;
uint8_t out_buffer[DATA_BYTES_LENGTH];
uint8_t in_buffer[DATA_BYTES_LENGTH];
flash_verify_status_t write_verify_status = FLASH_VERIFY_NOT_RUN;
flash_verify_status_t erase_verify_status = FLASH_VERIFY_NOT_RUN;

void main(void) {
  flash_status_t status;

  spi1_init();
  flash_init();

  write_verify_status = FLASH_VERIFY_NOT_RUN;
  erase_verify_status = FLASH_VERIFY_NOT_RUN;

  flash_read_jedec_id(&id);

  flash_read(page_address, out_buffer, DATA_BYTES_LENGTH);

  status = flash_block_64KB_erase(page_address);
  
  if (status == FLASH_STATUS_OK) {
    flash_read(page_address, out_buffer, DATA_BYTES_LENGTH);

    erase_verify_status = FLASH_VERIFY_OK;

    for (uint32_t i = 0; i < DATA_BYTES_LENGTH; i++) {
      if (out_buffer[i] != 0xFF) {
        erase_verify_status = FLASH_VERIFY_FAILED;
        break;
      } 
    }
  }

  for (uint32_t index = 0; index < DATA_BYTES_LENGTH; index++) {
    in_buffer[index] = (uint8_t)(index % 256);
  }

  status = flash_write(page_address, in_buffer, DATA_BYTES_LENGTH);

  if (status == FLASH_STATUS_OK) {
    flash_read(page_address, out_buffer, DATA_BYTES_LENGTH);

    write_verify_status = FLASH_VERIFY_OK;

    for (uint32_t i = 0; i < DATA_BYTES_LENGTH; i++) {
      if (in_buffer[i] != out_buffer[i]) {
        write_verify_status = FLASH_VERIFY_FAILED;
        break;
      }
    }
  }

  while(1) {}
}