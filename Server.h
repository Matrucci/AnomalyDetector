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

// Client handler interface.
class ClientHandler{
    public:
    virtual void handle(int clientID)=0;
};

/*******************************************
 * SocketIO class. Communicating via socket.
 * Will be implementing DefaultIO methods.
 ******************************************/
class SocketIO: public DefaultIO {
    int clientID;
public:
    /*****************************************
     * Constructor. Setting up the client ID.
     * @param _clientID - Client ID.
     ****************************************/
    explicit SocketIO(int _clientID) {
        clientID = _clientID;
    }

    /*******************************************************
     * Reading the from socket until a new line is detected.
     * @return - The line we read from the socket.
     *******************************************************/
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

    /*********************************************
     * Writing a text to the socket.
     * @param text - The string we want to send.
     *********************************************/
    void write(string text) override {
        send(clientID, text.c_str(), text.size(), 0);
    }

    /*******************************************
     * Writing a float to socket.
     * @param f - The number we want to send.
     *******************************************/
    void write(float f) override {
        string text = std::to_string(f);
        send(clientID, text.c_str(), text.size(), 0);
    }

    /**********************************************
     * Reading a float from socket.
     * @param f - Where we want to save the value.
     *********************************************/
    void read(float* f) override {
        *f = stof(read());
    }
};


//Anomaly detection handler class.
//Implementing ClientHandler.
class AnomalyDetectionHandler:public ClientHandler{
	public:
    virtual void handle(int clientID){
        SocketIO socketIo = SocketIO(clientID);
        CLI cli(&socketIo);
        cli.start();
    }
};

/***************
 * Server class.
 **************/
class Server {
	thread* t; // the thread to run the start() method in
    int fd;
    sockaddr_in server;
    sockaddr_in client;

public:
	Server(int port) throw (const char*);
	virtual ~Server();
	void start(ClientHandler& ch)throw(const char*);
	void stop();
};

#endif /* SERVER_H_ */
