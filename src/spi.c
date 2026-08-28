#include "stm32f103xx.h"
#include "spi.h"

void spi1_init(void) {
  RCC_APB2ENR |= RCC_APB2ENR_SPI1EN;
  RCC_APB2ENR |= RCC_APB2ENR_IOPAEN;

  GPIOA_CRL &= ~GPIOA_CRL_PIN5_MASK;
  GPIOA_CRL |= GPIOA_CRL_PIN5_AF_PP_10MHZ;
  GPIOA_CRL &= ~GPIOA_CRL_PIN6_MASK;
  GPIOA_CRL |= GPIOA_CRL_PIN6_INPUT_FLOATING;
  GPIOA_CRL &= ~GPIOA_CRL_PIN7_MASK;
  GPIOA_CRL |= GPIOA_CRL_PIN7_AF_PP_10MHZ;

  SPI1_CR1 = SPI1_CR1_RESET;
  SPI1_CR1 = SPI1_CR1_MSTR |  
             SPI1_CR1_BR_DIV_8 | 
             SPI1_CR1_SSI |
             SPI1_CR1_SSM;
  SPI1_CR1 |= SPI1_CR1_SPE;
}

uint8_t spi1_transfer(uint8_t byte) {
  while ((SPI1_SR & SPI1_SR_TXE) == 0) {};
  SPI1_DR = byte;
  while ((SPI1_SR & SPI1_SR_RXNE) == 0) {};
  return SPI1_DR;
}