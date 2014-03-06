#include <boost/test/unit_test.hpp>

#include "Params.h"

#include "KeyCreator.h"
#include "KeyParams.h"
#include "DecrKeyParams.h"
#include "Markerator.h"


BOOST_AUTO_TEST_SUITE(key_creator_test);


BOOST_AUTO_TEST_CASE(test_1)
{
	std::vector<DecrKeyParams> decrParams = 
		readDecrKeyParams(getSourceDir() + "configs/1.xml");

	int keystreamSize = 1024;
	KeyCreator keyCreator;

	std::vector<KeyParams> keyParams = keyCreator.createKeys(decrParams, keystreamSize);


	Markerator markEnc(bcc::Function(keyParams[0].m_filterFunc), 
		LFSR(keyParams[0].m_lfsrFunc, keyParams[0].m_lfsrInitVect));


	Markerator markDec1(bcc::Function(keyParams[1].m_filterFunc), 
		LFSR(keyParams[1].m_lfsrFunc, keyParams[1].m_lfsrInitVect));

	Markerator markDec2(bcc::Function(keyParams[2].m_filterFunc), 
		LFSR(keyParams[2].m_lfsrFunc, keyParams[2].m_lfsrInitVect));


	for(std::size_t i=0; i<keystreamSize; i++)
	{
		bool encVal = markEnc.getNext();
		bool decVal1 = markDec1.getNext();
		bool decVal2 = markDec2.getNext();

		switch(i)
		{
			case 1:
				BOOST_CHECK_EQUAL(encVal, false);
				BOOST_CHECK_EQUAL(encVal, decVal2);
				BOOST_CHECK_EQUAL(decVal1, false);
				break;
			case 3:
				BOOST_CHECK_EQUAL(encVal, false);
				BOOST_CHECK_EQUAL(encVal, decVal2);
				BOOST_CHECK_EQUAL(decVal1, true);
				break;
			case 4:
				BOOST_CHECK_EQUAL(encVal, false);
				BOOST_CHECK_EQUAL(encVal, decVal1);
				BOOST_CHECK_EQUAL(decVal2, false);
				break;
			case 5:
				BOOST_CHECK_EQUAL(encVal, false);
				BOOST_CHECK_EQUAL(encVal, decVal1);
				BOOST_CHECK_EQUAL(decVal2, true);
				break;
			case 8:
				BOOST_CHECK_EQUAL(encVal, false);
				BOOST_CHECK_EQUAL(encVal, decVal1);
				BOOST_CHECK_EQUAL(decVal2, true);
				break;
			case 9:
				BOOST_CHECK_EQUAL(encVal, false);
				BOOST_CHECK_EQUAL(encVal, decVal1);
				BOOST_CHECK_EQUAL(decVal2, true);
				break;
			default:
				BOOST_CHECK_EQUAL(encVal, decVal2);
				BOOST_CHECK_EQUAL(encVal, decVal1);
				break;			
		};

	}
}

BOOST_AUTO_TEST_SUITE_END()