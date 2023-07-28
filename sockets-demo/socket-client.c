#include <stdlib.h>
#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include "socketutil.h"
#include <stdbool.h>
#include <unistd.h>

int main() {
    int socketFD = createTCPIpv4Socket();
    printf("socket file descriptor: %d \n", socketFD);
    
    struct sockaddr_in *address = createIPv4Address("127.0.0.1", 2000);

    int result = connect(socketFD, address, sizeof(*address));

    if(result == 0) {
        printf("Connection was successful \n");
    }


    char *line = NULL;
    size_t lineSize = 0;
    printf("type we will send (type exit)... \n");
    while(true) {
        ssize_t charCount = getline(&line, &lineSize, stdin);

        if(charCount > 0){
            if(strcmp(line, "exit\n") == 0){
                break;
            }
            ssize_t amountWasSent = send(socketFD, line, charCount, 0);
        }
    }

    close(socketFD);
}

