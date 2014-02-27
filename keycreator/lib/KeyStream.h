#ifndef KEY_STREAM_H_
#define KEY_STREAM_H_

#include <boost/dynamic_bitset.hpp>

#include "DecrKeyParams.h"

class Markerator;

class KeyStream
{
	public:
		KeyStream(const DecrKeyParams &params);
		KeyStream(const DecrKeyParams &params, const Markerator &mark);
		bool getNext();

	private:
		bool getNextRand();
		bool getNextMark();

		unsigned int                                  m_block;
		std::size_t                                   m_currPos;
		std::shared_ptr<Markerator>		              m_pmark;
		std::vector<DecrKeyParams::ChangedValue>      m_changes;
};



#endif
