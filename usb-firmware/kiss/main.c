// ======================================================================
// USBtiny template application
//
// Copyright 2006-2010 Dick Streefland
//
// This is free software, licensed under the terms of the GNU General
// Public License as published by the Free Software Foundation.
// ======================================================================

#include "usb.h"
#include "../lib/usi.h"
#include "../../operations.h"

// ----------------------------------------------------------------------
// Handle a non-standard SETUP packet.
// ----------------------------------------------------------------------
extern byte_t usb_setup ( byte_t data[8] )
{
  switch(data[0]) {
    case COMMAND_USI_INIT:
      usi_spi_init(data[1]);
      return 0;
    case COMMAND_USI_STOP:
      usi_spi_stop();
      return 0;
    case COMMAND_USI_SPI:
      return 0xff;
  }
  return 0;
}

// ----------------------------------------------------------------------
// Handle an IN packet. (USBTINY_CALLBACK_IN==1)
// ----------------------------------------------------------------------
extern byte_t usb_in ( byte_t* data, byte_t len )
{
  usi_spi(data,data,len);
  return len;
}

// ----------------------------------------------------------------------
// Handle an OUT packet. (USBTINY_CALLBACK_OUT==1)
// ----------------------------------------------------------------------
extern void usb_out ( byte_t* data, byte_t len )
{
  usi_spi(data,data,len);
}

// ----------------------------------------------------------------------
// Main
// ----------------------------------------------------------------------
__attribute__((naked))
extern int main ( void )
{
 usb_init();
 for ( ;; )
 {
  usb_poll();
 }
}

void exit(int __status) {}
