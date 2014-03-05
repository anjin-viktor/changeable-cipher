#ifndef BITSTREAM_WRITER_H_
#define BITSTREAM_WRITER_H_

#include <cstddef>



class BitstreamWriter
{
	public:
		BitstreamWriter(unsigned char *ptr, std::size_t size);
		void writeBit(bool);
		void writeBit8(unsigned char);
		void writeBit32(unsigned int);
		void toNextBase();
		std::size_t writedBitsCount() const;

	private:
		unsigned char              *m_ptr;
		std::size_t                 m_size;
		std::size_t                 m_posBase;
		std::size_t                 m_posInBase;
};




#endif
