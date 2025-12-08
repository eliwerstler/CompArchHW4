CC=gcc
CFLAGS=-Wall -Wextra -O3 -march=native -mtune=native

all: sorting

sorting: sorting.c
	$(CC) $(CFLAGS) sorting.c -o sorting

test_sorting: sorting.c tests/test_sorting.c
	$(CC) $(CFLAGS) -DTESTING sorting.c tests/test_sorting.c -o test_sorting

test: test_sorting
	./test_sorting

run: sorting
	./sorting

clean:
	rm -f sorting test_sorting
