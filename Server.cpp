
#include "Server.h"

#include <sys/socket.h>
#include <iostream>
#include <netinet/in.h>
#include <pthread.h>
#include <thread>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>


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
    if (listen(fd, 3) < 0) {
        throw "Listen failure";
    }
}

void newThread(ClientHandler* ch, int _fd, sockaddr_in client) {
    cout << "Waiting for a client" << endl;
    socklen_t clientSize = sizeof(client);
    while (true) {
        int aClient = accept(_fd, (struct sockaddr *) &client, &clientSize);
        if (aClient < 0) {
            throw "Accept failure";
        }
        cout << "Client connected!" << endl;
        ch->handle(aClient);
        //cout <<"ydb" << endl;
        close(aClient);
        close(_fd);
    }
}

void Server::start(ClientHandler& ch) throw(const char*) {
    t = new thread(newThread, &ch, fd, client);
    t->detach();
}

void Server::stop(){
	t->join(); // do not delete this!
}

Server::~Server() {
}

