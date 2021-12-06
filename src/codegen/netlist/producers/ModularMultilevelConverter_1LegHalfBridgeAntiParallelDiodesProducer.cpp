/*

Copyright (C) 2019-2021 Matthew Milton

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
#include "codegen/netlist/producers/ModularMultilevelConverter_1LegHalfBridgeAntiParallelDiodesProducer.hpp"
#include "codegen/components/ModularMultilevelConverter_1LegHalfBridgeAntiParallelDiodes.hpp"

namespace lblmc
{

ModularMultilevelConverter_1LegHalfBridgeAntiParallelDiodesProducer::ModularMultilevelConverter_1LegHalfBridgeAntiParallelDiodesProducer() :
	ComponentProducer()
{
	type = "ModularMultilevelConverter_1LegHalfBridgeAntiParallelDiodes";
	producer_name = "ModularMultilevelConverter_1LegHalfBridgeAntiParallelDiodesProducer";
	num_parameters = 9;
	num_terminals  = 3;
}

ModularMultilevelConverter_1LegHalfBridgeAntiParallelDiodesProducer::ModularMultilevelConverter_1LegHalfBridgeAntiParallelDiodesProducer(const ModularMultilevelConverter_1LegHalfBridgeAntiParallelDiodesProducer& base) :
	ComponentProducer(base)
{
	type = base.type;
	num_parameters = base.num_parameters;
	num_terminals  = base.num_terminals;
}

ModularMultilevelConverter_1LegHalfBridgeAntiParallelDiodesProducer::ModularMultilevelConverter_1LegHalfBridgeAntiParallelDiodesProducer(ModularMultilevelConverter_1LegHalfBridgeAntiParallelDiodesProducer&& base) :
	ComponentProducer(base)
{
	type = std::move(base.type);
	num_parameters = base.num_parameters;
	num_terminals  = base.num_terminals;
}

std::unique_ptr<Component> ModularMultilevelConverter_1LegHalfBridgeAntiParallelDiodesProducer::operator()(const ComponentListing& component_def) const
{
	assertNetlistComponentInstanceValid(component_def);

	ModularMultilevelConverter_1LegHalfBridgeAntiParallelDiodes* comp = new ModularMultilevelConverter_1LegHalfBridgeAntiParallelDiodes
	(
		component_def.getLabel(),
		component_def.getParameter(0),
		component_def.getParameter(1),
		component_def.getParameter(2),
		component_def.getParameter(3),
		component_def.getParameter(4),
		component_def.getParameter(5),
		component_def.getParameter(6),
		component_def.getParameter(7),
		component_def.getParameter(8)
	);


    comp->setTerminalConnections
    (
		component_def.getTerminalConnection(0),
		component_def.getTerminalConnection(1),
		component_def.getTerminalConnection(2)
	);

    return std::unique_ptr<Component>(comp);
}

} //namespace lblmc




