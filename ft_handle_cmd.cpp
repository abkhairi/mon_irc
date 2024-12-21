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
