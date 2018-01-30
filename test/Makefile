CC=gcc
CFLAGS=-g -O2 

INCLUDE_PATH=-I./include
PROGRAM=userProgram

OBJS=file.o userPG.o
SRCS=$(OBJS:%.o=%.c)

$(PROGRAM): $(CBJS) $(SRCS)
	$(CC) $(CFLAGS) $(INCLUDE_PATH) -o $(PROGRAM) $(SRCS)