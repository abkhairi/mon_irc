#include "../server.hpp"

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
            commntaire = ":no respect rule";
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
