#ifndef BITSTREAM_READER_H_
#define BITSTREAM_READER_H_

#include <cstddef>



class BitstreamReader
{
	public:
		BitstreamReader(const unsigned char *ptr, std::size_t size);
		bool getBit();
		unsigned char getBit8();
		unsigned int getBit32();
		void toNextBase();
		std::size_t available();

	private:
		const unsigned char        *m_ptr;
		std::size_t                 m_size;
		std::size_t                 m_posBase;
		std::size_t                 m_posInBase;
};




#endif
