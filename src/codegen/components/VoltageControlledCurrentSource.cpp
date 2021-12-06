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

#include "codegen/components/VoltageControlledCurrentSource.hpp"
#include "codegen/SystemConductanceGenerator.hpp"
#include "codegen/SystemSourceVectorGenerator.hpp"
#include "codegen/ResistiveCompanionElements.hpp"

namespace lblmc
{

VoltageControlledCurrentSource::VoltageControlledCurrentSource(std::string comp_name) :
		Component(comp_name),
		TRANSCONDUCTANCE(1.0),
		M(0),
		N(0),
		P(0),
		Q(0)
{
	if(comp_name == "")
	{
		throw std::invalid_argument("VoltageControlledCurrentSource::constructor(): comp_name must be a valid, non-empty C++ label");
	}
}

VoltageControlledCurrentSource::VoltageControlledCurrentSource(std::string comp_name, double transconductance) :
		Component(comp_name),
		TRANSCONDUCTANCE(transconductance),
		M(0),
		N(0),
		P(0),
		Q(0)
{
	if(comp_name == "")
	{
		throw std::invalid_argument("VoltageControlledCurrentSource::constructor(): comp_name must be a valid, non-empty C++ label");
	}
}

VoltageControlledCurrentSource::VoltageControlledCurrentSource(const VoltageControlledCurrentSource& base) :
		Component(base.comp_name),
		TRANSCONDUCTANCE(base.TRANSCONDUCTANCE),
		M(base.M),
		N(base.N),
		P(base.P),
		Q(base.Q)
{}

void VoltageControlledCurrentSource::getResistiveCompanionControlledSourceElements
		(std::vector<ResistiveCompanionControlledSourceElement>& elements) const
{
    elements.clear();

    RCCSE rccse;
    rccse.name = appendName("g");
    rccse.p_source = P;
    rccse.n_source = Q;
    rccse.p_measure = M;
    rccse.n_measure = N;
    rccse.gain = TRANSCONDUCTANCE;
    rccse.type = RCCSE::Type::VOLTAGE_CONTROLLED_CURRENT_SOURCE;

    elements.push_back(rccse);
}

void VoltageControlledCurrentSource::stampConductance(SystemConductanceGenerator& gen)
{
	gen.stampTransconductance(TRANSCONDUCTANCE,M,N,P,Q);
}


} //namespace lblmc


