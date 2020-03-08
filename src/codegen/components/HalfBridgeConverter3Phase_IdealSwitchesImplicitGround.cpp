/*

Copyright (C) 2019 Matthew Milton

This file is part of the LB-LMC Solver C++ Code Generation Library.

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

#include "codegen/components/HalfBridgeConverter3Phase_IdealSwitchesImplicitGround.hpp"
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



HalfBridgeConverter3Phase_IdealSwitchesImplicitGround::HalfBridgeConverter3Phase_IdealSwitchesImplicitGround(std::string comp_name) :
	Component(comp_name),
	DT(1.0), CAP(1.0), IND(1.0), RES(1.0),
	P(0), N(0), A(0), B(0), C(0),
	source_id_P(0), source_id_N(0),
	source_id_A(0), source_id_B(0), source_id_C(0)
{
	if(comp_name.empty())
	{
		throw std::invalid_argument("HalfBridgeConverter3Phase_IdealSwitchesImplicitGround::constructor(): comp_name must be a valid, non-empty C++ label");
	}
}

HalfBridgeConverter3Phase_IdealSwitchesImplicitGround::HalfBridgeConverter3Phase_IdealSwitchesImplicitGround
(
	std::string comp_name,
	double dt,
	double cap,
	double ind,
	double res
) :
	Component(comp_name),
	DT(dt), CAP(cap), IND(ind), RES(res),
	P(0), N(0), A(0), B(0), C(0),
	source_id_P(0), source_id_N(0),
	source_id_A(0), source_id_B(0), source_id_C(0)
{
	if(comp_name.empty())
	{
		throw std::invalid_argument("HalfBridgeConverter3Phase_IdealSwitchesImplicitGround::constructor(): comp_name must be a valid, non-empty C++ label");
	}

	if(DT <= 0.0 || CAP <= 0.0 || IND <= 0.0)
	{
		throw std::invalid_argument("HalfBridgeConverter3Phase_IdealSwitchesImplicitGround::constructor(): parameters dt, cap, and ind must be positive nonzero values");
	}
}

HalfBridgeConverter3Phase_IdealSwitchesImplicitGround::HalfBridgeConverter3Phase_IdealSwitchesImplicitGround(const HalfBridgeConverter3Phase_IdealSwitchesImplicitGround& base) :
	Component(comp_name),
	DT(base.DT), CAP(base.CAP), IND(base.IND), RES(base.RES),
	P(base.P), N(base.N), A(base.A), B(base.B), C(base.C),
	source_id_P(base.source_id_P), source_id_N(base.source_id_N),
	source_id_A(base.source_id_A), source_id_B(base.source_id_B), source_id_C(base.source_id_C)
{}

void HalfBridgeConverter3Phase_IdealSwitchesImplicitGround::getSourceIds(std::vector<unsigned int>& ids) const
{
	ids.clear();
	ids.push_back(source_id_P);
	ids.push_back(source_id_N);
	ids.push_back(source_id_A);
	ids.push_back(source_id_B);
	ids.push_back(source_id_C);
}

void HalfBridgeConverter3Phase_IdealSwitchesImplicitGround::stampConductance(SystemConductanceGenerator& gen)
{
	//const static double HOC = DT/CAP;
	//const static double HOL = DT/IND;

	gen.stampConductance(CAP_CONDUCTANCE, P, 0);
	gen.stampConductance(CAP_CONDUCTANCE, N, 0);
	gen.stampConductance(IND_CONDUCTANCE, A, 0);
	gen.stampConductance(IND_CONDUCTANCE, B, 0);
	gen.stampConductance(IND_CONDUCTANCE, C, 0);

}

void HalfBridgeConverter3Phase_IdealSwitchesImplicitGround::stampSources(SystemSourceVectorGenerator& gen)
{
	source_id_P = gen.insertSource(P,0);
	source_id_N = gen.insertSource(N,0);
	source_id_A = gen.insertSource(A,0);
	source_id_B = gen.insertSource(B,0);
	source_id_C = gen.insertSource(C,0);
}

std::string HalfBridgeConverter3Phase_IdealSwitchesImplicitGround::generateParameters()
{
	const static double HOC = DT/CAP;
	const static double HOL = DT/IND;

	std::stringstream sstrm;
	sstrm <<
	std::setprecision(16) <<
	std::fixed <<
	std::scientific;

//	sstrm <<
//	"const static "<<"real "<<appendName("DT")<<" = "<<DT<<";\n" <<
//	"const static "<<"real "<<appendName("CAP")<<" = "<<CAP<<";\n" <<
//	"const static "<<"real "<<appendName("IND")<<" = "<<IND<<";\n" <<
//	"const static "<<"real "<<appendName("RES")<<" = "<<RES<<";\n" <<
//	"const static "<<"real "<<appendName("HOC")<<" = "<<HOC<<";\n" <<
//	"const static "<<"real "<<appendName("HOL")<<" = "<<HOL<<";\n" <<
//	"const static "<<"real "<<appendName("CAP_CONDUCTANCE")<<" = "<<CAP_CONDUCTANCE<<";\n" <<
//	"const static "<<"real "<<appendName("IND_CONDUCTANCE")<<" = "<<IND_CONDUCTANCE<<";\n";
    generateParameter(sstrm, "DT" , DT);
    generateParameter(sstrm, "CAP", CAP);
    generateParameter(sstrm, "IND", IND);
    generateParameter(sstrm, "RES", RES);
    generateParameter(sstrm, "HOC", HOC);
    generateParameter(sstrm, "HOL", HOL);
    generateParameter(sstrm, "CAP_CONDUCTANCE", CAP_CONDUCTANCE);
    generateParameter(sstrm, "IND_CONDUCTANCE", IND_CONDUCTANCE);
	return sstrm.str();
}

std::string HalfBridgeConverter3Phase_IdealSwitchesImplicitGround::generateFields()
{
	std::stringstream sstrm;
	sstrm <<
	std::setprecision(16) <<
	std::fixed <<
	std::scientific;

	generateField(sstrm, "vc1", 0);
	generateField(sstrm, "vc2", 0);
	generateField(sstrm, "il1", 0);
	generateField(sstrm, "il2", 0);
	generateField(sstrm, "il3", 0);
	generateField(sstrm, "ipos", 0);
	generateField(sstrm, "ineg", 0);
	generateField(sstrm, "epos_past" , 0);
	generateField(sstrm, "eneg_past" , 0);
	generateField(sstrm, "eout1_past", 0);
	generateField(sstrm, "eout2_past", 0);
	generateField(sstrm, "eout3_past", 0);
	generateField(sstrm, "vc1_past", 0);
	generateField(sstrm, "vc2_past", 0);
	generateField(sstrm, "il1_past", 0);
	generateField(sstrm, "il2_past", 0);
	generateField(sstrm, "il3_past", 0);

	generateBoolField(sstrm, "sw1", false);
	generateBoolField(sstrm, "sw2", false);
	generateBoolField(sstrm, "sw3", false);

	return sstrm.str();

}

std::string HalfBridgeConverter3Phase_IdealSwitchesImplicitGround::generateInputs()
{
	std::stringstream sstrm;
	sstrm <<
	std::setprecision(16) <<
	std::fixed <<
	std::scientific;

	//append name to input ports
	codegen::ArrayObject sw_ctrl("bool", appendName("sw_ctrl"), "", {3});
	codegen::Object sw_en("bool", appendName("sw_en"), "");

	sstrm <<
	sw_ctrl.generateArgument() << ",\n" <<
	sw_en.generateArgument();

	//sstrm << "\n";


	return sstrm.str();
}

std::string HalfBridgeConverter3Phase_IdealSwitchesImplicitGround::generateOutputs(std::string output)
{
	std::stringstream sstrm;
	sstrm <<
	std::setprecision(16) <<
	std::fixed <<
	std::scientific;

	//append name to output ports
	//codegen::ArrayObject i_ind("real", appendName("i_ind"), "", {3});
	//codegen::ArrayObject i_cap("real", appendName("i_cap"), "", {2});

	//sstrm <<
	//i_ind.generateArgument();
	//sstrm << "\n";
	//code = sstrm.str();

	if(output == "ALL")
	{
		codegen::Object cp_voltage("real *", appendName("cp_voltage"), "");
		codegen::Object cn_voltage("real *", appendName("cn_voltage"), "");
		codegen::Object la_current("real *", appendName("la_current"), "");
		codegen::Object lb_current("real *", appendName("lb_current"), "");
		codegen::Object lc_current("real *", appendName("lc_current"), "");

		sstrm <<
		cp_voltage.generateArgument() << ",\n" <<
		cn_voltage.generateArgument() << ",\n" <<
		la_current.generateArgument() << ",\n" <<
		lb_current.generateArgument() << ",\n" <<
		lc_current.generateArgument() ;

		return sstrm.str();
	}
	else if(output == "cp_voltage")
	{
		codegen::Object cp_voltage("real *", appendName("cp_voltage"), "");
		return cp_voltage.generateArgument();
	}
	else if(output == "cn_voltage")
	{
		codegen::Object cn_voltage("real *", appendName("cn_voltage"), "");
		return cn_voltage.generateArgument();
	}
	else if(output == "la_current")
	{
		codegen::Object la_current("real *", appendName("la_current"), "");
		return la_current.generateArgument();
	}
	else if(output == "lb_current")
	{
		codegen::Object lb_current("real *", appendName("lb_current"), "");
		return lb_current.generateArgument();
	}
	else if(output == "lc_current")
	{
		codegen::Object lc_current("real *", appendName("lc_current"), "");
		return lc_current.generateArgument();
	}
	else
	{
		return std::string();
	}

	return sstrm.str();
}

std::string HalfBridgeConverter3Phase_IdealSwitchesImplicitGround::generateOutputsUpdateBody(std::string output)
{
	std::stringstream sstrm;
	sstrm <<
	std::setprecision(16) <<
	std::fixed <<
	std::scientific;

    if(output == "ALL")
	{
		sstrm <<
		generateCPVoltageOutputUpdateBody() <<
		generateCNVoltageOutputUpdateBody() <<
		generateLACurrentOutputUpdateBody() <<
		generateLBCurrentOutputUpdateBody() <<
		generateLCCurrentOutputUpdateBody() ;
	}
	else if(output == "cp_voltage")
	{
		return generateCPVoltageOutputUpdateBody();
	}
	else if(output == "cn_voltage")
	{
		return generateCNVoltageOutputUpdateBody();
	}
	else if(output == "la_current")
	{
		return generateLACurrentOutputUpdateBody();
	}
	else if(output == "lb_current")
	{
		return generateLBCurrentOutputUpdateBody();
	}
	else if(output == "lc_current")
	{
		return generateLCCurrentOutputUpdateBody();
	}
	else
	{
		return std::string();
	}

	return sstrm.str();
}

static const std::string HALFBRIDGECONVERTER3PHASE_GENERATEUPDATEBODY_BASE_STRING =
R"(
epos_past = epos;
eneg_past = eneg;
eout1_past = eout1;
eout2_past = eout2;
eout3_past = eout3;
il1_past = il1;
il2_past = il2;
il3_past = il3;
vc1_past = vc1;
vc2_past = vc2;
sw1 = sw_ctrl1;
sw2 = sw_ctrl2;
sw3 = sw_ctrl3;

	//a, b, c are for inductors, a#, b# are for caps
NumType a1, a2, a3, b1, b2, b3, a, b, c;

if(sw_en) //switches are enabled
{
	if(sw1)
	{
		a1 = il1_past;
		b1 = 0.0;
		a = vc1_past;
	}
	else
	{
		a1 = 0.0;
		b1 = il1_past;
		a = vc2_past;
	}

	if(sw2)
	{
		a2 = il2_past;
		b2 = 0.0;
		b = vc1_past;
	}
	else
	{
		a2 = 0.0;
		b2 = il2_past;
		b = vc2_past;
	}

	if(sw3)
	{
		a3 = il3_past;
		b3 = 0.0;
		c = vc1_past;
	}
	else
	{
		a3 = 0.0;
		b3 = il3_past;
		c = vc2_past;
	}
}
else	//both switches are to be off
{
	// assume there are anti-parallel diodes on switches

	if(il1_past > NumType(0.0))
	{
		a = vc2_past;
		a1 = 0.0;
		b1 = -il1_past;
	}
	else
	{
		if(il1_past < NumType(0.0))
			a = vc1_past;
		else
			a = eout1_past;
		a1 = il1_past;
		b1 = 0.0;
	}

	if(il2_past > NumType(0.0))
	{
		b = vc2_past;
		a2 = 0.0;
		b2 = -il2_past;
	}
	else
	{
		if(il2_past < NumType(0.0))
			b = vc1_past;
		else
			b = eout2_past;
		a2 = il2_past;
		b2 = 0.0;
	}

	if(il3_past > NumType(0.0))
	{
		c = vc2_past;
		a3 = 0.0;
		b3 = -il3_past;
	}
	else
	{
		if(il3_past < NumType(0.0))
			c = vc1_past;
		else
			c = eout3_past;
		a3 = il3_past;
		b3 = 0.0;
	}


}

ipos = cap_conduct*(NumType(epos_past) - NumType(vc1_past));
ineg = cap_conduct*(NumType(eneg_past) - NumType(vc2_past));

il1 = NumType(il1_past) + hol*( a - NumType(eout1_past) - res*(il1_past));
il2 = NumType(il2_past) + hol*( b - NumType(eout2_past) - res*(il2_past));
il3 = NumType(il3_past) + hol*( c - NumType(eout3_past) - res*(il3_past));

vc1 = hoc*(NumType(ipos) - a1 - a2 - a3) + NumType(vc1_past);
vc2 = hoc*(NumType(ineg) - b1 - b2 - b3) + NumType(vc2_past);

*bpos = (vc1)*cap_conduct;
*bneg = (vc2)*cap_conduct;
*bout1 = il1;
*bout2 = il2;
*bout3 = il3;
)";

std::string HalfBridgeConverter3Phase_IdealSwitchesImplicitGround::generateUpdateBody()
{

	std::stringstream sstrm;
	sstrm <<
	std::setprecision(16) <<
	std::fixed <<
	std::scientific;

		//specialize converter update body code for component instance

	std::string body = HALFBRIDGECONVERTER3PHASE_GENERATEUPDATEBODY_BASE_STRING;
	codegen::StringProcessor str_proc(body);

		//specialize data type

	str_proc.replaceWordAll("NumType", "real");

		//specialize constant parameters

	str_proc.replaceWordAll("cap_conduct", appendName("CAP_CONDUCTANCE") );
	str_proc.replaceWordAll("dt", appendName("DT") );
	str_proc.replaceWordAll("cap", appendName("CAP") );
	str_proc.replaceWordAll("ind", appendName("IND") );
	str_proc.replaceWordAll("res", appendName("RES") );
	str_proc.replaceWordAll("hol", appendName("HOL") );
	str_proc.replaceWordAll("hoc", appendName("HOC") );

		//specialize internal temp parameters
	str_proc.replaceWordAll("a1", appendName("a1") );
	str_proc.replaceWordAll("a2", appendName("a2") );
	str_proc.replaceWordAll("a3", appendName("a3") );
	str_proc.replaceWordAll("b1", appendName("b1") );
	str_proc.replaceWordAll("b2", appendName("b2") );
	str_proc.replaceWordAll("b3", appendName("b3") );
	str_proc.replaceWordAll("a", appendName("a") );
	str_proc.replaceWordAll("b", appendName("b") );
	str_proc.replaceWordAll("c", appendName("c") );

		//specialize states and fields
	str_proc.replaceWordAll("vc1", appendName("vc1") );
	str_proc.replaceWordAll("vc2", appendName("vc2") );
	str_proc.replaceWordAll("il1", appendName("il1") );
	str_proc.replaceWordAll("il2", appendName("il2") );
	str_proc.replaceWordAll("il3", appendName("il3") );
	str_proc.replaceWordAll("ipos", appendName("ipos") );
	str_proc.replaceWordAll("ineg", appendName("ineg") );
	str_proc.replaceWordAll("epos_past", appendName("epos_past") );
	str_proc.replaceWordAll("eneg_past", appendName("eneg_past") );
	str_proc.replaceWordAll("eout1_past", appendName("eout1_past") );
	str_proc.replaceWordAll("eout2_past", appendName("eout2_past") );
	str_proc.replaceWordAll("eout3_past", appendName("eout3_past") );
	str_proc.replaceWordAll("vc1_past", appendName("vc1_past") );
	str_proc.replaceWordAll("vc2_past", appendName("vc2_past") );
	str_proc.replaceWordAll("il1_past", appendName("il1_past") );
	str_proc.replaceWordAll("il2_past", appendName("il2_past") );
	str_proc.replaceWordAll("il3_past", appendName("il3_past") );
	str_proc.replaceWordAll("sw1", appendName("sw1") );
	str_proc.replaceWordAll("sw2", appendName("sw2") );
	str_proc.replaceWordAll("sw3", appendName("sw3") );

		//specialize solution inputs and outputs
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
	sstrm << "x["<<A<<"]";
	str_proc.replaceWordAll("eout1", sstrm.str());

	sstrm.str("");
	sstrm.clear();
	sstrm << "x["<<B<<"]";
	str_proc.replaceWordAll("eout2", sstrm.str());

	sstrm.str("");
	sstrm.clear();
	sstrm << "x["<<C<<"]";
	str_proc.replaceWordAll("eout3", sstrm.str());

	sstrm.str("");
	sstrm.clear();
	sstrm << "b_components["<<source_id_P-1<<"]";
	str_proc.replaceWordAll("*bpos", sstrm.str());

	sstrm.str("");
	sstrm.clear();
	sstrm << "b_components["<<source_id_N-1<<"]";
	str_proc.replaceWordAll("*bneg", sstrm.str());

	sstrm.str("");
	sstrm.clear();
	sstrm << "b_components["<<source_id_A-1<<"]";
	str_proc.replaceWordAll("*bout1", sstrm.str());

	sstrm.str("");
	sstrm.clear();
	sstrm << "b_components["<<source_id_B-1<<"]";
	str_proc.replaceWordAll("*bout2", sstrm.str());

	sstrm.str("");
	sstrm.clear();
	sstrm << "b_components["<<source_id_C-1<<"]";
	str_proc.replaceWordAll("*bout3", sstrm.str());

		//specialize signal inputs and outputs
	str_proc.replaceWordAll("sw_ctrl1", appendName("sw_ctrl")+std::string("[0]"));
	str_proc.replaceWordAll("sw_ctrl2", appendName("sw_ctrl")+std::string("[1]"));
	str_proc.replaceWordAll("sw_ctrl3", appendName("sw_ctrl")+std::string("[2]"));
	str_proc.replaceWordAll("sw_en", appendName("sw_en"));

	return body;
}

std::string HalfBridgeConverter3Phase_IdealSwitchesImplicitGround::generateCPVoltageOutputUpdateBody()
{
	std::stringstream sstrm;
	sstrm <<
	std::setprecision(16) <<
	std::fixed <<
	std::scientific;

	sstrm << appendName("*cp_voltage") << " = " << appendName("vc1") << ";\n\n";

	return sstrm.str();
}

std::string HalfBridgeConverter3Phase_IdealSwitchesImplicitGround::generateCNVoltageOutputUpdateBody()
{
	std::stringstream sstrm;
	sstrm <<
	std::setprecision(16) <<
	std::fixed <<
	std::scientific;

	sstrm << appendName("*cn_voltage") << " = " << appendName("vc2") << ";\n\n";

	return sstrm.str();
}

std::string HalfBridgeConverter3Phase_IdealSwitchesImplicitGround::generateLACurrentOutputUpdateBody()
{
	std::stringstream sstrm;
	sstrm <<
	std::setprecision(16) <<
	std::fixed <<
	std::scientific;

	sstrm << appendName("*la_current") << " = " << appendName("il1") << ";\n\n";

	return sstrm.str();
}

std::string HalfBridgeConverter3Phase_IdealSwitchesImplicitGround::generateLBCurrentOutputUpdateBody()
{
	std::stringstream sstrm;
	sstrm <<
	std::setprecision(16) <<
	std::fixed <<
	std::scientific;

	sstrm << appendName("*lb_current") << " = " << appendName("il2") << ";\n\n";

	return sstrm.str();
}

std::string HalfBridgeConverter3Phase_IdealSwitchesImplicitGround::generateLCCurrentOutputUpdateBody()
{
	std::stringstream sstrm;
	sstrm <<
	std::setprecision(16) <<
	std::fixed <<
	std::scientific;

	sstrm << appendName("*lc_current") << " = " << appendName("il3") << ";\n\n";

	return sstrm.str();
}

} //namespace lblmc
