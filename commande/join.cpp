#include "../server.hpp"


void serverr::ft_join(std::vector<std::string> &vec_cmd,cliente &client_,size_t &_index_client)
{
    std::string nickname = client_.get_nickname();
    if (vec_cmd[1].empty())
        send_msg_to_clinet(client_.get_client_fd(), ERR_NEEDMOREPARAMS(client_.get_nickname(), host_ip));

    std::stringstream split_channel(vec_cmd[1]);
    std::stringstream split_key(vec_cmd[2]);
    std::string name_channel;
    std::string password;
    std::string list_operator;

    while (std::getline(split_channel, name_channel, ','))
    {
        std::string name_chan_lower = to_lower(name_channel);
        std::cout << "name channel = " << name_chan_lower << std::endl;
        std::map<std::string, channels>::iterator it = channels_.find(name_chan_lower);
        if (it == channels_.end()) // dos not channel so name name_chan_lower 
        {
            list_operator = "@"+nickname;
            channels new_channel(name_chan_lower);
            new_channel.push_to_map(true, client_);// push to map_user
            if (std::getline(split_key, password, ','))
            {
                new_channel.set_password(password);
                new_channel.set_flagpass(true);
            }
            new_channel.set_name_display(name_channel);
            new_channel.set_size_users(1);
            channels_.insert(std::make_pair(name_chan_lower,new_channel));
            send_msg_to_clinet(client_.get_client_fd(), RPL_JOIN(nickname, nickname,name_channel, host_ip));
            send_msg_to_clinet(client_.get_client_fd(), RPL_NAMREPLY(host_ip, list_operator, name_channel, nickname));
            send_msg_to_clinet(client_.get_client_fd(), RPL_ENDOFNAMES(host_ip, nickname, name_channel));
        }
        else if (it != channels_.end()) // deja kyna channel
        {
            // check if client into channel
            if (is_membre(client_.get_client_fd(), it->second) == true)
            {
                std::cout << "Client " << nickname << " is already a member in this channel" << std::endl;
                return ;
            }
            if (it->second.get_si_user_limit() == true)
            {
                if (it->second.get_size_user() >= it->second.get_limit())
                {
                    send_msg_to_clinet(client_.get_client_fd(), "471 " + nickname + " " +  it->second.get_name_chanel_display() + " :Cannot join channel, it's full (+l)\r\n");
                    return ;
                }
            }
            if (it->second.get_flag_pass() == true)
            {
                std::getline(split_key,password, ',');
                if(it->second.get_password() != password)
                {
                    send_msg_to_clinet(client_.get_client_fd(), ERR_BADCHANNELKEY(nickname, name_channel));
                    return ;
                }
            }
            if (it->second.get_inv() == true && it->second.isInvited(client_.get_client_fd()) == false)
            {
                //<client> <channel> :Cannot join channel (+i)
                send_msg_to_clinet(client_.get_client_fd(), "473 " + nickname + " " +  it->second.get_name_chanel_display() + " :Cannot join channel (+i)\r\n");
                return ;
            }
            it->second.push_to_map(false, client_);
            it->second.set_size_users(1);
            // it->second.removeInvitedClient(clientfd);
            list_operator = getListOfNames(it->second.get_map_user());
            // send msg to client for entrer to channel
            send_msg_to_clinet(client_.get_client_fd(), RPL_JOIN(nickname, nickname, it->second.get_name_chanel_display(), host_ip));
            // send msg all client for any client entrer channel 
            broadcastMessage(it->second, RPL_JOIN(nickname, nickname,it->second.get_name_chanel_display(), host_ip), client_.get_client_fd());
            // diaplay list the client in channels par exmple : 353 hicham = #chan1 :idryab hicham @abkhairi
            send_msg_to_clinet(client_.get_client_fd(), RPL_NAMREPLY(host_ip, list_operator, it->second.get_name_chanel_display(), nickname));
            //Sent as a reply to the NAMES command, this numeric specifies the end of a list of channel member names.
            send_msg_to_clinet(client_.get_client_fd(), RPL_ENDOFNAMES(host_ip, nickname, it->second.get_name_chanel_display()));
            broadcastMessage(it->second, RPL_ENDOFNAMES(host_ip, nickname, it->second.get_name_chanel_display()), client_.get_client_fd());
            // send msg to client topic the channel = sujet de channel
            send_msg_to_clinet(client_.get_client_fd(), RPL_TOPIC(host_ip, nickname, it->second.get_name_chanel_display(),it->second.getTopic()));
            // send msg to client specifically who set topic (sujet) the channel
            send_msg_to_clinet(client_.get_client_fd(),RPL_TOPICWHOTIME(host_ip, it->second.get_name_chanel_display(), nickname,it->second.gettopicseter(),client_.get_user(),it->second.gettopictime()));

        }
    }
    // here push name the channel in vector client : donc chaque client contient vector qui remplir par name des channels qui join
    client_.pushChannel(name_channel);
    print_channel(channels_);
    // print_name_channel(client_); for display all channel the obj client that join
    vec_cmd.clear();
}
