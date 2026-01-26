CFLAGS = -Wall -Wextra -pedantic

.PHONY: press test

press:
	gcc $(CFLAGS) compression.c -o press

test: test_compile
	./test

test_compile:
	gcc $(CFLAGS) test.c Hashmap.c -o test && ./test


