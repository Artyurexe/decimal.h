CC = gcc
CFLAGS = -Wall -Werror -Wextra -std=c11

all: s21_decimal.a

s21_decimal.a:
	$(CC) -c *.c
	ar rcs s21_decimal.a *.o
	ranlib s21_decimal.a
	rm *.o

test: rebuild
	$(CC) $(CFLAGS) -lcheck -lm -lpthread tests/*.c s21_decimal.a -o test
	./test
	rm test

gcov_report: s21_decimal.a
	$(CC) $(CFLAGS) -fprofile-arcs -ftest-coverage tests/*.c *.c -pthread -lcheck -pthread -lm -o test
	./test
	lcov -t "test_html" -o unit_test.info -c -d . --rc lcov_branch_coverage=1
	genhtml -o test_html unit_test.info
	open test_html/index.html

clean:
	rm -rf s21_decimal.a *.gc* test unit_test.info test_html/

rebuild: 
	make clean
	make all