#ifndef KEY_PARAMS_H_
#define KEY_PARAMS_H_

#include <string>
#include <boost/dynamic_bitset.hpp>

class KeyParams
{
	public:
		std::string                 m_id;
		std::string                 m_filterFunc;
		std::string                 m_lfsrFunc;
		boost::dynamic_bitset<>     m_lfsrInitVect;
		unsigned char               m_aesUserKey[16];
};


#endif
