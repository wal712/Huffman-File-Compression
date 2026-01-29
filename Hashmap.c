#include <stdio.h>
#include "Hashmap.h"

void map_init(Hashmap *map)
{
    map->capacity = START_CAPACITY;
}
