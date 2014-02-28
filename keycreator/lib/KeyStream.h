#ifndef KEY_STREAM_H_
#define KEY_STREAM_H_

#include <memory>

#include <boost/dynamic_bitset.hpp>

#include "DecrKeyParams.h"
#include "DisForm.h"

class Markerator;

class KeyStream
{
	public:
		KeyStream(const DecrKeyParams &params, std::size_t size);
		KeyStream(const DecrKeyParams &params, const Markerator &mark, std::size_t size);
		bool getNext();
		bool hasNext() const;

	private:
		bool getNextRand();
		bool getNextMark();

		unsigned int                                  m_block;
		std::size_t                                   m_currPos;
		std::size_t                                   m_size;
		std::shared_ptr<Markerator>                   m_pmark;
		std::vector<DecrKeyParams::ChangedValue>      m_changes;
};



#endif
