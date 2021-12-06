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


#include "codegen/components/FunctionalVoltageSource.hpp"
#include "codegen/SystemConductanceGenerator.hpp"
#include "codegen/SystemSourceVectorGenerator.hpp"
#include "codegen/Object.hpp"

#include <stdexcept>
#include <sstream>
#include <iomanip>

namespace lblmc
{

FunctionalVoltageSource::FunctionalVoltageSource(std::string comp_name) :
	Component(comp_name),
	RES(1.0),
	P(0),
	N(0),
	source_id(0)
{
	if(comp_name == "")
	{
		throw std::invalid_argument("FunctionalVoltageSource::constructor(): comp_name must be a valid, non-empty C++ label");
	}
}

FunctionalVoltageSource::FunctionalVoltageSource(std::string comp_name, double res) :
	Component(comp_name),
	RES(res),
	P(0),
	N(0),
	source_id(0)
{
	if(res <= 0)
	{
		throw std::invalid_argument("FunctionalVoltageSource::constructor(): res must be positive nonzero value");
	}

	if(comp_name == "")
	{
		throw std::invalid_argument("FunctionalVoltageSource::constructor(): comp_name must be a valid, non-empty C++ label");
	}
}

FunctionalVoltageSource::FunctionalVoltageSource(const FunctionalVoltageSource& base) :
	Component(base),
	RES(base.RES),
	P(base.P),
	N(base.N),
	source_id(base.source_id)
{}

void FunctionalVoltageSource::getSourceIds(std::vector<unsigned int>& ids) const
{
	ids.clear();
	ids.push_back(source_id);
}

void FunctionalVoltageSource::getResistiveCompanionElements(std::vector<ResistiveCompanionElement>& elements) const
{
	elements.clear();

    RCE rce;
    rce.name = appendName(std::string("b"));
    rce.p = P;
    rce.n = N;
    rce.fixed_source_value = 0.0;
    rce.parallel_conductance = 1.0/RES;
    rce.source_id = source_id;
    rce.is_fixed = false;
    rce.has_no_source = false;
    rce.is_ideal_voltage_source = false;

    elements.push_back(rce);
}

void FunctionalVoltageSource::stampConductance(SystemConductanceGenerator& gen)
{
	gen.stampConductance(1.0/RES, P, N);
}

void FunctionalVoltageSource::stampSources(SystemSourceVectorGenerator& gen)
{
	source_id = gen.insertSource(P,N);
}

std::string FunctionalVoltageSource::generateParameters()
{
	std::stringstream sstrm;
	sstrm <<
	std::setprecision(16) <<
	std::fixed <<
	std::scientific;

	sstrm <<
	"const static "<<"real "<<appendName("RES")<<" = "<<RES<<";\n";

	return sstrm.str();
}

std::string FunctionalVoltageSource::generateInputs()
{
	std::stringstream sstrm;
	sstrm <<
	std::setprecision(16) <<
	std::fixed <<
	std::scientific;

	//append name to input ports
	lblmc::Object v_in("real", appendName("v_in"), "");

	sstrm <<
	v_in.generateArgument();

	return sstrm.str();
}


std::string FunctionalVoltageSource::generateUpdateBody()
{
	std::stringstream sstrm;
	sstrm <<
	std::setprecision(16) <<
	std::fixed <<
	std::scientific;

	sstrm <<
	"b_components["<<source_id-1<<"]"<<" = "<<appendName("v_in")<<"*real("<<1.0/RES<<");\n";

	return sstrm.str();
}

} //namespace lblmc
