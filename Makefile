NAME = server

SRC = main.cpp\
	  Frame.cpp\
	  Socket.cpp\
	  Server.cpp\
	  Service.cpp

DFLAGS = -Wall -Wextra -Werror

all : $(NAME)

$(NAME) :
	clang++ -o $(NAME) $(SRC)

fclean :
	/bin/rm -f $(NAME)

re : fclean all
