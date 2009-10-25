#ifndef Spi_h
#define Spi_h

#include <inttypes.h>

class Spi
{
  public:
  virtual void begin()=0;
  /* send and receive one byte */
  virtual uint8_t txrx_byte(uint8_t dataout)=0;
  /* send a block of data and return last received byte */
  uint8_t tx(uint8_t * dataout,int count);
  /* send and receive a block of data */
  uint8_t txrx(uint8_t * datainout,int count);
  uint8_t txrx(const uint8_t * dataout, uint8_t * datain, int count);
  virtual void select()=0;
  virtual void deselect()=0;
};

#endif

