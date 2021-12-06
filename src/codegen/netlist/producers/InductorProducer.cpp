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

#include <utility>
#include "codegen/netlist/producers/InductorProducer.hpp"
#include "codegen/components/Inductor.hpp"

namespace lblmc
{

InductorProducer::InductorProducer() :
	ComponentProducer()
{
	type = "Inductor";
	producer_name = "InductorProducer";
	num_parameters = 2;
	num_terminals  = 2;
}

InductorProducer::InductorProducer(const InductorProducer& base) :
	ComponentProducer(base)
{
	type = base.type;
	num_parameters = base.num_parameters;
	num_terminals  = base.num_terminals;
}

InductorProducer::InductorProducer(InductorProducer&& base) :
	ComponentProducer(base)
{
	type = std::move(base.type);
	num_parameters = base.num_parameters;
	num_terminals  = base.num_terminals;
}

std::unique_ptr<Component> InductorProducer::operator()(const ComponentListing& component_def) const
{
	assertNetlistComponentInstanceValid(component_def);

	Inductor* comp = new Inductor( component_def.getLabel(), component_def.getParameter(0), component_def.getParameter(1) );
    comp->setTerminalConnections( component_def.getTerminalConnection(0), component_def.getTerminalConnection(1) );

    return std::unique_ptr<Component>(comp);
}

} //namespace lblmc


