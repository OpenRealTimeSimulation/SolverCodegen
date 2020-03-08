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
#include "codegen/netlist/producers/NortonPortProducer.hpp"
#include "codegen/components/NortonPort.hpp"

namespace lblmc
{

NortonPortProducer::NortonPortProducer() :
	ComponentProducer()
{
	type = "NortonPort";
	producer_name = "NortonPortProducer";
	num_parameters = 1; //minimum number of parameters in this case since parameter amount is variable
	num_terminals  = 2; //minimum number of terminals in this case since terminal amount is variable
}

NortonPortProducer::NortonPortProducer(const NortonPortProducer& base) :
	ComponentProducer(base)
{
	type = base.type;
	num_parameters = base.num_parameters;
	num_terminals  = base.num_terminals;
}

NortonPortProducer::NortonPortProducer(NortonPortProducer&& base) :
	ComponentProducer(base)
{
	type = std::move(base.type);
	num_parameters = base.num_parameters;
	num_terminals  = base.num_terminals;
}

std::unique_ptr<Component> NortonPortProducer::operator()(const ComponentListing& component_def) const
{
	assertTypeValid(component_def);
	assertLabelValid(component_def);

	auto parameter_count = component_def.getParametersCount();
	if(parameter_count < 1)
	{
		throw std::invalid_argument(producer_name+std::string("::operator() -- netlist component parameter amount is incorrect: amount of parameters supported is 1 or greater") );
	}

	auto terminal_count = component_def.getTerminalConnectionsCount();
	if( (terminal_count < 2) || ( (terminal_count-2)/2 != (parameter_count-1) ) )
	{
		throw std::invalid_argument(producer_name+std::string("::operator() -- netlist component terminal connection amount is incorrect: amount must be 2 + 2 * number of transconductances") );
	}

	//const std::vector<double>& parameters = component_def.getParameters();
	//auto parameters_iter = parameters.begin();
	//const std::vector<unsigned int>& terminal_connections = component_def.getTerminalConnections();
	//auto terminal_connections_iter = terminal_connections.begin();

	//auto conductance = *parameters_iter; parameters_iter++;
	//auto port_term_p = *terminal_connections_iter; terminal_connections_iter++;
	//auto port_term_n = *terminal_connections_iter; terminal_connections_iter++;

	NortonPort* comp = new NortonPort(component_def.getLabel(), component_def.getParameters());
	comp->setTerminalConnections(component_def.getTerminalConnections());

    return std::unique_ptr<Component>(comp);
}

} //namespace lblmc



