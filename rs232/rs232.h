#define rs232_INCLUDED

#include <stdio.h>
#include <string.h>

#include <termios.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <limits.h>

int OpenComport(char *, int);
int SendByte(unsigned char);
int SendBuf(unsigned char *, int);
