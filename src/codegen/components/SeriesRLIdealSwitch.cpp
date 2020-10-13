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

#include "codegen/components/SeriesRLIdealSwitch.hpp"
#include "codegen/SystemConductanceGenerator.hpp"
#include "codegen/SystemSourceVectorGenerator.hpp"
#include "codegen/SolverEngineGenerator.hpp"
#include "codegen/Object.hpp"
#include "codegen/ArrayObject.hpp"
#include "codegen/StringProcessor.hpp"
#include <string>
#include <stdexcept>
#include <sstream>
#include <iomanip>

namespace lblmc
{

SeriesRLIdealSwitch::SeriesRLIdealSwitch(std::string comp_name) :
	Component(comp_name),
	DT(1.0),
	L(1.0),
	R(1.0),
	P(0), N(0),
	source_id(0),
	integration_method(INTEGRATION_EULER_FORWARD)
{}

SeriesRLIdealSwitch::SeriesRLIdealSwitch(std::string comp_name, double dt, double l, double r) :
	Component(comp_name),
	DT(dt),
	L(l),
	R(r),
	P(0), N(0),
	source_id(0),
	integration_method(INTEGRATION_EULER_FORWARD)
{}

SeriesRLIdealSwitch::SeriesRLIdealSwitch(const SeriesRLIdealSwitch& base) :
	Component(base),
	DT(base.DT),
	L(base.L),
	R(base.R),
	P(base.P), N(base.N),
	source_id(base.source_id),
	integration_method(base.integration_method)
{}

void SeriesRLIdealSwitch::getResistiveCompanionElements(std::vector<ResistiveCompanionElement>& elements) const
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

void SeriesRLIdealSwitch::stampConductance(SystemConductanceGenerator& gen)
{
	//do nothing since using explicit integration methods
}

void SeriesRLIdealSwitch::stampSources(SystemSourceVectorGenerator& gen)
{
	source_id = gen.insertSource(P,N);
}

std::string SeriesRLIdealSwitch::generateParameters()
{
	std::stringstream sstrm;
	sstrm <<
	std::setprecision(16) <<
	std::fixed <<
	std::scientific;


	if(integration_method == INTEGRATION_EULER_FORWARD)
	{

			// dx/dt (t) = -R/L*x(t) + 1/L*u(t)
			// x(t+dt) = x(t) + HOL*( u(t) - R*x(t) )
		const static double HOL = DT/L;
		generateParameter(sstrm, "DT", DT);
		generateParameter(sstrm, "L", L);
		generateParameter(sstrm, "R", R);
		generateParameter(sstrm, "HOL", HOL);
	}
	else if(integration_method == INTEGRATION_RUNGE_KUTTA_4)
	{

			// dx/dt (t) = A0*x(t) + B0*u(t)
			// x(t+dt) = ARK4*x(t) + BRK4*u(t)
		const static double A0 = -R/L;
		const static double B0 = 1.0/L;
		const static double A1 = DT*A0;
		const static double B1 = DT*B0;
		const static double A2 = DT*A0 + 0.5*DT*A0*A1;
		const static double A3 = DT*A0 + 0.5*DT*A0*A2;
		const static double A4 = DT*A0 + 1.0*DT*A0*A3;
		const static double B2 = DT*B0 + 0.5*DT*A0*B1;
		const static double B3 = DT*B0 + 0.5*DT*A0*B2;
		const static double B4 = DT*B0 + 1.0*DT*A0*B3;
		const static double ARK4 = 1.0 + (1.0/6.0)*A1 + (1.0/3.0)*A2 + (1.0/3.0)*A3 + (1.0/6.0)*A4;
		const static double BRK4 = 0.0 + (1.0/6.0)*B1 + (1.0/3.0)*B2 + (1.0/3.0)*B3 + (1.0/6.0)*B4;

		generateParameter(sstrm, "DT", DT);
		generateParameter(sstrm, "ARK4", ARK4);
		generateParameter(sstrm, "BRK4", BRK4);
	}


	return sstrm.str();
}

std::string SeriesRLIdealSwitch::generateFields()
{
	std::stringstream sstrm;
	sstrm <<
	std::setprecision(16) <<
	std::fixed <<
	std::scientific;

	generateField(sstrm, "current_past", 0.0);
	generateBoolField(sstrm, "sw_past", false);

	return sstrm.str();
}

std::string SeriesRLIdealSwitch::generateInputs()
{
	std::stringstream sstrm;
	sstrm <<
	std::setprecision(16) <<
	std::fixed <<
	std::scientific;

	//append name to input ports
	lblmc::Object sw("bool", appendName("sw"), "");

	sstrm <<
	sw.generateArgument();

	return sstrm.str();
}

std::string SeriesRLIdealSwitch::generateOutputs(std::string output)
{
	if(output == "ALL" || output == "l_current")
	{
		lblmc::Object l_current("real*", appendName("l_current"), "");
		return l_current.generateArgument();
	}
	else
	{
		return std::string("");
	}
}

std::string SeriesRLIdealSwitch::generateOutputsUpdateBody(std::string output)
{
	std::stringstream sstrm;
	sstrm <<
	std::setprecision(16) <<
	std::fixed <<
	std::scientific;

	if(output == "ALL" || output == "l_current")
	{
		sstrm << appendName("*l_current") << " = " << appendName("current") << ";\n\n";
		return sstrm.str();
	}
	else
	{
		return std::string("");
	}
}

const static std::string SERIESRLIDEALSWITCH_GENERATEUPDATEBODY_BASE_EF_STRING =
R"(
NumType current;

if(sw_past)
{
	current = current_past + HOL*(epos - R*current_past - eneg); //Euler Forward (explicit)
}
else
{
	current = 0; //force de-energizing of inductor to zero when switch open
}

current_past = current;
sw_past = sw;

*bout = -current;
)";

const static std::string SERIESRLIDEALSWITCH_GENERATEUPDATEBODY_BASE_RK4_STRING =
R"(
real current;

if(sw_past)
{
	current = ARK4*current_past + BRK4*(epos-eneg); //Runge Kutta 4th Order (explicit)
}
else
{
	current = 0; //force de-energizing of inductor to zero when switch open
}

current_past = current;
sw_past = sw;

*bout = -current;
)";

std::string SeriesRLIdealSwitch::generateUpdateBody()
{

	if(integration_method == INTEGRATION_EULER_FORWARD)
		return generateUpdateBodyEulerForward();

	if(integration_method == INTEGRATION_RUNGE_KUTTA_4)
		return generateUpdateBodyRungeKutta4();

	return std::string("");
}

std::string SeriesRLIdealSwitch::generateUpdateBodyEulerForward()
{
	std::stringstream sstrm;
	sstrm <<
	std::setprecision(16) <<
	std::fixed <<
	std::scientific;

	std::string body = SERIESRLIDEALSWITCH_GENERATEUPDATEBODY_BASE_EF_STRING;
	lblmc::StringProcessor str_proc(body);

	str_proc.replaceWordAll("NumType", "real");

	str_proc.replaceWordAll("HOL", appendName("HOL"));
	str_proc.replaceWordAll("R", appendName("R"));
	str_proc.replaceWordAll("L", appendName("L"));
	str_proc.replaceWordAll("DT", appendName("DT"));

	str_proc.replaceWordAll("sw_past", appendName("sw_past"));
	str_proc.replaceWordAll("current", appendName("current"));
	str_proc.replaceWordAll("current_past", appendName("current_past"));
	str_proc.replaceWordAll("sw", appendName("sw"));

	sstrm.str("");
	sstrm.clear();
	sstrm << "x["<<P<<"]";
	str_proc.replaceWordAll("epos", sstrm.str());

	sstrm.str("");
	sstrm.clear();
	sstrm << "x["<<N<<"]";
	str_proc.replaceWordAll("eneg", sstrm.str());

	sstrm.str("");
	sstrm.clear();
	sstrm << "b_components["<<source_id-1<<"]";
	str_proc.replaceWordAll("*bout", sstrm.str());

	return body;
}

std::string SeriesRLIdealSwitch::generateUpdateBodyRungeKutta4()
{
	std::stringstream sstrm;
	sstrm <<
	std::setprecision(16) <<
	std::fixed <<
	std::scientific;

	std::string body = SERIESRLIDEALSWITCH_GENERATEUPDATEBODY_BASE_RK4_STRING;
	lblmc::StringProcessor str_proc(body);

	str_proc.replaceWordAll("NumType", "real");

	str_proc.replaceWordAll("ARK4", appendName("ARK4"));
	str_proc.replaceWordAll("BRK4", appendName("BRK4"));

	str_proc.replaceWordAll("sw_past", appendName("sw_past"));
	str_proc.replaceWordAll("current", appendName("current"));
	str_proc.replaceWordAll("current_past", appendName("current_past"));
	str_proc.replaceWordAll("sw", appendName("sw"));

	sstrm.str("");
	sstrm.clear();
	sstrm << "x["<<P<<"]";
	str_proc.replaceWordAll("epos", sstrm.str());

	sstrm.str("");
	sstrm.clear();
	sstrm << "x["<<N<<"]";
	str_proc.replaceWordAll("eneg", sstrm.str());

	sstrm.str("");
	sstrm.clear();
	sstrm << "b_components["<<source_id-1<<"]";
	str_proc.replaceWordAll("*bout", sstrm.str());

	return body;
}

} //namespace lblmc
