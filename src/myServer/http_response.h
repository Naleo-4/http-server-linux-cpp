//
// Created by kali on 6/27/24.
//

#ifndef HTTP_RESPONSE_H
#define HTTP_RESPONSE_H

#include "status_code.h"
#include <map>
#include <string>

class Http_response {
private:
    STATUS_CODE status{};
    std::string protocol{};
    std::map<std::string,std::string> header{};
    std::map<std::string,std::string> body{};
public:;
    friend std::ostream& operator<<(std::ostream& out, Http_response& request);
public:
    void set_header(std::string&&);
    void set_body(const std::string&&);
    void set_status(STATUS_CODE&&);
    void set_protocol(std::string&&);

    std::string to_string();
public:
    [[nodiscard]] STATUS_CODE get_status() const;
    [[nodiscard]] std::string get_protocol() const;
};



#endif //HTTP_RESPONSE_H
