#ifndef SETSOCKET_H
#define SETSOCKET_H

#include <iostream>
#include <thread>
#include <arpa/inet.h>
#include <netinet/tcp.h>
#include <string.h>
#include <unistd.h>

class SetSocket
{
public:
    int Connect_socket();
    int Disconnect_socket();
    int clnt_sock = -1;

private:
    std::string ip = "192.168.10.10";
    uint32_t port = 24000;
};
#endif