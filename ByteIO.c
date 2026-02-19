#include <assert.h>

#include "ByteIO.h"

void write_bytes(Byte_Buffer *buffer, unsigned long bytes, int length)
{
    assert(buffer);
    assert(buffer->length < BYTE_LENGTH);

    for (int i = length - 1; i >= 0; i--)
    {
        buffer->buffer = (buffer->buffer << 1) + (bytes & 1);
        buffer->length++;
        bytes = bytes << 1;
        
    }
}