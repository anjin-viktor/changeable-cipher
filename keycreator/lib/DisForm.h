#ifndef DIS_FORM_H_
#define DIS_FORM_H_

#include <vector>


class DisForm
{
	public:
		class Conjunct
		{
			public:
				boost::dynamic_bitset<> m_pos;
				boost::dynamic_bitset<> m_neg;
		};	


		std::vector<Conjunct>      m_conjuncts;
};



#endif
