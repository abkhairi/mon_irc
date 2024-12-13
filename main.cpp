#include "server.hpp"


int main(int ac, char** av)
{
    if (ac != 3)
    {
        std::cout << "invalid arugments" << std::endl;
        return  1;
    }
    std::string port = av[1];
    std::string pass = av[2];

    int port_int = parsing_port_and_pass(port, pass);
    if (port_int == 1)
    {
        std::cout << "Error or invalid port" << std::endl;
        return 1;
    }
    if (port_int == 2)
    {
        std::cout << "Error password" << std::endl;
        return 1;
    }
    serverr mon_server(port_int, pass);
    size_t &i = mon_server._index_client;
    mon_server.initializer_server(port_int, pass, i);
    return 0;
}
