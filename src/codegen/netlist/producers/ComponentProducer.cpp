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

#include "codegen/netlist/producers/ComponentProducer.hpp"

namespace lblmc
{

ComponentProducer::ComponentProducer() :
		type(""),
		producer_name("ComponentProducer"),
        num_parameters(0),
        num_terminals(0)
	{}

ComponentProducer::ComponentProducer(const ComponentProducer& base) :
		type(base.type),
		producer_name(base.producer_name),
		num_parameters(base.num_parameters),
		num_terminals(base.num_terminals)
	{}

ComponentProducer::ComponentProducer(ComponentProducer&& base) :
		type(std::move(base.type)),
		producer_name(std::move(base.producer_name)),
		num_parameters(base.num_parameters),
		num_terminals(base.num_terminals)
	{}

const std::string& ComponentProducer::getType() const { return type; }

bool ComponentProducer::isTypeValid(const ComponentListing& component_def) const
{
	return component_def.getType() == getType();
}

bool ComponentProducer::isLabelValid(const ComponentListing& component_def) const
{
	return component_def.getLabel() != std::string("");
}

bool ComponentProducer::isNumberOfParametersValid(const ComponentListing& component_def) const
{
	return num_parameters == component_def.getParametersCount();
}

bool ComponentProducer::isNumberOfTerminalsValid(const ComponentListing& component_def) const
{
	return num_terminals == component_def.getTerminalConnectionsCount();
}

void ComponentProducer::assertTypeValid(const ComponentListing& component_def) const
{
	if(!isTypeValid(component_def))
	{
		throw std::invalid_argument(producer_name+std::string("::assertTypeValid(x) -- netlist component type does not match this producer"));
	}
}

void ComponentProducer::assertLabelValid(const ComponentListing& component_def)  const
{
	if(!isLabelValid(component_def))
	{
		throw std::invalid_argument(producer_name+std::string("::assertLabelValid(x) -- netlist component label is not valid"));
	}
}

void ComponentProducer::assertNumberOfParametersValid(const ComponentListing& component_def)  const
{
	if(!isNumberOfParametersValid(component_def))
	{
		throw std::invalid_argument(producer_name+std::string("::assertNumberOfParametersValid(x) -- netlist component parameter amount is incorrect: amount of parameters supported is ")+std::to_string(num_parameters));
	}
}

void ComponentProducer::assertNumberOfTerminalsValid(const ComponentListing& component_def) const
{
	if(!isNumberOfTerminalsValid(component_def))
	{
		throw std::invalid_argument(producer_name+std::string("::assertNumberOfTerminalsValid(x) -- netlist component terminal amount is incorrect: amount of terminals supported is ")+std::to_string(num_terminals));
	}
}

void ComponentProducer::assertNetlistComponentInstanceValid(const ComponentListing& component_def) const
{
	assertTypeValid(component_def);
	assertLabelValid(component_def);
	assertNumberOfParametersValid(component_def);
	assertNumberOfTerminalsValid(component_def);
}

} //namespace lblmc
