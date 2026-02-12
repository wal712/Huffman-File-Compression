CFLAGS = -Wall -Wextra -pedantic

.PHONY: press test

press:
	gcc $(CFLAGS) Priorityqueue.c compression.c -o ./bin/press

test: test_compile
	./bin/test

test_compile:
	gcc $(CFLAGS) test.c Hashmap.c -o ./bin/test


