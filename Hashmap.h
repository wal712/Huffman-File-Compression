#ifndef HASHMAP_H
#define HASHMAP_H

#define START_CAPACITY 8

typedef struct {
    unsigned int value;
    char key;
} entry;

typedef struct {
	size_t capacity;
	size_t length;
	entry *arr;
} Hashmap;

void map_init(Hashmap *map);
void add_entry(Hashmap *map, char key, unsigned int value);
entry* get_entry(Hashmap *map, char key);

#endif
