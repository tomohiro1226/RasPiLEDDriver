#include <stdio.h>
#include <stdlib.h>
#include <file.h>


int main(void){

    int fd;
    char str[10];

    fd = fileOpen("/dev/myled", _WRITE);
    if(fd < 0){
        perror("cannot open");
        exit(EXIT_FAILURE);
    }
//    fileWrite(fd, "HG");
    while(1){

        scanf("%s",str);
        if(strcmp(str, "END") == 0)break;
        fileWrite(fd, str);
    }
    
    fileClose(fd);

    return 0;

}