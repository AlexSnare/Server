#include <iostream>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdio.h>
#include <utility>
#include <thread>
#include <stdlib.h>

#define MAX_CONNECTIONS 100

int Connections[MAX_CONNECTIONS];
std::thread threadProc[MAX_CONNECTIONS];
int Counter = 0;

void ClientHandler(int index)
{
    //char msg[256];
    int msg_size;
    while (true)
    {
        recv(Connections[index], (char*)&msg_size, sizeof(int), 0);
        char *msg = new char (msg_size - 1);
        msg[msg_size] = '\0';

        recv(Connections[index],msg,  msg_size, 0);

        //if (dataLen <= 0) continue;

        for (int i = 0; i < MAX_CONNECTIONS; ++i)
        {
            if (i == index) continue;
            if (Connections[i] < 0) continue;

            send(Connections[i], (char *)&msg_size, sizeof(int), 0);
            send(Connections[i], msg, msg_size, 0);
        }

        delete[] msg;
    }
}

int main(int argc, char *argv[])
{
    int server_socket;
    // pthread_t ptid;
    struct sockaddr_in server;

    for (int i = 0; i < MAX_CONNECTIONS; ++i) Connections[i] = -1;

    int sizeofaddr = sizeof(server);
    server.sin_addr.s_addr = inet_addr("127.0.0.1"); // ip адресс
    server.sin_port = htons(1111);                   // номер порта
    server.sin_family = AF_INET;                     // семейство интернет протоколов

    server_socket = socket(AF_INET, SOCK_STREAM, 0);
    bind(server_socket, (struct sockaddr *)&server, sizeof(server));
    listen(server_socket, MAX_CONNECTIONS); // прослушивание порта и максимальное кол-во запросов

    int newConnection; // храниит соединение с клиентом
    for (int i = 0; i < MAX_CONNECTIONS; ++i)
    {
        newConnection = accept(server_socket, (struct sockaddr  *)&server, (socklen_t *)&server);

        if (newConnection == 0)
        {
            std::cout << "Error" << std::endl;
        } else {
            std::cout << "Client connect" << std::endl;
            std::string msg = "Hi, it is my first server!";
            int msg_size = msg.size();
            send(newConnection, (char*)&msg_size, sizeof(int), 0);
            send(newConnection, msg.c_str(), msg_size, 0);

            Connections[i] = newConnection;
            Counter++;

            std::thread newThread(ClientHandler, i);
            threadProc[i].swap(newThread);
        }
    }
    return 0;
}