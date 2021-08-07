SRC=main.cpp Executor.cpp Frame.cpp Session.cpp Service.cpp \
	MainServer.cpp User.cpp Channel.cpp Socket.cpp

OBJ=$(SRC:.cpp=.o)
NAME=ircserv
TEMP=$(NAME:=.dSYM)
CFLAGS = -Wall -Werror -Wextra
DFLAGS = -fsanitize=address

all : $(NAME)

$(NAME) :
	clang++ $(DFLAGS) $(SRC) -o $(NAME) -g3

clean :
	rm -rf $(OBJ)

fclean: clean
	rm -rf $(NAME)
	rm -rf $(TEMP)
	
re : fclean all
