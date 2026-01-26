CFLAGS = -Wall -Wextra -pedantic

.PHONY: press

press:
	gcc $(CFLAGS) compression.c -o press

