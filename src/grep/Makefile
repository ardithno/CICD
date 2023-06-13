CCF = gcc -Wall -Wextra -Werror -std=c11 #-pedantic -fsanitize=address

all: s21_grep test

test:
	sh t_grep.sh

clang-format:
	cp ../../materials/linters/.clang-format .
	clang-format -n *.c *.h 2>fail.txt
	rm .clang-format


cppcheck:
	cppcheck --enable=all *.c *.h

s21_grep:
	$(CCF) s21_grep.c -o s21_grep

leaks:
	CK_FORK=no leaks --atExit -- ./s21_grep
	
clean:
	rm -rf log.txt s21_grep *.cfg grep.txt s21_grep.txt

rebuild: clean all
