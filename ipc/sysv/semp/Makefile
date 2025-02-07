CFLAGS = 
LDFLAGS = 

semp_test:
	$(CC) $(CFLAGS) -o outputs/semp.o semp.c $(LDFLAGS) 

.PHONY: test clean

test: semp_test	

clean:
	rm -f *.o rm -f outputs/*.o