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

#include "codegen/udc/ComponentConductance.hpp"
#include "codegen/udc/ComponentElement.hpp"
#include <utility>
#include <stdexcept>

namespace lblmc
{

ComponentConductance::ComponentConductance() :
	ComponentElement(),
	p("p"),
	n("n"),
	value("1.0")
{
	element_type = "ComponentConductance";
}

ComponentConductance::ComponentConductance
(
	const std::string& label,
	const std::string& p,
	const std::string& n,
	const std::string& value
) :
	ComponentElement(label),
	p(p),
	n(n),
	value(value)
{
	element_type = "ComponentConductance";
}

ComponentConductance::ComponentConductance(const ComponentConductance& base) :
	ComponentElement(base),
	p(base.p),
	n(base.n),
	value(base.value)
{}

ComponentConductance::ComponentConductance(ComponentConductance&& base) :
	ComponentElement(std::move(base)),
	p(std::move(base.p)),
	n(std::move(base.n)),
	value(std::move(base.value))
{}

void ComponentConductance::setTerminals(const std::string& p, const std::string& n)
{
	if(p.empty() || n.empty())
	{
		throw std::invalid_argument("ComponentConductance::setTerminals(p,n) -- p and/or n cannot be null strings");
	}

	this->p = p;
	this->n = n;
}

void ComponentConductance::setPositiveTerminal(const std::string& p)
{
	if(p.empty())
	{
		throw std::invalid_argument("ComponentConductance::setPositiveTerminals(p) -- p cannot be null string");
	}

	this->p = p;
}

void ComponentConductance::setNegativeTerminal(const std::string& n)
{
	if(n.empty())
	{
		throw std::invalid_argument("ComponentConductance::setNegativeTerminals(n) -- n cannot be null string");
	}

	this->n = n;
}

const std::string& ComponentConductance::getPositiveTerminal() const
{
	return p;
}

const std::string& ComponentConductance::getNegativeTerminal() const
{
	return n;
}

void ComponentConductance::setValue(const std::string& value)
{
	if(value.empty())
	{
		throw std::invalid_argument("ComponentConductance::setValue(value) -- value cannot be null string");
	}

	this->value = value;
}

const std::string& ComponentConductance::getValue() const
{
	return value;
}

std::string ComponentConductance::asString() const
{
	std::string ret =
	element_type + " " + label + " " + "("+ value + ")" + " " + "{ " + p + ", " + n + " }";

	return ret;
}

ComponentConductance::operator std::string() const
{
	std::string ret =
	element_type + " " + label + " " + "("+ value + ")" + " " + "{ " + p + ", " + n + " }";

	return ret;
}

} //namespace lblmc

