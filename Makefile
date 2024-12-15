NAME = ircserv

CC     = c++

CFLAGS = -Wall -Wextra -Werror -std=c++98

SRC    = main.cpp utilis.cpp client.cpp server.cpp authentication.cpp ft_handle_cmd.cpp
OBJ    = $(SRC:.cpp=.o)

 
%.o : %.cpp client.hpp server.hpp
	$(CC) $(FLAGS) -c $< -o $@

all : $(NAME) $(OBJ)

$(NAME) : $(OBJ)
	$(CC) $(CFLAGS) $(OBJ) -o $(NAME)

clean :
	rm -rf $(OBJ)

fclean: clean
	rm -rf $(NAME)

re : fclean all

.PHONY: clean