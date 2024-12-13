#ifndef SERVER_H
#define SERVER_H

#include <iostream>
#include <string>
#include <sys/socket.h> // Needed for socket creating and binding
#include <netinet/in.h> // Needed to use struct sockaddr_in
#include <fcntl.h> // for fcntl() function for non-blocking socket
#include <vector> // fpr vector
#include <poll.h> // for strcut pollfd
#include <arpa/inet.h>  // For inet_ntoa()
#include <unistd.h> // for close() function
#include <cctype> // for isdigit() function and tolower() function
#include <sstream> // for std::stringstream
#include <cstring> // for std::strstr
#include "client.hpp"

class cliente;

class serverr
{
    private:
        int         _fd_sock_serv;
        int         _port;
        std::string _pass;
        std::vector <cliente> vec_client;
        serverr();
    public:
        size_t      _index_client;
        std::vector<struct pollfd> vec_pollfd;
        std::vector<std::string> vec_of_cmd;
        serverr(int port, std::string pass);
        ~serverr();
        void    initializer_server(int  port, std::string pass, size_t &i);
        int     get_fd_sock_serv() { return _fd_sock_serv; }
        void    set_fd_sock_serv(int fd) { _fd_sock_serv = fd; }
        cliente& get_client_orgien(int socket_client);
        void    authenticate_client(std::string cmd,int socket_client, cliente &clienteref, size_t &_index_client);
        void    display();
        std::string receive_cmd(int fd_client, size_t &_index_client);
        void handeler_authen_and_commande(std::string cmd_final,size_t &_index_client);
};

int parsing_port_and_pass(std::string port, std::string pass);
void setNonBlocking(int fd) ;

#endif