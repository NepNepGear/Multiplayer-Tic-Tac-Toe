#include <iostream>
#include <vector>
#include <thread>
#include <unordered_set>
#include <queue>
#include <string>
#include <mutex>
#include <condition_variable>
#include "game.h"
#include <chrono>

#include "header.h"

using namespace std;

// GLOBAL VARS
const int SERVER_SOCKET = 12345;

struct Client
{
    sockaddr_in *addr;
    socklen_t addr_len;
    int socket;
    string charSocket; // identical to integer socket but in char format
};

queue<Client *> waitingList;
mutex waitingListMutex;

void addClientToWaitingList(Client *client)
{
    unique_lock<mutex> mlock(waitingListMutex);
    waitingList.push(client);
}

Client *getClientFromWaitingList()
{
    unique_lock<mutex> mlock(waitingListMutex);
    Client *client = waitingList.front();
    waitingList.pop();
    return client;
}

void printWaitingList()
{
    std::cout << "Clients in the Waiting List:\n";

    queue<Client *> waitingListCopy = waitingList;

    // Iterate through the waitingList
    while (!waitingListCopy.empty())
    {
        Client *client = waitingListCopy.front();
        waitingListCopy.pop();

        // Print client information
        std::cout << "Socket: " << client->socket << "\n";
        std::cout << "Address: " << inet_ntoa(client->addr->sin_addr) << "\n";
        std::cout << "Port: " << ntohs(client->addr->sin_port) << "\n";
        std::cout << "Address Length: " << client->addr_len << "\n";

        // Add a separator between clients
        std::cout << "-----------------\n";
    }

    std::cout << "End of Waiting List\n";
}

// Client A is setup as client, Client B is setup as server
void setupGame(Client *clientA)
{
    Client *clientB = getClientFromWaitingList();
    char ser = '0', cl = '1'; // ser = server, cl = client
    char response[248];
    char port[6];  

    // Client B is designated as server. Request port number from Client B
    send(clientB->socket, &ser, sizeof(ser), 0);
    recv(clientB->socket, response, sizeof(response), 0); 

    for (int i = 0; i < 5; i++)
        port[i] = response[i];
    port[6] = '\0';
    cout << endl;
    cout << "Received port from Client B: " << endl;
    cout << port << endl;;

    this_thread::sleep_for(chrono::seconds(1));

    // Client A is designated as client
    send(clientA->socket, &cl, sizeof(cl), 0);

    // Convert Client B IP address to string
    char ipAddress[INET_ADDRSTRLEN];
    inet_ntop(AF_INET, &(clientB->addr->sin_addr), ipAddress, INET_ADDRSTRLEN);

    // Send Client B IP address and port number to Client A
    send(clientA->socket, ipAddress, sizeof(ipAddress), 0);
    send(clientA->socket, port, sizeof(port), 0); 

    cout << "Sending ClientB ip and port to client A" << endl;
    cout << ipAddress << endl;
    cout << port << endl;
    cout << "New game started" << endl << endl;

    close(clientA->socket);
    close(clientB->socket);
}

// initializes each thread
int handleThread(Client *client)
{
    char gameMode;

    std::cout << "New player connected\n";

    // receive player gameMode
    //---------------------------------
    recv(client->socket, &gameMode, sizeof(gameMode), 0);

    if (gameMode == '1')
    { // play game with AI
        playGame_(client->socket);
    }
    else if (waitingList.empty())
    { // add player to queue
        cout << "Adding player to waiting list\n";
        addClientToWaitingList(client);
        // printWaitingList();
    }
    else
    { // play game with another player
        setupGame(client);
    }
    return 0;
}

int runServer()
{
    vector<thread> threads;

    int serverSocket, clientSocket;
    sockaddr_in serverAddr{}, clientAddr{};
    socklen_t addrLen = sizeof(clientAddr);

    serverSocket = (int)socket(AF_INET, SOCK_STREAM, 0);
    if (serverSocket == -1)
    {
        std::cerr << "Error creating socket\n";
        return -1;
    }

    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = INADDR_ANY;
    serverAddr.sin_port = htons(SERVER_SOCKET);

    if (bind(serverSocket, (struct sockaddr *)&serverAddr, sizeof(serverAddr)) == -1)
    {
        std::cerr << "Error binding socket\n";
        close(serverSocket);
        return -1;
    }

    if (listen(serverSocket, 10) == -1)
    {
        std::cerr << "Error listening for connections\n";
        close(serverSocket);
        return -1;
    }

    // accept new clients
    while (true)
    {
        std::cout << "Waiting for player ...\n";
        clientSocket = accept(serverSocket, (struct sockaddr *)&clientAddr, &addrLen);
        if (clientSocket == -1)
        {
            std::cerr << "Error accepting connection for player \n";
            close(serverSocket);
            return -1;
        }

        // create new Client
        Client *client = new Client();
        client->addr = &clientAddr;
        client->addr_len = addrLen;
        client->socket = clientSocket;

        client->charSocket = to_string(clientSocket);

        // create new thread
        thread th = thread(handleThread, client);
        th.detach();
    }

    close(serverSocket);
    return 0;
}
