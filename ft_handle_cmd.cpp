#include "server.hpp"

std::string get_str_channels(std::vector<std::string> v)
{
    std::string chan = v[1];
    // parsing = handeler par exmple [join #chan1,#chan2,,,,, key1,key2]
    return chan;
}

// std::map<std::string, channels> get_chan_key(std::vector<std::string> &vec_cmd, std::string str_channels)
// {
//     std::map<std::string, channels > channels_;
//     std::stringstream split_chan(str_channels);
//     std::string one_chan;
//     split_chan << str_channels;
//     if (vec_cmd.size() < 3)
//     {
//         while(std::getline(split_chan, one_chan, ','))
//             channels_.insert(std::make_pair(one_chan, ""));
//     }
//     else
//     {
//         std::stringstream split_keys;
// 		std::string key;
// 		split_keys << vec_cmd[2];
// 		while (std::getline(split_chan, one_chan, ','))
// 		{
// 			if (std::getline(split_keys, key, ','))
// 				channels_.insert(std::make_pair(one_chan, key));
// 			else
// 				channels_.insert(std::make_pair(one_chan, ""));
// 		}
//     }
//     return (channels_);
// }

void serverr::ft_join(std::vector<std::string> &vec_cmd,cliente &client_,size_t &_index_client)
{
    std::string nickname = client_.get_nickname();
    if (vec_cmd[1].empty())
        send_msg_to_clinet(client_.get_client_fd(), ERR_NEEDMOREPARAMS(client_.get_nickname(), host_ip));

    std::stringstream split_channel(vec_cmd[1]);
    std::stringstream split_key(vec_cmd[2]);
    std::string name_channel;
    std::string password;

    while (std::getline(split_channel, name_channel, ','))
    {
        std::string name_chan_lower = to_lower(name_channel);

        std::map<std::string, channels>::iterator it = channels_.find(name_chan_lower);
        if (it == channels_.end()) // dos not channel so name name_chan_lower 
        {
            std::string list_operator = "@"+nickname;
            std::cout << "mkynach channel\n";
            channels new_channel(name_chan_lower);
            new_channel.push_to_map(true, client_);
            if (std::getline(split_key, password, ','))
            {
                new_channel.set_password(password);
                new_channel.set_flagpass(true);
            }
            new_channel.set_size_users(1);
            channels_.insert(std::make_pair(name_chan_lower,new_channel));
            
        }
        else if (it != channels_.end()) // kyna channel
        {
            std::cout << "kyna channel\n";
        }
    }


    // std::string str_channels = get_str_channels(vec_cmd);
    // channels_ = get_chan_key(vec_cmd, str_channels);
    // for (std::vector<std::pair<std::string, std::string> >::iterator it = v_pair.begin(); it != v_pair.end(); ++it)
    // {
    //     if (it->first[0] == '#' && it->first.length() > 1)
    //     {

    //     }
    //     else
    //         send_msg_to_clinet(client_.get_client_fd(), ERR_NOSUCHCHANNEL(host_ip,it->first,client_.get_nickname()));
    // }

}

void serverr::ft_commande_j_m(std::vector<std::string> vec_cmd, size_t &_index_client, cliente &client_)
{
    std::cout << "\033[32m commande => \033[0m" << vec_cmd[0] << std::endl;
    if (client_.get_authen() == true)
    {
        if(to_lower(vec_cmd[0]) == "join")
        {
            if (vec_cmd.size() < 2)
                send_msg_to_clinet(client_.get_client_fd(), ERR_NEEDMOREPARAMS(client_.get_nickname(), host_ip));
            else
            {
                ft_join(vec_cmd, client_, _index_client);
            }
        }
        //cmd =  mode invite topic part kick quite ...
    }
    else
    {
        send_msg_to_clinet(client_.get_client_fd(), ERR_ALREADYREGISTERED(client_.get_nickname(), host_ip));
    }

}   