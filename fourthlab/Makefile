it:
	gcc main.c -o main

valgrind_it:
	valgrind --tool=memcheck --leak-check=yes --leak-check=full --show-leak-kinds=all --track-origins=yes -s ./main

my:
	gcc my.c -Werror -Wextra -Wall -std=c11 -o my

valgrind_my:
	valgrind --tool=memcheck --leak-check=yes --leak-check=full --show-leak-kinds=all --track-origins=yes -s ./my