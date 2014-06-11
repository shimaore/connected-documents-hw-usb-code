#ifndef __USI_H
#define __USI_H

#include <avr/io.h>

/*
 * USI in SPI Master mode
 */

#define  USI_PORT   PORTB
#define  USI_DDR    DDRB
#define  USI_PIN    PINB

#define  USI_RESET  PB0    // output
#define  USI_MOSI   PB1    // output
#define  USI_MISO   PB2    // input
#define  USI_SCK    PB3    // output

#define  USI_RESET_MASK   _BV(USI_RESET)
#define  USI_MOSI_MASK    _BV(USI_MOSI)
#define  USI_MISO_MASK    _BV(USI_MISO)
#define  USI_SCK_MASK     _BV(USI_SCK)

#define USI_SPI_MASK (USI_RESET_MASK | USI_SCK_MASK | USI_MOSI_MASK)
#define USI_SPI_PORT (USI_SPI_MASK | USI_MISO_MASK)

inline void usi_spi_init( uint8_t reset ) {
  USI_DDR |= USI_SPI_MASK;
  if(reset) {
    USI_PORT |= USI_RESET_MASK;
  } else {
    USI_PORT &= ~ USI_RESET_MASK;
  }
}

inline void usi_spi_stop() {
  USI_DDR &= ~ USI_SPI_MASK;
  USI_PORT &= ~ USI_SPI_PORT;
}

void usi_spi ( uint8_t* cmd, uint8_t* res, uint8_t n );

#endif
