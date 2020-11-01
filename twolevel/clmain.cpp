#include "ClientS.h"

int main()
{
    ClientS Client("127.0.0.1", 1111);
    Client.Start();
    return 0;
}