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


void send_msg_to_clinet(int fd_client, std::string mon_msg)
{
    ssize_t bytes = send(fd_client, mon_msg.c_str(), mon_msg.size(), 0);
    if (bytes == -1)
        perror("send: ");
}


std::string to_lower(std::string str)
{
    for (size_t i =0; str.size() > i; i++)
        str[i] = tolower(str[i]); 
    return (str);
}