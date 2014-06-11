#ifndef __USI_H
#define __USI_H

#include <avr/io.h>

/*
 * USI in SPI Master mode
 */

#define  USI_PORT   PORTB
#define  USI_DDR    DDRB
#define  USI_PIN    PINB

#define  USI_SELECT PB4    // output
#define  USI_MOSI   PB5    // output
#define  USI_MISO   PB6    // input
#define  USI_SCK    PB7    // output


#define  USI_SELECT_MASK  _BV(USI_SELECT)
#define  USI_MOSI_MASK    _BV(USI_MOSI)
#define  USI_MISO_MASK    _BV(USI_MISO)
#define  USI_SCK_MASK     _BV(USI_SCK)

#define USI_SPI_MASK (USI_SELECT_MASK | USI_SCK_MASK | USI_MOSI_MASK)
#define USI_SPI_PORT (USI_SPI_MASK | USI_MISO_MASK)

inline void usi_spi_init() {
  // Actively wait for the bus to be available.
  while((USI_PIN & USI_SPI_PORT) != USI_SPI_PORT) {
    // Another device is accessing.
  }
  // Force the select bit down (clear-to-select).
  USI_PORT &= ~ USI_SELECT_MASK;
  // Mark outputs.
  USI_DDR |= USI_SPI_MASK;
}

inline void usi_spi_stop() {
  // Mark all as inputs.
  USI_DDR &= ~ USI_SPI_PORT;
  // Enable pull-ups.
  USI_PORT |= USI_SPI_PORT;
}

void usi_spi ( uint8_t* cmd, uint8_t* res, uint8_t n );

#endif
