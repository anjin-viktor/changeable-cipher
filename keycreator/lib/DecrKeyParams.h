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
				ChangedValue()
				{
				}

				ChangedValue(std::size_t pos, bool val): m_pos(pos) ,m_val(val)
				{
				}

				std::size_t     m_pos;
				bool            m_val;

		};

		std::string                     m_id;
		std::vector<ChangedValue>       m_changes;
};


std::vector<DecrKeyParams> readDecrKeyParams(const std::string &fileName);

#endif
