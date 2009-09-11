#include <termios.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/signal.h>
#include <sys/types.h>


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
