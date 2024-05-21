#include <iostream>
#include "header.h"

// connects the client to the server
int connectToServer() {

    // create client socket
    int clientSocket = (int) socket(AF_INET, SOCK_STREAM, 0);
    if (clientSocket == -1) {
        std::cerr << "Error creating socket\n";
        return -1;
    }

    // get ip address of server from user
    char server_ip_addr[INET_ADDRSTRLEN];
    std::cout << "Enter the server ip address: ";
    std::cin >> server_ip_addr;

    // create server address and setup server settings
    sockaddr_in serverAddr{};
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = inet_addr(server_ip_addr);
    serverAddr.sin_port = htons(12345);

    if (connect(clientSocket, (struct sockaddr *)&serverAddr, sizeof(serverAddr)) == -1) {
        std::cerr << "Error connecting to server\n";
        close(clientSocket);
        return -1;
    }

    std::cout << "Connected to server\n";

    return clientSocket;
}

// get input from user and send move to server
void makeMove(int clientSocket, char board[3][3]) {
    int row, col;

    std::cout << "Enter your move (row and column): ";
    std::cout << "Enter your move row[1-3] and column[1-3]: ";
    std::cin >> row >> col;
    row--; col--;

    while (row < 0 || row > 2 || col < 0 || col > 2 || board[row][col] != ' ') {
        std::cout << "Invalid move. Try again." << std::endl;
        std::cout << "Enter your move row[1-3] and column[1-3]: ";
        std::cin >> row >> col;
        row--; col--;
    }

    send(clientSocket, reinterpret_cast<const char *>(&row), sizeof(row), 0);
    send(clientSocket, reinterpret_cast<const char *>(&col), sizeof(col), 0);
}
