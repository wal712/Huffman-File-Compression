#include <stdio.h>
#include <assert.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>

#include "Hashmap.h"
void test_map_init(Hashmap *map)
{
    map_init(map);
    assert(map->capacity == START_CAPACITY);
    assert(map->length == 0);
    
}
int main()
{
    int fd = open("test.txt", O_RDONLY);

    if (fd == -1)
    {
        fprintf(stderr, "Error opening file\n");
        return 1;
    }

    char c;
    Hashmap *map;
    test_map_init(map);
    return 0;
}
