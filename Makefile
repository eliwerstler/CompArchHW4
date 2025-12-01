CC=gcc
CFLAGS=-Wall -Wextra -O2

all: sorting

sorting: sorting.c
	$(CC) $(CFLAGS) sorting.c -o sorting

run: sorting
	./sorting

clean:
	rm -f sorting
