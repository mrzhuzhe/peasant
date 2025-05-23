// fifo_w.c

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <errno.h>
#include <fcntl.h>
#include <string.h>

// FIFO 文件名
#define FIFO_PATH "fifo_file"


int main() {
	// 创建 FIFO 文件，如果存在就不再创建
	if (mkfifo(FIFO_PATH, 0666) < 0 && errno != EEXIST) {
		perror("create fifo failed");
		return -1;
	} else {
		char cont_w[] = "I'm FIFO write.\n";
		// 以只写的方式打开
		int fd = open(FIFO_PATH, O_CREAT|O_WRONLY, 0666);
		if (fd > 0) {
			while (1) {
				// 循环写入内容
				write(fd, cont_w, strlen(cont_w));
				sleep(1);
				printf("write: %s\n", cont_w);
			}
			close(fd);
		}
	}

	return 0;
}