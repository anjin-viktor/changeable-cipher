#include <iostream>
#include <iterator>
#include <fstream>
#include <ctime>
#include <cstdlib>

#include <boost/program_options.hpp>
#include <boost/filesystem.hpp>

#include "KeyCreator.h"
#include "DecrKeyParams.h"
#include "KeyIO.h"


int main(int argc, char **argv)
{
	namespace po = boost::program_options;

	srand(time(NULL));

	try 
	{
		po::options_description generic("Generic options");
		generic.add_options()
			("help", "produce help message")
			("config,c", po::value<std::string>(), "name of a file with a configuration.")
		;

		po::options_description params("Configuration");
		params.add_options()
			("aes_key", po::value<std::string>(), 
				"path to the file with private key for AES-128")
			("decr_keys_config", po::value<std::string>(), 
				"path to the file with informations about decryption differences")
			("out_dir,o", po::value<std::string>(), "path to out_directory")
			("hash_idx_size", po::value<std::size_t>() -> default_value(24), 
					"size of the index for table with the results of intermediate calculations")
			("max_size,s", po::value<std::size_t>(), 
					"max size(in bytes) of encrypted files")
		;

		po::options_description cmdOptions;
		cmdOptions.add(generic)
		          .add(params);

		po::options_description configOptions;
		configOptions.add(params);

        po::options_description visible("");
        visible.add(generic)
               .add(params);

		po::variables_map vm;        
        po::positional_options_description p;

		store(po::command_line_parser(argc, argv).
				options(cmdOptions).positional(p).run(), vm);
		notify(vm);

		if(vm.count("config"))
		{
			std::ifstream ifs(vm["config"].as<std::string>().c_str());
			if (!ifs)
			{
				std::cout << "Can not open config file: " << vm["config"].as<std::string>() << "\n";
				return 0;
			}
			else
			{
				store(parse_config_file(ifs, configOptions), vm);
				notify(vm);
			}
		}

		if (vm.count("help")) 
		{
			std::cout << visible << "\n";
			return 0;
		}

		if(!vm.count("aes_key") || !vm.count("decr_keys_config") || !vm.count("out_dir") || 
				!vm.count("hash_idx_size") || !vm.count("max_size"))
		{
			std::cout << visible << "\n";
			return 1;
		}

		std::vector<DecrKeyParams> decrParams = readDecrKeyParams(vm["decr_keys_config"].as<std::string>());
		KeyCreator keyCreator;

		std::vector<KeyParams> keys = keyCreator.createKeys(decrParams, vm["max_size"].as<std::size_t>() * 8,
			vm["hash_idx_size"].as<std::size_t>() / 2);

		std::ifstream aesKeyIn(vm["aes_key"].as<std::string>().c_str(), std::ios_base::binary | std::ios_base::in);
		if(!aesKeyIn.is_open())
		{
			std::cout << "problem with opening file `" << vm["aes_key"].as<std::string>() << "` for reading\n";
			return 1;
		}
		
		aesKeyIn.read((char *) keys[0].m_aesUserKey, 16);

		if(aesKeyIn.gcount() != 16)
		{
			std::cout << "problem with reading AES-128 key from file `" << vm["aes_key"].as<std::string>() << "`\n";
			return 1;
		}

		for(std::size_t i=1; i<keys.size(); i++)
			memcpy(keys[i].m_aesUserKey, keys[0].m_aesUserKey, 16);


		boost::filesystem::create_directory(vm["out_dir"].as<std::string>());


		for(std::size_t i=0; i<keys.size(); i++)
		{
			std::size_t keyBufferSize = KeyIO::calcBufferSize(keys[i]);
			unsigned char *keyBuffer = new unsigned char[keyBufferSize];

			KeyIO::writeKey(keyBuffer, keyBufferSize, keys[i]);

			std::ofstream keyOut((vm["out_dir"].as<std::string>() + "/" + keys[i].m_id).c_str(),
				std::ios_base::binary | std::ios_base::out);

			if(!keyOut.is_open())
			{
				std::cout << "problem with open file " + vm["out_dir"].as<std::string>() + "/" + keys[i].m_id +
					" for write key\n";

				return 1;
			}

			keyOut.write((const char *) keyBuffer, keyBufferSize);

			if(!keyOut.good())
			{
				std::cout << "problem with write key into " + vm["out_dir"].as<std::string>() + 
					"/" + keys[i].m_id + "\n";

				return 1;
			}

			delete [] keyBuffer;
		}

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
