CC = gcc
CFLAGS = 
LDFLAGS = 

epoll_test:
	$(CC) $(CFLAGS) -o outputs/main.o main.c $(LDFLAGS) 

.PHONY: test clean

test: outputs/main.o
	outputs/main.o

clean:
	rm -f *.o rm -f outputs/*.o