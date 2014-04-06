#include "KeyCreator.h"

#include "KeyCreatorImpl.h"

KeyCreator::KeyCreator():
	m_pimpl(new KeyCreatorImpl)
{
}

std::vector<KeyParams> KeyCreator::createKeys(const std::vector<DecrKeyParams> &keyParams, std::size_t size)
{
	return m_pimpl -> createKeys(keyParams, size);
}
