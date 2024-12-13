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
    //check if authenticate or no
    if (get_client_by_index(_index_client - 1).get_authen() == false)
    {
        std::cout << "had client awel mara lih hhh\n";
        if (vec_of_commande.size() <= 1)
            send_msg_to_clinet(get_client_by_index(_index_client).get_client_fd(), );
        // check_authentication();
    }
    else
    {
        //deja authenticater that client ??????
        
        std::cout << "cmd0 = " << vec_of_commande[0] << std::endl;
        if (vec_of_commande[0] == "pass")
        {
            std::cout << "is a pass cmd" << std::endl;
        }
        else
            return ;
    }

}