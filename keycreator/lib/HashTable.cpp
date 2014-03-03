#include "HashTable.h"

#include <set>
#include <iostream>


HashTable::HashTable(std::size_t numBits):
	m_numIdxBits(numBits)
{

}


int HashTable::getIndex(const Conjunct &conj)
{
	int pos = 0;
	int neg = 0;
	int maskPos = 1;
	int maskNeg = 1 << m_numIdxBits;

	for(std::size_t i=0; i<m_numIdxBits; i++, maskPos <<= 1, maskNeg <<= 1)
	{
		if(conj.m_pos.test(i))
			pos |= maskPos;
		else if(conj.m_neg.test(i))
			neg |= maskNeg;
	}

	return neg | pos;	
}


bool HashTable::merge(const Conjunct &merged, Conjunct &mergedResult)
{
	Conjunct testConj = merged;
	Conjunct prevConj;
	int idx = getIndex(testConj);
	ExistsType exType = NotExists;
	std::shared_ptr<Conjunct> pconj;

	for(std::size_t i=0; i<merged.m_pos.size(); i++)
	{
		if(testConj.m_pos.test(i) || testConj.m_neg.test(i))
		{
			testConj.m_pos.flip(i);
			testConj.m_neg.flip(i);

			if(i <= m_numIdxBits)
				idx = getIndex(testConj);

			ExistsType exTypeTmp = isExists(testConj, m_table[idx], pconj);

			if(exTypeTmp == ExistsAsConj)
			{
				exType = exTypeTmp;
				prevConj = testConj;
				break;
			}
			else if(exTypeTmp == ExistsAsPartOfConj)
			{
				exType = exTypeTmp;
				prevConj = testConj;
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
		removeConjunct(pconj);
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
		ptr = *itr;

		if(((*itr) -> m_pos & conj.m_pos) == (*itr) -> m_pos &&
			((*itr) -> m_neg & conj.m_neg) == (*itr) -> m_neg)
		{
			if(((*itr) -> m_pos & conj.m_pos) == conj.m_pos && 
				((*itr) -> m_neg & conj.m_neg) == conj.m_neg)
			{
				res = ExistsAsConj;
				break;
			}
			else 
				res = ExistsAsPartOfConj;
		}
	}

	return res;
}


DisForm HashTable::get()
{
	std::set<std::shared_ptr<Conjunct> > set;

	std::map<int, std::list<std::shared_ptr<Conjunct> > >::const_iterator itr = m_table.begin();

	for(;itr != m_table.end(); itr++)
	{
		std::list<std::shared_ptr<Conjunct> >::const_iterator itrLst = itr -> second.begin();

		for(;itrLst != itr -> second.end(); itrLst++)
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
	bool inserted = false;
	std::shared_ptr<Conjunct> newConj(new Conjunct(conj));
	for(std::size_t i=0; i<m_numIdxBits; i++)
	{
		if(!conj.m_pos.test(i) && !conj.m_neg.test(i))
		{
			conj.m_pos[i] = true;
			int idx = getIndex(conj);
			m_table[idx].push_back(newConj);

			conj.m_pos[i] = false;
			conj.m_neg[i] = true;

			idx = getIndex(conj);
			m_table[idx].push_back(newConj);

			conj.m_neg[i] = false;
			inserted = true;
		}
	}

	if(!inserted)
	{
		int idx = getIndex(conj);
		m_table[idx].push_back(newConj);
	}
}



void HashTable::removeConjunct(const std::shared_ptr<Conjunct> &ptr)
{
	Conjunct conj = *ptr;
	bool removed = false;
	bool needIndexUpdate = false;
	int idx = getIndex(conj);
	for(std::size_t i=0; i<m_numIdxBits; i++)
	{
		if(!conj.m_pos.test(i) && !conj.m_neg.test(i))
		{
			conj.m_pos[i] = true;

			idx = getIndex(conj);
			m_table[idx].remove(ptr);

			conj.m_pos[i] = false;
			conj.m_neg[i] = true;

			idx = getIndex(conj);
			m_table[idx].remove(ptr);

			conj.m_neg[i] = false;
			removed = true;
		}
	}

	if(!removed)
	{
		idx = getIndex(conj);
		m_table[idx].remove(ptr);
	}
}
