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
#include <stdexcept>
#include <algorithm>
#include <utility>
#include "codegen/StringProcessor.hpp"
#include "codegen/netlist/ComponentListing.hpp"

#include <iostream>

namespace lblmc
{

ComponentListing::ComponentListing() :
	type(),
	label(),
	parameters(),
	terminal_connections()
{}

ComponentListing::ComponentListing(const ComponentListing& base) :
	type(base.type),
	label(base.label),
	parameters(base.parameters),
	terminal_connections(base.terminal_connections)
{}

ComponentListing::ComponentListing(ComponentListing&& base) :
	type(std::move(base.type)),
	label(std::move(base.label)),
	parameters(std::move(base.parameters)),
	terminal_connections(std::move(base.terminal_connections))
{}

ComponentListing::ComponentListing
(
	std::string& type,
	std::string& label,
	std::vector<double>& parameters,
	std::vector<unsigned int>& terminal_connections
) :
	type(type),
	label(label),
	parameters(parameters),
	terminal_connections(terminal_connections)
{}

ComponentListing::ComponentListing
(
	std::string&& type,
	std::string&& label,
	std::vector<double>&& parameters,
	std::vector<unsigned int>&& terminal_connections
) :
	type(std::move(type)),
	label(std::move(label)),
	parameters(std::move(parameters)),
	terminal_connections(std::move(terminal_connections))
{}

ComponentListing::ComponentListing(const std::string& listing) :
	ComponentListing()
{
    setFromNetlistLine(listing);
}

ComponentListing& ComponentListing::operator=(const ComponentListing& base)
{
	type = base.type;
	label = base.label;
	parameters = base.parameters;
	terminal_connections = base.terminal_connections;
	return *this;
}

void ComponentListing::setFromNetlistLine(const std::string& l)
{
	const static std::string WHITESPACE_CHARS = std::string(" \n\r\t\f\v");
	const static std::string NUMBER_CHARS = WHITESPACE_CHARS+std::string("+-.1234567890eE");
	const static std::string INDEX_CHARS = WHITESPACE_CHARS+std::string("+1234567890");
	int pos_begin = 0;
	int pos_end = 0;
	int pos_mid = 0;
	std::string type_str;
	std::string label_str;
	std::string error_message;
	std::vector<double> parsed_parameters;
	std::vector<unsigned int> parsed_node_indices;

	auto hasTwoSpacedWords = [] (std::string& str)
	{
		int pos_begin = str.find_first_not_of(WHITESPACE_CHARS, 0);
		int pos_mid   = str.find_first_of(WHITESPACE_CHARS,pos_begin);
		int pos_end   = str.find_first_not_of(WHITESPACE_CHARS, pos_mid);

		return bool( !(pos_begin == std::string::npos || pos_mid == std::string::npos || pos_end == std::string::npos) );
	};

		//get component type

	pos_begin = l.find_first_not_of(WHITESPACE_CHARS, pos_begin);
	if(pos_begin == std::string::npos)
	{
		error_message =  "line is all whitespace";
		goto ERROR_THROW;
	}

	pos_end = l.find_first_of(WHITESPACE_CHARS,pos_begin);
	if(pos_end == std::string::npos)
	{
		error_message = "couldn't find end of component type";
		goto ERROR_THROW;
	}

	type_str = l.substr(pos_begin, pos_end-pos_begin);
	pos_begin = pos_end;

		//get component label

	pos_begin = l.find_first_not_of(WHITESPACE_CHARS, pos_begin);
	if(pos_begin == std::string::npos)
	{
		error_message = "couldn't find start of component label";
		goto ERROR_THROW;
	}

	pos_end = l.find_first_of(std::string("(")+WHITESPACE_CHARS, pos_begin);
	if(pos_end == std::string::npos)
	{
		error_message = "couldn't find end of component label";
		goto ERROR_THROW;
	}

	label_str = l.substr(pos_begin, pos_end-pos_begin);
	pos_begin = pos_end;

		//get component parameters

    pos_begin = l.find_first_of(std::string("("), pos_begin);
    if(pos_begin == std::string::npos)
	{
		error_message = "couldn't find start of parameters";
		goto ERROR_THROW;
	}

	while(true)
	{
		pos_begin++;

		pos_end = l.find_first_of(std::string(",)"), pos_begin);
		if(pos_end == std::string::npos)
		{
			error_message = "couldn't find end of parameter(s)";
			goto ERROR_THROW;
		}

		std::string word = std::move( l.substr(pos_begin,pos_end-pos_begin) );
		int word_pos = word.find_first_not_of(WHITESPACE_CHARS, 0);

		if( (word_pos == std::string::npos) || word.empty() )
		{
			if(l[pos_begin-1] == ',' || l[pos_end] == ',')
			{
				error_message = "extra comma ',' found while parsing parameters";
				goto ERROR_THROW;
			}

			break;
		}

		if(word.find_first_not_of(NUMBER_CHARS) != std::string::npos)
		{
			error_message = "extracted parameter is not a proper number.";
			goto ERROR_THROW;
		}

		if(hasTwoSpacedWords(word))
		{
			error_message = "there is a missing comma ',' in the parameters.";
			goto ERROR_THROW;
		}

		try
		{
			parsed_parameters.push_back( std::stod( word ) );
		}
		catch(...)
		{
			error_message = "couldn't parse a parameter as number";
			goto ERROR_THROW;
		}

		pos_begin = pos_end;

		if(l[pos_end] == ')')
		{
			break;
		}

	}

	//get component node indices for terminal connections

    pos_begin = l.find_first_of(std::string("{"), pos_begin);
    if(pos_begin == std::string::npos)
	{
		error_message = "couldn't find start of node indices";
		goto ERROR_THROW;
	}

	while(true)
	{
		pos_begin++;

		pos_end = l.find_first_of(std::string(",}"), pos_begin);
		if(pos_end == std::string::npos)
		{
			error_message = "couldn't find end of node index/indices";
			goto ERROR_THROW;
		}

		std::string word = std::move( l.substr(pos_begin,pos_end-pos_begin) );
		int word_pos = word.find_first_not_of(WHITESPACE_CHARS, 0);

		if( (word_pos == std::string::npos) || word.empty() )
		{
			if(l[pos_begin-1] == ',' || l[pos_end] == ',')
			{
				error_message = "extra comma ',' found while parsing indices";
				goto ERROR_THROW;
			}

			break;
		}

		if(word.find_first_not_of(INDEX_CHARS) != std::string::npos)
		{
			error_message = "extracted index is not a positive integer number";
			goto ERROR_THROW;
		}

		if(hasTwoSpacedWords(word))
		{
			error_message = "there is a missing comma ',' in the indices";
			goto ERROR_THROW;
		}

		try
		{
			parsed_node_indices.push_back( std::stoul( word ) );
		}
		catch(...)
		{
			error_message = "couldn't parse a node index as a number";
			goto ERROR_THROW;
		}

		pos_begin = pos_end;

		if(l[pos_end] == '}')
		{
			break;
		}
	}

    type = type_str;
    label = label_str;
    parameters = std::move(parsed_parameters);
    terminal_connections = std::move(parsed_node_indices);

    return;

	ERROR_THROW: //go here to throw error since line is not valid syntax

	throw std::invalid_argument( std::string("ComponentListing::setFromNetlistLine(*) -- syntax error: ")+error_message );
}

inline
void ComponentListing::setType(std::string t)
{
	type = t;
}

inline
void ComponentListing::setLabel(std::string l)
{
	label = l;
}

inline
void ComponentListing::setParameters(const std::vector<double>& p)
{
	parameters = p;
}

inline
void ComponentListing::setParameters(std::vector<double>&& p)
{
	parameters = p;
}

inline
void ComponentListing::addParameter(double p)
{
	parameters.push_back(p);
}

inline
void ComponentListing::setTerminalConnections(const std::vector<unsigned int>& tc)
{
	terminal_connections = tc;
}

inline
void ComponentListing::setTerminalConnections(std::vector<unsigned int>&& tc)
{
	terminal_connections = tc;
}

inline
void ComponentListing::addTerminalConnection(unsigned int tc)
{
	terminal_connections.push_back(tc);
}

const std::string& ComponentListing::getType() const
{
	return type;
}

const std::string& ComponentListing::getLabel() const
{
	return label;
}

double ComponentListing::getParameter(unsigned int p) const
{
	return parameters.at(p);
}

const std::vector<double>& ComponentListing::getParameters() const
{
	return parameters;
}

unsigned int ComponentListing::getParametersCount() const
{
	return parameters.size();
}

unsigned int ComponentListing::getTerminalConnection(unsigned int tc) const
{
	return terminal_connections.at(tc);
}

const std::vector<unsigned int>& ComponentListing::getTerminalConnections() const
{
	return terminal_connections;
}

unsigned int ComponentListing::getTerminalConnectionsCount() const
{
	return terminal_connections.size();
}

} //namespace lblmc
