#include "usi.h"

void usi_spi ( uint8_t* cmd, uint8_t* res, uint8_t n )
{
  const uint8_t clock_off = _BV(USIWM0) | _BV(USITC);
  const uint8_t clock_on  = _BV(USIWM0) | _BV(USITC) | _BV(USICLK);

  while  ( n != 0 )
  {
    n--;
    USIDR = *cmd++;

    for( uint8_t i = 0; i < 8; i++ ) {
      USICR = clock_off;
      USICR = clock_on;
    }

    *res++ = USIDR;
  }
}
