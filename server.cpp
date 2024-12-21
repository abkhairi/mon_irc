
#include "server.hpp"

serverr::~serverr()
{}

channels & serverr::getChannel(std::string channel) 
{
    std::map<std::string, channels>::iterator it = channels_.find(to_lower(channel));
    if (it == channels_.end())
        throw "No channel found";
    return it->second;
}


bool   serverr::find_channel(std::string chan)
{
    std::map<std::string, channels>::iterator it = channels_.begin();
    for (it = channels_.begin(); it !=  channels_.end(); it++)
	{
            if (to_lower(it->first) == to_lower(chan))
            {
                std::cout << "true and name channel is = " << (it->first) << std::endl;
			    return true;
            }
	}
	return false;
}

serverr::serverr(int port, std::string pass)
{
    _port = port;
    _pass = pass;
}

cliente& serverr::get_client_orgien(int socket_client)
{
    for (size_t i = 0; vec_client.size() > i; i++)
    {
        if (vec_client[i].get_client_fd() == socket_client)
            return (vec_client[i]);
    }
    return vec_client[0];
}

void    serverr::authenticate_client(std::string cmd, int socket_client, cliente &clienteref, size_t &_index_client)
{
    for (size_t i = 0; i < cmd.size(); i++)
        cmd[i] = std::tolower(cmd[i]);
    std::stringstream ss(cmd);
    int i = 0;
    while (ss >> cmd)
    {
        vec_of_cmd.push_back(cmd);
        i++;
    }
    if (strstr(clienteref.get_recvline().c_str(), "\n"))
    {
        size_t position = clienteref.get_recvline().find_first_of("\n");
        if (position > clienteref.get_recvline().size())
            return;
        // std::cout << "position = " << position << std::endl;
        std::string cmd_final = clienteref.get_recvline().substr(0 , position + 1);
        handeler_authen_and_commande(cmd_final, _index_client);
    }
    else
        return ;
}

void    serverr::initializer_server(int  port, std::string pass, size_t &i)
{
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);

    int opt = 1;// setsockopt : function in network programming is used to configure options on a socket.
    setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
    fcntl(sockfd, F_SETFL, O_NONBLOCK);

    struct sockaddr_in serverAddr;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(port);
    serverAddr.sin_addr.s_addr = INADDR_ANY;

    bind(sockfd, (struct sockaddr*)&serverAddr, sizeof(serverAddr)); // bound [ip and port] to server socket
    listen(sockfd, 10);
    std::cout << "\033[32m++++++++++++++++++++++++++++++++++\033[0m" << std::endl <<"\033[32m+\033[0m";
    std::cout << "\033[31m server listen in :             " <<"\033[32m+" << std::endl;
    std::cout << "\033[32m+ Port\033[0m     = " << port << "\033[32m                +\033[0m"<< std::endl;
    std::cout << "\033[32m+ Password\033[0m = " << pass << "\033[32m                 +\033[0m"<<std::endl;
    std::cout << "\033[32m++++++++++++++++++++++++++++++++++\033[0m" << std::endl;

    set_fd_sock_serv(sockfd);
    struct pollfd mon_pollfd;

    mon_pollfd.fd = get_fd_sock_serv();
    mon_pollfd.events = POLLIN;

    vec_pollfd.push_back(mon_pollfd);
    while (1)
    {
        int res = poll(vec_pollfd.data(), vec_pollfd.size(), -1);
        if (res == -1)
        {
            std::cout << "error poll\n";
            return ;
        }
        for (i = 0; vec_pollfd.size() > i; i++)
        {
            if (POLLIN & vec_pollfd[i].revents)
            {
                if (vec_pollfd[i].fd == get_fd_sock_serv())
                {
                    // is a server here : is a handle new connction for client 
                    struct sockaddr_in client_addr;
                    socklen_t len = sizeof(client_addr);
                    int client_fd = accept(get_fd_sock_serv(), (struct sockaddr*)&client_addr, &len);
                    setNonBlocking(client_fd);

                    struct pollfd mon_pollfd2;
                        mon_pollfd2.fd = client_fd;
                        mon_pollfd2.events = POLLIN | POLLOUT;
                        mon_pollfd2.revents = 0;

                    vec_pollfd.push_back(mon_pollfd2);

                    std::string ip_address_client = inet_ntoa(client_addr.sin_addr);
                    cliente obj_client(client_fd, ip_address_client);
                    vec_client.push_back(obj_client);
                    std::cout << "New connection accepted: " << client_fd << std::endl;
                }
                else
                {
                    // is a client here : is a handle new msg
                    int socket_client = vec_pollfd[i].fd;
                    std::string cmd = receive_cmd(socket_client, i);
                    // std::cout << "Message from client " << socket_client << ": " << cmd << std::endl;
                    cliente &client_ref = get_client_orgien(socket_client);
                    client_ref.set_recv_data(cmd);
                    authenticate_client(cmd, socket_client, client_ref, i);  
                }
            }
        }
    }
}

std::string serverr::receive_cmd(int fd_client, size_t &_index_client)
{
    char buffer[1024];
    memset(buffer, 0, 1024);
    ssize_t bytes_received = recv(fd_client, buffer, sizeof(buffer) - 1, 0);
    if (bytes_received <= 0)
    {
        if (bytes_received == 0)
        {
            std::cout << "Client disconnected: " << fd_client << std::endl;
            vec_pollfd.erase(vec_pollfd.begin() + _index_client);
            _index_client--;
        }
        else
            perror("recv");
        close(fd_client); // close the socket if not present program infinite loop infoi click sur c
        // removeClient(fd_client);
        return "";
    }
    buffer[bytes_received] = '\0'; // add null terminator if not present => display garbej value
    std::string message(buffer);
    if (message == "\n")
        return "";
    return message;
}

void    serverr::display()
{
    for (std::vector<struct pollfd>::iterator it = vec_pollfd.begin(); it != vec_pollfd.end(); it++)
    {
        std::cout << "fd the struct  = " << it->fd << std::endl;
        std::cout << "event the struct  = " << it->events << std::endl;
        std::cout << "revent the struct  = " << it->revents << std::endl;
    }
}

cliente& serverr::get_client_by_index(size_t index)
{
    return (vec_client[index]);
}


void    serverr::SendToAll(channels channel_, std::string _message)
{
    std::map<std::pair<bool, int>, cliente> mapOfClients = channel_.get_map_user();
    std::map<std::pair<bool, int>, cliente>::iterator iter;
    for(iter = mapOfClients.begin(); iter != mapOfClients.end(); iter++)
        send_msg_to_clinet(iter->second.get_client_fd(), _message);
        // send_msg_to_clinet(iter->first.second, _message);
}

std::string serverr::get_hostip()
{
    return (host_ip);
}

// std::string  serverr::_time() 
// {
//     time_t now = time(0);
//     char buffer[20];
//     std::strftime(buffer, sizeof(buffer), "%s", std::localtime(&now));
//     return std::string(buffer);
// }

std::string serverr::_time() 
{
    time_t now = time(0);
    char buffer[30];
    std::strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", std::localtime(&now));
    return std::string(buffer) + " (" + std::to_string(now) + ")";
}
