#include <iostream>
#include <cstdlib>
#include <ctime>

using namespace std;

// Function to print the Tic-Tac-Toe board
void printBoard(char board[3][3])
{
    cout << endl;
    for (int i = 0; i < 3; i++)
    {
        for (int j = 0; j < 3; j++)
        {
            cout << "| " << board[i][j] << " ";
        }
        cout << "|" << endl;
    }
    cout << endl;
}

// Function to check if the current player has won
bool checkWin(char board[3][3], char player)
{
    // Check rows and columns
    for (int i = 0; i < 3; i++)
    {
        if ((board[i][0] == player && board[i][1] == player && board[i][2] == player) ||
            (board[0][i] == player && board[1][i] == player && board[2][i] == player))
        {
            return true;
        }
    }

    // Check diagonals
    if ((board[0][0] == player && board[1][1] == player && board[2][2] == player) ||
        (board[0][2] == player && board[1][1] == player && board[2][0] == player))
    {
        return true;
    }

    return false;
}

// Function to check if the board is full (a tie)
bool isBoardFull(char board[3][3])
{
    for (int i = 0; i < 3; i++)
    {
        for (int j = 0; j < 3; j++)
        {
            if (board[i][j] == ' ')
            {
                return false; // Empty space found, board is not full
            }
        }
    }
    return true; // No empty space found, board is full
}

// Function for person vs person mode
void pvp(char board[3][3])
{
    char currentPlayer = 'X';

    while (true)
    {
        // Print the current board
        printBoard(board);

        // Get the move from the current player
        int row, col;
        cout << "Player " << currentPlayer << ", enter your move (row and column): ";
        cin >> row >> col;
        row--;
        col--;

        // Check if the move is valid
        if (row < 0 || row > 2 || col < 0 || col > 2 || board[row][col] != ' ')
        {
            cout << "Invalid move. Try again.\n";
            continue;
        }

        // Make the move
        board[row][col] = currentPlayer;

        // Check if the current player has won
        if (checkWin(board, currentPlayer))
        {
            printBoard(board);
            cout << "Player " << currentPlayer << " wins!\n";
            break;
        }

        // Check if the board is full (tie)
        if (isBoardFull(board))
        {
            printBoard(board);
            cout << "It's a tie!\n";
            break;
        }

        // Switch to the other player
        currentPlayer = (currentPlayer == 'X') ? 'O' : 'X';
    }
}

// Function for person vs computer mode
void pvAI(char board[3][3])
{

    char humanPlayer = 'O', computerPlayer = 'X';

    char currentPlayer = 'O';

    while (true)
    {
        int row, col;
        if (currentPlayer == humanPlayer)
        { // Human player's turn

            printBoard(board);

            cout << "Enter your move (row and column): ";
            cin >> row >> col;
            row--;
            col--;

            if (row < 0 || row > 2 || col < 0 || col > 2 || board[row][col] != ' ')
            {
                cout << "Invalid move. Try again.\n";
                continue;
            }

            board[row][col] = humanPlayer;

            if (checkWin(board, humanPlayer))
            {
                printBoard(board);
                cout << "You win!\n";
                break;
            }
        }
        // Computer's turn
        else
        {
            // Generate random moves for the computer
            do
            {
                row = rand() % 3;
                col = rand() % 3;
            } while (board[row][col] != ' ');

            cout << "Computer chooses: " << row << " " << col << endl;
            board[row][col] = computerPlayer;

            if (checkWin(board, computerPlayer))
            {
                printBoard(board);
                cout << "Computer wins!\n";
                break;
            }
        }

        if (isBoardFull(board))
        {
            printBoard(board);
            cout << "It's a tie!\n";
            break;
        }

        currentPlayer = (currentPlayer == 'X') ? 'O' : 'X';
    }
}

int main()
{
    char board[3][3] = {{' ', ' ', ' '}, {' ', ' ', ' '}, {' ', ' ', ' '}};

    srand(static_cast<unsigned>(time(nullptr)));
    pvAI(board);
    return 0;
}
