CCF = gcc -Wall -Wextra -Werror -std=c11 #-pedantic -fsanitize=address

all: s21_cat test

test:
	sh t_cat.sh 1>test.txt

clang-format:
	cp ../../materials/linters/.clang-format .
	clang-format -n *.c *.h 2>fail.txt
	rm .clang-format


cppcheck:
	cppcheck --enable=all *.c *.h

s21_cat:
	$(CCF) s21_cat.c -o s21_cat

leaks:
	CK_FORK=no leaks --atExit -- ./s21_grep
	
clean:
	rm -rf log.txt s21_cat *.cfg cat.txt s21_cat.txt

rebuild: clean all
