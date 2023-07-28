#include "socketutil.h"
#include <arpa/inet.h>
#include <stdio.h>
#include <errno.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>

struct AcceptedSocket {
    int acceptedSocketFileDescriptor;
    struct sockaddr_in address;
    int error;
    bool acceptedSuccessfully;
};

struct AcceptedSocket * acceptIncomingConnection(int serverSocketFD);

int main() {
    int serverSocketFD = createTCPIpv4Socket();
    struct sockaddr_in *serverAddress = createIPv4Address("", 2000);

    int result = bind(serverSocketFD, serverAddress, sizeof(*serverAddress));
    if (result == -1) {
        fprintf(stderr, "Binding failed: %s\n", strerror(errno));
        close(serverSocketFD);
        return 1;
    } else {
        printf("Server socket is bound successfully.\n");
    }

    int listenResult = listen(serverSocketFD, 10);

    struct AcceptedSocket* clientSocket = acceptIncomingConnection(serverSocketFD);

    char buffer[1024];
    while(true) {
        ssize_t amountReceived = recv(clientSocket->acceptedSocketFileDescriptor, buffer, 1024, 0);

        if(amountReceived > 0){
            buffer[amountReceived] = 0;
            printf("Response was %s\n", buffer);
        }

        if(amountReceived == 0){
            break;
        }
    }

    close(clientSocket->acceptedSocketFileDescriptor);
    shutdown(serverSocketFD, SHUT_RDWR);

    return 0;
}

struct AcceptedSocket * acceptIncomingConnection(int serverSocketFD){
    struct sockaddr_in clientAddress;
    int clientAddressSize = sizeof(struct sockaddr_in);
    int clientSocketFD = accept(serverSocketFD, &clientAddress, &clientAddressSize);

    struct AcceptedSocket* acceptedSocket = malloc(sizeof(struct AcceptedSocket));
    acceptedSocket->address = clientAddress;
    acceptedSocket->acceptedSocketFileDescriptor = clientSocketFD;
    acceptedSocket->acceptedSuccessfully = clientSocketFD>0;

    if(!acceptedSocket->acceptedSuccessfully){
        acceptedSocket->error = clientSocketFD;
    }

    return acceptedSocket;
}