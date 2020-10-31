#pragma once

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <iostream>
#include <string>
#include <thread>
#include <vector>
#include <iostream>

#define MAX_CONNECTIONS 100

class ServerS
{
public:
    ServerS(const std::string& localAddr, uint16_t port);
    ~ServerS();

    bool    Listen();
    void    StartServ();

private:
    bool    IsInit();
    int     Accept();
    void    ClientHandler(int index);

private:
    struct sockaddr_in m_server;
    int                m_serverSocket;
    std::vector<int>   m_connections;
    int                m_newconnections;
    int                m_Connections[MAX_CONNECTIONS];
    std::thread        m_threadProc[MAX_CONNECTIONS];
    int                m_Counter;
};

