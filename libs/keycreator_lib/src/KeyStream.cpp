#include "KeyStream.h"
#include "KeyCreator.h"

#include <limits>

void KeyStream::init(const std::vector<std::size_t> &changes)
{
  m_changes = changes;

  if(!std::is_sorted(m_changes.begin(), m_changes.end()))
	std::sort(m_changes.begin(), m_changes.end());
}



bool KeyStream::get(std::size_t idx) const
{
	bool res = false;

	for(std::size_t i=0; !res && i < m_changes.size() && m_changes[i] <= idx ; i++)
	  if(idx == m_changes[i])
	    res = true;

	return res;
}
