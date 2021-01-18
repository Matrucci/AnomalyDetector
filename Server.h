/*
 * Server.h
 *
 *  Created on: Dec 13, 2020
 *      Author: Eli
 */

#ifndef SERVER_H_
#define SERVER_H_


#include <thread>
#include <sys/socket.h>
#include <iostream>
#include <netinet/in.h>
#include <pthread.h>
#include <thread>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "commands.h"
#include "CLI.h"

using namespace std;

// edit your ClientHandler interface here:
class ClientHandler{
    public:
    virtual void handle(int clientID)=0;
};


// you can add helper classes here and implement on the cpp file


class SocketIO: public DefaultIO {
    int clientID;
public:
    explicit SocketIO(int _clientID) {
        clientID = _clientID;
    }
    string read() override {
        char buffer;
        string output;
        recv(clientID, &buffer, sizeof(char), 0);
        while (buffer != '\n') {
            output += buffer;
            recv(clientID, &buffer, 1, 0);
        }
        return output;
    }
    void write(string text) override {
        send(clientID, text.c_str(), text.size(), 0);
    }
    void write(float f) override {
        string text = std::to_string(f);
        send(clientID, text.c_str(), text.size(), 0);
    }
    void read(float* f) override {
        *f = stof(read());
    }
};



// edit your AnomalyDetectionHandler class here
class AnomalyDetectionHandler:public ClientHandler{
	public:
    virtual void handle(int clientID){
        SocketIO socketIo = SocketIO(clientID);
        CLI cli(&socketIo);
        cli.start();
    }
};


// implement on Server.cpp
class Server {
	thread* t; // the thread to run the start() method in
    int fd;
    sockaddr_in server;
    sockaddr_in client;
    //bool run = false;

	// you may add data members

public:
	Server(int port) throw (const char*);
	virtual ~Server();
	void start(ClientHandler& ch)throw(const char*);
	void stop();
};

#endif /* SERVER_H_ */
