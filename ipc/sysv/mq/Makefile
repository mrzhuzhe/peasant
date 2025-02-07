CFLAGS = 
LDFLAGS = 

mq_test:
	$(CC) $(CFLAGS) -o outputs/mq_w.o mq_w.c $(LDFLAGS) 
	$(CC) $(CFLAGS) -o outputs/mq_r.o mq_r.c $(LDFLAGS)

.PHONY: test clean

test: mq_test	

clean:
	rm -f *.o rm -f outputs/*.o