#ifndef KEY_IO_H_
#define KEY_IO_H_

#include "KeyParams.h"


namespace KeyIO
{
	KeyParams readKey(const unsigned char *pdata, std::size_t size);
	std::size_t writeKey(unsigned char *pdata, std::size_t size, const KeyParams &key);
	std::size_t calcBufferSize(const KeyParams &key);
};




#endif
