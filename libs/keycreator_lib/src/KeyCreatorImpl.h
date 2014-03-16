#ifndef KEY_CREATOR__IMPL_H_
#define KEY_CREATOR__IMPL_H_

#include <vector>

#include "DecrKeyParams.h"
#include "KeyParams.h"
#include "DisForm.h"
#include "KeyStream.h"
#include "LFSR.h"


class KeyCreatorImpl
{
	public:
		std::vector<KeyParams> createKeys(const std::vector<DecrKeyParams> &keyParams, 
			std::size_t size, std::size_t idxSize = 24);
	private:
		KeyParams createEncKeyParams(const std::vector<std::size_t> changePositions, std::size_t size, DisForm &encDf);
		KeyParams createDecrKeyParams(const BDDCalculator &encDf, const KeyParams &encParams, const DecrKeyParams &decParams, std::size_t size);

		KeyStream createDecrKeyStream(const BDDCalculator &encDf, const KeyParams &encParams, const DecrKeyParams &decParams, std::size_t size);
		LFSR createRandLFSR(std::size_t size, KeyParams &keyParams);
		DisForm createFilterFunc(KeyStream &keyStream, LFSR &lfsr, KeyParams &keyParams);
		std::vector<std::size_t> createChangedPosVector(const std::vector<DecrKeyParams> &keyParams);

		std::size_t     m_idxSize;
};



#endif
