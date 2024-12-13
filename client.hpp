#ifndef CLIENT_H
#define CLIENT_H

#include "server.hpp"

class cliente
{
    private :
        int client_sock_fd;
        std::string ip_addr_client;
        std::string recvdata;
        bool         authenti;
        std::string _nickname;
        std::string _pass;
        std::string _user;
        // std::string nickname;
    public :
        cliente(int _client_sock_fd, std::string _ip_addr_client);
        void display_client();
        void set_recv_data(std::string b);
        int get_client_fd();
        std::string get_recvline();
        std::string get_nickname();
        std::string get_password();
        std::string get_user();
        bool        get_authen();


};



#endif