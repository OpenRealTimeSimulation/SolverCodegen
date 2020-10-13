/*

Copyright (C) 2020 Matthew Milton, Dhiman Chowdhury, Michele Difronzo

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

#include "codegen/components/DualActiveBridgeConverter_IdealSwitches.hpp"
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

DualActiveBridgeConverter_IdealSwitches::DualActiveBridgeConverter_IdealSwitches(std::string comp_name) :
	Component(comp_name),
	DT(1.0),
	R11(1.0),
	R22(1.0),
	R1(1.0),
	R2(1.0),
	RM(1.0),
	LM(1.0),
	C1(1.0),
	C2(1.0),
	L1(1.0),
	L2(1.0),
	N(1.0),
	P1(-1),N1(-1),P2(-2),N2(-1),
	source_id1(-1), source_id2(-1)
{
	if(comp_name.empty())
	{
		throw std::invalid_argument("DualActiveBridgeConverter_IdealSwitches::constructor(std::string): comp_name must be a valid, non-empty C++ label");
	}
}

DualActiveBridgeConverter_IdealSwitches::DualActiveBridgeConverter_IdealSwitches
(
	std::string comp_name,
	double dt,
	double r_input1,
	double r_input2,
	double r_transformer1,
	double r_transformer2,
	double r_magnetizing,
	double l_magnetizing,
	double c_filter1,
	double c_filter2,
	double l_transformer1,
	double l_transformer2,
	double turns_ratio
) :
	Component(comp_name),
	DT(dt),
	R11(r_input1),
	R22(r_input2),
	R1(r_transformer1),
	R2(r_transformer2),
	RM(r_magnetizing),
	LM(l_magnetizing),
	C1(c_filter1),
	C2(c_filter2),
	L1(l_transformer1),
	L2(l_transformer2),
	N(turns_ratio),
	P1(-1),N1(-1),P2(-2),N2(-1),
	source_id1(-1), source_id2(-1)
{
	if(comp_name.empty())
	{
		throw std::invalid_argument("DualActiveBridgeConverter_IdealSwitches::constructor(...): comp_name must be a valid, non-empty C++ label");
	}

	if
	(
		DT  <= 0.0  ||
		R11 <= 0.0  ||
		R22 <= 0.0  ||
		R1  <= 0.0  ||
		R2  <= 0.0  ||
		RM  <= 0.0  ||
		LM  <= 0.0  ||
		C1  <= 0.0  ||
		C2  <= 0.0  ||
		L1  <= 0.0  ||
		L2  <= 0.0  ||
		N   <= 0.0
	)
	{
		throw std::invalid_argument("DualActiveBridgeConverter_IdealSwitches::constructor(...): given parameters cannot be zero or negative");
	}
}

DualActiveBridgeConverter_IdealSwitches::DualActiveBridgeConverter_IdealSwitches(const DualActiveBridgeConverter_IdealSwitches& base) :
	Component(base),
	DT(base.DT),
	R11(base.R11),
	R22(base.R22),
	R1(base.R1),
	R2(base.R2),
	RM(base.RM),
	LM(base.LM),
	C1(base.C1),
	C2(base.C2),
	L1(base.L1),
	L2(base.L2),
	N(base.N),
	P1(base.P1),N1(base.N1),P2(base.P2),N2(base.N2),
	source_id1(base.source_id1), source_id2(base.source_id2)
{}

void DualActiveBridgeConverter_IdealSwitches::getSourceIds(std::vector<unsigned int>& ids) const
{
	ids = {source_id1,source_id2};
}

std::vector<unsigned int> DualActiveBridgeConverter_IdealSwitches::getSourceIds() const
{
	return std::vector<unsigned int>{source_id1,source_id2};
}

void DualActiveBridgeConverter_IdealSwitches::setParameters
(
	double dt,
	double r_input1,
	double r_input2,
	double r_transformer1,
	double r_transformer2,
	double r_magnetizing,
	double l_magnetizing,
	double c_filter1,
	double c_filter2,
	double l_transformer1,
	double l_transformer2,
	double turns_ratio
)
{
	if
	(
		dt             <= 0.0  ||
		r_input1       <= 0.0  ||
		r_input2       <= 0.0  ||
		r_transformer1 <= 0.0  ||
		r_transformer2 <= 0.0  ||
		r_magnetizing  <= 0.0  ||
		l_magnetizing  <= 0.0  ||
		c_filter1      <= 0.0  ||
		c_filter2      <= 0.0  ||
		l_transformer1 <= 0.0  ||
		l_transformer2 <= 0.0  ||
		turns_ratio    <= 0.0
	)
	{
		throw std::invalid_argument("DualActiveBridgeConverter_IdealSwitches::setParameters(...): given parameters cannot be zero or negative");
	}

	DT  = dt;
	R11 = r_input1;
	R22 = r_input2;
	R1  = r_transformer1;
	R2  = r_transformer2;
	RM  = r_magnetizing;
	LM  = l_magnetizing;
	C1  = c_filter1;
	C2  = c_filter2;
	L1  = l_transformer1;
	L2  = l_transformer2;
	N   = turns_ratio;
}

std::vector<std::string> DualActiveBridgeConverter_IdealSwitches::getSupportedOutputs() const
{
	return std::vector<std::string>
	{
		"V1out",
		"V2out",
		"I1out",
		"I2out",
		"I3out",
		"Vpout",
		"Vsout"
	};
}

void DualActiveBridgeConverter_IdealSwitches::stampConductance(SystemConductanceGenerator& gen)
{
	gen.stampConductance(1.0/R11, P1, N1);
	gen.stampConductance(1.0/R22, P2, N2);
}

void DualActiveBridgeConverter_IdealSwitches::stampSources(SystemSourceVectorGenerator& gen)
{
	source_id1 = gen.insertSource(P1, N1);
	source_id2 = gen.insertSource(P2, N2);
}

std::string DualActiveBridgeConverter_IdealSwitches::generateParameters()
{
	std::stringstream sstrm;
	sstrm <<
	std::setprecision(16) <<
	std::fixed <<
	std::scientific;

	//generateParameter(sstrm, )

	generateParameter(sstrm, "DT" , DT);
	generateParameter(sstrm, "R11", R11);
	generateParameter(sstrm, "R22", R22);
	generateParameter(sstrm, "R1" , R1);
	generateParameter(sstrm, "R2" , R2);
	generateParameter(sstrm, "RM" , RM);
	generateParameter(sstrm, "LM" , LM);
	generateParameter(sstrm, "C1" , C1);
	generateParameter(sstrm, "C2" , C2);
	generateParameter(sstrm, "L1" , L1);
	generateParameter(sstrm, "L2" , L2);
	generateParameter(sstrm, "N"  , N );

	double G11        = (1.0)/R11;
	double G22        = (1.0)/R22;
	double R11C1      = (2.0)/(R11*C1);
	double OneoverC1  = (1.0)/C1;
	double OneoverL1  = (1.0)/L1;
	double R1Rm       = R1 + RM;
	double R1RmL1     = ((2.0)*R1Rm)/L1;
	double RmL1       = ((2.0)*RM)/L1;
	double R22C2      = (2.0)/(R22*C2);
	double OneoverC2  = (1.0)/C2;
	double RmLm       = ((2.0)*RM)/LM;
	double NL2        = N*N*L2;
	double NR2        = N*N*R2;
	double RmNL2      = ((2.0)*RM)/NL2;
	double RmNR2      = RM + NR2;
	double RmNR2NL2   = ((2.0)*RmNR2)/NL2;
	double OneoverNL2 = (1.0)/NL2;

	generateParameter(sstrm, "G11",        G11       );
	generateParameter(sstrm, "G22",        G22       );
	generateParameter(sstrm, "R11C1",      R11C1     );
	generateParameter(sstrm, "OneoverC1",  OneoverC1 );
	generateParameter(sstrm, "OneoverL1",  OneoverL1 );
	generateParameter(sstrm, "R1Rm",       R1Rm      );
	generateParameter(sstrm, "R1RmL1",     R1RmL1    );
	generateParameter(sstrm, "RmL1",       RmL1      );
	generateParameter(sstrm, "R22C2",      R22C2     );
	generateParameter(sstrm, "OneoverC2",  OneoverC2 );
	generateParameter(sstrm, "RmLm",       RmLm      );
	generateParameter(sstrm, "NL2",        NL2       );
	generateParameter(sstrm, "NR2",        NR2       );
	generateParameter(sstrm, "RmNL2",      RmNL2     );
	generateParameter(sstrm, "RmNR2",      RmNR2     );
	generateParameter(sstrm, "RmNR2NL2",   RmNR2NL2  );
	generateParameter(sstrm, "OneoverNL2", OneoverNL2);

	return sstrm.str();
}

std::string DualActiveBridgeConverter_IdealSwitches::generateFields()
{
	std::stringstream sstrm;
	sstrm <<
	std::setprecision(16) <<
	std::fixed <<
	std::scientific;

	generatePersistentField(sstrm, "V1past", 0.0);
	generatePersistentField(sstrm, "V2past", 0.0);
	generatePersistentField(sstrm, "I1past", 0.0);
	generatePersistentField(sstrm, "I2past", 0.0);
	generatePersistentField(sstrm, "I3past", 0.0);
	generatePersistentField(sstrm, "Vprim" , 0.0);
	generatePersistentField(sstrm, "Vsec"  , 0.0);

	generateTemporaryField(sstrm, "V1", 0.0);
	generateTemporaryField(sstrm, "V2", 0.0);
	generateTemporaryField(sstrm, "I1", 0.0);
	generateTemporaryField(sstrm, "I2", 0.0);
	generateTemporaryField(sstrm, "I3", 0.0);
	generateTemporaryField(sstrm, "Vp", 0.0);
	generateTemporaryField(sstrm, "Vs", 0.0);
	generateTemporaryField(sstrm, "S", 0.0);
	generateTemporaryField(sstrm, "T", 0.0);
	generateTemporaryField(sstrm, "STC1",0.0);
	generateTemporaryField(sstrm, "SL1", 0.0);
	generateTemporaryField(sstrm, "SL2", 0.0);
	generateTemporaryField(sstrm, "STC2", 0.0);
	generateTemporaryField(sstrm, "Vdc1", 0.0);
	generateTemporaryField(sstrm, "Vdc2", 0.0);

	return sstrm.str();
}

std::string DualActiveBridgeConverter_IdealSwitches::generateInputs()
{
	std::stringstream sstrm;
	sstrm <<
	std::setprecision(16) <<
	std::fixed <<
	std::scientific;

	ArrayObject Sw("bool", appendName("Sw"), "", {8});

	sstrm << Sw.generateArgument();

	return sstrm.str();
}

std::string DualActiveBridgeConverter_IdealSwitches::generateOutputs(std::string output)
{
	std::stringstream sstrm;
	sstrm <<
	std::setprecision(16) <<
	std::fixed <<
	std::scientific;

	if(output == "ALL")
	{
		Object V1out("real&", appendName("V1out"), "");
		Object V2out("real&", appendName("V2out"), "");
		Object I1out("real&", appendName("I1out"), "");
		Object I2out("real&", appendName("I2out"), "");
		Object I3out("real&", appendName("I3out"), "");
		Object Vpout("real&", appendName("Vpout"), "");
		Object Vsout("real&", appendName("Vsout"), "");


		sstrm <<
		V1out.generateArgument() << ",\n" <<
		V2out.generateArgument() << ",\n" <<
		I1out.generateArgument() << ",\n" <<
		I2out.generateArgument() << ",\n" <<
		I3out.generateArgument() << ",\n" <<
		Vpout.generateArgument() << ",\n" <<
		Vsout.generateArgument();

		return sstrm.str();
	}
	else
	{
		return std::string();
	}

	return sstrm.str();
}

std::string DualActiveBridgeConverter_IdealSwitches::generateOutputsUpdateBody(std::string output)
{
	std::stringstream sstrm;
	sstrm <<
	std::setprecision(16) <<
	std::fixed <<
	std::scientific;

	std::string output_update_body =
R"(
	V1out = V1;
	V2out = V2;
	I1out = I1;
	I2out = I2;
	I3out = I3;
	Vpout = (Sw[0]*Sw[3]-Sw[1]*Sw[2])*V1;
	Vsout = (Sw[4]*Sw[7]-Sw[5]*Sw[6])*V2;
)";

	appendNameToWords
	(
		output_update_body,

		{
			"V1out",
			"V2out",
			"I1out",
			"I2out",
			"I3out",
			"Vpout",
			"Vsout",
			"V1",
			"V2",
			"I1",
			"I2",
			"I3",
			"Sw",
			"V1",
			"V2"
		}
	);

	if(output == "ALL")
	{
		sstrm <<
		output_update_body;
	}
	else
	{
		return std::string();
	}


	return sstrm.str();
}

const static std::string DUALACTIVEBRIDGECONVERTER_IDEALSWITCHES_GENERATEUPDATEBODY_BASE_STRING =
R"(
	//read past solutions

	Vdc1 = x[P1] - x[N1];
	Vdc2 = x[P2] - x[N2];

	//switching functions

	S = real(Sw[0]&&Sw[4]&&Sw[7]&&Sw[3]);
	T = -real(Sw[2]&&Sw[6]&&Sw[5]&&Sw[1]);

	STC1 = real((S+T)*OneoverC1);
	SL1  = real((Sw[0]*Sw[3]-Sw[1]*Sw[2])*OneoverL1);
	SL2  = real((Sw[5]*Sw[6]-Sw[4]*Sw[7])*OneoverNL2);
	STC2 = real((S+T)*OneoverC2);

	if ((S+T) == 1)
	{
	  	STC1 = OneoverC1;
	}
	if ((S+T) == -1)
	{
	   	STC1 = -OneoverC1;
	}
	if ((S+T) == 0)
	{
	   	STC1 = 0;
	}

	if ((Sw[0]*Sw[3]-Sw[1]*Sw[2]) == 1)
	{
	   	SL1 = OneoverL1;
	}
	if ((Sw[0]*Sw[3]-Sw[1]*Sw[2]) == -1)
	{
	   	SL1 = -OneoverL1;
	}
	if ((Sw[0]*Sw[3]-Sw[1]*Sw[2]) == 0)
	{
	   	SL1 = 0;
	}

	if ((Sw[5]*Sw[6]-Sw[4]*Sw[7]) == 1)
	{
	    SL2 = OneoverNL2;
	}
	if ((Sw[5]*Sw[6]-Sw[4]*Sw[7]) == -1)
	{
	    SL2 = -OneoverNL2;
	}
	if ((Sw[5]*Sw[6]-Sw[4]*Sw[7]) == 0)
	{
	    SL2 = 0;
	}

	if ((S+T) == 1)
	{
	    STC2 = OneoverC2;
	}
	if ((S+T) == -1)
	{
	    STC2 = -OneoverC2;
	}
	if ((S+T) == 0)
	{
	    STC2 = 0;
	}

	//update states

	V1 = V1past + DT * (-R11C1*V1past-STC1*I1past+R11C1*Vdc1);
	I1 = I1past + DT * (SL1*V1past-R1RmL1*I1past+RmL1*I2past+RmL1*I3past);
	V2 = V2past + DT * (-R22C2*V2past+STC2*I2past+R22C2*Vdc2);
	I2 = I2past + DT * (RmLm*I1past-RmLm*I2past-RmLm*I3past);
	I3 = I3past + DT * (RmNL2*I1past-RmNL2*I2past-RmNR2NL2*I3past+SL2*V2past);

	//store states

	V1past = V1;
	I1past = I1;
	V2past = V2;
	I2past = I2;
	I3past = I3;

	//update RC sources for b of Gx=b

	b1 = G11*V1;
	b2 = G22*V2;
)";

std::string DualActiveBridgeConverter_IdealSwitches::generateUpdateBody()
{
	std::string body = DUALACTIVEBRIDGECONVERTER_IDEALSWITCHES_GENERATEUPDATEBODY_BASE_STRING;

	//specialize code for instance of this component

	appendNameToWords
	(
		body,

		{
				//constants/parameters
			"DT",
			"R11",
			"R22",
			"R1",
			"R2",
			"Rm",
			"Lm",
			"C1",
			"C2",
			"L1",
			"L2",
			"N",
			"G11",
			"G22",
			"R11C1",
			"OneoverC1",
			"OneoverL1",
			"R1Rm",
			"R1RmL1",
			"RmL1",
			"R22C2",
			"OneoverC2",
			"RmLm",
			"NL2",
			"NR2",
			"RmNL2",
			"RmNR2",
			"RmNR2NL2",
			"OneoverNL2",
				//inputs
			"Sw",
				//outputs
			"V1out",
			"V2out",
			"I1out",
			"I2out",
			"I3out",
			"Vpout",
			"Vsout",
				//persistent variables
			"V1past",
			"V2past",
			"I1past",
			"I2past",
			"I3past",
			"Vprim",
			"Vsec",
				//temporary variables
			"V1",
			"V2",
			"I1",
			"I2",
			"I3",
			"Vp",
			"Vs",
			"S",
			"T",
			"STC1",
			"SL1",
			"SL2",
			"STC2",
			"Vdc1",
			"Vdc2"
		}
	);

	replaceTerminalConnectionNameWithIndex(body, "P1", P1);
	replaceTerminalConnectionNameWithIndex(body, "N1", N1);
	replaceTerminalConnectionNameWithIndex(body, "P2", P2);
	replaceTerminalConnectionNameWithIndex(body, "N2", N2);

	replaceSourceNameWithSourceContributionVector(body, "b1", source_id1);
	replaceSourceNameWithSourceContributionVector(body, "b2", source_id2);


	return body;
}


} //namespace lblmc

