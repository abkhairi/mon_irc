NAME = ircserv

CC     = c++

CFLAGS = -Wall -Wextra -Werror -std=c++98

SRC    = main.cpp utilis.cpp client.cpp server.cpp authentication.cpp ft_handle_cmd.cpp channels.cpp
OBJ    = $(SRC:.cpp=.o)

 
%.o : %.cpp client.hpp server.hpp channels.hpp
	$(CC) $(FLAGS) -c $< -o $@

all : $(NAME) $(OBJ)

$(NAME) : $(OBJ) channels.hpp client.hpp reply_msg.hpp server.hpp
	$(CC) $(CFLAGS) $(OBJ) -o $(NAME)

clean :
	rm -rf $(OBJ)

fclean: clean
	rm -rf $(NAME)

re : fclean all

.PHONY: clean