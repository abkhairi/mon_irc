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
#include <ctime>
#include <map>
#include <iomanip> // Nécessaire pour std::setfill et std::setw

#include "client.hpp"
#include "reply_msg.hpp"
#include "channels.hpp"


class cliente;
class channels;

class serverr
{
    private:
        int         _fd_sock_serv;
        int         _port;
        std::string _pass;
        std::string host_ip;
        std::vector <cliente> vec_client;
        serverr();
    public:
        size_t      _index_client;
        std::vector<struct pollfd> vec_pollfd;
        std::map<std::string, channels> channels_;
        std::vector<std::string> vec_of_cmd;
        serverr(int port, std::string pass);
        ~serverr();
        bool    find_channel(std::string chan);
        void    initializer_server(int  port, std::string pass, size_t &i);
        int     get_fd_sock_serv() { return _fd_sock_serv; }
        void    set_fd_sock_serv(int fd) { _fd_sock_serv = fd; }
        cliente& get_client_orgien(int socket_client);
        cliente& get_client_by_index(size_t index);
        std::string get_hostip();
        void    authenticate_client(std::string cmd,int socket_client, cliente &clienteref, size_t &_index_client);
        void    display();
        void is_registre(cliente &client_, std::string time_);
        std::string receive_cmd(int fd_client, size_t &_index_client);
        void handeler_authen_and_commande(std::string cmd_final,size_t &_index_client);
        void ft_commande_j_m(std::vector<std::string> cmd_final, size_t &_index_client, cliente &client_);
        bool is_membre(int fd_client, channels channel_);
        void    broadcastMessage(channels _channel, std::string _message, int _clientfd);
        channels & getChannel(std::string channel);
        void    SendToAll(channels channel_, std::string _message);
        std::string  _time();
        // commande 
        void ft_join(std::vector<std::string> &vec_cmd,cliente &client_,size_t &_index_client);
        void kick(std::vector<std::string > vec_cmd,size_t _index_client, cliente client_);
        void topic(std::vector<std::string > vec_cmd,size_t _index_client,cliente client_);

};




int parsing_port_and_pass(std::string port, std::string pass);
void setNonBlocking(int fd) ;

void send_msg_to_clinet(int fd_client, std::string mon_msg);
std::string to_lower(std::string str);



#endif