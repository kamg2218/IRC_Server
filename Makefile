SRC=main.cpp Message.cpp Frame.cpp
NAME=a.out

all : $(NAME)

$(NAME) :
	clang++ $(SRC) -o $(NAME)

clean :
	rm -rf main.o Message.o Frame.o

fclean: clean
	rm -rf $(NAME)
	
re : fclean all
