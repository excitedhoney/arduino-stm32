#ifndef __SERIAL_PORT_H__
#define __SERIAL_PORT_H__

#ifndef WIN32
#define serport_t int
#define SERIAL_NOT_OPEN (-1)
#else
#define serport_t HANDLE
#define SERIAL_NOT_OPEN INVALID_HANDLE_VALUE
#endif

extern serport_t openSerialPort(const char * portname, int portflags);
extern void closeSerialPort(serport_t port);
extern void setPortConfig(serport_t serport, int baud);

#endif
