
#include "Server.h"

#include <sys/socket.h>
#include <iostream>
#include <netinet/in.h>
#include <thread>
#include <unistd.h>

bool run;

/**
 * Creating the server and binding the port.
 * @param port - The port we want to bind to.
 */
Server::Server(int port) throw(const char*) {
    fd = socket(AF_INET, SOCK_STREAM,0);
    if (fd < 0) {
        throw "Socket failed";
    }
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_port = htons(port);

    if (bind(fd, (struct sockaddr*)&server, sizeof(server)) < 0) {
        throw "Bind failure";
    }
    //Accepting up to 3 requests.
    if (listen(fd, 3) < 0) {
        throw "Listen failure";
    }
}

/**************************************************************************************
 * Starting up the server and accepting new connections until stopped.
 * This will use a client handler in order to keep the client implementation separate.
 * @param ch - the client handler
 *************************************************************************************/
void Server::start(ClientHandler& ch) throw(const char*) {
    run = true;
    //Creating a new thread.
    t = new thread([&ch, this] () {
        socklen_t clientSize = sizeof(client);
        while (run) {
            alarm(1);
            //Accepting a new client.
            int aClient = accept(fd, (struct sockaddr *) &client, &clientSize);
            alarm(0);
            if (aClient < 0) {
                throw "Accept failure";
            }
            cout << "Client connected!" << endl;
            //Use the injected client handler to run the client side of things.
            ch.handle(aClient);
            close(aClient);
        }
        close(fd);
    });
}

/************************************************************************
 * Stopping the server from accepting new clients.
 * Once every running session will be done, the server will "shut down".
 ***********************************************************************/
void Server::stop(){
    run = false;
    t->join(); // do not delete this!
}

Server::~Server() {
}
