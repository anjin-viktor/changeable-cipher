#ifndef HASH_TABLE_H_
#define HASH_TABLE_H_

#include <memory>
#include <list>
#include <vector>

#include "DisForm.h"

class HashTable
{
	public: 
		HashTable(std::size_t numBits);
		bool merge(const Conjunct &merged, Conjunct &mergedResult);
		DisForm get();

	private:
		enum ExistsType
		{
			NotExists = 0,
			ExistsAsConj,
			ExistsAsPartOfConj
		};

		std::size_t getIndex(const Conjunct &conj);
		ExistsType isExists(const Conjunct &conj, 
			const std::list<std::shared_ptr<Conjunct> > &lst, std::shared_ptr<Conjunct> &ptr);
		void insertConjunct(Conjunct conj);
		void removeConjunct(const std::shared_ptr<Conjunct> &ptr);

		std::size_t                                                   m_numIdxBits;
		std::vector<std::list<std::shared_ptr<Conjunct> > >           m_table;
};

#endif
