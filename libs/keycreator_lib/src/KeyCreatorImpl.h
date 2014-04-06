#ifndef KEY_CREATOR__IMPL_H_
#define KEY_CREATOR__IMPL_H_

#include <vector>

#include "DecrKeyParams.h"
#include "KeyParams.h"

#include "LFSR.h"
#include "KeyCreator.h"


class KeyCreatorImpl
{
	public:
		std::vector<KeyParams> createKeys(const std::vector<DecrKeyParams> &keyParams, 
			std::size_t size);
	private:
		static void createRandLFSR(std::size_t size, std::string &lfsrFunc, 
				    boost::dynamic_bitset<> &lfsrInitVect);
};



#endif
