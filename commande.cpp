#include "server.hpp"
#include "client.hpp"


void serverr::handeler_authen_and_commande(std::string cmd_final,size_t &_index_client)
{
    std::vector<std::string> &vec_of_commande = vec_of_cmd;
    //check if authenticate or no

    // check if command is pass or user or ...
    if (vec_of_commande[0] == "pass")
    {

    }
}
    