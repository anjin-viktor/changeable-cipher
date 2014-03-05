#include <iostream>
#include <fstream>

#include "KeyIO.h"
#include "KeyParams.h"

int main(int argc, char **argv)
{
	if(argc != 2)
	{
		std::cerr << "usage: " << argv[0] << " `key_file`" << std::endl;
		return 1;
	}

	std::ifstream in(argv[1],  std::ios_base::in | std::ios_base::binary);

	if(!in.is_open())
	{
		std::cerr << "problem with opening file " << argv[1] << " for reading" << std::endl;
		return 2;
	}

	in.seekg (0, std::ios_base::end);
	std::size_t length = in.tellg();
	in.seekg (0, std::ios_base::beg);

	char *data = new char[length];
	in.read(data, length);

	if(!in.good())
	{
		std::cerr << "problem with reading data from " << argv[1] << std::endl;
		return 3;
	}

	KeyParams key = KeyIO::readKey((const unsigned char *) data, length);

	std::cout << "`" << argv[1] << "` info:" << std::endl;;
	std::cout << "\tkey_id: " << key.m_id << std::endl;
	std::cout << "\tfilter_func: " << key.m_filterFunc << std::endl;
	std::cout << "\tlfsr_func: " << key.m_lfsrFunc << std::endl;
	std::cout << "\tlfsr_init_vector: " << key.m_lfsrInitVect << std::endl;
	std::cout << "\taes_key: 0x" << std::hex;
	for(std::size_t i=0; i<16; i++)
		std::cout << (int) key.m_aesUserKey[i];

	std::cout << std::dec << std::endl;

	return 0;
}
