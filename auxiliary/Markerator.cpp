#include "Markerator.h"

Markerator::Markerator(const bcc::Function &func, const LFSR &lfsr):
	m_lfsr(lfsr)
	,m_func(func)
	,m_useDf(false)
{
}

Markerator::Markerator(const DisForm &df, const LFSR &lfsr):
	m_lfsr(lfsr)
	,m_df(df)
	,m_useDf(true)
{
}



bool Markerator::getNext()
{
	bool res = false;
	if(m_useDf)
	{
		Conjunct conj;
		conj.m_neg = conj.m_pos = m_lfsr.getVector();
		conj.m_neg.flip();

		for(std::size_t i=0; i<m_df.m_conjuncts.size() && !res; i++)
		{
			if((m_df.m_conjuncts[i].m_pos & conj.m_pos) == m_df.m_conjuncts[i].m_pos &&
				(m_df.m_conjuncts[i].m_neg & conj.m_neg) == m_df.m_conjuncts[i].m_neg)
			{
				res = true;
			}
		}
	}
	else
		res = m_func.calculate(m_lfsr.getVector());

	m_lfsr.nextState();

	return res;
}
