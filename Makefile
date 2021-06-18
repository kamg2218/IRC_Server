SRC=main.cpp Message.cpp Frame.cpp Session.cpp Service.cpp \
	Server.cpp

OBJ=$(SRC.c=.o)
NAME=a.out

all : $(NAME)

$(NAME) :
	clang++ $(SRC) -o $(NAME)

clean :
	rm -rf $(OBJ)

fclean: clean
	rm -rf $(NAME)
	
re : fclean all
