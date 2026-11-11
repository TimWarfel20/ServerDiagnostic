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

    //Initialize variables
    int clientSocket;
    struct sockaddr_in address;
    char buffer[BUFFER_SIZE];
    char recBuffer[BUFFER_SIZE];

    //Create a socket using Ipv4 and TCP
    clientSocket = socket(AF_INET, SOCK_STREAM, 0);

    //Show error if socket could not be created
    if(clientSocket < 0){
        printf("Error: Socket Failed to Initialize\n");
        exit(-1);
    }

    //initializes address struct with 0s
    memset(&address, 0, sizeof(address));

    //Fill address struct with IPv4 designation and a host to network(big-endian) number
    address.sin_family = AF_INET;
    address.sin_port = htons(PORT);

    //If the result is less than 0, the ip address is invalid, throw error and exit
    if(inet_pton(AF_INET, ADDRESS, &address.sin_addr ) <= 0){

        printf("Error: Invalid IP Address\n");
        close(clientSocket);
        exit(-1);
    }

    //Attempts to connect to ip address and port designated in address struct. If not, throw error and exit
    if(connect(clientSocket, (struct sockaddr *)&address, sizeof(address)) < 0){
        printf("Error: Connection Failed\n");
        close(clientSocket);
        exit(-1);
    }

    //Successful connection
    printf("Connected to Server on port %d\n", PORT);
    
    int choice = -1;

    //Presents menu until exit option is picked
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
        send(clientSocket, "whoami", strlen("whoami"), 0);    
        break;

        case 2:
        send(clientSocket, "uptime", strlen("uptime"), 0);    
        break;

        case 3:
        send(clientSocket, "free -h", strlen("free -h"), 0);    
        break;

        case 4:
        send(clientSocket, "hostname -I", strlen("hostname -I"), 0);    
        break;
        
        case 5:
        //Sends termination tag recognized by server program
        send(clientSocket, "TERMINATECONNECTION", strlen("TERMINATECONNECTION"), 0);
        close(clientSocket);
        printf("The connection has been terminated. Goodbye\n");
        return 0;
        }
        
        //Receiving section of code (Should probably be in a seperate function)

        /*
        This section of code works by adding a trailer onto the payload. The program begins by designating a totalReceived in addition to the bytesReceived. 
        Until the program hits the end sequence, the program will loop through the input from the server. It will add the message to the recBuffer array with a null terminator
        (This assumes it is the end of the message). If it is not the end, the program loops, adding the new message onto the end of the previous message within the same array
        (This is the function of the totalReceived variable. It keeps track of the end of one message and the end of another).
        When the end marker is found, we remove it using:

        char* markerPosition = '\0'

        From here, we print the full message 
        
        */
        int totalReceived = 0;

        while(1){

            //receives piece of message
            ssize_t bytesReceived = recv(clientSocket, recBuffer + totalReceived, sizeof(recBuffer) - totalReceived - 1, 0);
           
            //Throws error if nothing is received
            if(bytesReceived < 0){
                printf("Error: Network Error\n");
                break;
            }

            //Calculates end of current message segment
            totalReceived += bytesReceived;

            //Places null terminator on message if end of message
            recBuffer[totalReceived] = '\0';

            //Checks if the end marker is present within the message
            char* markerPosition = strstr(recBuffer, "$#@!");

            //If the end marker is present, remove the end marker and print the message
            if(markerPosition != NULL){
                *markerPosition = '\0';
                printf("\n%s", recBuffer);
                break;
            }
        }
    }

}