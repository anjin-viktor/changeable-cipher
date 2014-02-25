#include <boost/test/unit_test.hpp>

#include <string>

#include "DecrKeyParams.h"


BOOST_AUTO_TEST_SUITE(decr_key_params_parsing);

std::string getSourceDir()
{
	std::string fileName = __FILE__;
	std::size_t pos = fileName.find_last_of("\\/");

	return fileName.substr(0, pos + 1);
}


BOOST_AUTO_TEST_CASE(config_1)
{
	std::vector<DecrKeyParams> keysParams = 
		readDecrKeyParams(getSourceDir() + "configs/1.xml");

	BOOST_CHECK_EQUAL(keysParams.size(), 2);

	BOOST_CHECK_EQUAL(keysParams[0].m_id, "id_1");
	BOOST_CHECK_EQUAL(keysParams[0].m_changes.size(), 2);
	BOOST_CHECK_EQUAL(keysParams[0].m_changes[0].m_pos, 1);
	BOOST_CHECK_EQUAL(keysParams[0].m_changes[0].m_val, 0);
	BOOST_CHECK_EQUAL(keysParams[0].m_changes[1].m_pos, 3);
	BOOST_CHECK_EQUAL(keysParams[0].m_changes[1].m_val, 1);

	BOOST_CHECK_EQUAL(keysParams[1].m_id, "id_2");
	BOOST_CHECK_EQUAL(keysParams[1].m_changes.size(), 4);
	BOOST_CHECK_EQUAL(keysParams[1].m_changes[0].m_pos, 4);
	BOOST_CHECK_EQUAL(keysParams[1].m_changes[0].m_val, 0);
	BOOST_CHECK_EQUAL(keysParams[1].m_changes[1].m_pos, 5);
	BOOST_CHECK_EQUAL(keysParams[1].m_changes[1].m_val, 1);
	BOOST_CHECK_EQUAL(keysParams[1].m_changes[2].m_pos, 8);
	BOOST_CHECK_EQUAL(keysParams[1].m_changes[2].m_val, 1);
	BOOST_CHECK_EQUAL(keysParams[1].m_changes[3].m_pos, 9);
	BOOST_CHECK_EQUAL(keysParams[1].m_changes[3].m_val, 1);



}

BOOST_AUTO_TEST_SUITE_END()