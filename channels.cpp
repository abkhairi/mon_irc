#include "channels.hpp"

channels::channels(std::string name_channel)
{
    _name = name_channel;
}

void channels::push_to_map(bool privilege, cliente &client_)
{
    std::pair<bool, int> paire(privilege, client_.get_client_fd());
	_users.insert(std::make_pair(paire, client_));
}

std::string channels::get_name_chan()
{
    return (_name);
}

std::string channels::get_password()
{
    return (_password);
}

void channels::set_size_users(size_t size_user)
{
    _usersSize = size_user + _usersSize; 
}

bool channels::get_flag_pass()
{
    return (_flag_pass);
}
void channels::set_password(std::string password)
{
    _password = password;
}

void channels::set_flagpass(bool flagpass)
{
    _flag_pass = flagpass;
}

