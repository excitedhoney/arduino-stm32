#ifndef MMC_h
#define MMC_h

#include <inttypes.h>
#include "Spi.h"

#define READY_TOKEN 0xFF
#define DATA_TOKEN 0xFE

#define MMC_CMD_SIZE  6
class MMC
{
  public:
	MMC(Spi & _spi):spi(_spi){};
	uint8_t wait_response(uint8_t response, uint8_t mask);
	uint8_t command(const unsigned char *cmd, uint8_t * rxdata, int rlength);
	uint8_t mmc_init(void);

  private:
	Spi & spi;
	uint8_t mmc_cmd_buffer[MMC_CMD_SIZE];
	uint8_t mmc_response_buffer[6];
	uint8_t command_response (const uint8_t *cmd, uint8_t * rspdata, int rsplength);

};
#endif

