#ifndef DECR_KEY_PARAMS_H_
#define DECR_KEY_PARAMS_H_

#include <string>
#include <vector>

class DecrKeyParams
{
	public:
		class ChangedValue
		{
			public:
				std::size_t     m_pos;
				bool            m_val;

		};

		std::string                     m_id;
		std::vector<ChangedValue>       m_changes;
};


std::vector<DecrKeyParams> readDecrKeyParams(const std::string &fileName);

#endif
