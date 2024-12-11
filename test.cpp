#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <poll.h>
#include <netinet/in.h>
#include <unistd.h>
#include <fcntl.h>
#include <cstring>
#include <arpa/inet.h>

#define BUFFER_SIZE 512
#define MAX_CLIENTS 100

// Helper function to set a file descriptor to non-blocking mode
void setNonBlocking(int fd) {
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

class IRCServer {
private:
    int server_fd;
    int port;
    std::string password;
    std::vector<pollfd> poll_fds;
    std::map<int, std::string> clients; // Map file descriptors to nicknames

    void handleNewConnection() 
    {
        sockaddr_in client_addr;
        socklen_t client_len = sizeof(client_addr);
        int client_fd = accept(server_fd, (struct sockaddr*)&client_addr, &client_len);

        setNonBlocking(client_fd);

        pollfd pfd = {client_fd, POLLIN, 0};
        
        poll_fds.push_back(pfd);
        clients[client_fd] = ""; // Placeholder for the nickname

        std::cout << "New connection accepted: " << client_fd << std::endl;
    }

    void handleClientMessage(int client_fd) 
    {
        char buffer[BUFFER_SIZE];
        int bytes_received = recv(client_fd, buffer, sizeof(buffer) - 1, 0);

        if (bytes_received <= 0)
        {
            if (bytes_received == 0) 
                std::cout << "Client disconnected: " << client_fd << std::endl;
            else
                perror("recv");
            close(client_fd);
            removeClient(client_fd);
            return;
        }

        buffer[bytes_received] = '\0';
        std::string message(buffer);
        std::cout << "Message from client " << client_fd << ": " << message;

        // Handle authentication, nicknames, and commands here
        if (clients[client_fd].empty()) 
        {
            if (message.substr(0, 9) == "PASS ") 
            {
                if (message.substr(5) == password + "\r\n") 
                    send(client_fd, "+OK\n", 4, 0);
                else 
                {
                    send(client_fd, "-ERR Incorrect password\n", 24, 0);
                    close(client_fd);
                    removeClient(client_fd);
                }
            } 
            else
                send(client_fd, "-ERR You must authenticate first\n", 32, 0);
        }

        // Further command parsing goes here
    }

    void removeClient(int client_fd) 
    {
        for (std::vector<pollfd>::iterator it = poll_fds.begin(); it != poll_fds.end(); ++it) 
        {
            if (it->fd == client_fd) 
            {
                poll_fds.erase(it);
                break;
            }
        }
        clients.erase(client_fd);
    }

public:
    IRCServer(int port, const std::string& password) : port(port), password(password) {
        server_fd = socket(AF_INET, SOCK_STREAM, 0);
        if (server_fd == -1) {
            perror("socket");
            exit(EXIT_FAILURE);
        }

        setNonBlocking(server_fd);

        sockaddr_in server_addr = {};
        server_addr.sin_family = AF_INET;
        server_addr.sin_addr.s_addr = INADDR_ANY;
        server_addr.sin_port = htons(port);

        if (bind(server_fd, (struct sockaddr*)&server_addr, sizeof(server_addr)) == -1) {
            perror("bind");
            exit(EXIT_FAILURE);
        }

        if (listen(server_fd, SOMAXCONN) == -1) {
            perror("listen");
            exit(EXIT_FAILURE);
        }

        pollfd pfd = {server_fd, POLLIN, 0};
        poll_fds.push_back(pfd);

        std::cout << "Server started on port " << port << std::endl;
    }

    void run() {
        while (true) 
        {
            int poll_count = poll(poll_fds.data(), poll_fds.size(), -1);
            if (poll_count == -1) 
            {
                perror("poll");
                exit(EXIT_FAILURE);
            }

            for (size_t i = 0; i < poll_fds.size(); ++i) 
            {
                if (poll_fds[i].revents & POLLIN) 
                {
                    if (poll_fds[i].fd == server_fd)
                        handleNewConnection();
                    else
                        handleClientMessage(poll_fds[i].fd);
                }
            }
        }
    }

    ~IRCServer() {
        close(server_fd);
    }
};

int main(int argc, char* argv[]) {
    if (argc != 3) {
        std::cerr << "Usage: " << argv[0] << " <port> <password>\n";
        return EXIT_FAILURE;
    }

    int port = std::atoi(argv[1]);
    std::string password = argv[2];

    IRCServer server(port, password);
    server.run();

    return 0;
}
