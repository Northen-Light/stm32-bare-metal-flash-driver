#ifndef STM32F103xx_H
#define STM32F103xx_H

#include <stdint.h>

#define RCC_APB2ENR                                       *((volatile uint32_t *) 0x40021018U)
#define GPIOA_CRL                                         *((volatile uint32_t *) 0x40010800U)
#define GPIOA_BSRR                                        *((volatile uint32_t *) 0x40010810U)
#define SPI1_CR1                                          *((volatile uint32_t *) 0x40013000U)
#define SPI1_SR                                           *((volatile uint32_t *) 0x40013008U)
#define SPI1_DR                                           *((volatile  uint8_t *) 0x4001300CU)

#define RCC_APB2ENR_SPI1EN                                (1U << 12)
#define RCC_APB2ENR_IOPAEN                                (1U << 2)
#define GPIOA_CRL_PIN4_MASK                               (0x0FU << 16)
#define GPIOA_CRL_PIN4_GP_PP_10MHZ                        (0x01U << 16)
#define GPIOA_CRL_PIN5_MASK                               (0x0FU << 20)
#define GPIOA_CRL_PIN5_AF_PP_10MHZ                        (0x09U << 20)
#define GPIOA_CRL_PIN6_MASK                               (0x0FU << 24)
#define GPIOA_CRL_PIN6_INPUT_FLOATING                     (0x04U << 24)
#define GPIOA_CRL_PIN7_MASK                               (0x0FU << 28)
#define GPIOA_CRL_PIN7_AF_PP_10MHZ                        (0x09U << 28)
#define GPIOA_BSRR_PIN4_SET                               (1U << 4)
#define GPIOA_BSRR_PIN4_RESET                             (1U << (16 + 4))

#define SPI1_CR1_RESET                                    0
#define SPI1_CR1_MSTR                                     (1U << 2)
#define SPI1_CR1_BR_DIV_8                                 (0x02U << 3)
#define SPI1_CR1_SSI                                      (1U << 8)
#define SPI1_CR1_SSM                                      (1U << 9)
#define SPI1_CR1_SPE                                      (1U << 6)

#define SPI1_SR_RXNE                                      (1U << 0)
#define SPI1_SR_TXE                                       (1U << 1)

#endif