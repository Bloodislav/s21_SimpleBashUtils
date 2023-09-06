CC = gcc
CFLAGS = -Wall -Werror -Wextra -std=c11 -pedantic -c
LFLAGS =
DEBUG = -g

.PHONY = all build clean

all: s21_cat

s21_cat: s21_cat.o
	$(CC) $(LFLAGS) s21_cat.o -o s21_cat
	rm *.o

s21_cat.o: s21_cat.c
	$(CC) $(CFLAGS) $(DEBUG) s21_cat.c -o s21_cat.o

test: 
	bash test_files/test_cat.sh 

test_leks: 
	bash test_files/test_leaks.sh 

cpp:
	cppcheck s21_cat.*

clean:
	@rm -f *.o s21_cat