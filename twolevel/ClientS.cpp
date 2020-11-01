#include "ClientS.h"

ClientS::ClientS(const std::string& localAddr, uint16_t port)
{
    m_server.sin_addr.s_addr = inet_addr(localAddr.c_str()); // ip адресс
    m_server.sin_port = htons(port); // номер порта
    m_server.sin_family = AF_INET; // семейство интернет протоколов

    m_connection = socket(AF_INET, SOCK_STREAM, 0);

    if(connect(m_connection,(struct sockaddr *)&m_server, sizeof(m_server)) != 0)
    {
        std::cout << "Error: failed connect to server!" << std::endl;
    }
    std::cout << "Connect to server!" << std::endl;
}

ClientS::~ClientS()
{
    stop();
}

void ClientS::stop()
{
    close(m_serverSocket);
}

void ClientS::ClientHandler()
{
    int msg_size;

    while(true){
        recv(m_connection, (char *)&msg_size, sizeof(int), 0);
        char *msg = new char (msg_size - 1);
        msg[msg_size] = '\0';
        recv(m_connection, msg, msg_size, 0);
        std::cout << msg << std::endl;
        delete[] msg;
    }
}

void ClientS::Start()
{
    std::thread th1(&ClientS::ClientHandler, this);

    std::string msg;
    while(true)
    {
        std::getline(std::cin, msg);
        int msg_size = msg.size();
        send(m_connection, (char *)&msg_size, sizeof(int), 0);
        send(m_connection, msg.c_str(), msg_size, 0);
    }
}