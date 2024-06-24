#include "server.h"
#include <iostream>
#include <cstdlib>
#include <string>
#include <cstring>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netdb.h>

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
STATUS_CODE check_request_target(const std::string& str)
{
    int cur = str.find_first_of('/');
    if (str[cur + 1] != ' ')
    {
        std::string temp{str.substr(cur + 1)};
        int cur2 = temp.find_first_of(' ');
        std::string end_point{temp.substr(0, cur2)};
        std::cout << end_point;
        return NOT_FOUND;
    }
    return OK;
}

std::string get_end_point(const std::string& str)
{
    int cur = str.find_first_of('/');
    if (cur == str.npos) return str;
    std::string temp{str.substr(cur + 1)};
    int cur2 = temp.find_first_of(' ');
    std::string end_point{temp.substr(0, cur2)};
    return end_point;
}

int main(int argc, char** argv)
{
    // Flush after every std::cout / std::cerr
    std::cout << std::unitbuf;
    std::cerr << std::unitbuf;

    // You can use print statements as follows for debugging, they'll be visible when running tests.
    // std::cout << "Logs from your program will appear here!\n";

    // Uncomment this block to pass the first stage

    const int server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd < 0)
    {
        std::cerr << "Failed to create server socket\n";
        return 1;
    }

    // Since the tester restarts your program quite often, setting SO_REUSEADDR
    // ensures that we don't run into 'Address already in use' errors
    constexpr int reuse = 1;
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof(reuse)) < 0)
    {
        std::cerr << "setsockopt failed\n";
        return 1;
    }

    sockaddr_in server_addr{};
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(4221);
    if (bind(server_fd, (sockaddr*)&server_addr, sizeof(server_addr)) != 0)
    {
        std::cerr << "Failed to bind to port 4221\n";
        return 1;
    }

    constexpr int connection_backlog = 5;
    if (listen(server_fd, connection_backlog) != 0)
    {
        std::cerr << "listen failed\n";
        return 1;
    }

    sockaddr_in client_addr{};
    int client_addr_len = sizeof(client_addr);

    std::cout << "Waiting for a client to connect...\n";

    while (true)
    {
        int client_fd = accept(server_fd, (sockaddr*)&client_addr, (socklen_t*)&client_addr_len);

        constexpr ssize_t BUFF_LENGTH = 2048;
        char buff[BUFF_LENGTH]{};
        ssize_t rBuff = recv(client_fd, buff, BUFF_LENGTH, 0);
        if (rBuff < 0)
        {
            std::cerr << "error recceving message from client\n";
            close(client_fd);
        }
        std::string str(buff, rBuff);

        // std::string data{check_request(str,rBuff)};
        // std::cout << data;
        STATUS_CODE status{check_request_target(str)};

        std::string endpoint{get_end_point(str)};
        std::string header{};
        std::string endpoint2{get_end_point(endpoint)};
        if (endpoint.empty() || endpoint.contains("echo"))
        {
            status = OK;
            if(!endpoint.empty()){
                header = "Content-Type: text/plain\r\nContent-Length: "
                   + std::to_string(endpoint2.length()) + "\r\n";
            }
        }
        std::string message = "HTTP/1.1 " + std::to_string(status) + ' ' + format_status_string(
            status_code_to_string(status)) + "\r\n" + header + "\r\n";
        message += endpoint2;
        send(client_fd, message.c_str(), message.length(), 0);
        std::cout << "Client connected\n";
        close(client_fd);
        std::cout << "Client closed\n";
    }

    close(server_fd);

    return 0;
}
