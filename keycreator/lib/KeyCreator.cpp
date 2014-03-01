#include "KeyCreator.h"

#include <cmath>
#include <limits>
#include <string>

#include "Markerator.h"
#include "PrimitivePolynoms.h"

#define MAX_LFSR_SIZE 256


KeyParams KeyCreator::createEncKeyParams(const std::vector<std::size_t> changePositions, std::size_t size)
{
	DecrKeyParams encParams;

	for(std::size_t i=0; i<changePositions.size(); i++)
		encParams.m_changes.push_back(DecrKeyParams::ChangedValue(changePositions[i], 0));

	KeyStream encStream(encParams, size);

	KeyParams result;
	result.m_id = "enc";

	LFSR lfsr = createRandLFSR(size, result);

	createFilterFunc(encStream, lfsr, result);
	return result;
}


LFSR KeyCreator::createRandLFSR(std::size_t keystreamSize, KeyParams &keyParams)
{
	std::size_t size = log2(keystreamSize);	

	if(log2(keystreamSize) > size && keystreamSize != (std::size_t) exp2(size))
		size++;

	std::size_t idx = rand() % POLINOMS_NOMBER_IN_CLASS;

	if(size <= 20)
	{
		size = 20;
		keyParams.m_lfsrFunc = primPolynoms_20[idx];
	}
	else if(size <= 22)
	{
		size = 22;
		keyParams.m_lfsrFunc = primPolynoms_22[idx];
	}
	else if(size <= 24)
	{
		size = 24;
		keyParams.m_lfsrFunc = primPolynoms_24[idx];
	}
	else if(size <= 26)
	{
		size = 26;
		keyParams.m_lfsrFunc = primPolynoms_26[idx];
	}
	else if(size <= 28)
	{
		size = 28;
		keyParams.m_lfsrFunc = primPolynoms_28[idx];
	}
	else if(size <= 30)
	{
		size = 30;
		keyParams.m_lfsrFunc = primPolynoms_30[idx];
	}
	else
		throw std::runtime_error("requested keystream (" + std::to_string(keystreamSize) + 
			" bits) is too big (max value 2 ^ 30 bits)");

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


void KeyCreator::createFilterFunc(KeyStream &keyStream, LFSR &lfsr, KeyParams &keyParams)
{
	DisForm df;
	for(;keyStream.hasNext(); lfsr.nextState())
	{
		boost::dynamic_bitset<> v = lfsr.getVector();
		if(keyStream.getNext())
		{
			Conjunct conj;
			conj.m_neg = conj.m_pos = v;
			conj.m_neg.flip();

			Conjunct newConj;
			bool inserted = merge(df, conj, newConj);
			while(!inserted)
			{
				conj = newConj;
				inserted = merge(df, conj, newConj);
			}
		}
	}
	keyParams.m_filterFunc = df.toString();
}


bool KeyCreator::merge(DisForm &disForm, const Conjunct &conj, Conjunct &newConj)
{
	std::size_t mergedConjSize = std::numeric_limits<std::size_t>::max();
	std::size_t mergedConjPos = 0;

	for(std::size_t i=0; i<disForm.m_conjuncts.size(); i++)
	{
		if((disForm.m_conjuncts[i].m_neg & conj.m_pos).count() + 
			(disForm.m_conjuncts[i].m_pos & conj.m_neg).count() == 1)
		{
			if(disForm.m_conjuncts[i].m_neg.count() + disForm.m_conjuncts[i].m_pos.count() < mergedConjSize)
			{
				mergedConjSize = disForm.m_conjuncts[i].m_neg.count() + disForm.m_conjuncts[i].m_pos.count();
				mergedConjPos = i;
			}
		}
	}

	bool res = false;

	if(mergedConjSize != std::numeric_limits<std::size_t>::max())
	{
		if(mergedConjSize == conj.m_neg.count() + conj.m_pos.count())
		{
			disForm.m_conjuncts[mergedConjPos].m_neg &= conj.m_neg;
			disForm.m_conjuncts[mergedConjPos].m_pos &= conj.m_pos;
			newConj = disForm.m_conjuncts[mergedConjPos];
			res = true;
		}
		else
		{
			newConj = conj;
			boost::dynamic_bitset<> pos = disForm.m_conjuncts[mergedConjPos].m_neg & conj.m_pos;
			boost::dynamic_bitset<> neg = disForm.m_conjuncts[mergedConjPos].m_pos & conj.m_neg;

			if(pos.any())
				newConj.m_pos[pos.find_first()] = false;
			else
				newConj.m_neg[neg.find_first()] = false;
		}

	}
	else
	{
		disForm.m_conjuncts.push_back(conj);
		newConj = conj;
		res = true;
	}

	return res;
}


KeyStream KeyCreator::createDecrKeyStream(const KeyParams &encParams, const DecrKeyParams &decParams, std::size_t size)
{
	Markerator mark(bcc::Function(encParams.m_filterFunc, bcc::Function::THREE),
		LFSR(encParams.m_lfsrFunc, encParams.m_lfsrInitVect));

	return KeyStream(decParams, mark, size);
}


KeyParams KeyCreator::createDecrKeyParams(const KeyParams &encParams, const DecrKeyParams &decParams, std::size_t size)
{
	KeyParams result;
	result.m_id = decParams.m_id;
	LFSR lfsr = createRandLFSR(size, result);
	KeyStream keyStream(createDecrKeyStream(encParams, decParams, size));
	createFilterFunc(keyStream, lfsr, result);

	return result;
}



std::vector<KeyParams> KeyCreator::createKeys(const std::vector<std::size_t> changedPositions,
	const std::vector<DecrKeyParams> &keyParams, std::size_t size)
{
	std::vector<KeyParams> keys(keyParams.size() + 1);
	keys[0] = createEncKeyParams(changedPositions, size);

	for(std::size_t i=0; i<keyParams.size(); i++)
		keys[i+1] = createDecrKeyParams(keys[0], keyParams[i], size);

	return keys;
}