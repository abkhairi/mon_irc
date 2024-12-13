#include "client.hpp"


std::string cliente::get_recvline()
{
    return recvdata;
}

void cliente::set_recv_data(std::string b)
{
    recvdata = b;
}

void cliente::display_client()
{
        std::cout <<"fd the client " << this->client_sock_fd << std::endl;
        std::cout <<"ip the client " << this->ip_addr_client << std::endl;
}

cliente::cliente(int _client_sock_fd, std::string _ip_addr_client)
{
    client_sock_fd = _client_sock_fd;
    ip_addr_client = _ip_addr_client;
}

int cliente::get_client_fd()
{
    return (client_sock_fd);
}