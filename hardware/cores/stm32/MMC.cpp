#include <unistd.h>
#include "MMC.h"

const uint8_t CMD0_GO_IDLE_STATE[]={0x40,0,0,0,0,0x95};
const uint8_t CMD1_SEND_OP_COND[]={0x41,0,0,0,0,0xFF};
const uint8_t CMD5_IO_SEND_OP_COND[]={0x45,0,0,0,0,0xFF};
const uint8_t CMD8_IO_SEND_OP_COND[]={0x48,0,0,1,0xAA,0x87};
const uint8_t CMD13_SEND_STATUS[]={0x40+13,0,0,0,0,0xFF};
const uint8_t CMD55_APP_CMD[]={0x40+55,0,0,0,0,0xFF};
const uint8_t ACMD41_SD_SEND_OP_COND[]={0x40+41,0,0,0,0,0xFF};

/*******************************/ 
/* MMC commuications layer */
/*******************************/ 
extern volatile unsigned int timeout;
extern "C" void delay(unsigned int ms );

uint8_t MMC::wait_response(uint8_t response, uint8_t mask)
{
	uint8_t din;
	timeout = 500;	/* Wait for ready in timeout of 500ms */

	din = spi.txrx_byte(0xFF);
	while (( (din&mask) != (response&mask) ) && timeout)
	{
		din = spi.txrx_byte(0xFF);	
	};
	return din;	
};

uint8_t MMC::command_response (const uint8_t *cmd, uint8_t * rspdata, int rsplength)
{
	int count, retry=0;
	uint8_t rxchar;

	spi.select();	
	if (wait_response(READY_TOKEN, READY_TOKEN) != READY_TOKEN) 
	{
		spi.deselect();
		return 0xFF;
	}
	
	/******** Send command ************/
	spi.txrx(cmd,NULL,MMC_CMD_SIZE);
	spi.txrx_byte(0xFF);

	/******** Get response ************/
	rxchar = wait_response(0, 0x80); /* Wait for byte with MSB == 0 */
	if (rxchar != 0) 
	{
		spi.deselect();
		return rxchar;
	}

	rspdata[0]=rxchar;
	spi.txrx(NULL,rspdata+1,rsplength-1);

	return rxchar;
}


/******************************************
simple command without data phase
*******************************************/
uint8_t MMC::command(const uint8_t *cmd, uint8_t * rspdata, int rsplength)
{
	int count, retry=0;
	uint8_t rxchar;

	command_response (cmd, rspdata, rsplength);

	/******** Data phase ************/

	spi.deselect();
	spi.txrx_byte(0xFF);
	
	return rxchar;	
};

/******************************************
command with single data phase
*******************************************/


/******************************************
card initialisation
*******************************************/

uint8_t MMC::mmc_init(void)
{
	int count;
	
	spi.deselect();
	for (int i=0;i<10;i++) spi.txrx_byte(0xFF);
	
	command(CMD0_GO_IDLE_STATE, mmc_response_buffer, 1);
	
	count = 0;
	do 
	{
		spi.txrx(CMD55_APP_CMD,NULL,MMC_CMD_SIZE);
		command(ACMD41_SD_SEND_OP_COND, mmc_response_buffer, 1);
		/* Here we can check response to identify SD cards */
	} while ((mmc_response_buffer[0] != 0) && (++count<10));
	
	do 
	{
		command(CMD1_SEND_OP_COND, mmc_response_buffer, 1);
		/* Here we can check response to identify SD cards */
	} while ((mmc_response_buffer[0] != 0) && (++count<10));
	
	return 0;
};

