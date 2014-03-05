#include "BitstreamReader.h"

#include <climits>
#include <stdexcept>


BitstreamReader::BitstreamReader(const unsigned char *ptr, std::size_t size):
	m_ptr(ptr)
	,m_size(size)
	,m_posBase(0)
	,m_posInBase(CHAR_BIT - 1)
{
}


void BitstreamReader::toNextBase()
{
	m_posInBase = CHAR_BIT-1;
	m_posBase++;
}


std::size_t BitstreamReader::available()
{
	return (m_size - m_posBase -1) * CHAR_BIT + m_posInBase + 1;
}

bool BitstreamReader::getBit()
{
	if(m_posBase >= m_size)
		throw std::runtime_error("BitstreamReader: not enought data");

	bool res = m_ptr[m_posBase] & 1 << m_posInBase;

	m_posInBase--;

	if(m_posInBase > CHAR_BIT)
	{
		m_posInBase = CHAR_BIT-1;
		m_posBase++;
	}

	return res;
}


unsigned char BitstreamReader::getBit8()
{
	unsigned char result = 0;
	for(std::size_t i=0; i<8; i++)
		if(getBit())
			result |= 1 << (7-i);

	return result;
}

unsigned int BitstreamReader::getBit32()
{
	unsigned int result = 0;

	for(std::size_t i=0; i<32; i++)
		if(getBit())
			result |= 1 << (31-i);

	return result;
}

