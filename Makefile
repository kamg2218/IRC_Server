SRC=main.cpp Executor.cpp Frame.cpp Session.cpp Service.cpp \
	Server.cpp User.cpp Channel.cpp Socket.cpp 

OBJ=$(SRC.c=.o)
NAME=ircserv

all : $(NAME)

$(NAME) :
	clang++ $(SRC) -o $(NAME)

clean :
	rm -rf $(OBJ)

fclean: clean
	rm -rf $(NAME)
	
re : fclean all
