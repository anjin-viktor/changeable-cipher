#ifndef MARKERATOR_H_
#define MARKERATOR_H_

#include <BoolCalc.h>
#include <boost/dynamic_bitset.hpp>
#include "LFSR.h"
#include "DisForm.h"

class Markerator
{
	public:
		Markerator(const bcc::Function &func, const LFSR &lfsr);
		Markerator(const BDDCalculator &df, const LFSR &lfsr);

		bool getNext();
		unsigned char getNextByte();
	private:
		LFSR                  m_lfsr;
		bcc::Function         m_func;
		BDDCalculator         m_df;
		bool                  m_useDf;
};



#endif
