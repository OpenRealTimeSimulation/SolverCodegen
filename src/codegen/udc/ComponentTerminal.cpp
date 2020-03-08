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

#include "codegen/udc/ComponentTerminal.hpp"
#include "codegen/udc/ComponentElement.hpp"
#include <utility>
#include <stdexcept>
#include <string>

namespace lblmc
{

ComponentTerminal::ComponentTerminal() :
	ComponentElement(),
	index(ComponentTerminal::null_index)
{
	element_type = "ComponentTerminal";
}

ComponentTerminal::ComponentTerminal(const std::string& label, unsigned int index) :
	ComponentElement(label),
	index(index)
{
	element_type = "ComponentTerminal";
}

ComponentTerminal::ComponentTerminal(const ComponentTerminal& base) :
	ComponentElement(base),
	index(base.index)
{}

ComponentTerminal::ComponentTerminal(ComponentTerminal&& base) :
	ComponentElement(std::move(base)),
	index(std::move(base.index))
{}

void ComponentTerminal::setIndex(unsigned int index)
{
	if(index == ComponentTerminal::null_index)
	{
		throw std::invalid_argument("ComponentTerminal::setIndex(index) -- index cannot be null index (-1)");
	}

	this->index = index;
}

const unsigned int& ComponentTerminal::getIndex() const
{
	return index;
}

std::string ComponentTerminal::asString() const
{
	return label;
}

ComponentTerminal::operator std::string() const
{
	return label;
}

} //namespace lblmc
