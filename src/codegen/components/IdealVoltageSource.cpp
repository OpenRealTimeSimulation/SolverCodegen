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

#include "codegen/components/IdealVoltageSource.hpp"
#include "codegen/SystemConductanceGenerator.hpp"
#include "codegen/SystemSourceVectorGenerator.hpp"

#include <stdexcept>
#include <sstream>
#include <iomanip>

#include <iostream>

namespace lblmc
{

IdealVoltageSource::IdealVoltageSource(std::string comp_name) :
	Component(comp_name),
	VOLTAGE(1.0),
	P(0),
	N(0),
	source_id(0),
	solution_id(0)
{
	if(comp_name == "")
	{
		throw std::invalid_argument("IdealVoltageSource::constructor(): comp_name must be a valid, non-empty C++ label");
	}
}

IdealVoltageSource::IdealVoltageSource(std::string comp_name, double v) :
	Component(comp_name),
	VOLTAGE(v),
	P(0),
	N(0),
	source_id(0),
	solution_id(0)
{
	if(comp_name == "")
	{
		throw std::invalid_argument("IdealVoltageSource::constructor(): comp_name must be a valid, non-empty C++ label");
	}
}

IdealVoltageSource::IdealVoltageSource(const IdealVoltageSource& base) :
	Component(base),
	VOLTAGE(base.VOLTAGE),
	P(base.P),
	N(base.N),
	source_id(base.source_id),
	solution_id(base.solution_id)
{}

void IdealVoltageSource::getSourceIds(std::vector<unsigned int>& ids) const
{
	ids.clear();
	ids.push_back(source_id);
}

void IdealVoltageSource::getResistiveCompanionElements(std::vector<ResistiveCompanionElement>& elements) const
{
	elements.clear();

    RCE rce;
    rce.name = appendName(std::string("b"));
    rce.p = P;
    rce.n = N;
    rce.fixed_source_value = VOLTAGE;
    rce.parallel_conductance = 0.0;
    rce.source_id = source_id;
    rce.is_fixed = true;
    rce.has_no_source = false;
    rce.is_ideal_voltage_source = true;

    elements.push_back(rce);
}

void IdealVoltageSource::getSolutionIds(std::vector<unsigned int>& ids) const
{
	ids.clear();
	ids.push_back(solution_id);
}

void IdealVoltageSource::stampConductance(SystemConductanceGenerator& gen)
{
	gen.stampIdealVoltageSourceIncidence(solution_id, P, N);
}

void IdealVoltageSource::stampSources(SystemSourceVectorGenerator& gen)
{
	source_id = gen.insertIdealVoltageSource(solution_id);
}

std::string IdealVoltageSource::generateParameters()
{
	std::stringstream sstrm;
	sstrm <<
	std::setprecision(16) <<
	std::fixed <<
	std::scientific;

	sstrm <<
	"const static "<<"real "<<appendName("VOLTAGE")<<" = "<<VOLTAGE<<";\n";

	return sstrm.str();
}

std::string IdealVoltageSource::generateUpdateBody()
{
	std::stringstream sstrm;
	sstrm <<
	std::setprecision(16) <<
	std::fixed <<
	std::scientific;

	sstrm <<
	"b_components["<<source_id-1<<"]"<<" = "<<appendName("VOLTAGE")<<";\n";

	return sstrm.str();
}

} //namespace lblmc
