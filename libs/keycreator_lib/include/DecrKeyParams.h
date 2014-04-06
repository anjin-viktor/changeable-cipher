#ifndef DECR_KEY_PARAMS_H_
#define DECR_KEY_PARAMS_H_

#include <string>
#include <vector>

class DecrKeyParams
{
	public:
		std::string                     m_id;
		std::vector<std::size_t>        m_changes;
};


std::vector<DecrKeyParams> readDecrKeyParams(const std::string &fileName);

#endif
