#pragma once 

#include <iostream>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <thread>
#include <unistd.h>
#include <stdio.h>
#include <string>
#include <stdlib.h>

class ClientS
{
public:
    ClientS(const std::string& localAddr, uint16_t port);
    ~ClientS();

    void Start();

private:
    void ClientHandler();
    void stop();

private:
    int m_serverSocket;
    struct sockaddr_in m_server;
    int m_connection;

};


