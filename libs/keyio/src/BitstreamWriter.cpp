#include "BitstreamWriter.h"

#include <climits>
#include <stdexcept>


BitstreamWriter::BitstreamWriter(unsigned char *ptr, std::size_t size):
	m_ptr(ptr)
	,m_size(size)
	,m_posBase(0)
	,m_posInBase(CHAR_BIT - 1)
{
}


void BitstreamWriter::toNextBase()
{
	m_posInBase = CHAR_BIT-1;
	m_posBase++;
}


void BitstreamWriter::writeBit(bool value)
{
	if(m_posBase >= m_size)
		throw std::runtime_error("BitstreamWriter: buffer is ended");

	if(value)
		m_ptr[m_posBase] |= 1 << m_posInBase;
	else
		m_ptr[m_posBase] &= ~(1 << m_posInBase);


	m_posInBase--;

	if(m_posInBase > CHAR_BIT)
	{
		m_posInBase = CHAR_BIT-1;
		m_posBase++;
	}
}


void BitstreamWriter::writeBit8(unsigned char v)
{
	unsigned char result = 0;
	for(std::size_t i=0; i<8; i++)
	{
		bool val = v & (1 << (7-i));
		writeBit(val);
	}
}

void BitstreamWriter::writeBit32(unsigned int v)
{
	unsigned char result = 0;
	for(std::size_t i=0; i<32; i++)
	{
		bool val = v & (1 << (31-i));
		writeBit(val);
	}
}

