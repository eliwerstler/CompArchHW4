CC ?= gcc
CFLAGS ?= -Wall -Wextra -O0
RADIX_FLAGS ?= -DRADIX16

all: sorter

sorter: sorting.c
	$(CC) $(CFLAGS) $(RADIX_FLAGS) sorting.c -o sorter

test_sorting: sorting.c tests/test_sorting.c
	$(CC) $(CFLAGS) $(RADIX_FLAGS) -DTESTING sorting.c tests/test_sorting.c -o test_sorting

test: test_sorting
	./test_sorting

run: sorter
	./sorter

clean:
	rm -f sorter test_sorting
