//
// Created by kali on 6/25/24.
//
#include "http_server.h"
#include "http_client.h"
#include "http_request.h"
#include "http_response.h"
#include "status_code.h"

#include <iostream>
#include <fstream>
#include <cstdlib>

const Http_server* Http_server::instance = nullptr;
Thread_pool Http_server::thread_pool(4);
std::string Http_server::path;

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

bool find_value(const std::vector<std::string>& vec,const std::string& value)
{
    for(const auto& i : vec)
    {
        if (i == value) return true;
    }
    return false;
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

std::string get_file_content(const std::string&& path)
{
    std::ifstream ifs{path};
    std::string str;
    str.assign((std::istreambuf_iterator<char>(ifs)),
               (std::istreambuf_iterator<char>()));
    return str;
}

void append_file_content(const std::string&& path, const std::string&& str)
{
    std::ofstream ofs{path, std::ios::app};
    ofs << str;
}

void Http_server::set_path(std::string&& path)
{
    Http_server::path = path;
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
        Http_request http_request;
        http_request.parse_request(std::forward<std::string>(request));
        // std::cout << http_request;

        STATUS_CODE status{OK};
        Http_response http_response;
        std::string method{http_request.get_method()};
        std::string endpoint{http_request.get_endpoint().erase(0, 1)};
        std::string endpoint2{get_end_point(endpoint)};

        bool is_request_file = endpoint.contains("files");
        bool has_echo = endpoint.contains("echo");
        bool has_user_agent = endpoint.contains("user-agent");

        if (endpoint.empty() || has_echo || has_user_agent || is_request_file)
        {
            if (has_echo)
            {
                http_response.set_header("Content-Type","text/plain");
                std::string str{"gzip"};

                if (find_value(http_request.get_header("Accept-Encoding"),str))
                {
                    const std::string temp = "echo -n " + endpoint2 + " | gzip > cmd.txt";
                    const char* command = temp.c_str();
                    system(command);
                    http_response.set_header("Content-Encoding","gzip");
                    http_response.set_body(get_file_content("cmd.txt"));
                }
                else http_response.set_body(std::forward<std::string>(endpoint2));
            }
            else if (has_user_agent)
            {
                http_response.set_body(static_cast<std::string>(http_request.get_header("User-Agent")[0]));
                http_response.set_header("Content-Type","text/plain");
            }
            else if (is_request_file)
            {
                if (http_request.get_method() == "POST")
                {
                    append_file_content(std::forward<std::string>(path + "/" + endpoint2),
                                        http_request.get_body("body"));
                    status = CREATED;
                }
                else if (http_request.get_method() == "GET")
                {
                    if (std::filesystem::exists(path + "/" + endpoint2))
                    {
                        http_response.set_body(std::forward<std::string>(get_file_content(path + "/" + endpoint2)));
                        http_response.set_header("Content-Type","application/octet-stream");
                    }
                    else
                    {
                        status = NOT_FOUND;
                    }
                }
            }
        }
        else
        {
            status = NOT_FOUND;
        }

        http_response.set_protocol(http_request.get_protocol());
        http_response.set_status(std::forward<STATUS_CODE>(status));
        std::string rs{http_response.to_string()};
        // std::cout<<rs;
        send(client_fd, rs.c_str(), rs.length(), 0);
    }
    close(client_fd);
};
