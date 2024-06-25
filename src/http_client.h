//
// Created by kali on 6/25/24.
//

#ifndef HTTP_CLIENT_H
#define HTTP_CLIENT_H
#include "http_server.h"
#include <netdb.h>

class Http_client
{
public:
    const sockaddr_in client_addr{};
    const int client_fd;
    int client_addr_len = sizeof(client_addr);

public:
    explicit Http_client(const Http_server* server):
    client_fd(accept(server->server_fd, (sockaddr*)&client_addr,(socklen_t*)&client_addr_len))
    {

    }
};


#endif //HTTP_CLIENT_H
