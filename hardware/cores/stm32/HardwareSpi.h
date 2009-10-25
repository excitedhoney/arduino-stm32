#ifndef HardwareSpi_h
#define HardwareSpi_h

#include <inttypes.h>

#include "Spi.h"
#include "stm32f10x_map.h"

class HardwareSpi : public Spi
{
  public:
  HardwareSpi(SPI_TypeDef * _spi);
  void begin();
  /* send and receive one byte */
  uint8_t txrx_byte(uint8_t dataout);
  void select(){};
  void deselect(){};

  private:
  SPI_TypeDef * spi;
  void wait_empty();

};

#endif

