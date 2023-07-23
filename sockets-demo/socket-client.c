#include <stdlib.h>
#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include "socketutil.h"

int main() {
    int socketFD = createTCPIpv4Socket();
    printf("socket file descriptor: %d \n", socketFD);
    
    struct sockaddr_in *address = createIPv4Address("127.0.0.1", 2000);

    int result = connect(socketFD, address, sizeof(*address));

    if(result == 0) {
        printf("Connection was successful \n");
    }

    char* message;
    message = "GET \\ HTTP/1.1\r\nHost:gogle.com\r\n\r\n";
    send(socketFD, message, strlen(message), 0);

    char buffer[1024];
    recv(socketFD, buffer, 1024, 0);

    printf("Response was %s \n", buffer);

    return 0;
}

