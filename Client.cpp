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

int Connection;

void ClientHandler(){
    int msg_size;

    while(true){
        recv(Connection, (char *)&msg_size, sizeof(int), 0);
        char *msg = new char (msg_size - 1);
        msg[msg_size] = '\0';
        recv(Connection, msg, msg_size, 0);
        std::cout << msg << std::endl;
        delete[] msg;
    }
}

int main (int argc, char* argv[]){
    int server_socket;
    struct sockaddr_in server;

    int sizeofaddr = sizeof(server); 
    server.sin_addr.s_addr = inet_addr("127.0.0.1"); // ip адресс
    server.sin_port = htons(1111); // номер порта
    server.sin_family = AF_INET; // семейство интернет протоколов

    
    Connection = socket(AF_INET, SOCK_STREAM, 0);

    if(connect(Connection,(struct sockaddr *)&server, sizeof(server)) != 0){
        std::cout << "Error: failed connect to server!" << std::endl;
        return 1;
    }
    std::cout << "Connect to server!" << std::endl;
    //char message[256];
    //recv(Connection, message, sizeof(message), 0);
    //std::cout << message << std::endl;

    std::thread th1(ClientHandler);

    std::string msg;
    while(true){
        std::getline(std::cin, msg);
        int msg_size = msg.size();
        send(Connection, (char *)&msg_size, sizeof(int), 0);
        send(Connection, msg.c_str(), msg_size, 0);
        //sleep(10);
    }

    return 0;
}