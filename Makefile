.PHONY: all clean

all: test1 test2 out1.json out2.json

test1_input.h.pp: test1_input.h
	gcc -E test1_input.h > test1_input.h.pp

test2_input.h.pp: test2_input.h
	gcc -E test2_input.h > test2_input.h.pp

out1.c: test1_input.h.pp cparse.py
	./cparse.py test1_input.h.pp > out1.c 2> err1.txt

out2.c: test2_input.h.pp cparse.py
	./cparse.py test2_input.h.pp > out2.c 2> err2.txt

test1: test1.c test1_input.h util.c out1.c
	gcc -o test1 util.c test1.c -fsanitize=undefined -Wall -Wextra

test2: test2.c test2_input.h util.c out2.c
	gcc -o test2 util.c test2.c -fsanitize=undefined -Wall -Wextra

out1.json:
	./test1 > out1.json

out2.json:
	./test2 > out2.json

clean:
	rm -f *.o *.h.pp test1 out1.c test2 out2.c out1.json out2.json

