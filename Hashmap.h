#ifndef HASHMAP_H
#define HASHMAP_H

typedef struct {
    unsigned int value;
    char key;
} entry;

typedef struct {
	unsigned int capacity;
	unsigned int length;
	entry *arr;
} Hashmap;

void map_init(Hashmap *map);
void add_entry(Hashmap *map, char key, unsigned int value);
entry* get_entry(Hashmap *map, char key);

#endif
