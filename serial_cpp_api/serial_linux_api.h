
#ifndef __SERIAL_LINUX_API_H__
#define __SERIAL_LINUX_API_H__


#ifdef __cplusplus
extern "C" {
#endif


int serial_linux_open(const char *dev_name);

int serial_linux_config(int fd, int baudrate, int databits, char parity, int stopbits);


#ifdef __cplusplus
}
#endif


#endif

