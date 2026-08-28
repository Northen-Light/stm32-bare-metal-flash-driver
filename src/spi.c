#include "stm32f103xx.h"
#include "spi.h"

void spi1_init(void) {
  RCC_APB2ENR |= (1U << 12);
  RCC_APB2ENR |= (1U << 2);

  GPIOA_CRL &= ~(0x0FU << 20);
  GPIOA_CRL |= (0x09U << 20);
  GPIOA_CRL &= ~(0x0FU << 24);
  GPIOA_CRL |= (0x04U << 24);
  GPIOA_CRL &= ~(0x0FU << 28);
  GPIOA_CRL |= (0x09U << 28);

  SPI1_CR1 = 0;
  SPI1_CR1 |= (1U << 2);
  SPI1_CR1 |= (1U << 3);
  SPI1_CR1 |= (1U << 8);
  SPI1_CR1 |= (1U << 9);
  SPI1_CR1 |= (1U << 6);
}

uint8_t spi1_transfer(uint8_t byte) {
  while ((SPI1_SR & (1U << 1)) == 0) {};
  SPI1_DR = byte;
  while ((SPI1_SR & (1U << 0)) == 0) {};
  return SPI1_DR;
}