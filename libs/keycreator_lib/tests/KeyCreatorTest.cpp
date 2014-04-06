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


	Markerator markDec(bcc::Function(keyParams[1].m_filterFunc), 
		LFSR(keyParams[1].m_lfsrFunc, keyParams[1].m_lfsrInitVect));


	for(std::size_t i=0; i<keystreamSize; i++)
	{
		bool encVal = markEnc.getNext();
		bool decVal = markDec.getNext();

		switch(i)
		{
			case 1:
				BOOST_CHECK_EQUAL(encVal, true);
				BOOST_CHECK_EQUAL(decVal, false);
				break;
			case 3:
				BOOST_CHECK_EQUAL(encVal, true);
				BOOST_CHECK_EQUAL(decVal, false);
				break;
			case 4:
				BOOST_CHECK_EQUAL(encVal, false);
				BOOST_CHECK_EQUAL(decVal, true);
				break;
			case 5:
				BOOST_CHECK_EQUAL(encVal, false);
				BOOST_CHECK_EQUAL(decVal, true);
				break;
			case 8:
				BOOST_CHECK_EQUAL(encVal, false);
				BOOST_CHECK_EQUAL(decVal, true);
				break;
			case 9:
				BOOST_CHECK_EQUAL(encVal, false);
				BOOST_CHECK_EQUAL(decVal, true);
				break;
			default:
				BOOST_CHECK_EQUAL(encVal, false);
				BOOST_CHECK_EQUAL(decVal, false);
				break;			
		};
	}
}




BOOST_AUTO_TEST_CASE(test_2)
{
	std::vector<DecrKeyParams> decrParams = 
		readDecrKeyParams(getSourceDir() + "configs/2.xml");

	int keystreamSize = 65536;
	KeyCreator keyCreator;

	std::vector<KeyParams> keyParams = keyCreator.createKeys(decrParams, keystreamSize);


	Markerator markEnc(bcc::Function(keyParams[0].m_filterFunc), 
		LFSR(keyParams[0].m_lfsrFunc, keyParams[0].m_lfsrInitVect));


	Markerator markDec(bcc::Function(keyParams[1].m_filterFunc), 
		LFSR(keyParams[1].m_lfsrFunc, keyParams[1].m_lfsrInitVect));


	for(std::size_t i=0; i<keystreamSize; i++)
	{
		bool encVal = markEnc.getNext();
		bool decVal = markDec.getNext();

		switch(i)
		{
			case 2415:
				BOOST_CHECK_EQUAL(encVal, false);
				BOOST_CHECK_EQUAL(decVal, true);
				break;
			case 27045:
				BOOST_CHECK_EQUAL(encVal, false);
				BOOST_CHECK_EQUAL(decVal, true);
				break;
			case 3930:
				BOOST_CHECK_EQUAL(encVal, false);
				BOOST_CHECK_EQUAL(decVal, true);
				break;
			case 27834:
				BOOST_CHECK_EQUAL(encVal, false);
				BOOST_CHECK_EQUAL(decVal, true);
				break;
			case 14830:
				BOOST_CHECK_EQUAL(encVal, false);
				BOOST_CHECK_EQUAL(decVal, true);
				break;
			case 18926:
				BOOST_CHECK_EQUAL(encVal, false);
				BOOST_CHECK_EQUAL(decVal, true);
				break;
			case 22780:
				BOOST_CHECK_EQUAL(encVal, false);
				BOOST_CHECK_EQUAL(decVal, true);
				break;
			case 16649:
				BOOST_CHECK_EQUAL(encVal, false);
				BOOST_CHECK_EQUAL(decVal, true);
				break;
			case 16139:
				BOOST_CHECK_EQUAL(encVal, false);
				BOOST_CHECK_EQUAL(decVal, true);
				break;
			case 31259:
				BOOST_CHECK_EQUAL(encVal, false);
				BOOST_CHECK_EQUAL(decVal, true);
				break;
			case 1799:
				BOOST_CHECK_EQUAL(encVal, false);
				BOOST_CHECK_EQUAL(decVal, true);
				break;
			case 26773:
				BOOST_CHECK_EQUAL(encVal, false);
				BOOST_CHECK_EQUAL(decVal, true);
				break;
			case 6751:
				BOOST_CHECK_EQUAL(encVal, false);
				BOOST_CHECK_EQUAL(decVal, true);
				break;
			case 23501:
				BOOST_CHECK_EQUAL(encVal, false);
				BOOST_CHECK_EQUAL(decVal, true);
				break;
			case 27758:
				BOOST_CHECK_EQUAL(encVal, false);
				BOOST_CHECK_EQUAL(decVal, true);
				break;
			case 16240:
				BOOST_CHECK_EQUAL(encVal, false);
				BOOST_CHECK_EQUAL(decVal, true);
				break;
			case 10799:
				BOOST_CHECK_EQUAL(encVal, false);
				BOOST_CHECK_EQUAL(decVal, true);
				break;
			case 30376:
				BOOST_CHECK_EQUAL(encVal, false);
				BOOST_CHECK_EQUAL(decVal, true);
				break;
			case 14962:
				BOOST_CHECK_EQUAL(encVal, false);
				BOOST_CHECK_EQUAL(decVal, true);
				break;
			case 14626:
				BOOST_CHECK_EQUAL(encVal, false);
				BOOST_CHECK_EQUAL(decVal, true);
				break;
			case 12312:
				BOOST_CHECK_EQUAL(encVal, false);
				BOOST_CHECK_EQUAL(decVal, true);
				break;
			case 25753:
				BOOST_CHECK_EQUAL(encVal, false);
				BOOST_CHECK_EQUAL(decVal, true);
				break;
			case 2574:
				BOOST_CHECK_EQUAL(encVal, false);
				BOOST_CHECK_EQUAL(decVal, true);
				break;
			case 4767:
				BOOST_CHECK_EQUAL(encVal, false);
				BOOST_CHECK_EQUAL(decVal, true);
				break;		
			case 4546:
				BOOST_CHECK_EQUAL(encVal, false);
				BOOST_CHECK_EQUAL(decVal, true);
				break;
				
			default:
				BOOST_CHECK_EQUAL(encVal, false);
				BOOST_CHECK_EQUAL(decVal, false);
				break;
			
		  
		};
	}
}

BOOST_AUTO_TEST_SUITE_END()