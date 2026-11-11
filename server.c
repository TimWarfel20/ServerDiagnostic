#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

#define PORT 8080
#define BUFFER_SIZE 1024

int main(){

    int serverSocket;
    int clientSocket;
    struct sockaddr_in address;
    char buffer[BUFFER_SIZE];
    char commandOutput[BUFFER_SIZE];


    //Initialize socket using Ipv4 and TCP, returns a negative value if fails.
    serverSocket = socket (AF_INET, SOCK_STREAM, 0);

        if(serverSocket < 0){
            printf("Error: Socket Creation Failed");
            exit(-1);
        }

        //Populates address struct with 0s before usage. Creates a clean pallet to begin working
        memset(&address, 0, sizeof(address));
        
        //Sets to the struct that we are using IPv4, accepting all incoming traffic, and doing so on a given port
        //htons sets the port to Big Endian always
        address.sin_family = AF_INET;
        address.sin_addr.s_addr = INADDR_ANY;
        address.sin_port = htons(PORT);
    
        //Attempts to bind a socket to an IP Address and a Port. Returns a negative value if it fails
        if (bind( serverSocket, (struct sockaddr *) &address, sizeof(address)) < 0){
            printf("Error: Socket Failed to Bind");
            exit(-1);
        }
        
        //Socket is ready to begin listening
        printf("Socket was successfully bound to port %d\n", PORT);

        //Listens on the initialized socket accepting a queue of 1. Anything more will receive an error on the client side
        if (listen(serverSocket, 1) < 0){
            printf("Listen Failed");
            close(serverSocket);
            exit(-1);
        }

        printf("Server is listening on port %d\n", PORT);

        /*Any client attempting to connect to the server will be represented by the clientSocket
        We could take in information about the client in the sections we pass in NULL
        The second argument would be if we wanted information about the client's IP address. By passing NULL, we inform the OS we do not need this information.
        The third argument is the size of the struct returned if you are using the second argument. They work in tandem*/
        clientSocket = accept(serverSocket, NULL, NULL);

        printf("Client connected on port %d\n", PORT);

        //Fill the input buffer with 0s, same principle as before
        memset(buffer, 0, BUFFER_SIZE);

        /*
        recv() is how we read input from the socket once it is connected. To do this, we pass in the socket, the array(or rather a pointer to the first address
        within the array), the size of the array - 1, and flagsS(which we will normally set to 0).
        BUFFER_SIZE - 1 is used to make room for the null terminator. Otherwise, we could see buffer overflow.
        This returns a value. If the value is greater than 0, then we have received something. This something is stored in the buffer array
        */

        while(1){
        ssize_t bytesReceived = recv(clientSocket, buffer, BUFFER_SIZE - 1, 0);
       

        if(bytesReceived > 0){

             buffer[bytesReceived] = '\0';

            FILE *pipe = popen(buffer, "r");

            //DO error checking here later

            while(fgets(commandOutput, sizeof(commandOutput), pipe) != NULL){
               printf("Send to Client: %s", commandOutput);
               send(clientSocket, commandOutput, strlen(commandOutput), 0);
            }

            pclose(pipe);

            //End of message
            send(clientSocket, "$#@!", strlen("$#@!"), 0);


        }
    }

        printf("Closing Connection... Goodbye!\n");

        close(clientSocket);
        close(serverSocket);

        return 0;
}