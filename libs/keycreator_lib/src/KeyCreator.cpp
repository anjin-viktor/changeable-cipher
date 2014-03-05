#include "KeyCreator.h"

#include "KeyCreatorImpl.h"

KeyCreator::KeyCreator():
	m_pimpl(new KeyCreatorImpl)
{
}

std::vector<KeyParams> KeyCreator::createKeys(const std::vector<std::size_t> changePositions,
		const std::vector<DecrKeyParams> &keyParams, std::size_t size, std::size_t idxSize)
{
	return m_pimpl -> createKeys(changePositions, keyParams, size, idxSize);
}
