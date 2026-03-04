#ifndef BYTEIO_H
#define BYTEIO_H

#include <stdio.h>

#define BYTE_LENGTH 8

// Your job to clean up 
typedef struct {
    int length;
    unsigned char buffer;
} Byte_Buffer;

void write_bytes(Byte_Buffer *buffer, FILE *output, unsigned long bytes, int length);
void flush_write(Byte_Buffer *buffer, FILE *output);
#endif
