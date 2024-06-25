#include "server.h"
#include "http_server.h"
#include "http_client.h"
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

    std::cout << "Waiting for a client to connect...\n";

    while (true)
    {
        Http_client client {Http_client(server)};
        constexpr ssize_t BUFF_LENGTH = 2048;
        char buff[BUFF_LENGTH]{};
        ssize_t rBuff = recv(client.client_fd, buff, BUFF_LENGTH, 0);
        if (rBuff < 0)
        {
            std::cerr << "error recceving message from client\n";
            close(client.client_fd);
        }
        std::string str(buff, rBuff);

        // std::string data{check_request(str,rBuff)};
        // std::cout << data;
        STATUS_CODE status{check_request_target(str)};

        std::string endpoint{get_end_point(str)};
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
                std::string request_header = get_header(str);
                std::string line = get_line_with_key(request_header,"User-Agent");
                response_body = get_line_value(line);
                header += std::to_string(response_body.length());
            }
            header += "\r\n";
        }
        std::string response = "HTTP/1.1 " + std::to_string(status) + ' ' + format_status_string(
            status_code_to_string(status)) + "\r\n" + header + "\r\n";
        response += response_body;
        send(client.client_fd, response.c_str(), response.length(), 0);
        std::cout << "Client connected\n";
        close(client.client_fd);
        std::cout << "Client closed\n";
    }

    close(server->server_fd);

    return 0;
}
