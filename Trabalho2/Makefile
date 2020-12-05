test:
	gcc -g -lcmocka -Wall -Wextra $(wildcard tests/*.c) $(filter-out src/main.c, $(wildcard src/*.c)) -o test.out

run:
	gcc -g -Wall -Wextra $(wildcard src/*.c) -o main.out