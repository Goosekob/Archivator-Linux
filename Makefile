all: arcivator.o
	gcc *.o -o ./a.out
	
arcivator.o: arcivator.c 
	gcc -c -Wall -Werror -Wextra -fsanitize=address arcivator.c