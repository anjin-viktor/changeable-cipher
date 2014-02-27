#include "Markerator.h"

Markerator::Markerator(const bcc::Function &func, const LFSR &lfsr):
	m_lfsr(lfsr),
	m_func(func)
{
}


bool Markerator::getNext()
{
	bool res = m_func.calculate(m_lfsr.getVector());
	m_lfsr.nextState();

	return res;
}
