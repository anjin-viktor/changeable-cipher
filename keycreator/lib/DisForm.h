#ifndef DIS_FORM_H_
#define DIS_FORM_H_

#include <vector>
#include <string>

#include <boost/dynamic_bitset.hpp>

class Conjunct
{
	public:
		boost::dynamic_bitset<> m_pos;
		boost::dynamic_bitset<> m_neg;
};	


class DisForm
{
	public:
		std::vector<Conjunct>      m_conjuncts;
		std::string toString() const;
};



#endif
