#ifndef FLASH_INTERNAL_H
#define FLASH_INTERNAL_H

#define FLASH_CMD_JEDEC_ID                                  0x9F
#define FLASH_CMD_READ_DATA                                 0x03
#define FLASH_CMD_PAGE_PROGRAM                              0x02
#define FLASH_CMD_SECTOR_ERASE                              0x20
#define FLASH_CMD_BLOCK_64KB_ERASE                          0xD8
#define FLASH_CMD_CHIP_ERASE                                0x60
#define FLASH_CMD_WRITE_ENABLE                              0x06
#define FLASH_CMD_READ_REGISTER1                            0x05
#define FLASH_DUMMY_BYTE                                    0xFF

#define FLASH_STATUS_REGISTER1_BUSY                         (1U << 0)
#define FLASH_STATUS_REGISTER1_WEL                          (1U << 1)

#define FLASH_PAGE_PROGRAM_TIMEOUT_MS                       5
#define FLASH_SECTOR_ERASE_TIMEOUT_MS                       500
#define FLASH_BLOCK_64KB_ERASE_TIMEOUT_MS                   2500
#define FLASH_CHIP_ERASE_TIMEOUT_MS                         210000

#endif