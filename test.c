#include <stdio.h>
#include <assert.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>

#include "Hashmap.h"

int main()
{
    int fd = open("test.txt", O_RDONLY);

    if (fd == -1)
    {
        fprintf(stderr, "Error opening file\n");
        return 1;
    }

    char c;
//    for (int i = 0; i < 1000; i++)
 //   {
   //     read(fd, &c, sizeof(c));
     //   printf("%c\n", c);
    //}
    return 0;
}
