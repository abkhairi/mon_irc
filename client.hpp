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
        std::string _pass_client;
        std::string _user;
        bool        _flag_pass;
        bool        _flag_nick;
        bool        _flag_user;
        std::vector<std::string> channels;
    public :
        cliente(int _client_sock_fd, std::string _ip_addr_client);
        void          display_client();
        // getter
        int         get_client_fd();
        std::string get_recvline();
        std::string get_nickname();
        std::string get_password();
        std::string get_user();
        bool        get_authen();
        bool        get_flag_pass();
        bool        get_flag_nick();
        bool        get_flag_user();
        std::string get_ip_addr_client();
        std::vector<std::string > get_chan_name();
        //settter
        void        set_recv_data(std::string b);
        void        set_flag_pass(bool flag_pass);
        void        set_flag_nick(bool flag_nick);
        void        set_flag_user(bool flag_user);
        void       set_nickname(std::string nickname);
        void      set_password(std::string pass);
        void      set_user(std::string user);
        void        set_authen();
        //
        void    pushChannel(std::string channel);
        // void cliente::print_name_channel(cliente client_);
};


#endif