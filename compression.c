#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>

int main (int argc, char **argv) {
	if (argc < 2) 
	{
		printf("Usage: %s <filenames\n", argv[0]);
		return 1;
	}

	int fd = open(argv[1], O_RDONLY);
	if (fd == -1)
	{
		fprintf(stderr, "Coudldn't open file: %s\n", argv[1]);
		return 1;
	}
	
	close(fd);
	return 0;
}
