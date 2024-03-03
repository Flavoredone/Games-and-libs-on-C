GCC =   gcc 
FLAGS = -Werror -Wall -Wextra -std=c11 -lncurses
FILE =  game_of_life.c

all: clean style run

run:
	$(GCC) $(FLAGS) $(FILE) -o life

v1: run
	./life < assets/1.txt

v2: run
	./life < assets/2.txt

v3: run
	./life < assets/3.txt

v4: run
	./life < assets/4.txt

style:
	clang-format -style='{BasedOnStyle: Google, IndentWidth: 4, AlignConsecutiveMacros: AcrossEmptyLines}' -i *.c *.h
	clear

clean:
	-rm -rf life
	-$(cleanterm)

cleanterm: 
	-clear
