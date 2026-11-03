# ServerDiagnostic

## How to run
First, compile and run the server program using the following commands:
```
gcc server.c -o server.out
./server.out
```

The server should show that it is listening on the given port

Now, compile and run the client program:
```
gcc client.c -o client.out
./client.out
```

### Project Description
This project was created as a tool for me to learn a variety of systems-programming related topics. A partial list is as follows: <br>
- C <br>
- Berkeley Sockets <br>
- System Calls <br>
- Linux Commands <br>

You could also include some subsets of C in here, such as buffer handling and the like. However, these were my main goals for beginning this project.

The project is comprised of two sections: A server program and a client program. As previously stated, you being the program by running the server program on a Linux(Posix-compatible) system. For the sake of testing and simplicity, I have hard coded a port. However, this could be changed if desired. The system will begin listening on this port.

Next, you will run the client program. Again, I have hard coded the port and an ip address. In a real world scenario, you would change this to be an input from the user. Given the context of this program, I chose to not do this.

After a connection has been made, both programs will print that a connection has been made. The client program will begin prompting the user for a command. After the user has made a selection, the client program will send a given command over the socket connection to the server. The server will run the command and send the output back to the client. The client will print the command output.

### To-Do
- Fix commands where the output is longer than the 1024 byte buffer size
- Add more commands for diagnostic
- Add commenting to the client program, similar to how the server is set up
- Check for errors in connecting/sending in both programs
- Add error checking for the inputs the user puts in
- Display information to the server regarding what commands are being ran
- Show the server when a client disconnects (More important for when multithreading is added)
- Add multithreading (Should be done when everything else is perfect)
- Add a log, both for the commands that are sent to the server and for the server itself, save to file

I will add to this as I see fit. Right now, this is just a fun project that I am working on for self-education purposes.

Let it be known that I am also using this project as an opportunity to better learn Git/Github and version control concepts. Please be patient if this repo is not to best practice.
