#include <termios.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/signal.h>
#include <sys/types.h>

#include "serialport.h"

/*
http://notes.ump.edu.my/fkee/e-Books/C%20Programming%20&%20PC%20interfacing/Serial%20port%20programming%20for%20Windows%20and%20Linux.pdf

http://msdn.microsoft.com/en-us/library/ms810467.aspx#serial_topic6
*/

serport_t openSerialPort(const char * portname, int portflags)
{
#ifndef WIN32
	return open(portname, portflags);
#else	
	HANDLE fileHandle = CreateFile(portname,
						GENERIC_READ | GENERIC_WRITE,
						0,
						0,
						OPEN_EXISTING,
						0,
						0);
	return fileHandle;
#endif
};

void closeSerialPort(serport_t port)
{
	close(port);
};

void setPortConfig(int portfd, int baud)
{

	struct termios oldtio,newtio;
	unsigned int BAUDRATE = 0;
	
	tcgetattr(portfd,&oldtio); /* save current port settings */
	newtio = oldtio;
	
	if (baud==4800) BAUDRATE = B4800;
	if (baud==9600) BAUDRATE = B9600;
	if (baud==19200) BAUDRATE = B19200;
	if (baud==38400) BAUDRATE = B38400;
	if (baud==57600) BAUDRATE = B57600;
	if (baud==115200) BAUDRATE = B115200;
	if (baud==230400) BAUDRATE = B230400;

	if (BAUDRATE>0) cfsetspeed(&newtio, BAUDRATE);
	cfmakeraw(&newtio);
	
	newtio.c_cflag = (newtio.c_cflag& ~CSIZE)| CS8;
	newtio.c_cflag &= ~CRTSCTS;
	newtio.c_cflag |= CLOCAL | CREAD | PARENB;

	newtio.c_iflag = IGNPAR | ICRNL;
	
	newtio.c_oflag = 0;

	newtio.c_lflag = 0;

	newtio.c_cc[VMIN]=1;
	newtio.c_cc[VTIME]=0;

	tcdrain(portfd);
	tcflush(portfd, TCIFLUSH);
	tcsetattr(portfd,TCSANOW,&newtio);

}
