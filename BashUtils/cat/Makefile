CFLAGS = -Wall -Werror -Wextra -std=c11
OBJ = s21_cat
TEST= tests.sh

UTIL = -fsanitize=address
############################## general ##################################

all: style $(OBJ)

clean:
	-rm $(OBJ)

style:
	-cp ../../materials/linters/.clang-format .
	-clang-format -i ./*.c ./*.h
	-clang-format -n ./*.c ./*.h

leaks:
	cppcheck --enable=all --suppress=missingIncludeSystem .

good: clean all test
	sleep 3
	clear

############################## variable ##################################

s21_grep:
	gcc $(KEYS) ./$(OBJ).c -o ./$(OBJ)

test:
	bash $(TEST)

