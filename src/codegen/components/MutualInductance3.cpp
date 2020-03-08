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

#include "codegen/components/MutualInductance3.hpp"
#include "codegen/SystemConductanceGenerator.hpp"
#include "codegen/SystemSourceVectorGenerator.hpp"
#include "codegen/SimulationEngineGenerator.hpp"
#include "codegen/Object.hpp"
#include "codegen/ArrayObject.hpp"
#include "codegen/StringProcessor.hpp"
#include <string>
#include <stdexcept>
#include <sstream>
#include <iomanip>

namespace lblmc
{

MutualInductance3::MutualInductance3(std::string comp_name) :
	Component(comp_name),
	DT(0), L1(0), L2(0), L3(0), M12(0), M23(0), M31(0), D(0),
	K1(0), K2(0), K3(0), K4(0), K5(0), K6(0), K7(0), K8(0), K9(0),
	PA(0), NA(0), PB(0), NB(0), PC(0), NC(0),
	source_id_A(0), source_id_B(0), source_id_C(0)
{}

MutualInductance3::MutualInductance3
(
	std::string comp_name,
	double dt,
	double l1,
	double l2,
	double l3,
	double m12,
	double m23,
	double m31
) :
	Component(comp_name),
	DT(dt), L1(l1), L2(l2), L3(l3), M12(m12), M23(m23), M31(m31), D(0),
	K1(0), K2(0), K3(0), K4(0), K5(0), K6(0), K7(0), K8(0), K9(0),
	PA(0), NA(0), PB(0), NB(0), PC(0), NC(0),
	source_id_A(0), source_id_B(0), source_id_C(0)
{
	D = ( DT / (L3*M12*M12 - (2.0)*M12*M23*M31 + L1*M23*M23 + L2*M31*M31 - L1*L2*L3 ) );
    K1 = (M23*M23 - L2*L3  );
    K2 = (L3*M12  - M23*M31);
    K3 = (L2*M31  - M12*M23);
    K4 = (L3*M12  - M23*M31);
    K5 = (M31*M31 - L1*L3  );
    K6 = (L1*M23  - M12*M31);
    K7 = (L2*M31  - M12*M23);
    K8 = (L1*M23  - M12*M31);
    K9 = (M12*M12 - L1*L2  );

}

MutualInductance3::MutualInductance3(const MutualInductance3& base) :
	Component(base),
	DT(base.DT), L1(base.L1), L2(base.L2), L3(base.L3), M12(base.M12), M23(base.M23), M31(base.M31),
	D(base.D),
	K1(base.K1), K2(base.K2), K3(base.K3), K4(base.K4), K5(base.K5), K6(base.K6), K7(base.K7),
	K8(base.K8), K9(base.K9),
	PA(base.PA), NA(base.NA), PB(base.PB), NB(base.NB), PC(base.PC), NC(base.NC),
	source_id_A(base.source_id_A), source_id_B(base.source_id_B), source_id_C(base.source_id_C)
{}

void MutualInductance3::getSourceIds(std::vector<unsigned int>& ids) const
{
	ids.clear();
	ids.push_back(source_id_A);
	ids.push_back(source_id_B);
	ids.push_back(source_id_C);
}

void MutualInductance3::getResistiveCompanionElements(std::vector<ResistiveCompanionElement>& elements) const
{
	elements.clear();

    RCE rce_a{ appendName(std::string("bout1")), PA, NA, 0.0, 0.0, source_id_A, false, false, false};
    RCE rce_b{ appendName(std::string("bout2")), PB, NB, 0.0, 0.0, source_id_B, false, false, false};
    RCE rce_c{ appendName(std::string("bout3")), PC, NC, 0.0, 0.0, source_id_C, false, false, false};

    elements.push_back(rce_a);
    elements.push_back(rce_b);
    elements.push_back(rce_c);
}

void MutualInductance3::setParameters
(
	double dt,
	double l1,
	double l2,
	double l3,
	double m12,
	double m23,
	double m31
)
{
	DT = dt; L1 = l1; L2 = l2; L3 = l3; M12 = m12; M23 = m23; M31 = m31;

	D = ( DT / (L3*M12*M12 - (2.0)*M12*M23*M31 + L1*M23*M23 + L2*M31*M31 - L1*L2*L3 ) );
    K1 = (M23*M23 - L2*L3  );
    K2 = (L3*M12  - M23*M31);
    K3 = (L2*M31  - M12*M23);
    K4 = (L3*M12  - M23*M31);
    K5 = (M31*M31 - L1*L3  );
    K6 = (L1*M23  - M12*M31);
    K7 = (L2*M31  - M12*M23);
    K8 = (L1*M23  - M12*M31);
    K9 = (M12*M12 - L1*L2  );
}

void MutualInductance3::stampConductance(SystemConductanceGenerator& gen)
{
	//do nothing since using an explicit integration method with no step conductance
}

void MutualInductance3::stampSources(SystemSourceVectorGenerator& gen)
{

	source_id_A = gen.insertSource(PA, NA);
	source_id_B = gen.insertSource(PB, NB);
	source_id_C = gen.insertSource(PC, NC);

}

std::string MutualInductance3::generateParameters()
{
	std::stringstream sstrm;
	sstrm <<
	std::setprecision(16) <<
	std::fixed <<
	std::scientific;

	generateParameter(sstrm, "DT",  DT);
	generateParameter(sstrm, "L1",  L1);
	generateParameter(sstrm, "L2",  L2);
	generateParameter(sstrm, "L3",  L3);
	generateParameter(sstrm, "M12", M12);
	generateParameter(sstrm, "M23", M23);
	generateParameter(sstrm, "M31", M31);
	generateParameter(sstrm, "D",   D );
	generateParameter(sstrm, "K1",  K1);
	generateParameter(sstrm, "K2",  K2);
	generateParameter(sstrm, "K3",  K3);
	generateParameter(sstrm, "K4",  K4);
	generateParameter(sstrm, "K5",  K5);
	generateParameter(sstrm, "K6",  K6);
	generateParameter(sstrm, "K7",  K7);
	generateParameter(sstrm, "K8",  K8);
	generateParameter(sstrm, "K9",  K9);

	return sstrm.str();
}

std::string MutualInductance3::generateFields()
{
	std::stringstream sstrm;
	sstrm <<
	std::setprecision(16) <<
	std::fixed <<
	std::scientific;

	generateField(sstrm, "voltage1", 0);
	generateField(sstrm, "voltage2", 0);
	generateField(sstrm, "voltage3", 0);
	generateField(sstrm, "current1", 0);
	generateField(sstrm, "current2", 0);
	generateField(sstrm, "current3", 0);
	generateField(sstrm, "current_comp1", 0);
	generateField(sstrm, "current_comp2", 0);
	generateField(sstrm, "current_comp3", 0);

	return sstrm.str();
}

static const std::string MUTUALINDUCTANCE3_GENERATEUPDATEBODY_BASE_STRING =
R"(
voltage1 = epos1 - eneg1;
voltage2 = epos2 - eneg2;
voltage3 = epos3 - eneg3;

current_comp1 = current_comp1 - D*( K1*voltage1 + K2*voltage2 + K3*voltage3 );
current_comp2 = current_comp2 - D*( K4*voltage1 + K5*voltage2 + K6*voltage3 );
current_comp3 = current_comp3 - D*( K7*voltage1 + K8*voltage2 + K9*voltage3 );

*bout1 = current_comp1;
*bout2 = current_comp2;
*bout3 = current_comp3;
)";

std::string MutualInductance3::generateUpdateBody()
{
	std::stringstream sstrm;
	sstrm <<
	std::setprecision(16) <<
	std::fixed <<
	std::scientific;

	std::string body = MUTUALINDUCTANCE3_GENERATEUPDATEBODY_BASE_STRING;
	lblmc::StringProcessor proc(body);

	proc.replaceWordAll("D", appendName("D") );
	proc.replaceWordAll("K1", appendName("K1") );
	proc.replaceWordAll("K2", appendName("K2") );
	proc.replaceWordAll("K3", appendName("K3") );
	proc.replaceWordAll("K4", appendName("K4") );
	proc.replaceWordAll("K5", appendName("K5") );
	proc.replaceWordAll("K6", appendName("K6") );
	proc.replaceWordAll("K7", appendName("K7") );
	proc.replaceWordAll("K8", appendName("K8") );
	proc.replaceWordAll("K9", appendName("K9") );

	proc.replaceWordAll("current_comp1", appendName("current_comp1") );
	proc.replaceWordAll("current_comp2", appendName("current_comp2") );
	proc.replaceWordAll("current_comp3", appendName("current_comp3") );
	proc.replaceWordAll("voltage1", appendName("voltage1") );
	proc.replaceWordAll("voltage2", appendName("voltage2") );
	proc.replaceWordAll("voltage3", appendName("voltage3") );

	sstrm.str("");
	sstrm.clear();
	sstrm << "x["<<PA<<"]";
	proc.replaceWordAll("epos1", sstrm.str());

	sstrm.str("");
	sstrm.clear();
	sstrm << "x["<<NA<<"]";
	proc.replaceWordAll("eneg1", sstrm.str());

	sstrm.str("");
	sstrm.clear();
	sstrm << "x["<<PB<<"]";
	proc.replaceWordAll("epos2", sstrm.str());

	sstrm.str("");
	sstrm.clear();
	sstrm << "x["<<NB<<"]";
	proc.replaceWordAll("eneg2", sstrm.str());

	sstrm.str("");
	sstrm.clear();
	sstrm << "x["<<PC<<"]";
	proc.replaceWordAll("epos3", sstrm.str());

	sstrm.str("");
	sstrm.clear();
	sstrm << "x["<<NC<<"]";
	proc.replaceWordAll("eneg3", sstrm.str());

	sstrm.str("");
	sstrm.clear();
	sstrm << "b_components["<<source_id_A-1<<"]";
	proc.replaceWordAll("*bout1", sstrm.str());

	sstrm.str("");
	sstrm.clear();
	sstrm << "b_components["<<source_id_B-1<<"]";
	proc.replaceWordAll("*bout2", sstrm.str());

	sstrm.str("");
	sstrm.clear();
	sstrm << "b_components["<<source_id_C-1<<"]";
	proc.replaceWordAll("*bout3", sstrm.str());

	return body;
}

} //namespace lblmc
