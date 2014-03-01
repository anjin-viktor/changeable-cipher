#include "DisForm.h"
std::string DisForm::toString() const
{
	std::string res;

	for(std::size_t i=0; i<m_conjuncts.size(); i++)
	{
		if(!res.empty())
			res += " | ";
		bool needAnd = false;

		if(m_conjuncts[i].m_pos.test(0))
		{
			needAnd = true;
			res += "x0";
		}

		boost::dynamic_bitset<>::size_type pos = m_conjuncts[i].m_pos.find_next(0);
		for(;pos != boost::dynamic_bitset<>::npos; pos = m_conjuncts[i].m_pos.find_next(pos))
		{
			if(needAnd)
				res += " & ";

			needAnd = true;
			res += "x" + std::to_string(pos);
		}

		if(m_conjuncts[i].m_neg.test(0))
		{
			if(needAnd)
				res += " & ";

			needAnd = true;
			res += "!x0";
		}


		pos = m_conjuncts[i].m_neg.find_next(0);
		for(;pos != boost::dynamic_bitset<>::npos; pos = m_conjuncts[i].m_neg.find_next(pos))
		{
			if(needAnd)
				res += " & ";

			needAnd = true;
			res += "!x" + std::to_string(pos);
		}
	}

	return res;
}
