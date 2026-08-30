#ifndef FLASH_H
#define FLASH_H

#include <stdint.h>

typedef struct {
  uint8_t manufacturer_id;
  uint8_t memory_type;
  uint8_t capacity;
} jedec_id_t;

typedef enum {
  FLASH_STATUS_OK,
  FLASH_STATUS_WEL_NOT_SET,
  FLASH_STATUS_TIMEOUT
} flash_status_t;

void flash_init(void);
void flash_read_jedec_id(jedec_id_t *id);
void flash_read(uint32_t length, uint32_t address, uint8_t *out_buffer);
flash_status_t flash_write(uint32_t length, uint8_t *in_buffer, uint32_t address);
flash_status_t flash_sector_erase(uint32_t address);
flash_status_t flash_block_64KB_erase(uint32_t address);
flash_status_t flash_chip_erase(void);

#endif