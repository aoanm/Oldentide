// Filename:    Main.cc
// Author:      Joseph DeVictoria
// Date:        Jan_31_2016
// Purpose:     Contains the main method for the Oldentide dedicated server.

#include "OldentideServer.h"
#include <stdlib.h>

using namespace std;

int main(int argc, char* argv[]){
    int port = atoi(argv[0]);
    OldentideServer* server = new OldentideServer(port);
    server.run();
    return 0;
}