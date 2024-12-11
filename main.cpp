#include <iostream>
#include <string>
#include <sys/socket.h> // Needed for socket creating and binding
#include <netinet/in.h> // Needed to use struct sockaddr_in
#include <fcntl.h> // for fcntl() function for non-blocking socket
#include <vector> // fpr vector
#include <poll.h> // for strcut pollfd

class serverr
{
    private:
        int _fd_sock_serv;
        int _port;
        std::string _pass;
        serverr();
    public:
        std::vector<struct pollfd> vec_pollfd;
        serverr(int port, std::string pass);
        ~serverr();
        void    initializer_server(int port, std::string pass);
        int     get_fd_sock_serv() { return _fd_sock_serv; }
        void    set_fd_sock_serv(int fd) { _fd_sock_serv = fd; }
        // int     parsing_port_and_pass(std::string port, std::string pass);
};

serverr::~serverr()
{}

serverr::serverr(int port, std::string pass)
{
    _port = port;
    _pass = pass;
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

void    serverr::initializer_server(int  port, std::string pass)
{
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);

    int opt = 1;// setsockopt : function in network programming is used to configure options on a socket.
    setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
    fcntl(sockfd, F_SETFL, O_NONBLOCK);

    struct sockaddr_in serverAddr;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(port);
    serverAddr.sin_addr.s_addr = INADDR_ANY;

    bind(sockfd, (struct sockaddr*)&serverAddr, sizeof(serverAddr)); // bound [ip and port] to server socket
    listen(sockfd, 10);
    std::cout << "\033[32m++++++++++++++++++++++++++++++++++\033[0m" << std::endl <<"\033[32m+\033[0m";
    std::cout << "\033[31m server listen in :             " <<"\033[32m+" << std::endl;
    std::cout << "\033[32m+ Port\033[0m     = " << port << "\033[32m                +\033[0m"<< std::endl;
    std::cout << "\033[32m+ Password\033[0m = " << pass << "\033[32m                  +\033[0m"<<std::endl;
    std::cout << "\033[32m++++++++++++++++++++++++++++++++++\033[0m" << std::endl;

    set_fd_sock_serv(sockfd);
    struct pollfd mon_pollfd;
    mon_pollfd.fd = get_fd_sock_serv();
    mon_pollfd.events = POLLIN;
    vec_pollfd.push_back(mon_pollfd);
    // while(true)
    // {
    //     // int res = poll();
    // }
}

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
    mon_server.initializer_server(port_int, pass);
    std::vector<struct pollfd>::iterator it = mon_server.vec_pollfd.begin();
    std::cout << "fd = " << it->fd << std::endl;
    return 0;
}