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

#ifndef EXPRPAR_EXPRESSIONSYMBOLTABLE_HPP
#define EXPRPAR_EXPRESSIONSYMBOLTABLE_HPP

#include <map>
#include <string>

namespace ortis
{

/**
	\brief maps expression variable symbols to values

	\author Matthew Milton

	\date Created July 23, 2020
**/
class ExpressionSymbolTable
{
public:

	typedef std::map<std::string, double> SymbolValueMap; ///< map type that maps symbols to values
	typedef std::map<std::string, double>::value_type SymbolValuePair; ///< symbol-value pairing

private:

	SymbolValueMap symbol_value_assignments; ///< mapping of variable symbol names to values

public:

	/**
		\brief default constructor
	**/
    ExpressionSymbolTable();

    /**
		\brief constructs table from given symbol-value map

		The contents of the given map will be moved to constructed table.
    **/
    ExpressionSymbolTable(SymbolValueMap&& symbol_value_assignments);

    /**
		\brief copy constructor
    **/
    ExpressionSymbolTable(const ExpressionSymbolTable& base);

    /**
		\brief move constructor

		The contents of the given base will be moved to constructed table.
    **/
    ExpressionSymbolTable(ExpressionSymbolTable&& base);

    /**
		\brief gets SymbolValueMap within table
    **/
    inline
    const SymbolValueMap&
    getSymbolValueMap() const
    {
    	return symbol_value_assignments;
    }

    /**
		\brief adds new symbol-value mapping into the table

		If given symbol already exists in table, then the existing symbol is replaced in table with
		new value

		\param symbol symbol variable to be added/modified

		\param value value to assign to the symbol
    **/
    void
    addSymbol(const std::string& symbol, double value);

    /**
		\brief adds new symbol-value mapping into the table

		If given symbol-value pair already exists in table, then the existing pair is replaced in
		table with new pair

		\param symbol_value symbol-value pair to add/modify in table

    **/
    void
    addSymbol(const SymbolValuePair& symbol_value);

    /**
		\return value assigned to given symbol name

		\throw std::out_of_range if symbol does not exist in table
    **/
    double
    getSymbolValue(const std::string& symbol) const;

    /**
		\return value assigned to given symbol name

		\param symbol_value_map SymbolValueMap containing symbol from which of to get value

		\throw std::out_of_range if symbol does not exist in table
    **/
    static
    double
    getSymbolValue(const std::string& symbol, const SymbolValueMap& symbol_value_map);

    /**
		\return true if symbol exists in table; false otherwise
    **/
    bool
    containsSymbol(const std::string& symbol) const;

     /**
		\return true if symbol exists in table; false otherwise

		\param symbol_value_map SymbolValueMap containing symbol from which of to check
    **/
    static
    bool
    containsSymbol(const std::string& symbol, const SymbolValueMap& symbol_value_map);


	/**
		\brief finds value assigned to given symbol in table

		\param symbol symbol whose value is to be found

		\return observing raw pointer to value assigned to given symbol.
		If symbol does not exist in table, then nullptr is returned.
		The pointed to value will persist as long as table persists.
	**/
    const double*
    findSymbolValue(const std::string& symbol) const;

    /**
		\brief finds value assigned to given symbol in given symbol-value map

		\param symbol symbol whose value is to be found

		\param symbol_value_map symbol-value map to search through.

		\return observing raw pointer to value assigned to given symbol.
		If symbol does not exist in table, then nullptr is returned.
		The pointed to value will persist as long as given symbol_value_map persists.
	**/
    static
    const double*
    findSymbolValue(const std::string& symbol, const SymbolValueMap& symbol_value_map);


};

} //namespace ortis

#endif // EXPRPAR_EXPRESSIONSYMBOLTABLE_HPP
