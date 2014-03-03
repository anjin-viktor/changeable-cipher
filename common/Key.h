#ifndef KEY_H_
#define KEY_H_

#include <std::string>

#include <boost/dynamic_bitset.hpp>

#include "DisForm.h"

class Key
{
	public:
		std::string                 m_polinom
		boost::dynamic_bitset<>     m_initVector;
		DisForm                     m_filterFunc;
};



#endif
