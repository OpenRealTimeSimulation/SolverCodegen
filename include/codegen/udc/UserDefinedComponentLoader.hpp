
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

#ifndef LBLMC_USERDEFINEDCOMPONENTLOADER_HPP
#define LBLMC_USERDEFINEDCOMPONENTLOADER_HPP

#include <string>
#include <istream>

#include "codegen/udc/UserDefinedComponent.hpp"


namespace lblmc
{

/**
	\brief Loads a User Defined Component definition from streams, strings, and files

	\author Matthew Milton

	\date Created July 19, 2020

**/
class UserDefinedComponentLoader
{

public:

    static
    UserDefinedComponent
    loadFromStream(std::istream& strm);

    static
    UserDefinedComponent
    loadFromString(const std::string& str);

    static
    UserDefinedComponent
    loadFromFile(const std::string& filename);

private:

	const static std::vector<std::string> SUPPORTED_MACROS;
	const static std::vector<std::string> SUPPORTED_DATATYPES;

};

} //namespace lblmc

#endif // LBLMC_USERDEFINEDCOMPONENTLOADER_HPP
