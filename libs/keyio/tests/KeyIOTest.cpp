#include <boost/test/unit_test.hpp>

#include "Params.h"

#include "KeyIO.h"
#include "KeyParams.h"

#include <iostream>
#include <fstream>

BOOST_AUTO_TEST_SUITE(key_io_test);


BOOST_AUTO_TEST_CASE(test_read_1)
{
	char data[4096];
	std::ifstream in(getSourceDir() + "keys/key1", std::ios_base::in | std::ios_base::binary);
	in.read(data, 4096);

	KeyParams  key = KeyIO::readKey((const unsigned char *) data, in.gcount());

	BOOST_CHECK_EQUAL(key.m_filterFunc, "x1 & x0 & !x2 | x0 & !x1 | x2 & x1 & x0");
	BOOST_CHECK_EQUAL(key.m_lfsrFunc, "x0 + x1 + x2 & !x0 + 1");
	BOOST_CHECK_EQUAL(key.m_lfsrInitVect, boost::dynamic_bitset<>(std::string("011")));

	unsigned char userKey[16] = {85, 85, 85, 85, 85, 85, 85, 85, 85, 85, 85, 85, 85, 85, 85, 85};
	BOOST_CHECK_EQUAL_COLLECTIONS(userKey, userKey + 16, key.m_aesUserKey, key.m_aesUserKey + 16);
	BOOST_CHECK_EQUAL(key.m_id, "This is a key identificator");

}

BOOST_AUTO_TEST_SUITE_END()