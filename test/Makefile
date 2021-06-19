NAME = server

SRC = main.cpp\
	  Frame.cpp
	  Socket.cpp\
	  Server.cpp\
	  Service.cpp

DFLAGS = -Wall -Wextra -Werror

all : $(NAME)

$(NAME) :
	clang++ $(DFLAGS) -o $(NAME) $(SRC)

clean :
	/bin/rm -f *.o

fclean : clean
	/bin/rm -f $(NAME)

re : fclean all
