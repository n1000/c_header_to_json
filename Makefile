.PHONY: all clean

CFLAGS = -Wall -Wextra -fsanitize=undefined
LDFLAGS = -fsanitize=undefined
COMPILE.c = $(CC) $(DEPFLAGS) $(CFLAGS) -c
LINK.c = $(CC) $(LDFLAGS)

TEST_BINS = test1 test2

all: $(TEST_BINS) check

%.i : %.h
	$(CC) -E $^ > $@

# Generate a _out.c file from a preprocessed header file
%_out.c: %_input.i
	./c_header_to_json.py $^ > $@ 2> $(patsubst %_out.c,%_err.txt,$@)

# Leave these explicit rules so that make does not delete the *.i files at the
# end of building.
test1_out.c: test1_input.i
test2_out.c: test2_input.i

# Main binaries all depend on the utilities. The implicit rule covers test*'s
# dependency on test*.c
$(TEST_BINS): util.o

test1.o: test1_out.c test1_input.h
test2.o: test2_out.c test2_input.h

# Utilities
util.o: util.c util.h

# Test output json files
out1.json: test1
	./test1 > $@

out2.json: test2
	./test2 > $@

# TODO: loop over each target (in $? variable)
check: out1.json out2.json
	python3 -m json.tool < out1.json > /dev/null 2>&1
	python3 -m json.tool < out2.json > /dev/null 2>&1
	touch check

clean:
	rm -f *.o *.i $(TEST_BINS) test1_out.c test2_out.c out1.json out2.json test1_err.txt test2_err.txt check
