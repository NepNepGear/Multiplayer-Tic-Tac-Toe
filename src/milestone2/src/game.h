#include <iostream>
#include "header.h"


using namespace std;

// Function to print the Tic-Tac-Toe board
void printBoard(char board[3][3]) {
    cout << endl;
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            cout << "| " << board[i][j] << " ";
        }
        cout << "|"  << endl;
    }
    cout << endl;
}

// Function to check if the current player has won
bool checkWin(char board[3][3], char player) {
    // Check rows and columns
    for (int i = 0; i < 3; i++) {
        if ((board[i][0] == player && board[i][1] == player && board[i][2] == player) ||
            (board[0][i] == player && board[1][i] == player && board[2][i] == player)) {
            return true;
        }
    }

    // Check diagonals
    if ((board[0][0] == player && board[1][1] == player && board[2][2] == player) ||
        (board[0][2] == player && board[1][1] == player && board[2][0] == player)) {
        return true;
    }

    return false;
}

// Function to check if the board is full (a tie)
bool isBoardFull(char board[3][3]) {
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            if (board[i][j] == ' ') {
                return false; // Empty space found, board is not full
            }
        }
    }
    return true; // No empty space found, board is full
}

// Game loop that runs on the server
void playGame(int clientSocket) {

    char board[3][3] = {{' ', ' ', ' '}, {' ', ' ', ' '}, {' ', ' ', ' '}};
    char currentPlayer = 'X';

    // game loop runs until there is a win or a tie
    while (true) {

        send(clientSocket, &board, sizeof(board), 0);
        printBoard(board);

        int row, col;

        // Receive the move from the client
        if (currentPlayer == 'X') {

            cout << "waiting for player X..." << endl;
            recv(clientSocket, &row, sizeof(row), 0);
            recv(clientSocket, &col, sizeof(col), 0);
        }

        else {

            cout << "Enter your move row[1-3] and column[1-3]: ";
            cin >> row >> col;
            row--; col--;
            while (row < 0 || row > 2 || col < 0 || col > 2 || board[row][col] != ' ') {
                cout << "Invalid move. Try again." << endl;
                cout << "Enter your move row[1-3] and column[1-3]: ";
                cin >> row >> col;
                row--; col--;
            }
        }

        board[row][col] = currentPlayer;

        // check if the game is ended, if so send game condition to client and break the loop
        if (checkWin(board, 'X')) {
            printBoard(board);
            cout << "Unfortunately you loses !!!!" << endl;
            char st = '1';
            send(clientSocket, &st, sizeof(st), 0);
            break;
        }

        else if (checkWin(board, 'O')){
            printBoard(board);
            cout << "Wow you wins." << endl;
            char st = '0';
            send(clientSocket, &st, sizeof(st), 0);
            break;
        }

        if (isBoardFull(board)) {
            printBoard(board);
            cout << "It's a tie!" << endl;
            char st = '2';
            send(clientSocket, &st, sizeof(st), 0);
            break;
        }

        // send the move to the clientS
        char st = '3';
        send(clientSocket, &st, sizeof(st), 0);

        currentPlayer = (currentPlayer == 'X') ? 'O' : 'X';
    }

}