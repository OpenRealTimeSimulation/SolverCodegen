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

#ifndef LBLMC_COMPONENTTERMINAL_HPP
#define LBLMC_COMPONENTTERMINAL_HPP

#include <string>
#include "codegen/udc/ComponentElement.hpp"

namespace lblmc
{

/**
	\brief defines terminal elements of a Component in a system
**/
class ComponentTerminal : public ComponentElement
{
private:

	unsigned int index; ///< the terminal's assigned index in a system (node index)

public:

	ComponentTerminal();
	ComponentTerminal(const std::string& label, unsigned int index = -1);
	ComponentTerminal(const ComponentTerminal& base);
	ComponentTerminal(ComponentTerminal&& base);

	void setIndex(unsigned int index);
	const unsigned int& getIndex() const;

	std::string asString() const;
	explicit operator std::string() const;

};

} //namespace lblmc

#endif // LBLMC_TERMINAL_HPP

