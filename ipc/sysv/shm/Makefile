CFLAGS = 
LDFLAGS = 

shm_test:
	$(CC) $(CFLAGS) -o outputs/shm_w.o shm_w.c $(LDFLAGS) 
	$(CC) $(CFLAGS) -o outputs/shm_r.o shm_r.c $(LDFLAGS)

.PHONY: test clean

test: shm_test	

clean:
	rm -f *.o rm -f outputs/*.o