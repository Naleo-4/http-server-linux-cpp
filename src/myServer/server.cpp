#include "status_code.h"

#include <cstring>

#include "http_server.h"
#include <iostream>
#include <fstream>
#include <pthread.h>
#include <sys/stat.h>
// #include <filesystem>
// #include <thread>

int main(int argc, char* argv[])
{
    // std::filesystem::create_directories("./files");
    // std::filesystem::create_directories("./files/temp");
    // f.open("file/temp/temp");

    // std::ofstream f{"files/temp/temp"};
    // f << "test line1!";
    // f.close();
    for ( int i = 1 ; i < argc ; ++i )
    {
        if (!strcmp( argv[i], "--directory" ))
        {
            std::string path{argv[2]};
            Http_server::set_path(std::forward<std::string>(path));
        }
    }
    const Http_server* server = Http_server::get_instance();

    std::cout << "Waiting for a client to connect...\n";
    // Http_server::accept_connection();
    while (true)
    {
        Http_server::accept_connection();
    }

    close(server->server_fd);

    return 0;
}
