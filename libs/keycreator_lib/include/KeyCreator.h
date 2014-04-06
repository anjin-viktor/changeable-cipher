#ifndef KEY_CREATOR_H_
#define KEY_CREATOR_H_

#include <vector>
#include <memory>

#include "DecrKeyParams.h"
#include "KeyParams.h"

class KeyCreatorImpl;

class KeyCreator
{
	public:
		KeyCreator();
		std::vector<KeyParams> createKeys(const std::vector<DecrKeyParams> &keyParams, std::size_t size);
	private:
		std::shared_ptr<KeyCreatorImpl>              m_pimpl;
};



#endif
