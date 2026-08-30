#include <stdbool.h>

#include "flash.h"
#include "spi.h"
#include "stm32f103xx.h"
#include "flash_internal.h"
#include "systick.h"

#define PAGE_LENGTH                         256

static flash_status_t flash_page_program(uint32_t length, uint8_t *in_buffer, uint32_t address);
static flash_status_t flash_wait_until_ready(uint32_t timeout_ms);
static void flash_select(void);
static void flash_deselect(void);
static void flash_write_enable(void);
static bool flash_wel_is_set(void);
static void flash_send_address(uint32_t address);

void flash_init(void) {
  GPIOA_CRL &= ~GPIOA_CRL_PIN4_MASK;
  GPIOA_CRL |= GPIOA_CRL_PIN4_GP_PP_10MHZ;
}

void flash_read_jedec_id(jedec_id_t *id) {
  flash_select();
  spi1_transfer(FLASH_CMD_JEDEC_ID);
  id -> manufacturer_id = spi1_transfer(FLASH_DUMMY_BYTE);
  id -> memory_type = spi1_transfer(FLASH_DUMMY_BYTE);
  id -> capacity = spi1_transfer(FLASH_DUMMY_BYTE);
  flash_deselect();
}

void flash_read(uint32_t length, uint32_t address, uint8_t *out_buffer) {
  flash_select();
  spi1_transfer(FLASH_CMD_READ_DATA);
  flash_send_address(address);

  for (uint32_t index = 0; index < length; index++) {
    out_buffer[index] = spi1_transfer(FLASH_DUMMY_BYTE);
  }

  flash_deselect();
}

flash_status_t flash_write(uint32_t length, uint8_t *in_buffer, uint32_t address) {
  uint32_t page_address = address;
  uint32_t index = 0;
  uint32_t remaining_length = length;
  uint32_t buffer_length = 0;
  uint32_t partial_page_length = (0xFF - (page_address & 0xFF)) + 1;
  flash_status_t status;

  if (partial_page_length <= remaining_length) {
    buffer_length = partial_page_length;
    remaining_length -= partial_page_length;
  } else {
    buffer_length = remaining_length;
    remaining_length = 0;
  }

  status = flash_page_program(buffer_length, in_buffer + index, page_address);
  
  if (status != FLASH_STATUS_OK) {
    return status;
  }

  index += buffer_length;
  page_address += buffer_length;

  buffer_length = PAGE_LENGTH;

  while (remaining_length >= PAGE_LENGTH) {
    status = flash_page_program(buffer_length, in_buffer + index, page_address);

    if (status != FLASH_STATUS_OK) {
      return status;
    }

    remaining_length -= buffer_length;
    index += buffer_length;
    page_address += buffer_length;
  }

  if (remaining_length > 0) {
    status = flash_page_program(remaining_length, in_buffer + index, page_address);

    if (status != FLASH_STATUS_OK) {
      return status;
    }

    index += remaining_length;
    page_address += remaining_length;
    remaining_length = 0;
  }

  return FLASH_STATUS_OK;
}

flash_status_t flash_sector_erase(uint32_t address) {
  flash_write_enable();
  
  if (!flash_wel_is_set()) {
    return FLASH_STATUS_WEL_NOT_SET;
  }

  flash_select();
  spi1_transfer(FLASH_CMD_SECTOR_ERASE);
  flash_send_address(address);
  flash_deselect();
  
  return flash_wait_until_ready(FLASH_SECTOR_ERASE_TIMEOUT_MS);
}

flash_status_t flash_block_64KB_erase(uint32_t address) {
  flash_write_enable();
  
  if (!flash_wel_is_set()) {
    return FLASH_STATUS_WEL_NOT_SET;
  }

  flash_select();
  spi1_transfer(FLASH_CMD_BLOCK_64KB_ERASE);
  flash_send_address(address);
  flash_deselect();

  return flash_wait_until_ready(FLASH_BLOCK_64KB_ERASE_TIMEOUT_MS);
}

flash_status_t flash_chip_erase(void) {
  flash_write_enable();
  
  if (!flash_wel_is_set()) {
    return FLASH_STATUS_WEL_NOT_SET;
  }

  flash_select();
  spi1_transfer(FLASH_CMD_CHIP_ERASE);
  flash_deselect();

  return flash_wait_until_ready(FLASH_CHIP_ERASE_TIMEOUT_MS);
}

static flash_status_t flash_page_program(uint32_t length, uint8_t *in_buffer, uint32_t address) {
  flash_write_enable();

  if (!flash_wel_is_set()) {
    return FLASH_STATUS_WEL_NOT_SET;
  }

  flash_select();
  spi1_transfer(FLASH_CMD_PAGE_PROGRAM);
  flash_send_address(address);

  for (uint32_t index = 0; index < length; index++) {
    spi1_transfer(in_buffer[index]);
  }

  flash_deselect();

  return flash_wait_until_ready(FLASH_PAGE_PROGRAM_TIMEOUT_MS);
}

static flash_status_t flash_wait_until_ready(uint32_t timeout_ms) {
  uint8_t status;
  uint32_t start_time_ms = systick_get_systick_ms();
  uint32_t time_difference_ms;

  flash_select();
  spi1_transfer(FLASH_CMD_READ_REGISTER1);
  
  do {
    status = spi1_transfer(FLASH_DUMMY_BYTE);
    /*  For unint32_t, arithematic is modulo 2 ^ 32 
        i.e, (-5 % 2 ^ 32) = (-5 + 2 ^ 32) % 2 ^ 32 
    */
    time_difference_ms = (uint32_t)(systick_get_systick_ms() - start_time_ms);

    if (time_difference_ms >= timeout_ms) {
      return FLASH_STATUS_TIMEOUT;
    }
  } while ((status & FLASH_STATUS_REGISTER1_BUSY) != 0);

  flash_deselect();

  return FLASH_STATUS_OK;
}

static void flash_select(void) {
  GPIOA_BSRR = GPIOA_BSRR_PIN4_RESET;
}

static void flash_deselect(void) {
  GPIOA_BSRR = GPIOA_BSRR_PIN4_SET;
}

static void flash_write_enable(void) {
  flash_select();
  spi1_transfer(FLASH_CMD_WRITE_ENABLE);
  flash_deselect();
}

static bool flash_wel_is_set(void) {
  uint8_t status;

  flash_select();
  spi1_transfer(FLASH_CMD_READ_REGISTER1);
  status = spi1_transfer(FLASH_DUMMY_BYTE); 
  flash_deselect();

  return ((status & FLASH_STATUS_REGISTER1_WEL) != 0);
}

static void flash_send_address(uint32_t address) {
  spi1_transfer((address >> 16) & 0xFF);
  spi1_transfer((address >> 8) & 0xFF);
  spi1_transfer((address >> 0) & 0xFF);
}