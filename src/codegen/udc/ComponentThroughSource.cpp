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

#include "codegen/udc/ComponentThroughSource.hpp"
#include "codegen/udc/ComponentElement.hpp"
#include <utility>
#include <stdexcept>

namespace lblmc
{

ComponentThroughSource::ComponentThroughSource() :
	ComponentElement(),
	p("p"),
	n("n"),
	id(ComponentThroughSource::null_id)
{
	element_type = "ComponentThroughSource";
}

ComponentThroughSource::ComponentThroughSource
(
	const std::string& label,
	const std::string& p,
	const std::string& n,
	unsigned int id
) :
	ComponentElement(label),
	p(p),
	n(n),
	id(id)
{
	element_type = "ComponentThroughSource";
}

ComponentThroughSource::ComponentThroughSource(const ComponentThroughSource& base) :
	ComponentElement(base),
	p(base.p),
	n(base.n),
	id(base.id)
{}

ComponentThroughSource::ComponentThroughSource(ComponentThroughSource&& base) :
	ComponentElement(std::move(base)),
	p(std::move(base.p)),
	n(std::move(base.n)),
	id(std::move(base.id))
{}

void ComponentThroughSource::setTerminals(const std::string& p, const std::string& n)
{
	if(p.empty() || n.empty())
	{
		throw std::invalid_argument("ComponentThroughSource::setTerminals(p,n) -- p and/or n cannot be null strings");
	}

	this->p = p;
	this->n = n;
}

void ComponentThroughSource::setPositiveTerminal(const std::string& p)
{
	if(p.empty())
	{
		throw std::invalid_argument("ComponentThroughSource::setPositiveTerminals(p) -- p cannot be null string");
	}

	this->p = p;
}

void ComponentThroughSource::setNegativeTerminal(const std::string& n)
{
	if(n.empty())
	{
		throw std::invalid_argument("ComponentThroughSource::setNegativeTerminals(n) -- n cannot be null string");
	}

	this->n = n;
}

const std::string& ComponentThroughSource::getPositiveTerminal() const
{
	return p;
}

const std::string& ComponentThroughSource::getNegativeTerminal() const
{
	return n;
}

void ComponentThroughSource::setId(unsigned int id)
{
	if(id == ComponentThroughSource::null_id)
	{
		throw std::invalid_argument("ComponentThroughSource::setId(id) -- id cannot be equal to null_id (-1)");
	}

	this->id = id;
}

const unsigned int& ComponentThroughSource::getId() const
{
	return id;
}

std::string ComponentThroughSource::asString() const
{
	std::string ret =
	element_type + " " + label + " " + "()" + " " + "{ " + p + ", " + n + " }; " + "// id=" + std::to_string(id);

	return ret;
}

ComponentThroughSource::operator std::string() const
{
	std::string ret =
	element_type + " " + label + " " + "()" + " " + "{ " + p + ", " + n + " }; " + "// id=" + std::to_string(id);

	return ret;
}

} //namespace lblmc

