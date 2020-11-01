#include "ServerS.h"

ServerS::ServerS(const std::string& localAddr, uint16_t port)
{
    m_connections.reserve(MAX_CONNECTIONS);

    m_server.sin_addr.s_addr = inet_addr(localAddr.c_str()); // ip адресс
    m_server.sin_port = htons(port);                         // номер порта
    m_server.sin_family = AF_INET;                           // семейство интернет протоколов

    m_serverSocket = socket(AF_INET, SOCK_STREAM, 0);

    m_Counter = 0;

    if (!IsInit()) return;

    if(bind(m_serverSocket, (struct sockaddr *)&m_server, sizeof(m_server)) < 0)
    {
        std::cout << "Error bind" << std::endl;
    }
    
}

ServerS::~ServerS()
{
    stop();
}

void ServerS::stop(){
    // Остановка сервера
    close(m_serverSocket);
}

bool ServerS::Listen()
{
    return listen(m_serverSocket, MAX_CONNECTIONS) == 0;
}

bool ServerS::IsInit()
{
    return m_serverSocket >= 0; 
}

int ServerS::Accept(){
    m_newconnections = accept(m_serverSocket, (struct sockaddr  *)&m_server, (socklen_t *)&m_server);
    return m_newconnections;
}

void ServerS::ClientHandler(int index) // процедура работы с сокетом в новом потоке
{
    int msg_size;
    while (true)
    {
        recv(m_Connections[index], (char*)&msg_size, sizeof(int), 0);
        char *msg = new char (msg_size - 1);
        msg[msg_size] = '\0';

        recv(m_Connections[index],msg,  msg_size, 0);
        for (int i = 0; i < MAX_CONNECTIONS; ++i)
        {
            if (i == index) continue;
            if (m_Connections[i] < 0) continue;
            send(m_Connections[i], (char *)&msg_size, sizeof(int), 0);
            send(m_Connections[i], msg, msg_size, 0);
        }

        delete[] msg;
    }
}

void ServerS::StartServ(){
    for (int i = 0; i < MAX_CONNECTIONS; ++i)
    {
        if(Accept() == 0) 
        {
            std::cout << "Error" << std::endl;
        }
        else 
        {
            std::string msg = "Hi, it is my first server!";
            int msg_size = msg.size();
            send(m_newconnections, (char*)&msg_size, sizeof(int), 0);
            send(m_newconnections, msg.c_str(), msg_size, 0);

            m_Connections[i] = m_newconnections;
            m_Counter++;

            std::thread newThread(&ServerS::ClientHandler, this, i);
            m_threadProc[i].swap(newThread);
            std::cout << "Client connect" << std::endl;    
        }
    }
}
