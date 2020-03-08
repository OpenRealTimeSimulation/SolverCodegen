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

#include <stdexcept>
#include "codegen/components/NortonPort.hpp"
#include "codegen/SystemConductanceGenerator.hpp"
#include "codegen/SystemSourceVectorGenerator.hpp"
#include "codegen/Object.hpp"

namespace lblmc
{

NortonPort::NortonPort(std::string comp_name) :
	Component(comp_name),
	CONDUCTANCE(0.0),
	TRANSCONDUCTANCES(),
	P(0),
	N(0),
	OTHER_PORT_TERMINALS(),
	source_id(0)
{
	if(comp_name == "")
	{
		throw std::invalid_argument("NortonPort::constructor(): comp_name must be a valid, non-empty C++ label");
	}
}

NortonPort::NortonPort(std::string comp_name, const std::vector<double>& all_conductances) :
	Component(comp_name),
	CONDUCTANCE(0.0),
	TRANSCONDUCTANCES(),
	P(0),
	N(0),
	OTHER_PORT_TERMINALS(),
	source_id(0)
{
	if(comp_name == "")
	{
		throw std::invalid_argument("NortonPort::constructor(): comp_name must be a valid, non-empty C++ label");
	}

	if(all_conductances.size() < 1)
	{
		throw std::invalid_argument("NortonPort::constructor(): all_conductances must have at least 1 value for the port conductance");
	}

	CONDUCTANCE = all_conductances[0];
	TRANSCONDUCTANCES.insert(TRANSCONDUCTANCES.begin(), all_conductances.begin()+1, all_conductances.end() );
}

NortonPort::NortonPort(std::string comp_name, double conductance, const std::vector<double>& transconductances) :
	Component(comp_name),
	CONDUCTANCE(conductance),
	TRANSCONDUCTANCES(transconductances),
	P(0),
	N(0),
	OTHER_PORT_TERMINALS(),
	source_id(0)
{
	if(comp_name == "")
	{
		throw std::invalid_argument("NortonPort::constructor(): comp_name must be a valid, non-empty C++ label");
	}
}

NortonPort::NortonPort(const NortonPort& base) :
	Component(base.comp_name),
	CONDUCTANCE(base.CONDUCTANCE),
	TRANSCONDUCTANCES(base.TRANSCONDUCTANCES),
	P(base.P),
	N(base.N),
	OTHER_PORT_TERMINALS(base.OTHER_PORT_TERMINALS),
	source_id(base.source_id)
{}

void NortonPort::setTerminalConnections
(
	unsigned int p_this_port,
	unsigned int n_this_port,
	const std::vector<unsigned int>& pn_other_ports
)
{
	if( pn_other_ports.size()/2 != TRANSCONDUCTANCES.size() )
	{
		throw std::invalid_argument("NortonPort::setTerminalConnections() -- pn_other_ports must have 2 * number of transconductances of terminals in component");
	}

	P = p_this_port;
	N = n_this_port;
	OTHER_PORT_TERMINALS = pn_other_ports;
}

void NortonPort::setTerminalConnections(const std::vector<unsigned int>& all_terminals)
{
	if( (all_terminals.size() < 2) || ( (all_terminals.size()-2)/2 != (TRANSCONDUCTANCES.size()) ) )
	{
		throw std::invalid_argument("NortonPort::setTerminalConnections() -- given amount of component terminal connection amount is incorrect: connection amount must be 2 + 2 * number of transconductances" );
	}

	auto terminal = all_terminals.begin();
	P = *terminal; terminal++;
	N = *terminal; terminal++;
	OTHER_PORT_TERMINALS.clear();
	OTHER_PORT_TERMINALS.insert(OTHER_PORT_TERMINALS.begin(), terminal, all_terminals.end());
}

void NortonPort::setParameters(double conduct, const std::vector<double>& xconduct)
{
	CONDUCTANCE = conduct;
	TRANSCONDUCTANCES = xconduct;
}

void NortonPort::getSourceIds(std::vector<unsigned int>& ids) const
{
	ids.clear();
	ids.push_back(source_id);
}

void NortonPort::getResistiveCompanionElements(std::vector<ResistiveCompanionElement>& elements) const
{
	elements.clear();

	RCE rce;
    rce.name = appendName(std::string("bport"));
    rce.p = P;
    rce.n = N;
    rce.fixed_source_value = 0.0;
    rce.parallel_conductance = CONDUCTANCE;
    rce.source_id = source_id;
    rce.is_fixed = false;
    rce.has_no_source = false;
    rce.is_ideal_voltage_source = false;

    elements.push_back(rce);

}

void NortonPort::stampConductance(SystemConductanceGenerator& gen)
{
	if( ( OTHER_PORT_TERMINALS.size() != 0) && ( TRANSCONDUCTANCES.size() != 0) && (OTHER_PORT_TERMINALS.size() / TRANSCONDUCTANCES.size() != 2) )
	{
		throw std::runtime_error("NortonPort::stampConductance() -- number of transconductances does not correspond to number of port terminals; there must be 2 terminals per transconductance");
	}

	gen.stampConductance(CONDUCTANCE, P, N);

	for(unsigned int i = 0; i < TRANSCONDUCTANCES.size(); i++)
	{
		const auto& t1 = OTHER_PORT_TERMINALS[i*2 + 0];
		const auto& t2 = OTHER_PORT_TERMINALS[i*2 + 1];
		gen.stampTransconductance( TRANSCONDUCTANCES[i], t1, t2, P, N );
	}

}

void NortonPort::stampSources(SystemSourceVectorGenerator& gen)
{
	source_id = gen.insertSource(P,N);
}

std::string NortonPort::generateParameters()
{
	return std::string("");
}

std::string NortonPort::generateFields()
{
	return std::string("");
}

std::string NortonPort::generateInputs()
{
	std::stringstream sstrm;
	sstrm <<
	std::setprecision(16) <<
	std::fixed <<
	std::scientific;

	//append name to input ports
	lblmc::Object i_in("real", appendName("i_in"), "");

	sstrm <<
	i_in.generateArgument();

	return sstrm.str();
}

std::string NortonPort::generateOutputs(std::string output)
{
	return std::string("");
}

std::string NortonPort::generateUpdateBody()
{
	std::stringstream sstrm;
	sstrm <<
	std::setprecision(16) <<
	std::fixed <<
	std::scientific;

	sstrm <<
	"b_components["<<source_id-1<<"]"<<" = "<<appendName("i_in")<<";\n";

	return sstrm.str();
}

} //namespace lblmc
