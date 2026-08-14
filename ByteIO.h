#ifndef BYTEIO_H
#define BYTEIO_H

#include <stdio.h>

#define BYTE_LENGTH 8

// Your job to clean up
typedef struct
{
    int length;
    unsigned char buffer; // does this need to be an int to handle eof from reading bytes??
} Byte_Buffer;

void write_bytes(Byte_Buffer *buffer, FILE *output, unsigned long bytes, int length);
void flush_write(Byte_Buffer *buffer, FILE *output);
int read_bytes(Byte_Buffer *buffer, FILE *input);
#endif
