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

#ifndef LBLMC_COMPONENTTHROUGHSOURCE_HPP
#define LBLMC_COMPONENTTHROUGHSOURCE_HPP

#include "codegen/udc/ComponentElement.hpp"

namespace lblmc
{

/**
	\brief defines through source elements of a Component in a system

	This element can be used to define resistive companion current sources in electrical components.
**/
class ComponentThroughSource : public ComponentElement
{
private:

	std::string p;	///< references by name the terminal element for the positive terminal of this source
	std::string n;  ///< references by name the terminal element for the negative terminal of this source
	unsigned int id; ///< unique id number for this through source within a system

public:

	ComponentThroughSource();
	ComponentThroughSource
	(
		const std::string& label,
		const std::string& p = std::string("p"),
		const std::string& n = std::string("n"),
		unsigned int id = -1
	);
	ComponentThroughSource(const ComponentThroughSource& base);
	ComponentThroughSource(ComponentThroughSource&& base);

	void setTerminals(const std::string& p, const std::string& n);
	void setPositiveTerminal(const std::string& p);
	void setNegativeTerminal(const std::string& n);
	const std::string& getPositiveTerminal() const;
	const std::string& getNegativeTerminal() const;
	void setId(unsigned int id);
	const unsigned int& getId() const;

	std::string asString() const;
	explicit operator std::string() const;

};

} //namespace lblmc

#endif // LBLMC_COMPONENTTHROUGHSOURCE_HPP

