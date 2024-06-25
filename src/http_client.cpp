//
// Created by kali on 6/25/24.
//

#include "http_client.h"

Http_client::Http_client(const Http_server* server):
    client_fd(accept(server->server_fd, (sockaddr*)&client_addr, (socklen_t*)&client_addr_len))
{
}
Http_client::Http_client()
    : client_fd(socket(AF_INET, SOCK_STREAM, 0))
{
    if (client_fd < 0)
    {
        std::cerr << "Failed to create client socket\n";
        exit(1); // handle the error appropriately
    }

    client_addr.sin_family = AF_INET;
    client_addr.sin_port = htons(4221);
    client_addr.sin_addr.s_addr = INADDR_ANY; // Adjust this as necessary for the server's IP
}
void Http_client::connect_to_server() const
{
    if (connect(client_fd, (sockaddr*)&client_addr, sizeof(client_addr)) < 0)
    {
        std::cerr << "Failed to connect to server\n";
        close(client_fd);
        exit(1); // handle the error appropriately
    }
    std::cout << "Connected to server\n";
}

void Http_client::send_message(const std::string& message) const
{
    send(client_fd, message.c_str(), message.length(), 0);
}

void Http_client::receive_message() const
{
    char buffer[1024];
    int bytesRead = read(client_fd, buffer, sizeof(buffer) - 1);
    if (bytesRead > 0)
    {
        buffer[bytesRead] = '\0';
        std::cout << "Received message from server: " << buffer << std::endl;
    }
    else
    {
        std::cerr << "Failed to receive message\n";
    }
}
