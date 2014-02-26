#include "LFSRParams.h"
#include "PrimitivePolynoms.h"

#include <string>

void LFSRParams::genRand(std::size_t size)
{
	m_initVector.resize(size);
	for(std::size_t i=0; i<size; i++)
		m_initVector[i] = rand() % 2;

	std::size_t pos = rand() % POLINOMS_NOMBER_IN_CLASS;

	if(size <= 20)
		m_polynom = primPolynoms_20[pos];
	else if(size <= 22)
		m_polynom = primPolynoms_22[pos];
	else if(size <= 24)
		m_polynom = primPolynoms_24[pos];
	else if(size <= 26)
		m_polynom = primPolynoms_26[pos];
	else if(size <= 28)
		m_polynom = primPolynoms_28[pos];
	else if(size <= 30)
		m_polynom = primPolynoms_30[pos];
	else
		throw std::runtime_error(std::to_string(size) + "bits is too big for LFSM. Max is 30");
}