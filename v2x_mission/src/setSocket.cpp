#include "setSocket.h"

int SetSocket::Connect_socket()
{
    if (clnt_sock != -1)
    {
        printf("aleady create sock!!\n");
        return -1;
    }
    clnt_sock = socket(PF_INET, SOCK_STREAM, 0);
    if (clnt_sock == -1)
    {
        printf("sock create error\n");
        return -1;
    }

    int opt_val = 1;
    setsockopt(clnt_sock, IPPROTO_TCP, TCP_NODELAY, (void *)&opt_val, sizeof(opt_val));
    setsockopt(clnt_sock, SOL_SOCKET, SO_REUSEADDR, (void *)&opt_val, sizeof(opt_val));

    sockaddr_in serv_addr;
    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = inet_addr(ip.c_str());
    serv_addr.sin_port = htons(port);

    int res = -1;
    res = connect(clnt_sock, (sockaddr *)&serv_addr, sizeof(serv_addr));
    if (res == -1)
        printf("sock connect error\n");
    else
        printf("sock connect success\n");
    return res;
}

int SetSocket::Disconnect_socket()
{
    if (clnt_sock != -1)
    {
        shutdown(clnt_sock, SHUT_RDWR);
        close(clnt_sock);
        clnt_sock = -1;
        printf("sock disconnect\n");
    }
    return -1;
}