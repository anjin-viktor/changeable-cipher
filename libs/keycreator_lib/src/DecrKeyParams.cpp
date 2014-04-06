#include "DecrKeyParams.h"

#include <fstream>

#include <boost/property_tree/xml_parser.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/foreach.hpp>

std::vector<DecrKeyParams> readDecrKeyParams(const std::string &fileName)
{
	using namespace boost::property_tree;

	std::ifstream inStream(fileName.c_str());
	if(!inStream.good())
		throw std::runtime_error("problem with opening file `" + fileName + "` for reading");

	ptree pt;

	try
	{
		xml_parser::read_xml(inStream, pt, xml_parser::trim_whitespace);
	}
	catch(xml_parser_error err)
	{
		throw std::runtime_error("problem with parsing xml file `" + fileName + "`");
	}

	std::vector<DecrKeyParams> result;

	BOOST_FOREACH(ptree::value_type &v, pt)
	{
		DecrKeyParams keyParams;
		if(v.first == "key")
		{
			keyParams.m_id = v.second.get<std::string>("<xmlattr>.id", "");

			BOOST_FOREACH(ptree::value_type &change, v.second)
			{
				if(change.first == "change")
				{
					if(change.second.get_optional<std::size_t>("<xmlattr>.position"))
					{
						keyParams.m_changes.push_back(change.second.get<std::size_t>("<xmlattr>.position"));
					}
				}
			}
			result.push_back(keyParams);
		}
	}

	return result;
}
