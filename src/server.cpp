#include "server.h"
#include "http_server.h"
#include <iostream>
#include <cstdlib>
#include <string>
#include <cstring>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netdb.h>

#include "http_client.h"

// std::string check_request(const std::string& str, ssize_t length)
// {
//     using namespace std;
//     int cur{0},sub_cur{0};
//     string data{},sub{},sub2{},temp{str};
//     while (cur < length && !temp.empty())
//     {
//         cur = temp.find_first_of("\r\n");
//         sub = temp.substr(0,cur);
//         data.append(sub);
//         // data += ' ';
//         temp = temp.substr(cur+2);
//         // while (true)
//         // {
//         //     sub_cur = sub.find_first_of(' ');
//         //     data = sub.substr(0,sub_cur);
//         //     cout << data << "\n";
//         //     {
//         //
//         //     }
//         //     if (sub_cur == sub.npos) break;
//         //     sub = sub.substr(sub_cur+1);
//         // }
//     }
//     // data.pop_back();
//     // return data;
// }

int main(int argc, char** argv)
{
    // std::cout << std::unitbuf;
    // std::cerr << std::unitbuf;
    //
    // const int server_fd = socket(AF_INET, SOCK_STREAM, 0);
    // if (server_fd < 0)
    // {
    //     std::cerr << "Failed to create server socket\n";
    //     return 1;
    // }
    //
    // // Since the tester restarts your program quite often, setting SO_REUSEADDR
    // // ensures that we don't run into 'Address already in use' errors
    // constexpr int reuse = 1;
    // if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof(reuse)) < 0)
    // {
    //     std::cerr << "setsockopt failed\n";
    //     return 1;
    // }
    //
    // sockaddr_in server_addr{};
    // server_addr.sin_family = AF_INET;
    // server_addr.sin_addr.s_addr = INADDR_ANY;
    // server_addr.sin_port = htons(4221);
    // if (bind(server_fd, (sockaddr*)&server_addr, sizeof(server_addr)) != 0)
    // {
    //     std::cerr << "Failed to bind to port 4221\n";
    //     return 1;
    // }
    //
    // constexpr int connection_backlog = 5;
    // if (listen(server_fd, connection_backlog) != 0)
    // {
    //     std::cerr << "listen failed\n";
    //     return 1;
    // }

    const Http_server* server = Http_server::get_instance();

    Http_server::accept_connection();
    while (true)
    {
        Http_server::accept_connection();
        // int client_fd = accept(server->server_fd, (sockaddr*)&client_addr, (socklen_t*)&client_addr_len);
        // constexpr ssize_t BUFF_LENGTH = 2048;
        // char buff[BUFF_LENGTH]{};
        // ssize_t rBuff = recv(client_fd, buff, BUFF_LENGTH, 0);
        // if (rBuff < 0)
        // {
        //     std::cerr << "error recceving message from client\n";
        //     close(client_fd);
        // }
        // std::string str(buff, rBuff);
        //
        // STATUS_CODE status{check_request_target(str)};
        //
        // std::string endpoint{get_end_point(str)};
        // std::string header{};
        // std::string endpoint2{get_end_point(endpoint)};
        // std::string response_body{};
        // bool has_echo = endpoint.contains("echo");
        // bool has_user_agent = endpoint.contains("user-agent");
        // if (endpoint.empty() || has_echo || has_user_agent)
        // {
        //     status = OK;
        //     if (!endpoint.empty())
        //     {
        //         header = "Content-Type: text/plain\r\nContent-Length: ";
        //     }
        //     if (has_echo)
        //     {
        //         response_body = endpoint2;
        //         header += std::to_string(response_body.length());
        //     }
        //     if (has_user_agent)
        //     {
        //         std::string request_header = get_header(str);
        //         std::string line = get_line_with_key(request_header,"User-Agent");
        //         response_body = get_line_value(line);
        //         header += std::to_string(response_body.length());
        //     }
        //     header += "\r\n";
        // }
        // std::string response = "HTTP/1.1 " + std::to_string(status) + ' ' + format_status_string(
        //     status_code_to_string(status)) + "\r\n" + header + "\r\n";
        // response += response_body;
        // send(client_fd, response.c_str(), response.length(), 0);
        // std::cout << "Client connected\n";
        // close(client_fd);
        // std::cout << "Client closed\n";
    }

    close(server->server_fd);

    return 0;
}
