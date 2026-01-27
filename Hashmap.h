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

#endif
