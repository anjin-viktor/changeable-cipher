#include "LFSR.h"

#include <BoolCalc.h>

LFSR::LFSR(const std::string &polinom, const boost::dynamic_bitset<> &initVector):
m_vector(initVector)
{
	try
	{
		m_ppolinom = std::shared_ptr<bcc::Function>(new bcc::Function(polinom, bcc::Function::MAP));
	}
	catch(std::runtime_error &)
	{
		m_ppolinom = std::shared_ptr<bcc::Function>(new bcc::Function(polinom));
	}
}

const boost::dynamic_bitset<> &LFSR::getLFSRVector() const
{
	return m_vector;
}


void LFSR::nextState()
{
	bool newValue = m_ppolinom -> calculate(m_vector);
	m_vector <<= 1;
	m_vector.set(0, newValue);
}
