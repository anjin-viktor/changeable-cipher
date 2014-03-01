#ifndef KEY_CREATOR_H_
#define KEY_CREATOR_H_

#include <vector>

#include "DecrKeyParams.h"
#include "KeyParams.h"
#include "DisForm.h"
#include "KeyStream.h"
#include "LFSR.h"

class KeyCreator
{
	public:
		std::vector<KeyParams> createKeys(const std::vector<std::size_t> changePositions,
				const std::vector<DecrKeyParams> &keyParams, std::size_t size);


	private:
		KeyParams createEncKeyParams(const std::vector<std::size_t> changePositions, std::size_t size);
		KeyParams createDecrKeyParams(const KeyParams &encParams, const DecrKeyParams &decParams, std::size_t size);

		KeyStream createDecrKeyStream(const KeyParams &encParams, const DecrKeyParams &decParams, std::size_t size);
		LFSR createRandLFSR(std::size_t size, KeyParams &keyParams);
		void createFilterFunc(KeyStream &keyStream, LFSR &lfsr, KeyParams &keyParams);
		bool merge(DisForm &disForm, const Conjunct &conj, Conjunct &newConj);
};



#endif
