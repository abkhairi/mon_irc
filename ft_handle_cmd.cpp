#include "server.hpp"




void print_name_channel(cliente client_)
{
    std::vector<std::string> channels_name = client_.get_chan_name();
    std::vector<std::string>::iterator it = channels_name.begin();
    for (it = channels_name.begin(); it != channels_name.end(); it++)
    {
        std::cout << "name of channel qui " << client_.get_nickname() << " to join = "<< *it << std::endl;
    }
}


std::string get_str_channels(std::vector<std::string> v)
{
    std::string chan = v[1];
    // parsing = handeler par exmple [join #chan1,#chan2,,,,, key1,key2]
    return chan;
}

bool serverr::is_membre(int fd_client, channels channel_)
{
    std::map<std::pair<bool,int>, cliente > user_map = channel_.get_map_user();
    for(std::map<std::pair<bool,int>, cliente >::iterator it = user_map.begin(); it != user_map.end(); it++)
    {
        if (fd_client == it->second.get_client_fd())
            return  true; 
    }
    return false;
}

std::string getListOfNames(std::map<std::pair<bool, int>, cliente> _users)
{
    std::string listOfNames;
    std::map<std::pair<bool, int>, cliente>::iterator iter;
    for (iter = _users.begin(); iter != _users.end(); iter++)
    {
        if (iter->first.first == true)
            listOfNames += "@" + iter->second.get_nickname() + " ";
        else
            listOfNames += iter->second.get_nickname() + " ";
    }
    return listOfNames;
}


void print_info_user(channels  &obj)
{
    std::map<std::pair<bool, int>, cliente> user_map = obj.get_map_user();
    std::map<std::pair<bool, int>, cliente>::iterator it2;
    for (it2 = user_map.begin() ; it2 != user_map.end(); it2++)
    {
        std::cout << WHITE << "+" << RESET << BLUE << "          nickname : "<< RESET << WHITE<< it2->second.get_nickname()<< RESET<< "                  "<<  std::endl;
        std::cout << WHITE << "+" << RESET << BLUE << "          sock_fd  : "<< RESET << WHITE<< it2->second.get_client_fd()<< RESET<< "                         "<<  std::endl;
        std::cout << WHITE << "+" << RESET << BLUE << "          authentic: "<< RESET << WHITE<< it2->second.get_authen()<< RESET<< "                         "<<  std::endl;
        std::cout << WHITE << "+" << RESET << BLUE << "          operator : "<< RESET << WHITE<< it2->first.first<< RESET<< "                         "<<  std::endl;
        std::cout << WHITE << "---------------------------------------" << RESET << std::endl; 
    }
}

void print_channel(std::map<std::string, channels> channels_)
{
    //print channel name and code 
    std::map<std::string, channels>::iterator it = channels_.begin();
    for (it = channels_.begin(); it != channels_.end(); it++)
    {
        std::cout << WHITE << std::setfill('+') << std::setw(15) << "+" << RESET;   
        std::cout << YELLOW << " Channel " << RESET << WHITE<< std::setfill('+') << std::setw(15) << "+"<<RESET << std::endl;              
        std::cout << WHITE << "+" << RESET << BLUE << " NAME of channel : "<< RESET << WHITE<< it->first<< RESET<< "            +"<<  std::endl;
        std::cout << WHITE << "+" << RESET << BLUE << " PASS of channel : "<< RESET << WHITE<< it->second.get_password()<< RESET<< "              +"<< std::endl;
        std::cout << WHITE << "+++++++++++++++++++++++++++++++++++++++" << RESET << std::endl; 
        print_info_user(it->second);
    }
}

void    serverr::broadcastMessage(channels _channel, std::string _message, int _clientfd)
{
    std::map<std::pair<bool, int>, cliente> mapOfClients = _channel.get_map_user();
    std::map<std::pair<bool, int>, cliente>::iterator iter;
    for(iter = mapOfClients.begin(); iter != mapOfClients.end(); iter++)
    {
        if (iter->second.get_client_fd() != _clientfd)
            send_msg_to_clinet(iter->second.get_client_fd(), _message);
    }
}

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


void serverr::kick(std::vector<std::string > vec_cmd, size_t _index_client, cliente client_)
{
    std::stringstream users(vec_cmd[2]);
    std::string user;
    std::string commntaire;

    // std::string nick = client_.get_nickname();
    while (std::getline(users,user, ',')) 
    {
        if (client_.get_authen() == false)
        {
            send_msg_to_clinet(client_.get_client_fd(),ERR_NOTREGISTERED(client_.get_nickname(), host_ip));
            return ;
        }
        if (vec_cmd.size() < 3) {
            send_msg_to_clinet(client_.get_client_fd(),ERR_NEEDMOREPARAMS(client_.get_nickname(), host_ip));
            return ;
        }
        channels &obj_chan = getChannel(vec_cmd[1]);
        std::string str = vec_cmd[1];
        // commantaire c est a partir de : commaintaire 
        if (vec_cmd.size() == 3 || (vec_cmd.size() == 4 && vec_cmd[3] == ":"))
            commntaire = ":Break the Rules";
        else 
        {
            for (size_t i = 3; i < vec_cmd.size(); i++) 
            {
                commntaire = commntaire + vec_cmd[i];
                if (i < vec_cmd.size()) 
                    commntaire = commntaire + " ";
            }
        }
        if (str[0] == '#' && str.length() > 1)
            vec_cmd[0] = vec_cmd[0].substr(1);
        else 
        {
            send_msg_to_clinet(client_.get_client_fd(),ERR_NOSUCHCHANNEL(host_ip, str, client_.get_nickname()));
            return ;
        }
        if (!(find_channel(vec_cmd[1]))) {
            send_msg_to_clinet(client_.get_client_fd(),ERR_NOSUCHCHANNEL(host_ip, str, client_.get_nickname()));
            return ;
        }
        if (obj_chan.check_if_operator(client_.get_nickname()) == false) 
        {
            send_msg_to_clinet(client_.get_client_fd(),ERR_NOTOP(host_ip, str));
            return ;
        }
        if (obj_chan.existe_nick(user) == false) {
            send_msg_to_clinet(client_.get_client_fd(), ERR_NOSUCHNICK(host_ip, str, vec_cmd[2]));
            return;
        }
        // if (check_if_nick_in_chan(vec[1], vec[2]) == false) {
        // 	sendTo(ERR_USERNOTINCHANNEL(Server::_hostname, vec[1]));
        // 	return ;
        // }
        else 
        {
            SendToAll(obj_chan, RPL_KICK(get_client_by_index(_index_client -1).get_nickname(), client_.get_user(), serverr::get_hostip(), user, vec_cmd[1], commntaire));
            obj_chan.deletClient(user);
            obj_chan.set_size_users(-1);
        }
    }
}





void serverr::ft_commande_j_m(std::vector<std::string> vec_cmd, size_t &_index_client, cliente &client_)
{
    std::cout << "\033[32m commande => \033[0m" << vec_cmd[0] << std::endl;
    if (client_.get_authen() == true)
    {
        //cmd =  mode invite topic part kick quite ...
        if (to_lower(vec_cmd[0]) == "join")
        {
            if (vec_cmd.size() < 2)
                send_msg_to_clinet(client_.get_client_fd(), ERR_NEEDMOREPARAMS(client_.get_nickname(), host_ip));
            else
                ft_join(vec_cmd, client_, _index_client);
        }
        else if (to_lower(vec_cmd[0]) == "kick")
        {
            kick(vec_cmd, _index_client, client_);
        }
        else if (to_lower(vec_cmd[0]) == "topic")
        {
            topic(vec_cmd, _index_client, client_);
        }

    }
    else
        send_msg_to_clinet(client_.get_client_fd(), ERR_ALREADYREGISTERED(client_.get_nickname(), host_ip));
}   

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

