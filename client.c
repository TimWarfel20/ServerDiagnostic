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

//Add commenting for this entire program, similar to server

int main(){

    int clientSocket;
    struct sockaddr_in address;
    char buffer[BUFFER_SIZE];
    char recBuffer[BUFFER_SIZE];

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
    
    int choice = -1;

    while(choice != 5){
        printf("Please pick a command:\n");
        printf("1. Show Hostname\n");
        printf("2. Show Uptime\n");
        printf("3. Show RAM Usage\n");
        printf("4. Show IP Address\n");
        printf("5. Exit\n");

        scanf("%d", &choice);

        

        switch(choice){

        case 1:
        send(clientSocket, "whoami", strlen("whoamI"), 0);    
        break;

        case 2:
        send(clientSocket, "uptime", strlen("uptime"), 0);    
        break;

        case 3:
        send(clientSocket, "free -h", strlen("free -h"), 0);    
        break;

        case 4:
        send(clientSocket, "hostname -I", strlen("hostname -i"), 0);    
        break;
        }

        //I think this recv() needs to also be in a loop, so if bytesReceived is == 1024, then it runs again, because this means it did not receive
        //everything in the buffer
        ssize_t bytesReceived = recv(clientSocket, recBuffer, BUFFER_SIZE - 1, 0);

        if(bytesReceived > 0){
            printf("\n%s", recBuffer);
        }

    }


    close(clientSocket);

    printf("The connection has been terminated. Goodbye\n");

    return 0;

}