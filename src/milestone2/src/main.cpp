#include <iostream>

#include "header.h"
#include "server.h"
#include "client.h"
#include "game.h"

using namespace std;

int main() {

    int mode;
    cout << "Select your mode: " << endl
         << "1. Play as a server" << endl
         << "2. Play as a client" << endl;
    cin >> mode;

    // Server mode
    if (mode == 1) {

        int serverSocket, clientSocket;
        if (getClientAccess(&serverSocket, &clientSocket) == -1) return -1;

        // start the game between client and server
        playGame(clientSocket);

        // Close the client sockets
        close(clientSocket);
        close(serverSocket);
    }
    
    // Client mode
    else if (mode == 2) {

        int serverSocket = connectToServer();
        char currentPlayer = 'X';
 
        while (true) {
            char board[3][3];
            char state;

            // get move from server
            recv(serverSocket, &board, sizeof(board), 0);
            
            printBoard(board);
            cout << endl;

            if (currentPlayer == 'X'){
                // contains logic to send move to server
                makeMove(serverSocket, board);
            }
            else 
                cout << "waiting for player O..." << endl;

            // get state from server
            recv(serverSocket, &state, sizeof(state), 0);

            switch (state) {
                case '0':
                    cout << "Unfortunately you loses !!!!" << endl;
                    goto end;
                case '1':
                    cout << "Wow you wins." << endl;
                    goto end;
                case '2':
                    cout << "It's a tie!" << endl; 
                    goto end;
            }

            currentPlayer = (currentPlayer == 'X') ? 'O' : 'X';

        }

        end:
        close(serverSocket);
    }
    else {
        std::cerr << "Invalid mode. Exiting...\n";
        return -1;
    }
    return 0;
}
