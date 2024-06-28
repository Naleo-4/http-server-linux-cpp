//
// Created by kali on 6/27/24.
//

#ifndef HTTP_REQUEST_H
#define HTTP_REQUEST_H
#include <map>
#include <vector>
#include <string>
#include <iostream>
#include <cstring>

class Http_request {
private:
    std::string method;
    std::string endpoint;
    std::string protocol;
    std::map<std::string,std::vector<std::string>> header;
    std::map<std::string,std::string> body;
public:
    std::string get_request(std::string&&);
    void parse_request(std::string&&);
    friend std::ostream& operator<<(std::ostream& out, Http_request& request);

private:
    void parse_header(std::string&&);
    void parse_body(std::string&&);

    void set_method(std::string&&);
    void set_endpoint(std::string&&);
    void set_protocol(std::string&&);

public:
    [[nodiscard]] const std::vector<std::string>&& get_header(std::string&&);
    [[nodiscard]] const std::string&& get_body(std::string&&);
    [[nodiscard]] std::string get_method() const;
    [[nodiscard]] std::string get_endpoint() const;
    [[nodiscard]] std::string get_protocol() const;
};

#endif //HTTP_REQUEST_H
