#include "KeyIO.h"

#include <limits>
#include <string>

#include <iostream>

#include "BitstreamReader.h"
#include "BitstreamWriter.h"



KeyParams KeyIO::readKey(const unsigned char *pdata, std::size_t size)
{
	std::size_t position = 0;
	BitstreamReader bs(pdata, size);

	std::size_t numVars = bs.getBit8();
	std::size_t numDFMonoms = bs.getBit32();
	KeyParams res;

	for(std::size_t i=0; i<numDFMonoms; i++)
	{
		std::string monom;
		for(std::size_t j=0; j<numVars; j++)
		{
			if(bs.getBit())
			{
				if(monom.empty())
					monom = "x" + std::to_string(numVars - j - 1);
				else
					monom += " & x" + std::to_string(numVars - j - 1);
			}
		}

		for(std::size_t j=0; j<numVars; j++)
		{
			if(bs.getBit())
			{
				if(monom.empty())
					monom = "!x" + std::to_string(numVars - j - 1);
				else
					monom += " & !x" + std::to_string(numVars - j - 1);
			}
		}

		if(res.m_filterFunc.empty())
			res.m_filterFunc = monom;
		else
			res.m_filterFunc += " | " + monom;
	}

	std::size_t numLFSRMonoms = bs.getBit32();

	for(std::size_t i=0; i<numLFSRMonoms; i++)
	{
		bool isEmpty = true;
		std::string monom;
		for(std::size_t j=0; j<numVars; j++)
		{
			if(bs.getBit())
			{
				if(monom.empty())
					monom = "x" + std::to_string(numVars - j - 1);
				else
					monom += " & x" + std::to_string(numVars - j - 1);
				isEmpty = false;
			}
		}

		for(std::size_t j=0; j<numVars; j++)
		{
			if(bs.getBit())
			{
				if(monom.empty())
					monom = "!x" + std::to_string(numVars - j - 1);
				else
					monom += " & !x" + std::to_string(numVars - j - 1);
				isEmpty = false;
			}
		}

		if(isEmpty)
		{
			if(res.m_lfsrFunc.empty())
				res.m_lfsrFunc = "1";
			else
				res.m_lfsrFunc += " + 1";
		}
		else
		{
			if(res.m_lfsrFunc.empty())
				res.m_lfsrFunc = monom;
			else
				res.m_lfsrFunc += " + " + monom;
		}

	}

	res.m_lfsrInitVect.resize(numVars);
	for(std::size_t i=0; i<numVars; i++)
		res.m_lfsrInitVect[numVars-i - 1] = bs.getBit();
	BitstreamWriter aesWriter(res.m_aesUserKey, 16);

	for(std::size_t i=0; i<128; i++)
		aesWriter.writeBit(bs.getBit());

	bs.toNextBase();

	std::size_t idSize = bs.available() / CHAR_BIT + 1;
	unsigned char *id = new unsigned char[idSize];
	id[idSize - 1] = '\0';

	BitstreamWriter idWriter(id, idSize - 1);

	for(std::size_t i=0; i<idSize-1; i++)
		idWriter.writeBit8(bs.getBit8());

	res.m_id = std::string((const char *) id);
	delete [] id;

	return res;
}
