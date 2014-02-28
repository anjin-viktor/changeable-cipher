#include <boost/test/unit_test.hpp>

#include "Params.h"

#include "KeyCreator.h"
#include "KeyParams.h"
#include "DecrKeyParams.h"

#include <iostream>

BOOST_AUTO_TEST_SUITE(key_creator_test);

BOOST_AUTO_TEST_CASE(test_1)
{
	std::vector<DecrKeyParams> decrParams = 
		readDecrKeyParams(getSourceDir() + "configs/1.xml");

	std::vector<std::size_t> chagedPositions {1, 3, 4, 5, 8, 9};
	KeyCreator keyCreator;
	std::vector<KeyParams> keyParams = keyCreator.createKeys(chagedPositions, decrParams, 65536);

	std::cerr << keyParams[0].m_filterFunc << std::endl << keyParams[0].m_lfsrFunc << std::endl;

}

BOOST_AUTO_TEST_SUITE_END()