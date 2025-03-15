#include <stdio.h>

#include <unistd.h>
#include <sys/epoll.h>
#include <string.h>   

int main(){
    printf("test epoll\n");

    struct epoll_event event;
	int epoll_fd = epoll_create1(0);
	
	if (epoll_fd == -1) {
		fprintf(stderr, "Failed to create epoll file descriptor\n");
		return 1;
	}
	
	event.events = EPOLLIN;
	event.data.fd = 0;
	
	if (epoll_ctl(epoll_fd, EPOLL_CTL_ADD, 0, &event)) {
		fprintf(stderr, "Failed to add file descriptor to epoll\n");
		close(epoll_fd);
		return 1;
	}
	
	if (close(epoll_fd)) {
		fprintf(stderr, "Failed to close epoll file descriptor\n");
		return 1;
	}

    return 0;
}