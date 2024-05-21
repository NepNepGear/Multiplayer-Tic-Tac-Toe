#include <iostream>
#include <vector>
#include <cstring>
#include <thread>

#include "header.h"
#include "server.h"
#include "client.h"
#include "game.h"

using namespace std;

int main()
{

    int mode;

    cout << "Select mode: " << endl
         << "1. Run as a server." << endl
         << "2. Run as client." << endl;
    cin >> mode;

    // Server mode
    if (mode == 1)
    {
        return runServer();
    }

    // Client mode
    else if (mode == 2)
    {
        return runClient();
    }
    else
    {
        std::cerr << "Invalid mode. Exiting...\n";
        return -1;
    }

    return 0;
}
