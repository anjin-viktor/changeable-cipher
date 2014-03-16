#include <boost/test/unit_test.hpp>

#include "Params.h"

#include "HashTable.h"
#include "DisForm.h"

#include <boost/dynamic_bitset.hpp>

BOOST_AUTO_TEST_SUITE(hash_table_test);

BOOST_AUTO_TEST_CASE(test_1)
{
	Conjunct c, mergedConj, compareConj;
	c.m_pos = boost::dynamic_bitset<>(std::string("0010101001110010"));
	c.m_neg = boost::dynamic_bitset<>(std::string("1101010110001101"));

	HashTable table(10);

	bool res = table.merge(c, mergedConj);
	BOOST_CHECK_EQUAL(res, true);
	BOOST_CHECK_EQUAL(c.m_pos, mergedConj.m_pos);
	BOOST_CHECK_EQUAL(c.m_neg, mergedConj.m_neg);


	c.m_pos = boost::dynamic_bitset<>(std::string("0011101001110010"));
	c.m_neg = boost::dynamic_bitset<>(std::string("1100010110001101"));

	compareConj.m_pos = boost::dynamic_bitset<>(std::string("0010101001110010"));
	compareConj.m_neg = boost::dynamic_bitset<>(std::string("1100010110001101"));
	res = table.merge(c, mergedConj);
	BOOST_CHECK_EQUAL(res, false);
	BOOST_CHECK_EQUAL(mergedConj.m_pos, compareConj.m_pos);
	BOOST_CHECK_EQUAL(mergedConj.m_neg, compareConj.m_neg);


	c = mergedConj;
	res = table.merge(c, mergedConj);
	BOOST_CHECK_EQUAL(res, true);
	BOOST_CHECK_EQUAL(mergedConj.m_pos, compareConj.m_pos);
	BOOST_CHECK_EQUAL(mergedConj.m_neg, compareConj.m_neg);



	c.m_pos = boost::dynamic_bitset<>(std::string("0000101001110010"));
	c.m_neg = boost::dynamic_bitset<>(std::string("1111010110001101"));

	compareConj.m_pos = boost::dynamic_bitset<>(std::string("0000101001110010"));
	compareConj.m_neg = boost::dynamic_bitset<>(std::string("1101010110001101"));
	res = table.merge(c, mergedConj);
	BOOST_CHECK_EQUAL(res, false);
	BOOST_CHECK_EQUAL(mergedConj.m_pos, compareConj.m_pos);
	BOOST_CHECK_EQUAL(mergedConj.m_neg, compareConj.m_neg);


	c = mergedConj;
	res = table.merge(c, mergedConj);
	BOOST_CHECK_EQUAL(res, true);
	BOOST_CHECK_EQUAL(mergedConj.m_pos, compareConj.m_pos);
	BOOST_CHECK_EQUAL(mergedConj.m_neg, compareConj.m_neg);


	c.m_pos = boost::dynamic_bitset<>(std::string("0001101001110010"));
	c.m_neg = boost::dynamic_bitset<>(std::string("1110010110001101"));

	compareConj.m_pos = boost::dynamic_bitset<>(std::string("0000101001110010"));
	compareConj.m_neg = boost::dynamic_bitset<>(std::string("1110010110001101"));
	res = table.merge(c, mergedConj);
	BOOST_CHECK_EQUAL(res, false);


	c = mergedConj;
	compareConj.m_pos = boost::dynamic_bitset<>(std::string("0000101001110010"));
	compareConj.m_neg = boost::dynamic_bitset<>(std::string("1100010110001101"));

	res = table.merge(c, mergedConj);
	BOOST_CHECK_EQUAL(res, false);
	BOOST_CHECK_EQUAL(mergedConj.m_pos, compareConj.m_pos);
	BOOST_CHECK_EQUAL(mergedConj.m_neg, compareConj.m_neg);


	c.m_pos = boost::dynamic_bitset<>(std::string("1110101111000101"));
	c.m_neg = boost::dynamic_bitset<>(std::string("0001010000111010"));

	res = table.merge(c, mergedConj);
	BOOST_CHECK_EQUAL(res, true);
	BOOST_CHECK_EQUAL(mergedConj.m_pos, c.m_pos);
	BOOST_CHECK_EQUAL(mergedConj.m_neg, c.m_neg);

	c.m_pos = boost::dynamic_bitset<>(std::string("0011101000110010"));
	c.m_neg = boost::dynamic_bitset<>(std::string("1100010111001101"));

	compareConj.m_pos = boost::dynamic_bitset<>(std::string("0011101000110010"));
	compareConj.m_neg = boost::dynamic_bitset<>(std::string("1100010110001101"));
	res = table.merge(c, mergedConj);
	BOOST_CHECK_EQUAL(res, false);
	BOOST_CHECK_EQUAL(mergedConj.m_pos, compareConj.m_pos);
	BOOST_CHECK_EQUAL(mergedConj.m_neg, compareConj.m_neg);

	c = mergedConj;
	res = table.merge(c, mergedConj);
	BOOST_CHECK_EQUAL(res, true);
	BOOST_CHECK_EQUAL(mergedConj.m_pos, c.m_pos);
	BOOST_CHECK_EQUAL(mergedConj.m_neg, c.m_neg);

	DisForm df = table.get();

	BOOST_CHECK_EQUAL(df.m_conjuncts.size(), 4);
}


BOOST_AUTO_TEST_SUITE_END();

