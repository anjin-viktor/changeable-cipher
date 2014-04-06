#include <iostream>
#include <fstream>
#include <cstdio>

#include <boost/program_options.hpp>

#include <openssl/aes.h>

#include "KeyIO.h"
#include "LFSR.h"
#include "BoolCalc.h"
#include "Markerator.h"
#include "KeyParams.h"

#define BUFFER_SIZE 1024

unsigned char IV[16] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 0xA, 0xB, 0xC, 0xD, 0xE, 0xF};

enum ProcessType
{
	None,
	Dec,
	Enc
};


void processMarkerator(Markerator &mark, unsigned char *buf, std::size_t size)
{
	std::size_t baseCounter = 0;
	std::size_t bitCounter = 7;

	for(;baseCounter != size;)
	{
		buf[baseCounter] ^= mark.getNextByte();
		baseCounter++;
	}
}




void process(const std::string &in, const std::string &out, const KeyParams &key, ProcessType type)
{
	std::ifstream fileIn(in.c_str(), std::ios_base::in | std::ios_base::binary);
	if(!fileIn)
		throw std::runtime_error("problem with opening file `" + in + "` for reading");

	std::ofstream fileOut(out.c_str(), std::ios_base::out | std::ios_base::binary);
	if(!fileOut)
		throw std::runtime_error("problem with opening file `" + out + "` for writing");

	char tmpFileName[L_tmpnam];
	tmpnam(tmpFileName);

	std::ofstream tmpOut(tmpFileName, std::ios_base::out | std::ios_base::binary);

	if(!tmpOut)
		throw std::runtime_error("problem with creating temporary file");

	Markerator mark(bcc::Function(key.m_filterFunc, bcc::Function::LIST_OF_MONOMS_WITH_ALL_VARS/*bcc::Function::BDD_DF*/), 
		LFSR(key.m_lfsrFunc, key.m_lfsrInitVect));

	unsigned char iv[16];
	memcpy(iv, IV, 16);

	if(type == Enc)
	{
		while(!fileIn.eof())
		{
			unsigned char block[BUFFER_SIZE];
			std::size_t size = BUFFER_SIZE;
			fileIn.read((char *) block, size);
			size = fileIn.gcount();

			processMarkerator(mark, block, size);

			tmpOut.write((char *) block, size);
		}

		tmpOut.close();

		std::ifstream tmpIn(tmpFileName, std::ios_base::in | std::ios_base::binary);
		int num = 0;
		AES_KEY decKey;
		AES_set_decrypt_key(key.m_aesUserKey, 128, &decKey);

		while(!tmpIn.eof())
		{
			unsigned char block[BUFFER_SIZE];
			unsigned char outBlock[BUFFER_SIZE];
			std::size_t size = BUFFER_SIZE;
			tmpIn.read((char *) block, size);
			size = tmpIn.gcount();
			AES_ofb128_encrypt (block, outBlock, size, &decKey, iv, &num);
			fileOut.write((char *) outBlock, size);
		}
	}
	else if(type == Dec)
	{
		int num = 0;
		AES_KEY encKey;
		AES_set_decrypt_key(key.m_aesUserKey, 128, &encKey);

		while(!fileIn.eof())
		{
			unsigned char block[BUFFER_SIZE];
			unsigned char outBlock[BUFFER_SIZE];
			std::size_t size = BUFFER_SIZE;
			fileIn.read((char *) block, size);
			size = fileIn.gcount();
			AES_ofb128_encrypt (block, outBlock, size, &encKey, iv, &num);

			tmpOut.write((char *) outBlock, size);
		}
		tmpOut.close();

		std::ifstream tmpIn(tmpFileName, std::ios_base::in | std::ios_base::binary);
		while(!tmpIn.eof())
		{
			unsigned char block[BUFFER_SIZE];
			std::size_t size = BUFFER_SIZE;
			tmpIn.read((char *) block, size);
			size = tmpIn.gcount();

			processMarkerator(mark, block, size);

			fileOut.write((char *) block, size);
		}
	}
}


int main(int argc, char **argv)
{
	namespace po = boost::program_options;
	try {
		po::options_description desc("Options");
		desc.add_options()
			("help", "produce help message")
			("input,i", po::value<std::string>(), 
				"path to in file")
			("output,o", po::value<std::string>(), 
				"path to out file")
			("key,k", po::value<std::string>(), "path to key file")
			("dec,d", "decryption")
			("enc,e", "encryption")
		;

		po::variables_map vm;        
		po::store(po::parse_command_line(argc, argv, desc), vm);
		po::notify(vm);    

		if (vm.count("help")) 
		{
			std::cout << desc << "\n";
			return 0;
		}

		if(!vm.count("input") || !vm.count("output") || !vm.count("key") || vm.count("dec") == vm.count("enc"))
		{
			std::cout << desc << "\n";
			return 1;
		}

		std::ifstream in(vm["key"].as<std::string>().c_str(), std::ios_base::in | std::ios_base::binary);

		if(!in.is_open())
		{
			std::cerr << "problem with opening file `" << vm["key"].as<std::string>().c_str() 
					<< "` with key for reading" << std::endl;
			return 1;
		}

		in.seekg (0, std::ios_base::end);
		std::size_t length = in.tellg();
		in.seekg (0, std::ios_base::beg);

		char *data = new char[length];
		in.read(data, length);

		if(!in.good())
		{
			std::cerr << "problem with reading key from `" << vm["key"].as<std::string>() << "`" <<  std::endl;
			return 1;
		}

		KeyParams key = KeyIO::readKey((const unsigned char *) data, length);
		delete [] data;

		ProcessType type;
		if(vm.count("dec"))
			type = Dec;
		else
			type = Enc;

		process(vm["input"].as<std::string>(), vm["output"].as<std::string>(), key, type);
	}
	catch(std::exception& e) {
		std::cerr << "Error: " << e.what() << "\n";
		return 1;
	}
	catch(...) {
		std::cerr << "Exception of unknown type!\n";
	}

	return 0;
}
