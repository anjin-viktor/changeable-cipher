#ifndef MARKERATOR_H_
#define MARKERATOR_H_

#include <BoolCalc.h>
#include "LFSR.h"

class Markerator
{
	public:
		Markerator(const bcc::Function &func, const LFSR &lfsr);

		bool getNext();
	private:
		LFSR                  m_lfsr;
		bcc::Function         m_func;
};



#endif
