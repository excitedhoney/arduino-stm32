/*
stmdude

A minimal uploader for STM32 Cortex-M3 chips with a command line interface similar to avrdude 

Linux build:
$gcc -o stmdude main.c hexfile.c serialport.c


2009, Copyright Magnus Lundin
lundin@mlu.mine.nu

*/
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#include "hexfile.h"

#define FLASHBASE 0x08000000
#define RAMBASE   0x20000000

int baudrate = 38400;
char * port = "/dev/ttyUSB0";
int portfd = -1;
int filefd = -1;
int verbose = 0;

struct 
{
	char commands[16];
	char id[8];	
	int pagesize;	
}	
target =
{
	{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,0},
	0
};

void opentargetport()
{
	if (portfd<0)
	{
		portfd = open(port,O_NONBLOCK | O_RDWR);
		if (portfd<0)
		{
			fprintf(stderr,"Could not open port [%s] \n",port);
			_exit(-1);
		}
	}
	// Set serial port parameters E81, raw
	setPortConfig(portfd, baudrate);
}

void closeport()
{
	if (portfd>0) close(portfd);
	portfd = -1;
}

/***********************************************************************

stm32 bootloader  command handlers

************************************************************************/

int bootloader_receive_byte()
{
	int errorcount = 0;
	int result;
	unsigned char buf[2];
	while (errorcount++<100)
	{
		result = read(portfd,buf,1);
		if (result<0) 
		{
			if (errno != 11) break;
			usleep(10000);
		}
		else
			break;
	}
	if (result<0) 
	{
		printf("Error %i:%s\n",errno,strerror(errno));
	}
	return buf[0];
}

int bootloader_wait_ack()
{
	unsigned char ack = bootloader_receive_byte();
	if (ack != 0x79)
	{
		if (ack == 0x1F )
		{
			printf("NACK\n");
			return 0;
		}
		else
		{
			printf("Read ack: 0x%02x\n",ack);
			return 0;
		}
	}
	else
	{
		if (verbose>3) printf("ACK\n");
		return 1;
	}
}

int bootloader_send_command(unsigned char cmd)
{
	unsigned char buf[2];
	buf[0]=cmd;
	buf[1]=~cmd;
	write(portfd,buf,2);
	return bootloader_wait_ack();
}

int bootloader_send_address(unsigned int addr)
{
	int k;
	unsigned char buf[5];
	buf[4]=0;
	for (k = 3; k>=0; k--)
	{
		buf[k] = addr&0xFF;
		addr = addr>>8;
		buf[4] = buf[4]^buf[k];
	}
	write(portfd,buf,5);
	return bootloader_wait_ack();
}

void bootloader_connect()
{
	int ack=0;
	int count=0;
	unsigned char buf[2]={0x7F,0x7F};
	while (!ack & count++<2)
	{
		write(portfd,buf,1);
		ack = bootloader_wait_ack();
	}
}

void bootloader_get_id()
{
	int k,i,data;
	int ack;
	if (verbose>=0) printf("Bootloader get id\n");
	ack = bootloader_send_command(0x02);
	if (! ack ) return;
	k = bootloader_receive_byte();
	if (verbose>=2) printf("N-1 is %i\n",k);
	for (i=0;i<=k;i++)
	{
		data = bootloader_receive_byte(); 
		if (verbose>=1) printf("\t%x\n",data);
	}
	bootloader_wait_ack();
}

void bootloader_get_commands()
{
	int k,i,data;
	int ack;
	if (verbose>=0) printf("Bootloader get commands\n");
	ack = bootloader_send_command(0x00);
	if (! ack ) return;
	k = bootloader_receive_byte();
	if (verbose>=2) printf("N-1 is %i\n",k);
	for (i=0;i<=k;i++)
	{
		data = bootloader_receive_byte(); 
		if (verbose>=1) printf("\t%x\n",data);
	}
	bootloader_wait_ack();
}

void bootloader_go(unsigned int addr)
{
	int k;
	printf("Bootloader go\n");
}

void bootloader_read_memory(int addr,int len,char * readbuffer)
{
	int count=0, thisblock, k;
	if (verbose>=0) printf("Bootloader read 0x%08x (%i)\n",addr,len);
	while (count<len)
	{
		int ack;
		thisblock = len-count;
		if (thisblock>256) thisblock = 256;
		ack = bootloader_send_command(0x11);
		if (! ack ) return;
		ack = bootloader_send_address(addr+count);
		if (! ack ) return;
		ack = bootloader_send_command(thisblock-1);
		if (! ack ) return;
		for (k=0; k<thisblock; k++)
		{
			readbuffer[k+count] = bootloader_receive_byte();
		}
		count += thisblock;
	}
	for(k=0;k<len;k++) 
	{
		if (k%32 == 0) printf("\n0x%08X:  ",addr+k);
		printf("%2.2x ",(unsigned char)readbuffer[k]);
	}
	printf("\n\n");
}

int bootloader_write_memory(int addr,int len,char * outbuffer)
{
	int count=0, thisblock, k;
	if (verbose>1) printf("Bootloader write memory 0x%08x (%i)\n",addr,len);
	while (count<len)
	{
		int ack;
		unsigned char check, n;
		thisblock = len-count;
		if (thisblock>256) thisblock = 256;
		ack = bootloader_send_command(0x31);
		if (! ack ) return -1;
		ack = bootloader_send_address(addr+count);
		if (! ack ) return -1;
		n = thisblock-1;
		check = n;
		write(portfd,&n,1);
		for (k=0; k<thisblock; k++)
		{
			write(portfd,&outbuffer[k+count],1);
			check = check^outbuffer[k+count];
		}
		write(portfd,&check,1);
		ack = bootloader_wait_ack();
		if (! ack ) return -1;
		count += thisblock;
	}
	return 0;
}

void bootloader_erase_memory()
{
	int k;
	int ack;
	if (verbose>0) printf("Bootloader erase memory\n");
	ack = bootloader_send_command(0x43);
	if (! ack ) return;
	/* Erase All */
	ack = bootloader_send_command(0xFF);
	if (! ack ) return;
}

void bootloader_write_unprotect()
{
	int k;
	printf("Bootloader write unprotect\n");
}



/***********************************************************************

avrdude command handlers

************************************************************************/


void memop(char * cmdstr)
{
	char * memtype;
	char * op;
	char * filename;
	char * format = "i";
	intelln_t * hexdata;
	
	int k=0;
	int result;
	int len = strlen(cmdstr);

	memtype = cmdstr;
	while( (k<len) && (cmdstr[k]!=':')) k++;
	if (k>=len) return;
	cmdstr[k++] = 0;
	op = cmdstr+k;
	while( (k<len) && (cmdstr[k]!=':')) k++;
	if (k>=len) return;
	cmdstr[k++] = 0;
	filename = cmdstr+k;
	while( (k<len) && (cmdstr[k]!=':')) k++;
	if (cmdstr[k]==':')
	{
		cmdstr[k++] = 0;
		format = cmdstr+k;
	}

	if (verbose>0) printf("Memoryoperation %s to %s with data from %s and format %s\n",op,memtype,filename,format);

	if (( op[0]=='w' )||( op[0]=='v' ))
		hexdata = loadhexfile(filename);
	
	// Write hexdata to target
	if (hexdata && ( op[0]=='w' ))
	{
		intelln_t * curhexline = hexdata;
		/* Send write command */
		char outbuffer[256];
		int addr = 0;
		int len = 0;
		while (curhexline && (curhexline->type==0))
		{
			if (len==0) addr=curhexline->addr;
			if (strcmp(memtype,"flash")==0) addr |= FLASHBASE;
			if (strcmp(memtype,"ram")==0) addr |= RAMBASE;
			memcpy(outbuffer+len,curhexline->data,curhexline->len);
			len += curhexline->len;
			if ((len==256) || (curhexline->len<16))
			{
				result = bootloader_write_memory(addr,len,outbuffer);
				len = 0;
				if (result < 0)
				{
					fprintf(stderr,"Error in memory write\n");
					return;
				}
			}
			curhexline = curhexline->next;
		}
		if (verbose>=0) printf("Memory write finished without error\n");
	}
	
	// Read data from target
	if (( op[0]=='r' ))
	{
		/* We need address and length */
	
	}

	if (hexdata) deletehexdata(hexdata);
	hexdata = NULL;
}

int main(int argc, char * argv[])
{
	int k;
	int eraseflag = 0;
	int lastopt = 0;
	unsigned char data[16000];
	
	for (k=0; k<argc;k++)
	{
		printf("[%s]",argv[k]);
	}
	printf("\n");

	for (k=1; k<argc;k++)
	{
		if (strncmp("-P",argv[k],2)==0)
		{
			if (argv[k][2])
			{
				port = argv[k]+2;
			}
			else
			{
				port = argv[++k];
			}
			lastopt = k;
			closeport();
		}
		if (strncmp("-b",argv[k],2)==0)
		{
			if (argv[k][2])
			{
				baudrate = atoi(argv[k]+2);
			}
			else
			{
				baudrate = atoi(argv[++k]);
			}
			lastopt = k;
			closeport();
		}
		if (strcmp("-e",argv[k])==0)
		{
			eraseflag = 1;;
			lastopt = k;
		}
		if (strcmp("-v",argv[k])==0)
		{
			verbose += 1;;
			lastopt = k;
		}
		if (strcmp("-q",argv[k])==0)
		{
			verbose -= 1;;
			lastopt = k;
		}
	}
	
	printf("Using port %s with baudrate %i \n",port,baudrate);
	opentargetport();
	bootloader_connect();
	bootloader_get_commands();
	bootloader_get_id();
	if (eraseflag) bootloader_erase_memory();

	for (k=1; k<argc;k++)
	{
		if (strncmp("-U",argv[k],2)==0)
		{
			if (argv[k][2])
			{
				memop(argv[k]+2);
			}
			else
			{
				memop(argv[++k]);			
			}
			lastopt = k;
		}
	}

	closeport();
	return 0;
}
