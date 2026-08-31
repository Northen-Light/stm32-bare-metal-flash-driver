#include <stdbool.h>
#include <stdint.h>
#include "spi.h"
#include "flash.h"

#define DATA_BYTES_LENGTH                 4096

typedef enum {
  FLASH_VERIFICATION_STATUS_NOT_VERIFIED,
  FLASH_VERIFICATION_STATUS_VERIFIED,
  FLASH_VERIFICATION_STATUS_FAILURE
} flash_verification_status_t;

const uint32_t page_address = 0x0000E0;
jedec_id_t id;
uint8_t out_buffer[DATA_BYTES_LENGTH];
uint8_t in_buffer[DATA_BYTES_LENGTH];
flash_verification_status_t \
  flash_write_verfication_status = FLASH_VERIFICATION_STATUS_NOT_VERIFIED;

flash_verification_status_t \
  flash_erase_verification_status = FLASH_VERIFICATION_STATUS_NOT_VERIFIED;

void main(void) {
  flash_status_t status;

  spi1_init();
  flash_init();

  flash_write_verfication_status = FLASH_VERIFICATION_STATUS_NOT_VERIFIED;
  flash_erase_verification_status = FLASH_VERIFICATION_STATUS_NOT_VERIFIED;

  flash_read_jedec_id(&id);

  flash_read(page_address, out_buffer, DATA_BYTES_LENGTH);

  status = flash_block_64KB_erase(page_address);
  
  if (status == FLASH_STATUS_OK) {
    flash_read(page_address, out_buffer, DATA_BYTES_LENGTH);

    flash_erase_verification_status = FLASH_VERIFICATION_STATUS_VERIFIED;

    for (uint32_t i = 0; i < DATA_BYTES_LENGTH; i++) {
      if (out_buffer[i] != 0xFF) {
        flash_erase_verification_status = FLASH_VERIFICATION_STATUS_FAILURE;
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

    flash_write_verfication_status = FLASH_VERIFICATION_STATUS_VERIFIED;

    for (uint32_t i = 0; i < DATA_BYTES_LENGTH; i++) {
      if (in_buffer[i] != out_buffer[i]) {
        flash_write_verfication_status = FLASH_VERIFICATION_STATUS_FAILURE;
        break;
      }
    }
  }

  while(1) {}
}