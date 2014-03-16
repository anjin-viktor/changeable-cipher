#include "HashTable.h"

#include <set>

HashTable::HashTable(std::size_t numBits):
	m_numIdxBits(numBits)
	,m_table(exp2(numBits * 2))
{
}


std::size_t HashTable::getIndex(const Conjunct &conj)
{
	std::size_t result = 0;
	std::size_t mask = 1;

	for(std::size_t i=0; i<m_numIdxBits && i<conj.m_pos.size(); i++, mask <<= 1)
		if(conj.m_pos.test(i))
			result |= mask;

	return result;	
}


bool HashTable::merge(const Conjunct &merged, Conjunct &mergedResult)
{
	Conjunct testConj = merged;
	Conjunct prevConj;
	int idx = getIndex(testConj);
	ExistsType exType = NotExists;
	std::shared_ptr<Conjunct> pconj;
	std::shared_ptr<Conjunct> pPrevConj;

	for(std::size_t i=0; i<merged.m_pos.size(); i++)
	{
		if(testConj.m_pos.test(i) || testConj.m_neg.test(i))
		{
			testConj.m_pos.flip(i);
			testConj.m_neg.flip(i);
			if(i <= m_numIdxBits)
				idx = getIndex(testConj);

			ExistsType exTypeTmp = isExists(testConj, m_table[idx], pconj);

			if(exTypeTmp != NotExists)
			{
				exType = exTypeTmp;
				prevConj = testConj;
				pPrevConj = pconj;
	
				if(exTypeTmp == ExistsAsConj)
					break;
			}

			testConj.m_pos.flip(i);
			testConj.m_neg.flip(i);
		}
	}

	if(exType == NotExists)
	{
		insertConjunct(merged);
		mergedResult = merged;
		return true;
	}
	else if(exType == ExistsAsConj)
	{
		mergedResult = merged;
		removeConjunct(pPrevConj);
		mergedResult.m_pos &= prevConj.m_pos;
		mergedResult.m_neg &= prevConj.m_neg;

		return false;
	}
	else
	{
		mergedResult = merged;
		mergedResult.m_pos &= prevConj.m_pos;
		mergedResult.m_neg &= prevConj.m_neg;

		return false;
	}
}



HashTable::ExistsType HashTable::isExists(const Conjunct &conj, 
		const std::list<std::shared_ptr<Conjunct> > &lst, std::shared_ptr<Conjunct> &ptr)
{
	std::list<std::shared_ptr<Conjunct> >::const_iterator itr = lst.begin();
	ExistsType res = NotExists;
	for(;itr!=lst.end(); itr++)
	{
		if(((*itr) -> m_pos & conj.m_pos) == (*itr) -> m_pos &&
			((*itr) -> m_neg & conj.m_neg) == (*itr) -> m_neg)
		{
			if((*itr) -> m_pos == conj.m_pos && 
				(*itr) -> m_neg == conj.m_neg)
			{
				ptr = *itr;
				res = ExistsAsConj;
				break;
			}
			else 
			{
				ptr = *itr;
				res = ExistsAsPartOfConj;
			}
		}
	}

	return res;
}


DisForm HashTable::get()
{
	std::set<std::shared_ptr<Conjunct> > set;

	std::vector<std::list<std::shared_ptr<Conjunct> > >::const_iterator itr = m_table.begin();

	for(;itr != m_table.end(); itr++)
	{
		std::list<std::shared_ptr<Conjunct> >::const_iterator itrLst = itr -> begin();

		for(;itrLst != itr -> end(); itrLst++)
			set.insert(*itrLst);
	}

	DisForm res;
	std::set<std::shared_ptr<Conjunct> >::const_iterator itrSet = set.begin();
	for(; itrSet!=set.end(); itrSet++)
		res.m_conjuncts.push_back(**itrSet);

	return res;
}



void HashTable::insertConjunct(Conjunct conj)
{
	std::shared_ptr<Conjunct> newConj(new Conjunct(conj));
	std::vector<std::size_t> positions;
	positions.reserve(m_numIdxBits);

	for(std::size_t i=0; i<m_numIdxBits; i++)
		if(!conj.m_pos.test(i) && !conj.m_neg.test(i))
			positions.push_back(i);

	std::size_t numBits = positions.size();

	unsigned long value = 0;
	for(;value < (1 << numBits); value++)
	{
		boost::dynamic_bitset<> b(numBits, value);
		for(std::size_t i=0; i<numBits; i++)
		{
			if(b[i])
				conj.m_pos[positions[i]] = true;
			else
				conj.m_neg[positions[i]] = true;
		}
		
		int idx = getIndex(conj);
		m_table[idx].push_back(newConj);

		for(std::size_t i=0; i<numBits; i++)
		{
			conj.m_pos[positions[i]] = false;
			conj.m_neg[positions[i]] = false;
		}
	}

	if(positions.size() == 0)
	{
		int idx = getIndex(conj);
		m_table[idx].push_back(newConj);
	}

}



void HashTable::removeConjunct(const std::shared_ptr<Conjunct> &ptr)
{
	Conjunct conj = *ptr;
	std::vector<std::size_t> positions;
	positions.reserve(m_numIdxBits);

	for(std::size_t i=0; i<m_numIdxBits; i++)
		if(!conj.m_pos.test(i) && !conj.m_neg.test(i))
			positions.push_back(i);

	std::size_t numBits = positions.size();

	unsigned long value = 0;
	for(;value < (1 << numBits); value++)
	{
		boost::dynamic_bitset<> b(numBits, value);
		for(std::size_t i=0; i<numBits; i++)
		{
			if(b[i])
				conj.m_pos[positions[i]] = true;
			else
				conj.m_neg[positions[i]] = true;
		}

		int idx = getIndex(conj);
		int size = m_table[idx].size();
		m_table[idx].remove(ptr);

		for(std::size_t i=0; i<numBits; i++)
		{
			conj.m_pos[positions[i]] = false;
			conj.m_neg[positions[i]] = false;
		}
	}

	if(positions.size() == 0)
	{
		int idx = getIndex(conj);
		m_table[idx].remove(ptr);
	}
}

