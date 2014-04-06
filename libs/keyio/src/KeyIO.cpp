#include "KeyIO.h"

#include <limits>
#include <string>
#include <ctype.h>

#include <boost/lexical_cast.hpp>

#include "BitstreamReader.h"
#include "BitstreamWriter.h"


static void parseDF(const std::string &str, 
	std::vector <std::pair<unsigned long long, unsigned long long> > &monoms)
{
	std::size_t pos = 0;
	monoms.clear();

	for(;pos < str.length();)
	{
		std::size_t monomEndPos = str.find('|', pos);
		if(monomEndPos > str.length())
			monomEndPos = str.length();

		std::pair<unsigned long long, unsigned long long> monom = std::make_pair(0, 0);

		std::string m(str.substr(pos, monomEndPos - pos));
		bool isNot = false;
		for(std::size_t i=0; i<m.length(); i++)
		{
			if(m[i] == '!')
				isNot = true;
			if(m[i] == '&')
				isNot == false;
			if(isdigit(m[i]))
			{
				std::size_t k=1;
				while(isdigit(m[i + k]))
					k++;

				unsigned long long value = boost::lexical_cast<unsigned long long> (m.substr(i, k));
//				if(value > monomSize)
//					monomSize = value;

				if(isNot)
					monom.second |= 1 << value;
				else
					monom.first |= 1 << value;

				i += k - 1;
			}
		}

		monoms.push_back(monom);
		pos = monomEndPos + 1;
	}

//	if(!str.empty())
//	  monomSize++;
}



static void parseAF(const std::string &str, 
	std::vector <std::pair<unsigned long long, unsigned long long> > &monoms)
{
	std::size_t pos = 0;
	monoms.clear();

	for(;pos <= str.length();)
	{
		std::size_t monomEndPos = str.find('+', pos);
		if(monomEndPos > str.length())
			monomEndPos = str.length();

		std::pair<unsigned long long, unsigned long long> monom = std::make_pair(0, 0);

		std::string m(str.substr(pos, monomEndPos - pos));
		bool isNot = false;
		bool isConst = true;
		for(std::size_t i=0; i<m.length(); i++)
		{
			if(m[i] == '!')
				isNot = true;
			if(m[i] == 'x')
				isConst = false;
			if(m[i] == '&')
				isNot == false;
			if(isdigit(m[i]) && !isConst)
			{
				std::size_t k=1;
				while(isdigit(m[i + k]))
					k++;

				unsigned long long value = boost::lexical_cast<unsigned long long> (m.substr(i, k));

				if(isNot)
					monom.second |= 1 << value;
				else
					monom.first |= 1 << value;

				i += k - 1;
			}
		}

		monoms.push_back(monom);
		pos = monomEndPos + 1;
	}
}


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


std::size_t KeyIO::writeKey(unsigned char *pdata, std::size_t size, const KeyParams &key)
{
	std::vector <std::pair<unsigned long long, unsigned long long> > monoms;
	std::size_t numVars = key.m_lfsrInitVect.size();

	parseDF(key.m_filterFunc, monoms);

	BitstreamWriter bs(pdata, size);

	bs.writeBit8(numVars);
	bs.writeBit32(monoms.size());

	for(std::size_t i=0; i<monoms.size(); i++)
	{
		for(std::size_t j=0; j<numVars; j++)
			bs.writeBit(monoms[i].first & (1 << (numVars - j - 1)));

		for(std::size_t j=0; j<numVars; j++)
			bs.writeBit(monoms[i].second & (1 << (numVars - j - 1)));
	}

	parseAF(key.m_lfsrFunc, monoms);
	bs.writeBit32(monoms.size());

	for(std::size_t i=0; i<monoms.size(); i++)
	{
		for(std::size_t j=0; j<numVars; j++)
			bs.writeBit(monoms[i].first & (1 << (numVars - j - 1)));

		for(std::size_t j=0; j<numVars; j++)
			bs.writeBit(monoms[i].second & (1 << (numVars - j - 1)));
	}

	for(std::size_t i=0; i<key.m_lfsrInitVect.size(); i++)
		bs.writeBit(key.m_lfsrInitVect[numVars - i - 1]);

	for(std::size_t i=0; i<16; i++)
		bs.writeBit8(key.m_aesUserKey[i]);

	bs.toNextBase();

	for(std::size_t i=0; i<key.m_id.length(); i++)
		bs.writeBit8(key.m_id[i]);

	return bs.writedBitsCount() / CHAR_BIT;
}


std::size_t KeyIO::calcBufferSize(const KeyParams &key)
{
	std::size_t result = 32 + 8;
	std::size_t numVars = key.m_lfsrInitVect.size();
	std::vector <std::pair<unsigned long long, unsigned long long> > monoms;

	parseDF(key.m_filterFunc, monoms);

	result += numVars * monoms.size() * 2;
	result += 32;

	parseAF(key.m_lfsrFunc, monoms);
	result += numVars * monoms.size() * 2;
	result += key.m_lfsrInitVect.size();
	result += 16 * 8;

	if(result % 8)
		result = ((result / 8) + 1) * 8;

	result += 8 * key.m_id.length();

	return result / 8;

}
