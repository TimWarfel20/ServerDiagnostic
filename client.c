#include <stdio.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>

#define PORT 8080
#define BUFFER_SIZE 1024
#define ADDRESS "127.0.0.1"

int main(){

    int clientSocket;
    struct sockaddr_in address;
    char buffer[BUFFER_SIZE];

    clientSocket = socket(AF_INET, SOCK_STREAM, 0);

    if(clientSocket < 0){
        printf("Error: Socket Failed to Initialize\n");
        exit(-1);
    }

    memset(&address, 0, sizeof(address));

    address.sin_family = AF_INET;
    address.sin_port = htons(PORT);

    if(inet_pton(AF_INET, ADDRESS, &address.sin_addr ) <= 0){

        printf("Error: Invalid IP Address\n");
        close(clientSocket);
        exit(-1);
    }

    if(connect(clientSocket, (struct sockaddr *)&address, sizeof(address)) < 0){
        printf("Error: Connection Failed\n");
        close(clientSocket);
        exit(-1);
    }

    printf("Connected to Server on port %d\n", PORT);

    const char *message = "This is a message from the client";

    send(clientSocket, message, strlen(message), 0);

    close(clientSocket);

    printf("The connected has been terminated. Goodbye\n");

    return 0;

}