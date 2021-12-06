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

#include "codegen/components/CurrentSource.hpp"
#include "codegen/SystemConductanceGenerator.hpp"
#include "codegen/SystemSourceVectorGenerator.hpp"

#include <stdexcept>
#include <sstream>
#include <iomanip>

namespace lblmc
{

CurrentSource::CurrentSource(std::string comp_name) :
	Component(comp_name),
	CURRENT(1.0),
	P(0),
	N(0),
	source_id(0)
{
	if(comp_name == "")
	{
		throw std::invalid_argument("CurrentSource::constructor(): comp_name must be a valid, non-empty C++ label");
	}
}

CurrentSource::CurrentSource(std::string comp_name, double i) :
	Component(comp_name),
	CURRENT(i),
	P(0),
	N(0),
	source_id(0)
{
	if(comp_name == "")
	{
		throw std::invalid_argument("CurrentSource::constructor(): comp_name must be a valid, non-empty C++ label");
	}
}

CurrentSource::CurrentSource(const CurrentSource& base) :
	Component(base),
	CURRENT(base.CURRENT),
	P(base.P),
	N(base.N),
	source_id(base.source_id)
{}

void CurrentSource::getSourceIds(std::vector<unsigned int>& ids) const
{
	ids.clear();
	ids.push_back(source_id);
}

void CurrentSource::getResistiveCompanionElements(std::vector<ResistiveCompanionElement>& elements) const
{
	elements.clear();

    RCE rce;
    rce.name = appendName(std::string("b"));
    rce.p = P;
    rce.n = N;
    rce.fixed_source_value = CURRENT;
    rce.parallel_conductance = 0.0;
    rce.source_id = source_id;
    rce.is_fixed = true;
    rce.has_no_source = false;
    rce.is_ideal_voltage_source = false;

    elements.push_back(rce);
}

void CurrentSource::stampConductance(SystemConductanceGenerator& gen)
{
	//do nothing
}

void CurrentSource::stampSources(SystemSourceVectorGenerator& gen)
{
	source_id = gen.insertSource(P,N);
}

std::string CurrentSource::generateParameters()
{
	std::stringstream sstrm;
	sstrm <<
	std::setprecision(16) <<
	std::fixed <<
	std::scientific;

	sstrm <<
	"const static "<<"real "<<appendName("SRC_CURRENT")<<" = "<<CURRENT<<";\n";

	return sstrm.str();
}

std::string CurrentSource::generateUpdateBody()
{
	std::stringstream sstrm;
	sstrm <<
	std::setprecision(16) <<
	std::fixed <<
	std::scientific;

	sstrm <<
	"b_components["<<source_id-1<<"]"<<" = "<<appendName("SRC_CURRENT")<<";\n";

	return sstrm.str();
}

} //namespace lblmc
