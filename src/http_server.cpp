//
// Created by kali on 6/25/24.
//
#include "http_server.h"
#include "http_client.h"
#include "server.h"
#include <iostream>
const Http_server* Http_server::instance = nullptr;
Thread_pool Http_server::thread_pool(4);

std::string get_line(const std::string& str, int& cur)
{
    std::string temp_str;
    while (str[cur] != '\r')
    {
        temp_str += str[cur++];
    }
    cur += 2;
    return temp_str;
}

STATUS_CODE check_request_target(const std::string& str)
{
    int cur = str.find_first_of('/');
    if (str[cur + 1] != ' ')
    {
        std::string temp{str.substr(cur + 1)};
        int cur2 = temp.find_first_of(' ');
        std::string end_point{temp.substr(0, cur2)};
        // std::cout << end_point << "\n";
        return NOT_FOUND;
    }
    return OK;
}

std::string get_end_point(const std::string& str)
{
    int cur = str.find_first_of('/');
    if (cur == std::string::npos) return str;
    std::string temp{str.substr(cur + 1)};
    int cur2 = temp.find_first_of(' ');
    std::string end_point{temp.substr(0, cur2)};
    return end_point;
}

std::string get_header(const std::string& str)
{
    int cur = str.find_first_of("\r\n");
    if (cur == std::string::npos) return str;
    std::string temp{str.substr(cur + 2)};
    int cur2 = temp.find_last_of("\r\n");
    std::string header{temp.substr(0, cur2)};
    return header;
}

std::string get_method(const std::string& str)
{
    int cur = str.find_first_of(' ');
    if (cur == std::string::npos) return "";
    std::string method{str.substr(0, cur)};
    return method;
}

auto get_line_with_key(const std::string& str, const std::string& key) -> std::string
{
    int cur = str.find_first_of("\r\n");
    if (cur == std::string::npos) return "";
    std::string sub1{str.substr(0, cur)};
    if (sub1.contains(key)) return sub1;
    std::string temp{str.substr(cur + 2)};
    return get_line_with_key(temp, key);
}

std::string get_line_value(const std::string& str)
{
    int cur = str.find_first_of(':');
    if (cur == std::string::npos) return "";
    std::string temp{str.substr(cur + 2)};
    int cur2 = temp.find_first_of("\r\n");
    std::string value{temp.substr(0, cur2)};
    return value;
}

Http_server::Http_server(): server_fd(socket(AF_INET, SOCK_STREAM, 0)), server_addr({
                                .sin_family = AF_INET,
                                .sin_port = htons(4221),
                                .sin_addr = {.s_addr = INADDR_ANY},
                                .sin_zero{0}
                            })
{
    std::cout << std::unitbuf;
    std::cerr << std::unitbuf;

    if (server_fd < 0)
    {
        std::cerr << "Failed to create server socket\n";
        exit(1);
        // return 1;
    }

    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof(reuse)) < 0)
    {
        std::cerr << "setsockopt failed\n";
        exit(1);
        // return 1;
    }

    if (bind(server_fd, (sockaddr*)&server_addr, sizeof(server_addr)) != 0)
    {
        std::cerr << "Failed to bind to port 4221\n";
        exit(1);
        // return 1;
    }
    if (listen(server_fd, connection_backlog) != 0)
    {
        std::cerr << "listen failed\n";
        // return 1;
        exit(1);
    }
}

const Http_server* Http_server::get_instance()
{
    if (!instance)
    {
        instance = new Http_server();
    }
    return instance;
}

void Http_server::accept_connection()
{
    // sockaddr_in client_addr{
    //     .sin_family = AF_INET,
    //     .sin_port = htons(4221),
    //     .sin_addr = {.s_addr = INADDR_ANY},
    //     .sin_zero = {0}
    // };
    // int client_addr_len = sizeof(client_addr);

    int client_fd = accept(get_instance()->server_fd, nullptr, nullptr);
    if (client_fd < 0)
    {
        std::cerr << "Failed to accept client connection\n";
        return;
    }
    thread_pool.enqueue([client_fd] { handle_client(client_fd); });
};

void Http_server::handle_client(int client_fd)
{
    char buff[Http_client::BUFF_LENGTH]{};
    char buffer[2048];
    ssize_t bytesRead = recv(client_fd, buff, Http_client::BUFF_LENGTH, 0);
    if (bytesRead < 0)
    {
        std::cerr << "error recceving message from client\n";
        close(client_fd);
    }
    if (bytesRead > 0)
    {
        std::string request(buff, bytesRead);
        STATUS_CODE status{check_request_target(request)};
        std::string method{get_method(request)};
        std::string endpoint{get_end_point(request)};
        std::string header{};
        std::string endpoint2{get_end_point(endpoint)};
        std::string response_body{};
        bool has_echo = endpoint.contains("echo");
        bool has_user_agent = endpoint.contains("user-agent");
        if (endpoint.empty() || has_echo || has_user_agent)
        {
            status = OK;
            if (!endpoint.empty())
            {
                header = "Content-Type: text/plain\r\nContent-Length: ";
            }
            if (has_echo)
            {
                response_body = endpoint2;
                header += std::to_string(response_body.length());
            }
            if (has_user_agent)
            {
                std::string request_header = get_header(request);
                std::string line = get_line_with_key(request_header, "User-Agent");
                response_body = get_line_value(line);
                header += std::to_string(response_body.length());
            }
            header += "\r\n";
        }
        std::string response = "HTTP/1.1 " + std::to_string(status) + ' ' + format_status_string(
            status_code_to_string(status)) + "\r\n" + header + "\r\n";
        response += response_body;
        // std::cout << response;
        send(client_fd, response.c_str(), response.length(), 0);
    }
    close(client_fd);
};
