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
    sockaddr_in client_addr{
        .sin_family = AF_INET,
        .sin_port = htons(4221),
        .sin_addr = {.s_addr = INADDR_ANY},
        .sin_zero = {0}
    }; // Adjust this as necessary for the server's IP};
    const int client_fd;
    int client_addr_len = sizeof(client_addr);
    constexpr static ssize_t BUFF_LENGTH = 2048;

public:
    explicit Http_client(const Http_server* server);
    Http_client();
    void connect_to_server() const;
    void send_message(const std::string& message) const;
    void receive_message() const;
};


#endif //HTTP_CLIENT_H
