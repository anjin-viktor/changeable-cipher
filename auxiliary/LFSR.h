#ifndef LFSR_H_
#define LFSR_H_

#include <string>
#include <memory>

#include <boost/dynamic_bitset.hpp>

namespace bcc
{
	class Function;
}

class LFSR
{
	public:
		LFSR(const std::string &polinom, const boost::dynamic_bitset<> &initVector);

		void nextState();
		const boost::dynamic_bitset<> &getLFSRVector() const;


	private:
		boost::dynamic_bitset<>               m_vector;
		std::shared_ptr<bcc::Function>        m_ppolinom;
};


#endif
