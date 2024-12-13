#include "server.hpp"
#include "client.hpp"


std::string ft_gethostname()
{
    char hostname[256]; // Buffer to store the hostname
    if (gethostname(hostname, sizeof(hostname)) == 0)
        std::cout << "Hostname: " << hostname << std::endl;
    else
        std::cerr << "Error retrieving hostname: " << strerror(errno) << std::endl;
    return (hostname);
}

void serverr::handeler_authen_and_commande(std::string cmd_final,size_t &_index_client)
{
    std::vector<std::string> &vec_of_commande = vec_of_cmd;
    std::string host_ip = ft_gethostname();

    std::cout << "hostname is " << host_ip << std::endl;
    cliente &client_ = get_client_by_index(_index_client - 1);

    if (client_.get_authen() == false)
    {
        // std::cout << "here\n";
        if (vec_of_commande.size() <= 1)
        {
            if (vec_of_commande[0] == "nick")
                send_msg_to_clinet(client_.get_client_fd(), ":irc.abkhairi.chat 431 :No nickname given\r\n");
            vec_of_commande.clear();
            return ;
        }   
        if (vec_of_commande[0] == "pass" && vec_of_commande[1] == _pass && vec_of_commande.size() == 2 && client_.get_flag_pass() == false)
        {
            std::cout << "is a pass cmd" << std::endl;
            client_.set_flag_pass(true);
            client_.set_password(vec_of_commande[1]);
            vec_of_commande.clear();
            return ;
        }
        else if (vec_of_commande[0] == "pass" && vec_of_commande[1] != _pass && vec_of_commande.size() == 2 && client_.get_flag_pass() == false)
        {
            send_msg_to_clinet(client_.get_client_fd(), ":irc.abkhairi.chat 464 :Password incorrect\r\n");
            vec_of_commande.clear();
            return ;
        }
        if (vec_of_commande[0] == "nick" && vec_of_commande.size() == 2 && client_.get_flag_nick() == false)
        {
            // check if any client has the same nickname ft_check_nickname()
            client_.set_flag_nick(true);
            client_.set_nickname(vec_of_commande[1]);
            vec_of_commande.clear();
            return ;
        }
        else if (vec_of_commande[0] == "nick" && vec_of_commande.size() > 2 && client_.get_flag_nick() == false)
        {
            send_msg_to_clinet(client_.get_client_fd(), ":irc.abkhairi.chat 432 :Erroneous nickname\r\n");
            vec_of_commande.clear();
            return ;
        }
        if (vec_of_commande[0] == "user" && client_.get_flag_user() == false)
        {
            if (vec_of_commande.size() != 5)
            {
                send_msg_to_clinet(client_.get_client_fd(), ":irc.abkhairi.chat 461 :Need more parameters\r\n");
                vec_of_commande.clear();
                return ;
            }
            else
            {
                client_.set_flag_user(true);
                client_.set_user(cmd_final);
                std::cout << "user is " << client_.get_user() << std::endl;
                vec_of_commande.clear();
                return ;
            }
        }
        else
        {
            vec_of_commande.clear();
            return ;
        }
    }
    else
    {
        //deja authenticater that client ??????

        // std::cout << "cmd0 = " << vec_of_commande[0] << std::endl;
        // if (vec_of_commande[0] == "pass")
        // {
        //     std::cout << "is a pass cmd" << std::endl;
        // }
        // else
        //     return ;
    }

}