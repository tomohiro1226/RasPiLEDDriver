#include <stdio.h>
#include <stdlib.h>
#include <file.h>


int main(void){

    int fd;

    fd = fileOpen("/dev/myled", _WRITE);
    if(fd < 0){
        perror("cannot open");
        exit(EXIT_FAILURE);
    }
    fileWrite(fd, "HG");
    fileClose(fd);

    return 0;

}