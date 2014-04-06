#include <boost/test/unit_test.hpp>

#include <string>

#include "Params.h"

#include "DecrKeyParams.h"


BOOST_AUTO_TEST_SUITE(decr_key_params_parsing);


BOOST_AUTO_TEST_CASE(config_1)
{
	std::vector<DecrKeyParams> keysParams = 
		readDecrKeyParams(getSourceDir() + "configs/1.xml");

	BOOST_CHECK_EQUAL(keysParams.size(), 2);

	BOOST_CHECK_EQUAL(keysParams[0].m_id, "id_1");
	BOOST_CHECK_EQUAL(keysParams[0].m_changes.size(), 2);
	BOOST_CHECK_EQUAL(keysParams[0].m_changes[0], 1);
	BOOST_CHECK_EQUAL(keysParams[0].m_changes[1], 3);

	BOOST_CHECK_EQUAL(keysParams[1].m_id, "id_2");
	BOOST_CHECK_EQUAL(keysParams[1].m_changes.size(), 4);
	BOOST_CHECK_EQUAL(keysParams[1].m_changes[0], 4);
	BOOST_CHECK_EQUAL(keysParams[1].m_changes[1], 5);
	BOOST_CHECK_EQUAL(keysParams[1].m_changes[2], 8);
	BOOST_CHECK_EQUAL(keysParams[1].m_changes[3], 9);



}

BOOST_AUTO_TEST_SUITE_END()