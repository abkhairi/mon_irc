#ifndef CHANNELS_HPP
#define CHANNELS_HPP

#include "server.hpp"
#include "client.hpp"

class cliente;
class server;

class channels
{
    private:
        bool			_inv;
        bool			_flag_pass;
        bool			_topc;
        bool            _userlimit;
        size_t          _limit;
        size_t			_usersSize;
        std::string		_name;
        std::string		topicseter;
        std::string		topictime;
        std::string		_password;
        std::string		_topic;
        std::map<std::pair<bool, int>, cliente> _users;
        std::string		_nameToDisplay;
        std::vector<int> _invited;
    public:
        channels(std::string name_channel);
        void push_to_map(bool privilege, cliente &client_);
        //setter
        void set_password(std::string password);
        void set_flagpass(bool flagpass);
        void set_size_users(size_t size_user);
        //getter
        std::string get_password();
        bool get_flag_pass();
        std::string get_name_chan();
};


#endif