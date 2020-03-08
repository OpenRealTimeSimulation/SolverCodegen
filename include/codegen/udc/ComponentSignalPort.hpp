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

#ifndef LBLMC_COMPONENTSIGNALPORT_HPP
#define LBLMC_COMPONENTSIGNALPORT_HPP

#include <string>
#include "codegen/udc/ComponentElement.hpp"

namespace lblmc
{

/**
	\brief defines input and output signal ports for components in a system
**/
class ComponentSignalPort : public ComponentElement
{
private:

	std::string type;          ///< data type of the port; generally primitive C++ types or real
	unsigned long array_size;  ///< size (number of elements) of port if array; 0 for scalar(non-array) or positive nonzero for array

public:

	ComponentSignalPort();
	ComponentSignalPort
	(
		const std::string& type,
		const std::string& label,
		unsigned long array_size = 0
	);
	ComponentSignalPort(const ComponentSignalPort& base);
	ComponentSignalPort(ComponentSignalPort&& base);

	void setType(const std::string& type);
	const std::string& getType() const;
	void setArraySize(unsigned long array_size);
	const unsigned long& getArraySize() const;

	std::string asString() const;
	explicit operator std::string() const;

};

} //namespace lblmc

#endif // LBLMC_COMPONENTSIGNALPORT_HPP



