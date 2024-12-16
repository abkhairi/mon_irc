#include "server.hpp"


void serverr::ft_join(std::vector<std::string> &vec_cmd,cliente &client_,size_t &_index_client)
{
    
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