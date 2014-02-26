#include "KeyStream.h"

#define BLOCK_SIZE 4096

void KeyStream::genRand(std::size_t size)
{
	m_stream.clear();
	m_stream.resize(size * 8);

	unsigned char block[BLOCK_SIZE];
	do
	{
		for(std::size_t i=0; i<BLOCK_SIZE; i++)
			block[i] = rand();

		m_stream.append(&block[0],
			&block[0] + ((BLOCK_SIZE < size) ? BLOCK_SIZE : size));

		size -= (BLOCK_SIZE < size) ? BLOCK_SIZE : size;
	} while(size > 0);
}


void KeyStream::change(const DecrKeyParams &params)
{
	for(std::size_t i=0, size=params.m_changes.size(); i<size; i++)
		m_stream[params.m_changes[i].m_pos] = params.m_changes[i].m_val;
}

