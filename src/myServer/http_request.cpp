//
// Created by kali on 6/27/24.
//

#include "http_request.h"

std::string Http_request::get_method() const
{
    return method;
};

void Http_request::set_method(std::string&& method)
{
    this->method = method;
};

std::string Http_request::get_endpoint() const
{
    return endpoint;
};

void Http_request::set_endpoint(std::string&& endpoint)
{
    this->endpoint = endpoint;
};

std::string Http_request::get_protocol() const
{
    return protocol;
};

void Http_request::set_protocol(std::string&& protocol)
{
    this->protocol = protocol;
};

void Http_request::parse_request(std::string&& request)
{
    std::string temp{request.substr(0, request.find("\r\n"))};
    set_method(request.substr(0, request.find(' ')));
    set_endpoint(temp.substr(temp.find(' ') + 1, temp.find_last_of(' ') - temp.find(' ') - 1));
    set_protocol(temp.substr(temp.find_last_of(' ') + 1, temp.find("\r\n") - temp.find_last_of(' ') - 1));
    parse_header(std::forward<std::string>(request));
    parse_body(std::forward<std::string>(request));
}

std::string Http_request::get_request(std::string&& request)
{
    return std::forward<std::string>(request);
}

void Http_request::parse_header(std::string&& request)
{
    size_t pos{request.find_first_of("\r\n")};
    std::string header{request.substr(pos + 2, request.find_last_of("\r\n") - pos - 4)};
    size_t cur{};
    size_t temp{};
    while (cur != header.npos)
    {
        cur = header.find("\r\n");
        temp = header.find(':');
        this->header[header.substr(0, temp)] = header.substr(temp + 2, cur - temp - 2 );
        header.erase(0, cur+2);
    }
}

const std::string&& Http_request::get_header(std::string&& key)
{
    return std::forward<std::string>(header[key]);
}

void Http_request::parse_body(std::string&& request)
{
    if (request.find_last_of("\r\n") + 2 > request.length()) return;
    std::string body{request.substr(request.find_last_of("\r\n") + 1)};
    this->body["body"] = body;
}

const std::string&& Http_request::get_body(std::string&& key)
{
    return std::forward<std::string>(body[key]);
}

std::ostream& operator<<(std::ostream& out, Http_request& request)
{
    out << request.get_method() << " " << request.get_endpoint() << " " << request.get_protocol() << "\r\n";
    for (auto key : request.header)
    {
        out << key.first << ": " << key.second << "\r\n";
    }
    out << "\r\n" << request.body["body"];
    return out;
}
