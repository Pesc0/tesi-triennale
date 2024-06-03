#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 5005
#define BUF_SIZE 65535

int main() {

    int sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd < 0) {
        perror("Could not create socket");
        exit(EXIT_FAILURE);
    }

    struct sockaddr_in servaddr;
    memset(&servaddr, 0, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = INADDR_ANY;
    servaddr.sin_port = htons(PORT);

    if (bind(sockfd, (const struct sockaddr *)&servaddr, sizeof(servaddr)) < 0) {
        perror("Socket bind failed");
        close(sockfd);
        exit(EXIT_FAILURE);
    }

    char buffer[BUF_SIZE];
    float value;
    while (1) {
	if (recv(sockfd, &buffer, BUF_SIZE, MSG_WAITALL == 4)) { //Mi assicuro di aver ricevuto un float (4 bytes)
	    memcpy(&value, buffer, sizeof(value));
	    printf("Received: %f\n", value);
    	}
    }

    close(sockfd);
    return 0;
}
