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
        std::string		name_channel_display;
        std::vector<int> _invited;
    public:
        channels(std::string name_channel);
        void push_to_map(bool privilege, cliente &client_);
        //setter
        void set_password(std::string password);
        void set_flagpass(bool flagpass);
        void set_size_users(size_t size_user);
        void set_name_display(std::string name);
        void set_inv(bool in);
        void setTopic(std::string topic) ;
        void  setTopicAttr(std::string topic, bool topc, std::string seter, std::string time);


        //getter
        bool get_inv();
        bool get_si_user_limit();
        std::string get_password();
        bool get_flag_pass();
        size_t get_size_user();
        std::string get_name_chan();
        size_t get_limit();
        std::string get_name_chanel_display();
        std::map<std::pair<bool,int>, cliente >& get_map_user();
        std::string getTopic();
        bool        get_topic_bool();
        std::string     gettopicseter();
        std::string     gettopictime();
        bool    check_if_operator(std::string nickname);
        bool    existe_nick(std::string user);
        //
        bool	isInvited(int sockfd);
        void deletClient(std::string nick);
};



#endif