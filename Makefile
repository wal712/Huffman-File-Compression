CFLAGS = -Wall -g -Wextra -pedantic -std=gnu2x

.PHONY: press test

press:
	gcc $(CFLAGS) Priorityqueue.c compression.c -o ./bin/press




