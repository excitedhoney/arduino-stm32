#include <unistd.h>
#include "HardwareSpi.h"
#include "wiring_private.h"

/************************************************************************
How do we handle slave select ?

Client code uses digitalWrite(CSpin, 0/1) ?

Client code calls spi->select()/deselect() ?

The select pin is not an attribute of the Spi interface, it is rather the 
attibute of a connected Spi slave unit. The SpiSlave class uses a Spi interface
and when the slave calls select then all other slaves using the same Spi interface
must be deselected.

Each Spi slave can also have different mode and bitorder settings.

************************************************************************/
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
#define SPE 0x0040

HardwareSpi::HardwareSpi(SPI_TypeDef * _spi)
{
  spi = _spi;
/* Configure pads */

/* Configure SPI interface */

/* Enable SPI interface */

};

void HardwareSpi::begin(/* khz, master*/)
{
	// Enable perhipherial clock
	if (spi == SPI1 )
	{
		RCC->APB2ENR = RCC_APB2ENR_SPI1EN;
//		RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1, ENABLE);
		// NVIC->ISER[SPI1_IRQChannel/32] = (1<<(SPI1_IRQChannel%32));
		// Configure peripherial pins, NSS - PA4, SCK - PA5, MISO - PA6, MOSI PA7
		// Master does not use NSS
		GPIOA->CRL = (GPIOA->CRL & 0x000FFFFF) | 0xA4A00000;
	}
	if (spi == SPI2 )
	{
		// USART2 runs on APB1 clock which is HCLK/2 = 36 MHz
		RCC->APB1ENR = RCC_APB1ENR_SPI2EN;
//		RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI2, ENABLE);
		// NVIC->ISER[SPI2_IRQChannel/32] = (1<<(SPI2_IRQChannel%32));
		// Configure peripherial pins, NSS - PB12, SCK - PB13, MISO - PB14, MOSI PB15
		// Master does not use NSS
		GPIOB->CRH = (GPIOB->CRH & 0x000FFFFF) | 0xA4A00000;
	}
	if (spi == SPI3 )
	{
		// USART3 runs on APB1 clock which is HCLK/2 = 36 MHz
		RCC->APB1ENR = RCC_APB1ENR_SPI3EN;
//		RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI3, ENABLE);
		// NVIC->ISER[SPI2_IRQChannel/32] = (1<<(SPI2_IRQChannel%32));
		// Configure peripherial pins, NSS - PA15, SCK - PB3, MISO - PB4, MOSI PB5
		// Master does not use NSS
		GPIOB->CRL = (GPIOB->CRL & 0xFF000FFF) | 0x00A4A000;
	}
	spi->CR1 = SPI_CR1_SPE | SPI_CR1_SSM | SPI_CR1_MSTR | SPI_BaudRatePrescaler_64; /* LSB | SSM | SSI | ENABLE | MASTER | BAUDRATE | POLARITY | PHASE */
	spi->CR2 = 0;
};

uint8_t HardwareSpi::txrx_byte(uint8_t dataout)
{
  uint8_t datain;
  spi->DR = dataout;
  while (spi->SR & SPI_SR_RXNE) {};
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

