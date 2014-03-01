#include "KeyStream.h"

#include <limits>

#include "Markerator.h"


KeyStream::KeyStream(const DecrKeyParams &params, std::size_t size):
	m_changes(params.m_changes)
	,m_block(0)
	,m_size(size)
	,m_currPos(0)
{
}

KeyStream::KeyStream(const DecrKeyParams &params, const Markerator &mark, std::size_t size):
	m_changes(params.m_changes)
	,m_pmark(new Markerator(mark))
	,m_block(0)
	,m_size(size)
	,m_currPos(0)
{
}


bool KeyStream::getNextRand()
{
	if(m_currPos % std::numeric_limits<unsigned int>::digits == 0)
		m_block = rand();

	unsigned int mask = 1 << (m_currPos % std::numeric_limits<unsigned int>::digits);
	return m_block & mask;
}


bool KeyStream::getNextMark()
{
	return m_pmark -> getNext();
}

bool KeyStream::getNext()
{
	bool res;
	std::size_t i=0;
	for(std::size_t size=m_changes.size(); i<size; i++)
	{
		if(m_changes[i].m_pos == m_currPos)
		{
			res = m_changes[i].m_val;
			break;
		}
	}

	if(m_pmark)
	{
		bool markRes = getNextMark();
		if(i == m_changes.size())
			res = markRes;
	}
	else
	{
		if(i == m_changes.size())
			res = getNextRand();
	}

	m_currPos++;	

	return res;
}


bool KeyStream::hasNext() const
{
	return m_currPos < m_size;
}