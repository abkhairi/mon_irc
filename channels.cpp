#include "channels.hpp"

channels::channels(std::string name_channel)
{
    _name = name_channel;
    _inv = false;
    _flag_pass = false;
    _topc = false;
    _limit = -1;
    _usersSize = 0;
}


bool	channels::isInvited(int sockfd)
{
	std::vector<int>::iterator it = std::find(_invited.begin(), _invited.end(), sockfd);
    if(it != _invited.end())
    {
        return true;
    }
    return false;
}


bool channels::get_inv()
{
    return (_inv);
}

void channels::set_inv(bool in)
{
    _inv = in;
}

bool channels::get_si_user_limit()
{
    return (_userlimit);
}

size_t channels::get_size_user()
{
    return (_usersSize);
}

size_t channels::get_limit()
{
    return (_limit);
}

std::string channels::get_name_chanel_display()
{
    return (name_channel_display);
}

void channels::set_name_display(std::string name)
{
    name_channel_display = name;
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

std::map<std::pair<bool,int>, cliente >& channels::get_map_user()
{
	return _users;
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

