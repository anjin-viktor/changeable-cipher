#include "Markerator.h"

Markerator::Markerator(const bcc::Function &func, const LFSR &lfsr):
	m_lfsr(lfsr)
	,m_func(func)
	,m_useDf(false)
{
}

Markerator::Markerator(const BDDCalculator &df, const LFSR &lfsr):
	m_lfsr(lfsr)
	,m_df(df)
	,m_useDf(true)
{
}



bool Markerator::getNext()
{
	bool res = false;
	if(m_useDf)
		res = m_df.calculate(m_lfsr.getVector());
	else
		res = m_func.calculate(m_lfsr.getVector());

	m_lfsr.nextState();

	return res;
}



unsigned char Markerator::getNextByte()
{
	unsigned char res = 0;
	for(unsigned char mask = 1 << (CHAR_BIT - 1); mask != 0; mask >>= 1)
	{
	  bool v;
	  if(m_useDf)
		  v = m_df.calculate(m_lfsr.getVector());
	  else
		  v = m_func.calculate(m_lfsr.getVector());
	  if(v)
	    res |= mask;
	  
	  m_lfsr.nextState();
	}
	return res;
}
