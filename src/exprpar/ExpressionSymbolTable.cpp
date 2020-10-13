/*

Copyright (C) 2020 Matthew Milton

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

#include "exprpar/ExpressionSymbolTable.hpp"

#include <string>
#include <map>
#include <utility>
#include <stdexcept>

namespace ortis
{

ExpressionSymbolTable::ExpressionSymbolTable()
	: symbol_value_assignments()
{}

ExpressionSymbolTable::ExpressionSymbolTable
(
	std::map<std::string, double>&& symbol_value_assignments
)
	: symbol_value_assignments(std::move(symbol_value_assignments))
{}

ExpressionSymbolTable::ExpressionSymbolTable(const ExpressionSymbolTable& base)
	: symbol_value_assignments(base.symbol_value_assignments)
{}

ExpressionSymbolTable::ExpressionSymbolTable(ExpressionSymbolTable&& base)
	: symbol_value_assignments(std::move(base.symbol_value_assignments))
{}

void
ExpressionSymbolTable::addSymbol(const std::string& symbol, double value)
{
	if(symbol.empty())
	{
		throw
		std::invalid_argument
		(
			"ExpressionSymbolTable::addSymbol(const std::string& symbol, double value) --"
			"symbol cannot be empty"
		);
	}

	symbol_value_assignments[symbol] = value;
}

void
ExpressionSymbolTable::addSymbol(const SymbolValuePair& symbol_value)
{
	if(symbol_value.first.empty())
	{
		throw
		std::invalid_argument
		(
			"ExpressionSymbolTable::addSymbol(const SymbolValuePair& symbol_value) --"
			"symbol cannot be empty"
		);
	}

	symbol_value_assignments[symbol_value.first] = symbol_value.second;
}

double
ExpressionSymbolTable::getSymbolValue(const std::string& symbol) const
{
	try
	{
		return symbol_value_assignments.at(symbol);
	}
	catch(const std::out_of_range& error)
	{
		throw
		std::out_of_range
		(
			std::string("ExpressionSymbolTable::getSymbolValue(const std::string& symbol) const --")
			+ error.what()
		);
	}
}

double
ExpressionSymbolTable::getSymbolValue
(
	const std::string& symbol,
	const SymbolValueMap& symbol_value_map
)
{
	try
	{
		return symbol_value_map.at(symbol);
	}
	catch(const std::out_of_range& error)
	{
		throw
		std::out_of_range
		(
			std::string("ExpressionSymbolTable::getSymbolValue(const std::string& symbol) const --")
			+ error.what()
		);
	}
}

bool
ExpressionSymbolTable::containsSymbol(const std::string& symbol) const
{
	return ( symbol_value_assignments.count(symbol) != 0  );
}

bool
ExpressionSymbolTable::containsSymbol
(
	const std::string& symbol,
	const SymbolValueMap& symbol_value_map
)
{
	return ( symbol_value_map.count(symbol) != 0  );
}

const double*
ExpressionSymbolTable::findSymbolValue(const std::string& symbol) const
{
	if(symbol_value_assignments.count(symbol) == 0)
	{
		return nullptr;
	}

	return &(symbol_value_assignments.at(symbol));
}

const double*
ExpressionSymbolTable::findSymbolValue(const std::string& symbol, const SymbolValueMap& symbol_value_map)
{
	if(symbol_value_map.count(symbol) == 0)
	{
		return nullptr;
	}

	return &(symbol_value_map.at(symbol));
}

} //namespace ortis
