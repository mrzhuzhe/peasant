#include <stdio.h>
#include "common.h"
#include <netinet/in.h>
#include <sys/socket.h>
#include <ctype.h>
#include <string.h>
#include <arpa/inet.h>

#include <stdlib.h>
#include <sys/epoll.h>

#define MAX_EVENTS 5

int main(int argc, char *argv[]){
    printf("test abc \n");

    struct sockaddr_in svaddr, claddr;
    int sfd = 0, j;
    socklen_t len;
    char buf[100];
    char claddrStr[INET_ADDRSTRLEN];
    ssize_t numBytes;

    sfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sfd == -1) printf("sv socket fail\n"); 

    //memset(&svaddr, 0 , sizeof(svaddr));
    svaddr.sin_family = AF_INET;
    char *p_url = "127.0.0.1";
    inet_pton(AF_INET, p_url, &svaddr.sin_addr.s_addr);
    //svaddr.sin_addr.s_addr  = htonl(INADDR_ANY);
    svaddr.sin_port = htons(50001);    

    if (bind(sfd, (struct sockaddr *)&svaddr, sizeof(svaddr)) ==-1 ) printf("sv bind fail\n");

    memset((void*)buf, 0, 100*sizeof(char));

    struct epoll_event event, events[MAX_EVENTS];
	int epoll_fd = epoll_create1(EPOLL_CLOEXEC);

    if (epoll_fd == -1) {
		fprintf(stderr, "Failed to create epoll file descriptor\n");
		return 1;
	}

	event.events = EPOLLIN;
    event.data.fd = sfd;

	if(epoll_ctl(epoll_fd, EPOLL_CTL_ADD, sfd, &event))
	{
		fprintf(stderr, "Failed to add file descriptor to epoll\n");
		close(epoll_fd);
		return 1;
	}
    int event_count = 0;
    int i = 0;
    for (;;) {
		event_count = epoll_wait(epoll_fd, events, MAX_EVENTS, -1);
		printf("%d ready events\n", event_count);
		for (i = 0; i < event_count; i++) {  
            int cur_fd = events[i].data.fd;

            len = sizeof(struct sockaddr_in);
            numBytes = recvfrom(cur_fd, buf, 100, 0, (struct sockaddr *)&claddr, &len);
            if (numBytes == -1) {
                printf("sv recvfrom fail\n");
            }
            if (inet_ntop(AF_INET, &claddr.sin_addr, claddrStr, INET_ADDRSTRLEN) == NULL){
                printf("Could not convert client address to string \n");
            } else {
                printf("Server received %ld bytes from %s %u \n", (long)numBytes, claddrStr, ntohs(claddr.sin_port));
            }
            
            // bussiness  
            for (j=0;j<numBytes;j++){
                buf[j] = toupper((unsigned char)buf[j]);           
            }
            printf("%s \n", buf);

            // bussiness  

            if (sendto(cur_fd, buf, numBytes, 0, (struct sockaddr *)&claddr, len) != numBytes) {
                printf("sv sendto fail\n");
            }
        }
    }

    return 0;
}
