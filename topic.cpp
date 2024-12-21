#include "server.hpp"

void serverr::topic(std::vector<std::string > vec_cmd,size_t _index_client,cliente client_)
{
    if (!client_.get_authen())
	{
		send_msg_to_clinet(client_.get_client_fd(), ERR_NOTREGISTERED(client_.get_nickname(), host_ip));
		return ;
	}
	if (vec_cmd.size() < 2)
	{
		send_msg_to_clinet(client_.get_client_fd(), ERR_NEEDMOREPARAMS(client_.get_nickname(), host_ip));
		return;
	}
    // check if channel exicte or no
    if (!find_channel(vec_cmd[1]))
    {
            send_msg_to_clinet(client_.get_client_fd(),ERR_NOSUCHCHANNEL(host_ip, vec_cmd[1], client_.get_nickname()));
            return ;
    }
    channels &channel_ = getChannel(vec_cmd[1]);
    if(vec_cmd.size() == 2) 
    {
        if (channel_.get_topic_bool() == false) 
            send_msg_to_clinet(client_.get_client_fd(),RPL_NOTOPIC(host_ip, client_.get_nickname(),channel_.get_name_chan()));
        else 
        {
            send_msg_to_clinet(client_.get_client_fd(),RPL_TOPICDISPLAY(host_ip, client_.get_nickname(),channel_.get_name_chan(),channel_.getTopic()));
            send_msg_to_clinet(client_.get_client_fd(),RPL_TOPICWHOTIME(host_ip ,channel_.get_name_chan(), client_.get_nickname(),channel_.gettopicseter(),client_.get_user(),channel_.gettopictime()));
        }
    }
    if (vec_cmd.size() >= 3) 
    {
            std::string res; // res c est le sujet de channel
            for (size_t i = 2; i < vec_cmd.size(); i++) 
            {
                res += vec_cmd[i];
                if (i != vec_cmd.size())
                    res = res + " ";
            }
            std::cout << "|" << res << "|" << std::endl;
            channel_.setTopicAttr(res, true, client_.get_nickname(), _time());
            if (channel_.existe_nick(client_.get_nickname()) == false)
                send_msg_to_clinet(client_.get_client_fd(), ERR_NOTONCHANNEL(host_ip,client_.get_nickname()));
            else if (channel_.check_if_operator(client_.get_nickname()) == false) 
                send_msg_to_clinet(client_.get_client_fd(), ERR_CHANOPRIVSNEEDED(host_ip,client_.get_nickname()));
            else      
                SendToAll(channel_, RPL_NEWTOPICSETTED(client_.get_nickname(), channel_.gettopicseter(), host_ip, channel_.get_name_chan(),res));
    }
}

