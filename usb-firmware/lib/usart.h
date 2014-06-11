#ifndef __USART_H
#define __USART_H

#include <avr/io.h>

/*
 * USART in SPI mode
 */

#define  USART_PORT   PORTD
#define  USART_DDR    DDRD
#define  USART_PIN    PIND

#define  USART_CS   PD6   // output
#define  USART_XCK  PD2   // output
#define  USART_RxD  PD0   // output
#define  USART_TxD  PD1   // input

#define  USART_CS_MASK    _BV(USART_CS)
#define  USART_XCK_MASK   _BV(USART_XCK)
#define  USART_RxD_MASK   _BV(USART_RxD)
#define  USART_TxD_MASK   _BV(USART_TxD)

#define USART_SPI_MASK  (USART_XCK_MASK|USART_TxD_MASK|USART_CS_MASK)
#define USART_SPI_PORT  (USART_SPI_MASK | USART_RxD_MASK)

#define  XCK_MASK         _BV(XCK)

#define  UMSEL0 6
#define  UMSEL1 7
#define  UCPHA  1

/* See doc8246, page 149 */

inline void usart_spi_init( uint8_t reset ) {
  UBRRH = 0;
  UBRRL = 0;
  /* Setting the XCK port pin as output, enables master mode. */
  USART_DDR |= USART_SPI_MASK;
  /* Set MSPI mode of operation and SPI data mode 0. */
  UCSRC = (1<<UMSEL1)|(1<<UMSEL0)|(0<<UCPHA)|(0<<UCPOL);
  /* Enable receiver and transmitter. */
  UCSRB = (1<<RXEN)|(1<<TXEN);
  /* Set baud rate. */
  /* IMPORTANT: The Baud Rate must be set after the transmitter is enabled
   * */

  /* Max Speed */
  UBRRH = 0;
  UBRRL = 0;

  USART_DDR |= USART_SPI_MASK;
  if(reset) {
    USART_PORT |= USART_CS_MASK;
  } else {
    USART_PORT &= ~ USART_CS_MASK;
  }
}

inline void usart_spi_stop() {
  USART_DDR &= ~ USART_SPI_MASK;
  USART_PORT &= ~ USART_SPI_PORT;
}

void usart_spi ( uint8_t* cmd, uint8_t* res, uint8_t n );

#endif
