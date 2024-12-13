#include "server.hpp"

void setNonBlocking(int fd) 
{
    int flags = fcntl(fd, F_GETFL, 0);
    if (flags == -1) {
        perror("fcntl");
        exit(EXIT_FAILURE);
    }
    if (fcntl(fd, F_SETFL, flags | O_NONBLOCK) == -1) {
        perror("fcntl");
        exit(EXIT_FAILURE);
    }
}

int parsing_port_and_pass(std::string port, std::string pass)
{
    if (port.empty())
        return 1;
    for (int i = 0; i < port.size(); i++)
    {
        if (isdigit(port[i]) == 0)
            return 1;
    }
    int port_int = atoi(port.c_str());
    if (port_int < 0 || port_int > 65535)
        return 1;
    if (port_int < 1024)
        return 1;
    if (pass == "" || pass.empty())
        return 2;
    return port_int;
}
