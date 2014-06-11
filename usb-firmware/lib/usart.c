#include "usart.h"

void usart_spi ( uint8_t* cmd, uint8_t* res, uint8_t n ) {
  while  ( n != 0 )
  {
    n--;

    /* Wait for empty transmit buffer */
    while ( !( UCSRA & _BV(UDRE)) );
    /* Put data into buffer, sends the data */
    UDR = *cmd++;

    /* Wait for data to be received */
    while ( !(UCSRA & _BV(RXC)) );
    /* Get and return received data from buffer */
    *res++ = UDR;
  }
}
