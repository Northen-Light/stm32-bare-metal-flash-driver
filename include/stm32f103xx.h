#ifndef STM32F103xx_H
#define STM32F103xx_H
                                                                
#define RCC_APB2ENR                     *((volatile uint32_t *) 0x40021018U)

#define GPIOA_CRL                       *((volatile uint32_t *) 0x40010800U)
#define GPIOA_BSRR                      *((volatile uint32_t *) 0x40010810U)

#define SPI1_CR1                        *((volatile uint32_t *) 0x40013000U)
#define SPI1_SR                         *((volatile uint32_t *) 0x40013008U)
#define SPI1_DR                         *((volatile  uint8_t *) 0x4001300CU)

#endif