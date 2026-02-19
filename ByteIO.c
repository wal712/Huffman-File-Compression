#include <assert.h>

#include "ByteIO.h"

void write_bytes(Byte_Buffer *buffer, unsigned long bytes, int length)
{
    assert(buffer);
    assert(buffer->length < BYTE_LENGTH);

    for (int i = length - 1; i >= 0; i--)
    {
        assert(buffer->length < BYTE_LENGTH);
        buffer->buffer = (buffer->buffer << 1) + ((bytes >> i) & 1); // grabs leftmost bit, and appends to buffer, on loop
        buffer->length++;

        if (buffer->length == BYTE_LENGTH)
        {
            // write to file here
            // TODO: decide if im keeping file descriptor in byte_buffer itself or pass to each function
        }
    }
}