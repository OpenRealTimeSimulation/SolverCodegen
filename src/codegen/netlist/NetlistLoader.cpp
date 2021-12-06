/*

Copyright (C) 2019-2020 Matthew Milton

This file is part of the LB-LMC Solver C++ Code Generation Library, included in the
Open Real-Time Simulation (ORTiS) Framework.

LB-LMC Solver C++ Code Generation Library is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

LB-LMC Solver C++ Code Generation Library is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with LB-LMC Solver C++ Code Generation Library.  If not, see <https://www.gnu.org/licenses/>.

*/

#include <string>
#include <sstream>
#include <fstream>
#include <stdexcept>
#include <utility>
#include <map>

#include "codegen/netlist/Netlist.hpp"
#include "codegen/netlist/ComponentListing.hpp"
#include "codegen/netlist/NetlistLoader.hpp"
#include "codegen/StringProcessor.hpp"

namespace lblmc
{

const std::string NetlistLoader::WHITESPACE_CHARS = std::string(" \n\r\t\f\v");
const std::string NetlistLoader::BAD_START_CHARS  = std::string("1234567890.!@$^&*()-+=\\|;:\"'<>,?/~`[]{}");
const std::string NetlistLoader::VALID_NAME_CHARS = std::string("1234567890_abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ");
const std::string NetlistLoader::VALID_NUMBER_CHARS = std::string("1234567890.eE+-");

NetlistLoader::NetlistLoader() {}

Netlist NetlistLoader::loadFromStream(std::istream& strm)
{
	std::string line;
	std::string constant_name;
	std::string constant_value;
	int line_count = 0;
	int model_name_count = 0;
	size_t line_pos = 0;
	Netlist netlist;
	ComponentListing component;
	std::map<std::string, std::string> constants{};

	while( std::getline(strm, line) )
	{
		++line_count;
		LineType line_type = checkLineType(line, line_pos);

		switch(line_type)
		{
			case LineType::ERROR :
			{
				throw std::invalid_argument(std::string("NetlistLoader::loadFromStream(*) -- unsupported syntax at line ")+std::to_string(line_count));
				break;
			}
			case LineType::LINE_START_ERROR :
			{
				throw std::invalid_argument(std::string("NetlistLoader::loadFromStream(*) -- line starts with unsupported sequence, character, or command at line ")+std::to_string(line_count));
				break;
			}
			case LineType::EMPTY :
			{
				break;
			}
			case LineType::COMMENT :
				break;

			case LineType::NAME :
				++model_name_count;
				if(model_name_count > 1)
				{
					throw std::invalid_argument(std::string("NetlistLoader::loadFromStream(*) -- redefined model name at line ")+std::to_string(line_count));
				}
				try
				{
					netlist.setModelName(extractModelName(line, line_pos));
				}
				catch(std::invalid_argument& e)
				{
					throw std::invalid_argument(std::string("NetlistLoader::loadFromStream(*) -- model name error at line ")
													+std::to_string(line_count)+std::string(": ")+e.what());
				}
				break;

			case LineType::CONSTANT :
				constant_value = extractConstantValue(line, line_pos, constant_name);
				if(constants.find(constant_name) != constants.end())
				{
					throw std::invalid_argument(std::string("NetlistLoader::loadFromStream(*) -- redefined constant at line ")+std::to_string(line_count));
				}
				constants[constant_name] = constant_value;
				break;

			case LineType::COMPONENT :
				component = extractComponent(line, constants);
				if(netlist.hasComponent(component.getLabel()))
				{
					throw std::invalid_argument(std::string("NetlistLoader::loadFromStream(*) -- redefined component with same label at line ")+std::to_string(line_count));
				}
				netlist.addComponent(std::move(component));
				break;

			default:
				break;
		}
	}

	if(model_name_count < 1)
	{
		throw std::invalid_argument(std::string("NetlistLoader::loadFromStream(*) -- model name not defined"));
	}

	return netlist;
}

Netlist NetlistLoader::loadFromString(const std::string& netlist_str)
{
	std::stringstream sstrm(netlist_str, std::stringstream::in);

	try
	{
		return loadFromStream(sstrm);
	}
	catch(const std::invalid_argument& e)
	{
		throw std::invalid_argument
		(
			std::string("NetlistLoader::loadFromString(*) -- error occurred during netlist load: ")+
			e.what()
		);
	}
}

Netlist NetlistLoader::loadFromFile(const std::string& filename)
{
	std::fstream file(filename, std::fstream::in);

	if(!file.is_open())
	{
		throw std::runtime_error("NetlistLoader::loadFromFile(*) -- failed to open given file");
	}

	try
	{
		return loadFromStream(file);
	}
	catch(const std::invalid_argument& e)
	{
		throw std::invalid_argument
		(
			std::string("NetlistLoader::loadFromFile(*) -- error occurred during netlist load: ")+
			e.what()
		);
	}
}

NetlistLoader::LineType NetlistLoader::checkLineType(const std::string& line, size_t& line_pos)
{
	size_t pos_begin = line.find_first_not_of(WHITESPACE_CHARS, 0);
	size_t pos_bad  = line.find_first_of(BAD_START_CHARS, 0);
	size_t pos_end   = 0;

	if(pos_begin == std::string::npos)
	{
		line_pos = 0;
		return LineType::EMPTY;
	}

	if(pos_begin == pos_bad)
	{
		line_pos = pos_begin;
		return LineType::LINE_START_ERROR;
	}

	char first_char = line[pos_begin];
	switch (first_char)
	{
		case '%' :
		{
			line_pos = pos_begin;
			return LineType::COMMENT;
			break;
		}
		case '#':
		{
			pos_end = line.find_first_of(WHITESPACE_CHARS,pos_begin);
			std::string word = std::move( line.substr(pos_begin,pos_end-pos_begin) );

			if(word == std::string("#const"))
			{
				line_pos = pos_end;
				return LineType::CONSTANT;
			}
			else if(word == std::string("#name"))
			{
				line_pos = pos_end;
				return LineType::NAME;
			}
			else
			{
				line_pos = pos_end;
				return LineType::ERROR;
			}

			break;
		}
		default :
		{
			break;
		}
	}

	line_pos = 0;
	return LineType::COMPONENT;
}

std::string NetlistLoader::extractModelName(const std::string& line, const size_t& line_pos)
{
	std::string model_name;

	size_t pos_begin = line.find_first_not_of(WHITESPACE_CHARS,line_pos);
	if(pos_begin == std::string::npos)
	{
		throw std::invalid_argument("NetlistLoader::extractModelName(*) -- missing model name in #name command");
	}

	size_t pos_end  = line.find_first_of(WHITESPACE_CHARS,pos_begin);
	size_t pos_end2 = line.find_first_not_of(WHITESPACE_CHARS,pos_end);
	if(pos_end2 != std::string::npos)
	{
		throw std::invalid_argument("NetlistLoader::extractModelName(*) -- whitespace found in middle of model name");
	}

	model_name = std::move(line.substr(pos_begin, pos_end-pos_begin));

	if(model_name.find_first_not_of(VALID_NAME_CHARS,0) != std::string::npos)
	{
		throw std::invalid_argument("NetlistLoader::extractModelName(*) -- model name has invalid characters");
	}

	if(BAD_START_CHARS.find_first_of( model_name.substr(0,1) ) != std::string::npos)
	{
		throw std::invalid_argument("NetlistLoader::extractModelName(*) -- model name starts with invalid character");
	}

	return model_name;
}

std::string NetlistLoader::extractConstantValue(const std::string& line, const size_t& line_pos, std::string& name)
{
	std::string constant_name;
	std::string value;

    size_t pos_begin = line.find_first_not_of(WHITESPACE_CHARS, line_pos);
    if(pos_begin == std::string::npos)
	{
		throw std::invalid_argument("NetlistLoader::extractConstantValue(*) -- missing constant definition in #const command");
	}

	size_t pos_end = line.find_first_of(WHITESPACE_CHARS, pos_begin);
	if(pos_end == std::string::npos)
	{
		throw std::invalid_argument("NetlistLoader::extractConstantValue(*) -- cannot find whitespace at end of constant name or constant value is missing");
	}

	constant_name = std::move(line.substr(pos_begin, pos_end-pos_begin));

	if(constant_name.find_first_not_of(VALID_NAME_CHARS,0) != std::string::npos)
	{
		throw std::invalid_argument("NetlistLoader::extractConstantValue(*) -- constant name has invalid characters");
	}

	if(BAD_START_CHARS.find_first_of( constant_name.substr(0,1) ) != std::string::npos)
	{
		throw std::invalid_argument("NetlistLoader::extractConstantValue(*) -- constant name starts with invalid character");
	}

	pos_begin = line.find_first_not_of(WHITESPACE_CHARS, pos_end);
	if(pos_begin == std::string::npos)
	{
		throw std::invalid_argument("NetlistLoader::extractConstantValue(*) -- missing constant value");
	}

	pos_end = line.find_first_of(WHITESPACE_CHARS, pos_begin);
	size_t pos_end2 = line.find_first_not_of(WHITESPACE_CHARS, pos_end);
	if(pos_end2 != std::string::npos)
	{
		throw std::invalid_argument("NetlistLoader::extractConstantValue(*) -- whitespace found in middle of constant value");
	}

	value = std::move(line.substr(pos_begin, pos_end-pos_begin));

	name = std::move(constant_name);
	return value;
}

ComponentListing NetlistLoader::extractComponent(const std::string& line, const std::map<std::string,std::string>& constants)
{
	std::string str_listing = line;
	size_t start_pos = str_listing.find_first_of("(",0);
	lblmc::StringProcessor sproc(str_listing);

	for(const auto& name_value : constants)
	{
		sproc.replaceWordAll( name_value.first, name_value.second, start_pos );
	}

	try
	{
		return ComponentListing(str_listing);
	}
	catch(const std::invalid_argument& e)
	{
		throw std::invalid_argument( std::string("NetlistLoader::extractComponent(*) -- given component listing has invalid syntax: ")+e.what() );
	}
}

} //namespace lblmc


