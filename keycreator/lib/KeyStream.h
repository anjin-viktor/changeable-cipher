#ifndef KEY_STREAM_H_
#define KEY_STREAM_H_

#include <boost/dynamic_bitset.hpp>

#include "DecrKeyParams.h"


class DisForm;

class KeyStream
{
	public:
		void genRand(std::size_t size);
		void change(const DecrKeyParams &params);

		boost::dynamic_bitset<unsigned char>   m_stream;
};



#endif
