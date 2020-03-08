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

#ifndef LBLMC_COMPONENTCONDUCTANCE_HPP
#define LBLMC_COMPONENTCONDUCTANCE_HPP

#include "codegen/udc/ComponentElement.hpp"

namespace lblmc
{

/**
	\brief defines conductance elements in a component of a system

	This element type defines conductances/resistances for electrical systems.
**/
class ComponentConductance : public ComponentElement
{
private:

	std::string p;	   ///< references by name the terminal element for the positive terminal of the conductance
	std::string n;     ///< references by name the terminal element for the negative terminal of the conductance
	std::string value; ///< numerical value or expression for the conductance; default is "1.0"

public:

	ComponentConductance();
	ComponentConductance
	(
		const std::string& label,
		const std::string& p = std::string("p"),
		const std::string& n = std::string("n"),
		const std::string& value = std::string("1.0")
	);
	ComponentConductance(const ComponentConductance& base);
	ComponentConductance(ComponentConductance&& base);

	void setTerminals(const std::string& p, const std::string& n);
	void setPositiveTerminal(const std::string& p);
	void setNegativeTerminal(const std::string& n);
	const std::string& getPositiveTerminal() const;
	const std::string& getNegativeTerminal() const;
	void setValue(const std::string& value);
	const std::string& getValue() const;

	std::string asString() const;
	explicit operator std::string() const;

};

} //namespace lblmc

#endif // LBLMC_COMPONENTCONDUCTANCE_HPP



