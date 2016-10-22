CC = gcc
CFLAGS = -std=gnu99 -Wall
LIBS = -lpthread

sobj = server.o handlers.o common.o

server : $(sobj)
	$(CC) $(CFLAGS) -o server $(sobj) $(LIBS)

.PHONY .SILENT: all
all : server

.PHONY .SILENT: clean
clean :
	$(RM) server $(sobj)

.PHONY : run
run : server
	sudo ./server
