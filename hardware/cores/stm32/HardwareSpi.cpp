#include <unistd.h>
#include "HardwareSpi.h"

uint8_t Spi::txrx(const uint8_t * dataout, uint8_t * datain, int count)
{
  uint8_t recv,send=0xFF;
  while (count>0)
  {
    if (dataout) send = *dataout++;
    recv = txrx_byte(send);
    if (datain) *datain++ = recv;
    count--;
  }
  return recv;
};

uint8_t Spi::txrx(uint8_t * datainout,int count)
{
  return txrx(datainout,datainout,count);
};

uint8_t Spi::tx(uint8_t * dataout,int count)
{
  return txrx(dataout,NULL,count);
};

/*******************************/ 
/* Hardware SPI implementation */
/*******************************/ 

HardwareSpi::HardwareSpi(SPI_TypeDef * _spi)
{
  spi = _spi;
/* Configure pads */

/* Configure SPI interface */

/* Enable SPI interface */

};

void HardwareSpi::begin()
{

};

uint8_t HardwareSpi::txrx_byte(uint8_t dataout)
{
  uint8_t datain;
  spi->DR = dataout;
  while (spi->SR & 0x1) {};
  datain = spi->DR;
  return datain;
};

#if 0
#ifdef __cplusplus
extern "C" {
#endif
void SPI1_IRQHandler(void);
void SPI2_IRQHandler(void);
void SPI3_IRQHandler(void);
#ifdef __cplusplus
};
#endif
#endif

