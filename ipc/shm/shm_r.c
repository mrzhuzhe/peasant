//	https://stackoverflow.com/questions/14509662/performance-difference-between-ipc-shared-memory-and-threads-memory
#include <stdio.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

#define STORAGE_ID "/SHM_TEST"
#define STORAGE_SIZE 32

int main(int argc, char *argv[])
{
	int res;
	int fd;
	char data[STORAGE_SIZE];
	pid_t pid;
	void *addr;

	pid = getpid();

	// get shared memory file descriptor (NOT a file)
	fd = shm_open(STORAGE_ID, O_RDONLY, S_IRUSR | S_IWUSR);
	if (fd == -1)
	{
		perror("open");
		return 10;
	}

	// map shared memory to process address space
	addr = mmap(NULL, STORAGE_SIZE, PROT_READ, MAP_SHARED, fd, 0);
	if (addr == MAP_FAILED)
	{
		perror("mmap");
		return 30;
	}

	// place data into memory
	memcpy(data, addr, STORAGE_SIZE);

	//printf("PID %d: Read from shared memory: \"%s\"\n", pid, data);
	printf("PID %d: Read from shared memory: \"%s\"\n", pid, (char*)addr);

	return 0;
}