/// ======================================================================
// USB AVR programmer and SPI interface
//
// http://www.ladyada.net/make/usbtinyisp/
//
// This code works for both v1.0 and v2.0 devices.
//
// Copyright 2006-2010 Dick Streefland
//
// This is free software, licensed under the terms of the GNU General
// Public License as published by the Free Software Foundation.
// ======================================================================

#include <avr/io.h>
#include "def.h"
#include "usb.h"

#include "usart.h"
#include "usi.h"

#define mcu_spi usart_spi
#define mcu_spi_init usart_spi_init
#define mcu_spi_stop usart_spi_stop

#define flash_spi usi_spi
#define flash_spi_init usi_spi_init
#define flash_spi_stop usi_spi_stop

inline void led() {
  DDRB |= _BV(PINB1);
  PORTB &= ~ _BV(PINB1);
}

enum
{
  // Generic requests
  USBTINY_ECHO,    // echo test
  USBTINY_READ,    // read byte
  USBTINY_WRITE,    // write byte
  USBTINY_CLR,    // clear bit
  USBTINY_SET,    // set bit
  // Programming requests
  USBTINY_POWERUP,      // apply power (wValue:SCK-period, wIndex:RESET)
  USBTINY_POWERDOWN,    // remove power from chip
  USBTINY_SPI,          // issue SPI command (wValue:c1c0, wIndex:c3c2)
  USBTINY_POLL_BYTES,   // set poll bytes for write (wValue:p1p2)
  USBTINY_FLASH_READ,   // read flash (wIndex:address)
  USBTINY_FLASH_WRITE,  // write flash (wIndex:address, wValue:timeout)
  USBTINY_EEPROM_READ,  // read eeprom (wIndex:address)
  USBTINY_EEPROM_WRITE, // write eeprom (wIndex:address, wValue:timeout)
  USBTINY_DDRWRITE,     // set port direction
  USBTINY_SPI1,         // a single SPI command

  USBFLASH_INIT,          // select flash (bIndex:RESET)
  USBFLASH_STOP,          // unselect flash
  USBFLASH_COMMAND_BLOCK  // issue SPI command

};

// ----------------------------------------------------------------------
// Local data
// ----------------------------------------------------------------------
static  byte_t    sck_period;  // SCK period in microseconds (1..250)
static  byte_t    poll1;    // first poll byte for write
static  byte_t    poll2;    // second poll byte for write
static  uint_t    address;  // read/write address
static  enum { use_mcu, use_flash } use_type;
static  uint_t    timeout;  // write timeout in usec
static  byte_t    cmd0;    // current read/write command byte
static  byte_t    cmd[4];    // SPI command buffer
static  byte_t    res[4];    // SPI result buffer

// ----------------------------------------------------------------------
// Create and issue a read or write SPI command.
// ----------------------------------------------------------------------
static  void  spi_rw ( void )
{
  uint_t  a;

  a = address++;
  if  ( cmd0 & 0x80 )
  {  // eeprom
    a <<= 1;
  }
  cmd[0] = cmd0;
  if  ( a & 1 )
  {
    cmd[0] |= 0x08;
  }
  cmd[1] = a >> 9;
  cmd[2] = a >> 1;
  mcu_spi( cmd, res, 4 );
}

// ----------------------------------------------------------------------
// Handle a non-standard SETUP packet.
// ----------------------------------------------------------------------
extern  byte_t  usb_setup ( byte_t data[8] )
{
  byte_t  req;

  // Generic requests
  req = data[1];
  address = * (uint_t*) & data[4];
  use_type = use_mcu;
  led();

  switch(req) {
    case USBFLASH_INIT:
      use_type = use_flash;
      flash_spi_init(data[4]);
      return 0;

    case USBFLASH_STOP:
      use_type = use_flash;
      flash_spi_stop();
      return 0;

    case USBFLASH_COMMAND_BLOCK:
      use_type = use_flash;
      return 0xff;

    // Programming requests
    case USBTINY_POWERUP:
      sck_period = data[2];
      mcu_spi_init(data[4]);
      return 0;

    case USBTINY_POWERDOWN:
      mcu_spi_stop();
      return 0;

    case USBTINY_SPI:
      mcu_spi( data + 2, data + 0, 4 );
      return 4;

    case USBTINY_SPI1:
      mcu_spi( data + 2, data + 0, 1 );
      return 1;

    case USBTINY_POLL_BYTES:
      poll1 = data[2];
      poll2 = data[3];
      return 0;

    case USBTINY_FLASH_READ:
      cmd0 = 0x20;
      return 0xff;  // usb_in() will be called to get the data

    case USBTINY_EEPROM_READ:
      cmd0 = 0xa0;
      return 0xff;  // usb_in() will be called to get the data

    case USBTINY_FLASH_WRITE:
      cmd0 = 0x40;
      timeout = * (uint_t*) & data[2];
      return 0;  // data will be received by usb_out()

    case USBTINY_EEPROM_WRITE:
      cmd0 = 0xc0;
      timeout = * (uint_t*) & data[2];
      return 0;  // data will be received by usb_out()

  }
  return 0;
}

// ----------------------------------------------------------------------
// Handle an IN packet.
// ----------------------------------------------------------------------
extern  byte_t  usb_in ( byte_t* data, byte_t len )
{
  if(use_type == use_flash) {
    flash_spi(data,data,len);
    return len;
  }

  byte_t  i;

  for  ( i = 0; i < len; i++ )
  {
    spi_rw();
    data[i] = res[3];
  }
  return len;
}

// ----------------------------------------------------------------------
// Handle an OUT packet.
// ----------------------------------------------------------------------
extern  void  usb_out ( byte_t* data, byte_t len )
{
  if(use_type == use_flash) {
    flash_spi(data,data,len);
    return;
  }

  byte_t  i;
  uint_t  usec;
  byte_t  r;

  for  ( i = 0; i < len; i++ )
  {
    cmd[3] = data[i];
    spi_rw();
    cmd[0] ^= 0x60;  // turn write into read
    for  ( usec = 0; usec < timeout; usec += 32 * sck_period )
    {  // when timeout > 0, poll until byte is written
      mcu_spi( cmd, res, 4 );
      r = res[3];
      if  ( r == cmd[3] && r != poll1 && r != poll2 )
      {
        break;
      }
    }
  }
}

// ----------------------------------------------------------------------
// Main
// ----------------------------------------------------------------------
__attribute__((naked))
extern  int  main ( void )
{
  usb_init();
  for  ( ;; )
  {
    usb_poll();
    led();
  }
}

void exit(int __status) {}
