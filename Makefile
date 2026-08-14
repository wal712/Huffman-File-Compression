CFLAGS = -Wall -g -Wextra -pedantic 

.PHONY: press clean

press:
	clang $(CFLAGS) ByteIO.c Priorityqueue.c compression.c -o ./bin/press

clean:
	rm compressed*.txt
	rm uncompressed*.txt

