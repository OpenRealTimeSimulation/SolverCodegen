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


#include "codegen/components/FunctionalCurrentSource.hpp"
#include "codegen/SystemConductanceGenerator.hpp"
#include "codegen/SystemSourceVectorGenerator.hpp"
#include "codegen/Object.hpp"

#include <stdexcept>
#include <sstream>
#include <iomanip>

namespace lblmc
{

FunctionalCurrentSource::FunctionalCurrentSource(std::string comp_name) :
	Component(comp_name),
	P(0),
	N(0),
	source_id(0)
{
	if(comp_name == "")
	{
		throw std::invalid_argument("FunctionalCurrentSource::constructor(): comp_name must be a valid, non-empty C++ label");
	}
}

FunctionalCurrentSource::FunctionalCurrentSource(const FunctionalCurrentSource& base) :
	Component(base),
	P(base.P),
	N(base.N),
	source_id(base.source_id)
{}

void FunctionalCurrentSource::getSourceIds(std::vector<unsigned int>& ids) const
{
	ids.clear();
	ids.push_back(source_id);
}

void FunctionalCurrentSource::getResistiveCompanionElements(std::vector<ResistiveCompanionElement>& elements) const
{
	elements.clear();

    RCE rce;
    rce.name = appendName(std::string("b"));
    rce.p = P;
    rce.n = N;
    rce.fixed_source_value = 0.0;
    rce.parallel_conductance = 0.0;
    rce.source_id = source_id;
    rce.is_fixed = false;
    rce.has_no_source = false;
    rce.is_ideal_voltage_source = false;

    elements.push_back(rce);
}

void FunctionalCurrentSource::stampConductance(SystemConductanceGenerator& gen)
{
	//do nothing
}

void FunctionalCurrentSource::stampSources(SystemSourceVectorGenerator& gen)
{
	source_id = gen.insertSource(P,N);
}

std::string FunctionalCurrentSource::generateParameters()
{
	return std::string("");
}

std::string FunctionalCurrentSource::generateInputs()
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

	//sstrm << "\n";


	return sstrm.str();
}


std::string FunctionalCurrentSource::generateUpdateBody()
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
