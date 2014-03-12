#include "KeyCreatorImpl.h"

#include <cmath>
#include <limits>
#include <string>
#include <algorithm>

#include "Markerator.h"
#include "PrimitivePolynoms.h"
#include "HashTable.h"

#define MAX_LFSR_SIZE 256
/*

static void checkDF(const DisForm &df)
{
	for(std::size_t i=0; i<df.m_conjuncts.size(); i++)
	{
		for(std::size_t j=0; j<df.m_conjuncts.size(); j++)
		{
			if(i != j)
			{
				if((df.m_conjuncts[j].m_pos & df.m_conjuncts[i].m_pos) == df.m_conjuncts[i].m_pos &&
					(df.m_conjuncts[j].m_neg & df.m_conjuncts[i].m_neg) == df.m_conjuncts[i].m_neg)
				{
				//	assert(false);
					std::cerr << df.m_conjuncts[j].m_pos << " " << df.m_conjuncts[j].m_neg << std::endl;
					std::cerr << df.m_conjuncts[i].m_pos << " " << df.m_conjuncts[i].m_neg << std::endl;
					std::cerr << "================" << std::endl;

				}
			}
		}
	}
}

*/

KeyParams KeyCreatorImpl::createEncKeyParams(const std::vector<std::size_t> changePositions, 
	std::size_t size, DisForm &encDf)
{
	DecrKeyParams encParams;

	for(std::size_t i=0; i<changePositions.size(); i++)
		encParams.m_changes.push_back(DecrKeyParams::ChangedValue(changePositions[i], 0));

	KeyStream encStream(encParams, size);

	KeyParams result;
	result.m_id = "enc";

	LFSR lfsr = createRandLFSR(size, result);

	encDf = createFilterFunc(encStream, lfsr, result);
	return result;
}


LFSR KeyCreatorImpl::createRandLFSR(std::size_t keystreamSize, KeyParams &keyParams)
{
	std::size_t size = log2(keystreamSize);	

	if(log2(keystreamSize) > size && keystreamSize != (std::size_t) exp2(size))
		size++;

	std::size_t idx = rand() % POLINOMS_NOMBER_IN_CLASS;
	if(size < 14)
		size = 14;

	if(size > 30)
		throw std::runtime_error("requested keystream (" + std::to_string(keystreamSize) + 
			" bits) is too big (max value 2 ^ 30 bits)");

	switch(size)
	{
		case 14:
			keyParams.m_lfsrFunc = primPolynoms_14[idx];
			break;
		case 15:
			keyParams.m_lfsrFunc = primPolynoms_15[idx];
			break;
		case 16:
			keyParams.m_lfsrFunc = primPolynoms_16[idx];
			break;
		case 17:
			keyParams.m_lfsrFunc = primPolynoms_17[idx];
			break;
		case 18:
			keyParams.m_lfsrFunc = primPolynoms_18[idx];
			break;
		case 19:
			keyParams.m_lfsrFunc = primPolynoms_19[idx];
			break;
		case 20:
			keyParams.m_lfsrFunc = primPolynoms_20[idx];
			break;
		case 21:
			keyParams.m_lfsrFunc = primPolynoms_21[idx];
			break;
		case 22:
			keyParams.m_lfsrFunc = primPolynoms_22[idx];
			break;
		case 23:
			keyParams.m_lfsrFunc = primPolynoms_23[idx];
			break;
		case 24:
			keyParams.m_lfsrFunc = primPolynoms_24[idx];
			break;
		case 25:
			keyParams.m_lfsrFunc = primPolynoms_25[idx];
			break;
		case 26:
			keyParams.m_lfsrFunc = primPolynoms_26[idx];
			break;
		case 27:
			keyParams.m_lfsrFunc = primPolynoms_27[idx];
			break;
		case 28:
			keyParams.m_lfsrFunc = primPolynoms_28[idx];
			break;
		case 29:
			keyParams.m_lfsrFunc = primPolynoms_29[idx];
			break;
		case 30:
			keyParams.m_lfsrFunc = primPolynoms_30[idx];
			break;
	}

	char initVal[MAX_LFSR_SIZE];
	std::size_t pos=0;
	for(; pos<size;)
	{
		unsigned int block = rand();
		unsigned int mask = 1;

		while(mask && pos < size)
		{
			if(block & mask)
				initVal[pos] = '0';
			else
				initVal[pos] = '1';

			pos++;
			mask <<= 1;
		}
	}
	initVal[pos] = '\0';
	keyParams.m_lfsrInitVect = boost::dynamic_bitset<>(std::string(initVal));

	return LFSR(keyParams.m_lfsrFunc, keyParams.m_lfsrInitVect);
}


DisForm KeyCreatorImpl::createFilterFunc(KeyStream &keyStream, LFSR &lfsr, KeyParams &keyParams)
{
	DisForm df;
	HashTable table(m_idxSize);
	for(;keyStream.hasNext(); lfsr.nextState())
	{
		boost::dynamic_bitset<> v = lfsr.getVector();
		if(keyStream.getNext())
		{
			Conjunct conj;
			conj.m_neg = conj.m_pos = v;
			conj.m_neg.flip();
			Conjunct newConj;
			bool inserted = table.merge(conj, newConj);
			while(!inserted)
			{
				conj = newConj;
				inserted = table.merge(conj, newConj);
			}
		}
	}
	df = table.get();
	keyParams.m_filterFunc = df.toString();
	return df;
}


KeyStream KeyCreatorImpl::createDecrKeyStream(const BDDCalculator &encDf, const KeyParams &encParams, const DecrKeyParams &decParams, std::size_t size)
{
	Markerator mark(encDf, LFSR(encParams.m_lfsrFunc, encParams.m_lfsrInitVect));
	return KeyStream(decParams, mark, size);
}


KeyParams KeyCreatorImpl::createDecrKeyParams(const BDDCalculator &encDf, const KeyParams &encParams, const DecrKeyParams &decParams, std::size_t size)
{
	KeyParams result;
	result.m_id = decParams.m_id;
	LFSR lfsr = createRandLFSR(size, result);
	KeyStream keyStream(createDecrKeyStream(encDf, encParams, decParams, size));
	createFilterFunc(keyStream, lfsr, result);

	return result;
}


std::vector<KeyParams> KeyCreatorImpl::createKeys(const std::vector<DecrKeyParams> &keyParams, std::size_t size, std::size_t idxSize)
{
	m_idxSize = idxSize;
	std::vector<KeyParams> keys(keyParams.size() + 1);
	DisForm encDf;

	keys[0] = createEncKeyParams(createChangedPosVector(keyParams), size, encDf);
//	checkDF(encDf);

	BDDCalculator calc(encDf);

	for(std::size_t i=0; i<keyParams.size(); i++)
		keys[i+1] = createDecrKeyParams(calc, keys[0], keyParams[i], size);

	return keys;
}


std::vector<std::size_t> KeyCreatorImpl::createChangedPosVector(const std::vector<DecrKeyParams> &keyParams)
{
	std::vector<std::size_t> result;

	for(std::size_t i=0; i<keyParams.size(); i++)
		for(std::size_t j=0; j<keyParams[i].m_changes.size(); j++)
			if(std::find(result.begin(), result.end(), keyParams[i].m_changes[j].m_pos) == result.end())
				result.push_back(keyParams[i].m_changes[j].m_pos);

	return result;
}