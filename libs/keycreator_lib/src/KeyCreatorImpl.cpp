#include "KeyCreatorImpl.h"

#include <cmath>
#include <limits>
#include <string>
#include <algorithm>

#include "PrimitivePolynoms.h"
#include "DisForm.h"
#include "KeyStream.h"

#include <iostream>

#define MAX_LFSR_SIZE 256

void KeyCreatorImpl::createRandLFSR(std::size_t keystreamSize, std::string &lfsrFunc, boost::dynamic_bitset<> &lfsrInitVect)
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
			lfsrFunc = primPolynoms_14[idx];
			break;
		case 15:
			lfsrFunc = primPolynoms_15[idx];
			break;
		case 16:
			lfsrFunc = primPolynoms_16[idx];
			break;
		case 17:
			lfsrFunc = primPolynoms_17[idx];
			break;
		case 18:
			lfsrFunc = primPolynoms_18[idx];
			break;
		case 19:
			lfsrFunc = primPolynoms_19[idx];
			break;
		case 20:
			lfsrFunc = primPolynoms_20[idx];
			break;
		case 21:
			lfsrFunc = primPolynoms_21[idx];
			break;
		case 22:
			lfsrFunc = primPolynoms_22[idx];
			break;
		case 23:
			lfsrFunc = primPolynoms_23[idx];
			break;
		case 24:
			lfsrFunc = primPolynoms_24[idx];
			break;
		case 25:
			lfsrFunc = primPolynoms_25[idx];
			break;
		case 26:
			lfsrFunc = primPolynoms_26[idx];
			break;
		case 27:
			lfsrFunc = primPolynoms_27[idx];
			break;
		case 28:
			lfsrFunc = primPolynoms_28[idx];
			break;
		case 29:
			lfsrFunc = primPolynoms_29[idx];
			break;
		case 30:
			lfsrFunc = primPolynoms_30[idx];
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
	lfsrInitVect = boost::dynamic_bitset<>(std::string(initVal));
}



std::vector<KeyParams> KeyCreatorImpl::createKeys(const std::vector<DecrKeyParams> &keyParams, std::size_t size)
{
	std::vector<KeyParams> keys(keyParams.size());
	
	if(keys.size() == 0)
	  return keys;

	createRandLFSR(size, keys[0]. m_lfsrFunc, keys[0].m_lfsrInitVect);

	for(std::size_t i=1; i<keys.size(); i++)
	{
	  keys[i]. m_lfsrFunc = keys[0]. m_lfsrFunc;
	  keys[i].m_lfsrInitVect = keys[0].m_lfsrInitVect;
	}
	
	std::vector<std::size_t> allChanges(keyParams[0].m_changes);

	std::vector<DisForm> dforms(keys.size());
	std::vector<KeyStream> streams(keys.size());
	LFSR lfsr(keys[0]. m_lfsrFunc, keys[0].m_lfsrInitVect);
	std::size_t max = 0;

	for(std::size_t i=0; i<keys.size(); i++)
	{
	  streams[i].init(keyParams[i].m_changes);
	  keys[i].m_id = keyParams[i].m_id;
	  for(std::size_t j=0; j<keyParams[i].m_changes.size(); j++)
	  {
	    if(max < keyParams[i].m_changes[j])
	      max = keyParams[i].m_changes[j];
	  
	    bool finded = false;
	    for(std::size_t k=0; k<allChanges.size() && !finded; k++)
	    {
	      if(allChanges[k] == keyParams[i].m_changes[j])
		finded = true;
	    }

	    if(!finded)
	      allChanges.push_back(keyParams[i].m_changes[j]);
	  }
	}

	KeyStream testStream;
	testStream.init(allChanges);

	for(std::size_t i=0; i<=max; i++)
	{
	  if(testStream.get(i))
	  {
	    Conjunct conj;
	    conj.m_neg = conj.m_pos = lfsr.getVector();
	    conj.m_neg.flip();

	    for(std::size_t j=0; j<streams.size(); j++)
	      if(streams[j].get(i))
		dforms[j].m_conjuncts.push_back(conj);
	  }
	  lfsr.nextState();
	}

	for(std::size_t i=0; i<keys.size(); i++)
	  keys[i].m_filterFunc = dforms[i].toString();

	return keys;
}
