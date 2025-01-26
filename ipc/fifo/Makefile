CFLAGS = 
LDFLAGS = 

fifo_test:
	$(CC) $(CFLAGS) -o outputs/fifo_w.o fifo_w.c $(LDFLAGS) 
	$(CC) $(CFLAGS) -o outputs/fifo_r.o fifo_r.c $(LDFLAGS)

.PHONY: test clean

test: fifo_test	

clean:
	rm -f *.o rm -f outputs/*.o