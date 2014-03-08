#include <iostream>
#include <fstream>
#include <climits>

int main(int argc, char **argv)
{
	if(argc != 3)
	{
		std::cerr << "Usage: " << argv[0] << " `file1` `file2`" << std::endl;
		return 1;
	}


	std::ifstream in1(argv[1], std::ios_base::binary | std::ios_base::in);
	std::ifstream in2(argv[2], std::ios_base::binary | std::ios_base::in);

	if(!in1.is_open())
	{
		std::cerr << "Problem with opening file " << argv[1] << std::endl;
		return 2;
	}

	if(!in2.is_open())
	{
		std::cerr << "Problem with opening file " << argv[1] << std::endl;
		return 2;
	}
	int ndiff = 0;
	std::size_t position = 0;
	while(!in1.eof() && !in2.eof())
	{
		unsigned char ch1, ch2, res;
		in1.read((char *)&ch1, 1);
		in2.read((char *)&ch2, 1);

		res = ch1 ^ ch2;

		if(res)
		{
			unsigned char mask = 1 << (CHAR_BIT-1);
			for(;mask;position++, mask>>=1)
				if(res & mask)
					std::cout << position << std::endl;
		}
		else
			position += CHAR_BIT;
	}
	return 0;
}
