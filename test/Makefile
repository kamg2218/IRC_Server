NAME = server

SRC = main.cpp\
	  Frame.cpp
	  Socket.cpp\
	  Server.cpp\
	  Service.cpp

OBJECTS = *.o
CFLAGS = -L../utils -lft -L../utils/libstack -lstack
DFLAGS = -Wall -Wextra -Werror

all : $(NAME)

$(NAME) :
	gcc $(DFLAGS) -o $(NAME) $(SRC)

clean :
	/bin/rm -f *.o

fclean : clean
	/bin/rm -f $(NAME)

re : fclean all
