.PHONY: all clean

all: test1

test1_input.h.pp: test1_input.h
	gcc -E test1_input.h > test1_input.h.pp

out1.c: test1_input.h.pp
	./cparse.py test1_input.h.pp > out1.c 2> err.txt

test1: test1.c test1_input.h util.c out1.c
	gcc -o test1 util.c test1.c -fsanitize=undefined -Wall -Wextra

clean:
	rm -f *.o test1 out1.c *.h.pp

