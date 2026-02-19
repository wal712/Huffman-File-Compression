#ifndef BYTEIO_H
#define BYTEIO_H

#define BYTE_LENGTH 8

typedef struct {
    int length;
    unsigned char buffer;
} Byte_Buffer;

void write_bytes(Byte_Buffer *buffer, unsigned long bytes, int length);
#endif