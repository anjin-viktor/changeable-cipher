#ifndef LFSR_PARAMS_H_
#define LFSR_PARAMS_H_

#include <boost/dynamic_bitset.hpp>

class LFSRParams
{
	public:	
		void genRand(std::size_t size);

		std::string                  m_polynom;
		boost::dynamic_bitset<>      m_initVector;
};

#endif
