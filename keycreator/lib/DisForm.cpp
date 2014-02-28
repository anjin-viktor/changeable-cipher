#include "DisForm.h"

std::string DisForm::toString() const
{
	std::string res;

	for(std::size_t i=0; i<m_conjuncts.size(); i++)
	{
		if(!res.empty())
			res += " | ";

		boost::dynamic_bitset<>::size_type pos = m_conjuncts[i].m_pos.find_next(0);
		for(;pos = boost::dynamic_bitset<>::npos; pos = m_conjuncts[i].m_pos.find_next(pos))
		{
			if(!res.empty())
				res += " & ";

			res += "x" + std::to_string(pos);
		}

		pos = m_conjuncts[i].m_neg.find_next(0);
		for(;pos = boost::dynamic_bitset<>::npos; pos = m_conjuncts[i].m_neg.find_next(pos))
		{
			if(!res.empty())
				res += " & ";

			res += "!x" + std::to_string(pos);
		}
	}

	return res;
}
