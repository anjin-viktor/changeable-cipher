#ifndef KEY_STREAM_H_
#define KEY_STREAM_H_

#include <vector>

class KeyStream
{
	public:
		void init(const std::vector<std::size_t> &params);
		bool get(std::size_t) const;
	private:
		std::vector<std::size_t>       m_changes;
};



#endif
