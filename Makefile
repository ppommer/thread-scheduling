all: threadsched

threadsched: threadsched.c list.c list.h
	gcc -g -Wall threadsched.c list.c -o $@

