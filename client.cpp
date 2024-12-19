#include "client.hpp"



void    cliente::pushChannel(std::string channel) 
{
    channels.push_back(channel);
}

std::string cliente::get_ip_addr_client()
{
    return (ip_addr_client);
}

std::vector<std::string > cliente::get_chan_name()
{
    return (channels);
}



std::string cliente::get_recvline()
{
    return recvdata;
}

void cliente::set_authen()
{
    authenti = true;
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
    client_sock_fd  = _client_sock_fd;
    ip_addr_client  = _ip_addr_client;
    authenti        = false;
    _nickname       = "";
    _pass_client    = "";
    _user           = "";
    _flag_pass      = false;
    _flag_nick      = false;
    _flag_user      = false;
}

int cliente::get_client_fd()
{
    return (client_sock_fd);
}

std::string cliente::get_nickname(){
    return (_nickname);
}

std::string cliente::get_password(){
    return (_pass_client);
}

std::string cliente::get_user()
{
    return (_user);
}

bool cliente::get_authen()
{
    return (authenti);
}

bool        cliente::get_flag_pass()
{
    return (_flag_pass);
}
bool        cliente::get_flag_nick()
{
    return (_flag_nick);
}
bool        cliente::get_flag_user()
{
    return (_flag_user);
}

void        cliente::set_flag_pass(bool flag_pass){
    _flag_pass = flag_pass;
}

void        cliente::set_flag_nick(bool flag_nick){
    _flag_nick = flag_nick;
}

void        cliente::set_flag_user(bool flag_user){
    _flag_user = flag_user;
}

void cliente::set_nickname(std::string nickname)
{
    _nickname = nickname;
}

void      cliente::set_password(std::string pass)
{
    _pass_client = pass;
}
void      cliente::set_user(std::string user)
{
    _user = user.substr(5, user.size() - 5);
}