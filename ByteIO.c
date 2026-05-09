#include <assert.h>
#include <stdio.h>

#include "ByteIO.h"

void write_bytes(Byte_Buffer *buffer, FILE *output, unsigned long bytes, int length)
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
			assert(output);
			fputc(buffer->buffer, output);
			buffer->length = 0;
		}
	}
}

void flush_write(Byte_Buffer *buffer, FILE *output)
{
	assert(buffer);
	if (buffer->length > 0)
	{
		int shift_size = BYTE_LENGTH - buffer->length;
		assert(shift_size < BYTE_LENGTH);
		assert(shift_size >= 0);

		buffer->buffer = buffer->buffer << shift_size;
		assert(output);

		fputc(buffer->buffer, output);
		buffer->length = 0;
	}
	buffer->buffer = 0;
}
