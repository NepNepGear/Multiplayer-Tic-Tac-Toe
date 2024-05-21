# TicTac Project

This project allows users to play a game of tic tac toe over a Local Area Network. The project is divided into three milestones.

## Milestone 1
A user plays a game of tic tac toe against an AI on their local machine. This game is not played over a network.

### 1. To run the program navigate to `/src/milestone1` in terminal and run the comand
```bash
make run
```
If any failures occure, use `make clean` to remove the output file and try again.


## Milestone 2
A user plays a game of tic tac toe against another user over the Local Area Network. This milestone is set up as a peer-to-peer architecture where one peer acts as the host and the other peer acts as the client. 

### 1. To run the program navigate to `/src/milestone2` in terminal and type the command 
```bash
make run
``` 
Then select the option to run as a server. 

### 2. On a second device navigate to `/src/milestone2` in terminal and type the command
```bash
make run
``` 
Then select the option to run as a client. Enter the IP address of the server. A game of tic tac toe will now begin. If any failures occur, use `make clean` to remove the output file and try again. If running the program from the same machine, use callback address `127.0.0.1`.

## Milestone 3
A user plays a game of tic tac toe against an AI or another player. This milestone is set up as a centralized peer-to-peer architecture. 

When a user connects to the server, the server will check if there are any users in the waiting list. If there are no users to play with, the user will be given the option to play against an AI or wait for another player. When a user plays against an AI, the moves are sent from the user (client) to the server. When a user plays against another user, the server facilitates a peer-to-peer connection between the two users using the architecture presented in Milestone 2. 

### 1. To run the program navigate to `/src/milestone3` in terminal and type the command 
```bash
make run
```

Select option 1 to run as a server. 

### 2. On a second device navigate to `/src/milestone3` in terminal and type the command 
```bash
make run
```
Select option 2 to run as a client. Select if you wish to play against an AI or against a player. Now enter the IP address of the server from step 1. 

At this point the client should be connected to the server. If there is another user in the waiting list, a game will automatically start with the user that is waiting. If there is no one in the waiting list, the user will be added to the waiting list.

If any failures occur, use `make clean` to remove the output file and try again. If running the program from the same machine, use callback address `127.0.0.1`.

## References
ChatGPT was used to help build this program. The following code is boiler plate code that is widely available on the internet and was used to setup the connection between the server and client.

```cpp
int connectToServer(const char *server_ip_addr, int port)
{

    int clientSocket = (int)socket(AF_INET, SOCK_STREAM, 0);
    if (clientSocket == -1)
    {
        std::cerr << "Error creating socket\n";
        return -1;
    }

    sockaddr_in serverAddr{};
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = inet_addr(server_ip_addr);
    serverAddr.sin_port = htons(port);

    if (connect(clientSocket, (struct sockaddr *)&serverAddr, sizeof(serverAddr)) == -1)
    {
        std::cerr << "Error connecting to server\n";
        close(clientSocket);
        return -1;
    }

    std::cout << "Connected to server\n";

    return clientSocket;
}
```

There is also code used to setup the server that is very similar to the above snippet. This code is nearly identical to what can be found through the internet/ChatGPT. The design of the program and the use of the ussage of ports after setting up the initial connection is original work.
