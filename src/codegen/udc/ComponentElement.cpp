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
#include <stdexcept>
#include <utility>
#include "codegen/udc/ComponentElement.hpp"
#include "codegen/Cpp.hpp"

namespace lblmc
{

ComponentElement::ComponentElement() :
	label("label"),
	element_type("ComponentElement")
{}

ComponentElement::ComponentElement(const std::string& label) :
	label(label),
	element_type("ComponentElement")
{
	if(!Cpp::isNameValid(label))
	{
		throw std::invalid_argument("ComponentElement::ComponentElement(const std::string& label) -- label must be a valid, non-null C++ label");
	}
}

ComponentElement::ComponentElement(const ComponentElement& base) :
	label(base.label),
	element_type(base.element_type)
{}

ComponentElement::ComponentElement(ComponentElement&& base) :
	label(std::move(base.label)),
	element_type(std::move(base.element_type))
{}


void ComponentElement::setLabel(const std::string& label)
{
	if(!Cpp::isNameValid(label))
	{
		throw std::invalid_argument("ComponentElement::setLabel(const std::string& label) -- label must be a valid, non-null C++ label");
	}

	this->label = label;
}

const std::string& ComponentElement::getLabel() const
{
	return label;
}

const std::string& ComponentElement::getElementType() const
{
	return element_type;
}

std::string ComponentElement::asString() const { return std::string(""); }
ComponentElement::operator std::string() const { return std::string(""); }

} //namespace lblmc
