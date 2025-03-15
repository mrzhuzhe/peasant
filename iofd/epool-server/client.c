#include <stdio.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <string.h>
#include <arpa/inet.h>

int main(int argc, char *argv[]){
    
    struct sockaddr_in svaddr;
    int sfd = 0, j;
    size_t msgLen;
    ssize_t numBytes;
    char resp[100] = {0};

    //memset(&svaddr, 0 , sizeof(svaddr));
    // AF_UNIX is on same host 
    sfd = socket(AF_INET, SOCK_DGRAM, 0);
    svaddr.sin_family = AF_INET;
    // char *p_url = "127.0.0.1";
    // inet_pton(AF_INET, p_url, &svaddr.sin_addr.s_addr);
    svaddr.sin_port = htons(50001);

    if(inet_pton(AF_INET, "127.0.0.1", &svaddr.sin_addr) <= 0){
        printf("inet_pton fail \n");
    }
    char * msg = "test msg";
    msgLen = strlen(msg);
    if (sendto(sfd, msg, msgLen, 0, (struct sockaddr *)&svaddr, sizeof(struct sockaddr_in)) != msgLen){
        printf("sendto fail \n");
    }

    if (recvfrom(sfd, resp, 100, 0, NULL, NULL) == -1) {
         printf("recvfrom fail \n");
    }
    
    printf("Response %d: %s\n", 123, resp);

    return 0;
}
