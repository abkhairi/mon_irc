#include <iostream>
#include <string>
#include <sys/socket.h> // Needed for socket creating and binding
#include <netinet/in.h> // Needed to use struct sockaddr_in
#include <fcntl.h> // for fcntl() function for non-blocking socket
#include <vector> // fpr vector
#include <poll.h> // for strcut pollfd
#include <arpa/inet.h>  // For inet_ntoa()
#include <unistd.h> // for close() function
#include <cctype> // for isdigit() function and tolower() function
#include <sstream> // for std::stringstream
#include <cstring> // for std::strstr

class cliente
{
    private :
        int client_sock_fd;
        std::string ip_addr_client;
        std::string recvdata;
        // std::string nickname;
    public :
        cliente(int _client_sock_fd, std::string _ip_addr_client);
        void display_client();
        void set_recv_data(std::string b);
        int get_client_fd();
        std::string get_recvline();

};

std::string cliente::get_recvline()
{
    return recvdata;
}

class serverr
{
    private:
        int         _fd_sock_serv;
        int         _port;
        std::string _pass;
        std::vector <cliente> vec_client;
        serverr();
    public:
        std::vector<struct pollfd> vec_pollfd;
        serverr(int port, std::string pass);
        ~serverr();
        void    initializer_server(int port, std::string pass);
        int     get_fd_sock_serv() { return _fd_sock_serv; }
        void    set_fd_sock_serv(int fd) { _fd_sock_serv = fd; }
        cliente& get_client_orgien(int socket_client);
        void    display();
};

void cliente::set_recv_data(std::string b)
{
    recvdata = b;
}
void cliente::display_client()
{
        std::cout <<"fd the client " << this->client_sock_fd << std::endl;
        std::cout <<"ip the client " << this->ip_addr_client << std::endl;
}

cliente::cliente(int _client_sock_fd, std::string _ip_addr_client)
{
    client_sock_fd = _client_sock_fd;
    ip_addr_client = _ip_addr_client;
}

void setNonBlocking(int fd) 
{
    int flags = fcntl(fd, F_GETFL, 0);
    if (flags == -1) {
        perror("fcntl");
        exit(EXIT_FAILURE);
    }
    if (fcntl(fd, F_SETFL, flags | O_NONBLOCK) == -1) {
        perror("fcntl");
        exit(EXIT_FAILURE);
    }
}

serverr::~serverr()
{}

serverr::serverr(int port, std::string pass)
{
    _port = port;
    _pass = pass;
}

int parsing_port_and_pass(std::string port, std::string pass)
{
    if (port.empty())
        return 1;
    for (int i = 0; i < port.size(); i++)
    {
        if (isdigit(port[i]) == 0)
            return 1;
    }
    int port_int = atoi(port.c_str());
    if (port_int < 0 || port_int > 65535)
        return 1;
    if (port_int < 1024)
        return 1;
    if (pass == "" || pass.empty())
        return 2;
    return port_int;
}

int cliente::get_client_fd()
{
    return (client_sock_fd);
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

std::string receive_cmd(int fd_client)
{
    char buffer[1024];
    memset(buffer, 0, 1024);
    ssize_t bytes_received = recv(fd_client, buffer, sizeof(buffer) - 1, 0);
    if (bytes_received <= 0)
    {
        if (bytes_received == 0)
            std::cout << "Client disconnected: " << fd_client << std::endl;
        else
            perror("recv");
        close(fd_client); // close the socket if not present program infinite loop infoi click sur c
        // removeClient(fd_client);
        return "";
    }
    buffer[bytes_received] = '\0'; // add null terminator if not present => display garbej value
    std::string message(buffer);
    return message;
}

void    authenticate_client(std::string cmd,int socket_client, cliente &clienteref)
{
    for (size_t i = 0; i < cmd.size(); i++)
        cmd[i] = std::tolower(cmd[i]);
    std::stringstream ss(cmd);
    int i = 0;
    std::vector<std::string> vec_of_cmd_authen;
    while (ss >> cmd)
    {
        vec_of_cmd_authen.push_back(cmd);
        i++;
    }
    if (i == 2)
    {
        std::cout << "here\n";
        if (strstr(clienteref.get_recvline().c_str(), "\n"))
        {
            size_t position = clienteref.get_recvline().find_first_of("\n");
            std::cout << "position = " << position << std::endl;
            std::string cmd_final = clienteref.get_recvline().substr(0 , position + 1);
            std::cout << "commande final => " << cmd_final << std::endl;
        }
        // if (vec_of_cmd_authen[0] == "pass")
        // {
        //     std::cout << "is a pass here" << std::endl;
        // }
        // else if (vec_of_cmd_authen[0] == "nick")
        // {
        //     std::cout << "is a nick here" << std::endl;
        // }
        // else if (vec_of_cmd_authen[0] == "user")
        // {
        //     std::cout << "is a user here" << std::endl;
        // }
        // else
        //     return ;
    }
    else
        return ;
}

// for(std::vector<std::string>::iterator it = vec_of_cmd_authen.begin(); it != vec_of_cmd_authen.end(); it++)
// {
//     std::cout << "hado homa cmd = " << *it << std::endl;
// }


void    serverr::initializer_server(int  port, std::string pass)
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
        for (size_t i = 0; vec_pollfd.size() > i; i++)
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
                    std::string cmd = receive_cmd(socket_client);
                    std::cout << "Message from client " << socket_client << ": " << cmd << std::endl;
                    cliente &client_ref = get_client_orgien(socket_client);
                    client_ref.set_recv_data(cmd);
                    authenticate_client(cmd, socket_client, client_ref);
                    
                    // handler_commande(cmd);
                }
            }
        }
    }
}


int main(int ac, char** av)
{
    if (ac != 3)
    {
        std::cout << "invalid arugments" << std::endl;
        return  1;
    }
    std::string port = av[1];
    std::string pass = av[2];

    int port_int = parsing_port_and_pass(port, pass);
    if (port_int == 1)
    {
        std::cout << "Error or invalid port" << std::endl;
        return 1;
    }
    if (port_int == 2)
    {
        std::cout << "Error password" << std::endl;
        return 1;
    }
    serverr mon_server(port_int, pass);
    mon_server.initializer_server(port_int, pass);
    // mon_server.display();
    return 0;
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