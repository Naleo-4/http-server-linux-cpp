//
// Created by kali on 6/27/24.
//

#include "http_response.h"

#include <iostream>
STATUS_CODE Http_response::get_status() const
{
    return status;
};

void Http_response::set_status(STATUS_CODE&& status)
{
    this->status = status;
};

std::string Http_response::get_protocol() const
{
    return protocol;
};

void Http_response::set_protocol(std::string&& protocol)
{
    this->protocol = protocol;
};

void Http_response::set_header(std::string&& key, std::string&& content_type)
{
    header[key] = content_type;
}

void Http_response::set_body(const std::string&& body)
{
    this->body["body"] = body;
    header["Content-Length"] = std::to_string(this->body["body"].length());
}

std::ostream& operator<<(std::ostream& out, Http_response& response)
{
    out << response.get_protocol() << " " << response.get_status() << " " << format_status_string(status_code_to_string(response.get_status())) << "\r\n";
    for (auto key : response.header)
    {
        out << key.first << ": " << key.second << "\r\n";
    }
    out << "\r\n" << response.body["body"];
    return out;
}

std::string Http_response::to_string()
{
    std::string str;
    str += get_protocol() + " " + std::to_string(get_status()) + " " + format_status_string(status_code_to_string(get_status())) + "\r\n";
    for (auto key : header)
    {
        str += key.first + ": " + key.second + "\r\n";
    }
    str += "\r\n";
    if (body.contains("body")) str +=body["body"];
    return str;
}

