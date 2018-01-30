#ifndef __file_
#define __file_

#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

int fileWrite(int fd, char *ch);
int fileOpen(char *fileName, int type);
int fileClose(int fd);
int fileRead(int fd, char *ch, size_t size);

#define _READ O_RDONLY | O_SYNC
#define _WRITE O_WRONLY | O_SYNC

#define _INBUF_SIZE 256

#endif