CC = gcc
CFLAGS = -Wall -Werror -Wextra -std=c11 -pedantic -c
LFLAGS =
DEBUG = -g
DIRCONF = ../config/

.PHONY = all build clean

all: s21_grep

s21_grep: s21_grep.o
	$(CC) $(LFLAGS) s21_grep.o -o s21_grep
	rm *.o

s21_grep.o: s21_grep.c
	$(CC) $(CFLAGS) $(DEBUG) s21_grep.c -o s21_grep.o

test:
	bash tests/tests_grep.sh

test_leaks: 
	bash tests/test_leaks.sh 

cpp:
	cppcheck s21_grep.c
	cppcheck s21_grep.h

clean:
	@rm -f *.o s21_grep
