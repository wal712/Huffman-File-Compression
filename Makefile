CFLAGS = -Wall -g -Wextra -pedantic

.PHONY: press test

press:
	gcc $(CFLAGS) Priorityqueue.c compression.c -o ./bin/press




