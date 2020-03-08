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

#include "codegen/components/Capacitor.hpp"
#include "codegen/SystemConductanceGenerator.hpp"
#include "codegen/SystemSourceVectorGenerator.hpp"

#include <stdexcept>
#include <sstream>
#include <iomanip>

namespace lblmc
{

Capacitor::Capacitor(std::string comp_name) :
	Component(comp_name),
	DT(1.0),
    CAP(1.0),
    HOC2(2.0*CAP/DT),
    P(0),
    N(0),
    source_id(0)
{
	if(comp_name == "")
	{
		throw std::invalid_argument("Capacitor::constructor(): comp_name must be a valid, non-empty C++ label");
	}
}

Capacitor::Capacitor(std::string comp_name, double dt, double cap) :
	Component(comp_name),
	DT(dt),
	CAP(cap),
	HOC2(0.0),
	P(0),
	N(0),
	source_id(0)
{
	if(comp_name == "")
	{
		throw std::invalid_argument("Capacitor::constructor(): comp_name must be a valid, non-empty C++ label");
	}

	if(DT <= 0.0 || CAP <= 0.0)
	{
		throw std::invalid_argument("Capacitor::constructor(): parameters dt and cap must be positive nonzero values");
	}

	HOC2 = 2.0*CAP/DT;
}

Capacitor::Capacitor(const Capacitor& base) :
	Component(base),
	DT(base.DT),
	CAP(base.CAP),
	HOC2(base.HOC2),
	P(base.P),
	N(base.N),
	source_id(base.source_id)
{}

void Capacitor::getSourceIds(std::vector<unsigned int>& ids) const
{
	ids.clear();
	ids.push_back(source_id);
}

void Capacitor::getResistiveCompanionElements(std::vector<ResistiveCompanionElement>& elements) const
{
	elements.clear();

	RCE rce;
	rce.name = appendName(std::string("b"));
	rce.p = P;
	rce.n = N;
	rce.parallel_conductance = HOC2;
	rce.source_id = source_id;

	elements.push_back(rce);
}

void Capacitor::setTerminalConnections(unsigned int p, unsigned int n)
{
	P = p; N = n;
}

void Capacitor::stampConductance(SystemConductanceGenerator& gen)
{
	//const double HOC2 = 2.0*CAP/DT;

	gen.stampConductance(HOC2, P, N);
}

void Capacitor::stampSources(SystemSourceVectorGenerator& gen)
{
	source_id = gen.insertSource(P,N);
}

std::string Capacitor::generateParameters()
{
	std::stringstream sstrm;
	sstrm <<
	std::setprecision(16) <<
	std::fixed <<
	std::scientific;

	const double HOC2 = 2.0*CAP/DT;

	sstrm <<
	"const static "<<"real "<<"DT"  <<"_"<<comp_name<<" = "<<DT<<";\n" <<
	"const static "<<"real "<<"CAP" <<"_"<<comp_name<<" = "<<CAP<<";\n"<<
	"const static "<<"real "<<"HOC2"<<"_"<<comp_name<<" = "<<HOC2<<";\n";

	return sstrm.str();
}

std::string Capacitor::generateFields()
{
	std::stringstream sstrm;
	sstrm <<
	std::setprecision(16) <<
	std::fixed <<
	std::scientific;

	sstrm <<
	"static "<<"real "<<"epos_past"      <<"_"<<comp_name<<" = "<<0.0<<";\n" <<
	"static "<<"real "<<"eneg_past"      <<"_"<<comp_name<<" = "<<0.0<<";\n" <<
	"static "<<"real "<<"delta_v"        <<"_"<<comp_name<<" = "<<0.0<<";\n" <<
	"static "<<"real "<<"current"        <<"_"<<comp_name<<" = "<<0.0<<";\n" <<
	"static "<<"real "<<"current_eq"     <<"_"<<comp_name<<" = "<<0.0<<";\n" <<
	"static "<<"real "<<"current_eq_past"<<"_"<<comp_name<<" = "<<0.0<<";\n" ;

	return sstrm.str();
}

std::string Capacitor::generateUpdateBody()
{
	std::stringstream sstrm;
	sstrm <<
	std::setprecision(16) <<
	std::fixed <<
	std::scientific;

	//epos_past = epos;
	//eneg_past = eneg;
	//current_eq_past = current_eq;

	sstrm <<
	"epos_past"      <<"_"<<comp_name<<" = "<<"x["<<P<<"]"<<";\n" <<
	"eneg_past"      <<"_"<<comp_name<<" = "<<"x["<<N<<"]"<<";\n" <<
	"current_eq_past"<<"_"<<comp_name<<" = "<<"current_eq"<<"_"<<comp_name<<";\n" ;


	//delta_v = AddSubType(epos_past) - AddSubType(eneg_past);
	//current = (hoc2)*(delta_v) - (current_eq_past);
	//current_eq = (current) + (hoc2)*(delta_v);
	//*bout = current_eq;

	sstrm <<
	"delta_v"<<"_"<<comp_name<<" = "<<"epos_past"<<"_"<<comp_name<<" - "<<"eneg_past"<<"_"<<comp_name<<";\n" <<
	"current"<<"_"<<comp_name<<" = "<< "HOC2"<<"_"<<comp_name <<" * "<< "delta_v"<<"_"<<comp_name <<" - " << "current_eq_past"<<"_"<<comp_name << ";\n" <<
	appendName("current_eq")<<" = "<<appendName("current")<<" + "<<appendName("HOC2")<<"*"<<appendName("delta_v")<<";\n" <<
	"b_components["<<source_id-1<<"]"<<" = "<<appendName("current_eq")<<";\n";

	return sstrm.str();
}

} //namespace lblmc
