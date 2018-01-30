#include <file.h>

int fileRead(int fd, char *ch, size_t size){

    int ret;

    ret = read(fd, ch, size);
    if( ret == -1 ){
        perror("read err");
        return -1;
    }
    if( ret == 0 ){
        ch = NULL;
    }
    *(ch + ret) = '\0';
    
    return 0;
}

int fileWrite(int fd, char *ch){
    
    int ret;

    size_t len;

    len = strlen(ch);
    ret = write(fd, ch, len); 
    if( ret < len ){
        perror("writing　err");
        return -1;
    }

    return ret;
}

int fileOpen(char *fileName, int type){

    int fd = open(fileName, type);
    if( fd < 0 ){
        perror("fopen err");
        return -1;
    }
    return fd;
}

int fileClose(int fd){
    if(close(fd) == -1){
        perror("fclose err");
        return -1;
    }
    return 0;
}