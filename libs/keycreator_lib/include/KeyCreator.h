#ifndef KEY_CREATOR_H_
#define KEY_CREATOR_H_

#include <vector>

#include "DecrKeyParams.h"
#include "KeyParams.h"

class KeyCreatorImpl;

class KeyCreator
{
	public:
		KeyCreator();

		std::vector<KeyParams> createKeys(const std::vector<std::size_t> changePositions,
				const std::vector<DecrKeyParams> &keyParams, std::size_t size, std::size_t idxSize = 12);
	private:
		std::shared_ptr<KeyCreatorImpl>              m_pimpl;           
};



#endif
