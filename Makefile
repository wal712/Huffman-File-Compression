CFLAGS = -Wall -g -Wextra -pedantic -std=gnu2x

.PHONY: press clean

press:
	gcc $(CFLAGS) ByteIO.c Priorityqueue.c compression.c -o ./bin/press

clean:
	rm compressed*.txt

