#include "ServerS.h"

int main()
{
    ServerS Serv("127.0.0.1", 1111); // Создание экз и передача парам в конст
    Serv.Listen();
    Serv.StartServ();

    return 0;
}