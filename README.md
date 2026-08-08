# STM32 Bare-Metal Flash Driver

A bare-metal Flash memory driver for the STM32F103 written in C using register-level programming.

This project interfaces an STM32F103 microcontroller with a Winbond W25Qxx SPI NOR Flash memory chip using SPI1. It implements core Flash operations such as reading the JEDEC ID, reading the status register, write enable, page program, sector erase, block erase, chip erase, and multi-page writes.

---

## Features

- Bare-metal SPI1 driver
- Register-level STM32F103 programming
- Manual Chip Select control using GPIO
- Winbond W25Qxx SPI NOR Flash support
- JEDEC ID read
- Status Register-1 read
- Write Enable command
- Read Data command
- Page Program command
- Sector Erase
- 64 KB Block Erase
- Chip Erase
- Multi-page flash write support
- Page boundary handling
- Modular driver architecture

---

## Hardware Used

- STM32F103C8T6 Blue Pill
- W25Qxx SPI Flash module
- ST-Link programmer/debugger
- Breadboard and jumper wires

---

## Pin Connections

| STM32F103 Pin | SPI Flash Pin | Description |
|--------------|---------------|-------------|
| PA5 | CLK | SPI Clock |
| PA6 | DO | MISO |
| PA7 | DI | MOSI |
| PA4 | CS | Chip Select |
| 3.3V | VCC | Power |
| GND | GND | Ground |

---

## Project Structure

```text
bare-metal-SPI-flash-driver
|
├── include
│   ├── flash_commands.h
│   ├── flash.h
│   ├── spi.h
│   ├── stdint.h
│   └── stm32f103xx.h
├── linker
│   └── stm32f103.ld
├── src
│   ├── flash.c
│   ├── main.c
│   └── spi.c
├── startup
│   └── startup.c
├── Makefile
└── README.md
