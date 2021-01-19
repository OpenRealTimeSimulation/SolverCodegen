/*

Copyright (C) 2019-2020 Michele Difronzo, Matthew Milton

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

#include "codegen/components/ModularMultilevelConverter_HalfBridgeModules.hpp"
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



ModularMultilevelConverter_HalfBridgeModules::ModularMultilevelConverter_HalfBridgeModules(std::string comp_name) :
	Component(comp_name),
	MMC_LEVELS(2),
	DT(1.0),
	RB(1.0),
	RARM(1.0),
	LARM(1.0),
	SUBMOD_CAP(1.0),
	DTOC(1.0),
	DTOL(1.0),
	LODT(1.0),
	INVRFC(1.0),
	NUM_ARM_SUBMOD(1),
	CAP_SUBMOD_INIT(1.0),
	P(0),
	N(0),
	A(0),
	B(0),
	C(0),
	source_id_P(0),
	source_id_N(0),
	source_id_A(0),
	source_id_B(0),
	source_id_C(0)
{
	if(comp_name.empty())
	{
		throw std::invalid_argument("ModularMultilevelConverter_HalfBridgeModules::constructor(): comp_name must be a valid, non-empty C++ label");
	}
}

ModularMultilevelConverter_HalfBridgeModules::ModularMultilevelConverter_HalfBridgeModules
(
	std::string comp_name,
	double dt,
	double bleeding_resistance,
	double arm_inductance,
	double arm_resistance,
	double switching_module_capacitance,
	double initial_cap_voltage,
	unsigned int number_of_levels
) :
	Component(comp_name),
	MMC_LEVELS(2),
	DT(1.0),
	RB(1.0),
	RARM(1.0),
	LARM(1.0),
	SUBMOD_CAP(1.0),
	DTOC(1.0),
	DTOL(1.0),
	LODT(1.0),
	INVRFC(1.0),
	NUM_ARM_SUBMOD(1),
	CAP_SUBMOD_INIT(1.0),
	P(0),
	N(0),
	A(0),
	B(0),
	C(0),
	source_id_P(0),
	source_id_N(0),
	source_id_A(0),
	source_id_B(0),
	source_id_C(0)
{
	if(comp_name.empty())
	{
		throw std::invalid_argument("ModularMultilevelConverter_HalfBridgeModules::constructor(): comp_name must be a valid, non-empty C++ label");
	}

	if(dt <= 0.0 || bleeding_resistance <= 0.0 || arm_inductance <= 0.0 || switching_module_capacitance <= 0.0 || bleeding_resistance <= 0.0)
	{
		throw std::invalid_argument("ModularMultilevelConverter_HalfBridgeModules::constructor(): parameters dt, bleeding_resistance, arm_inductance, switching_module_capacitance, and bleeding_resistance must be positive nonzero values");
	}

	if(number_of_levels < 2)
	{
		throw std::invalid_argument("ModularMultilevelConverter_HalfBridgeModules::constructor(): parameter num_of_levels must be 2 or greater");
	}

	MMC_LEVELS = number_of_levels;
	DT = dt;
	RB = bleeding_resistance;
	RARM = arm_resistance;
	LARM = arm_inductance;
	SUBMOD_CAP = switching_module_capacitance;
	DTOC = dt/switching_module_capacitance;
	DTOL = dt/arm_inductance;
	LODT = arm_inductance / dt;
	INVRFC = 1.0/(bleeding_resistance*switching_module_capacitance);
	NUM_ARM_SUBMOD = number_of_levels - 1;
	CAP_SUBMOD_INIT = initial_cap_voltage;
}

ModularMultilevelConverter_HalfBridgeModules::ModularMultilevelConverter_HalfBridgeModules(const ModularMultilevelConverter_HalfBridgeModules& base) :
	Component(base),
	MMC_LEVELS(base.MMC_LEVELS),
	DT(base.DT), RB(base.RB), RARM(base.RARM), LARM(base.LARM), SUBMOD_CAP(base.SUBMOD_CAP),
	DTOC(base.DTOC), DTOL(base.DTOL), LODT(base.LODT), INVRFC(base.INVRFC),
	NUM_ARM_SUBMOD(base.NUM_ARM_SUBMOD), CAP_SUBMOD_INIT(base.CAP_SUBMOD_INIT),
	P(base.P), N(base.N), A(base.A), B(base.B), C(base.C),
	source_id_P(base.source_id_P), source_id_N(base.source_id_N),
	source_id_A(base.source_id_A), source_id_B(base.source_id_B), source_id_C(base.source_id_C)
{}

void ModularMultilevelConverter_HalfBridgeModules::getSourceIds(std::vector<unsigned int>& ids) const
{
	ids.clear();
	ids.push_back(source_id_P);
	ids.push_back(source_id_N);
	ids.push_back(source_id_A);
	ids.push_back(source_id_B);
	ids.push_back(source_id_C);
}

void ModularMultilevelConverter_HalfBridgeModules::getResistiveCompanionElements(std::vector<ResistiveCompanionElement>& elements) const
{
    elements.clear();

    RCE rce_p{ appendName(std::string("bpos")),  P, 0, 0.0, 0.0, source_id_P, false, false, false };
    RCE rce_n{ appendName(std::string("bneg")),  N, 0, 0.0, 0.0, source_id_N, false, false, false };
    RCE rce_a{ appendName(std::string("bout1")), A, 0, 0.0, 0.0, source_id_A, false, false, false };
    RCE rce_b{ appendName(std::string("bout2")), B, 0, 0.0, 0.0, source_id_B, false, false, false };
    RCE rce_c{ appendName(std::string("bout3")), C, 0, 0.0, 0.0, source_id_C, false, false, false };

    elements.push_back(rce_p);
    elements.push_back(rce_n);
    elements.push_back(rce_a);
    elements.push_back(rce_b);
    elements.push_back(rce_c);
}

void ModularMultilevelConverter_HalfBridgeModules::setParameters
(
	double dt,
	double bleeding_resistance,
	double arm_inductance,
	double arm_resistance,
	double switching_module_capacitance,
	double initial_arm_voltage,
	unsigned int number_of_levels
)
{
	if(dt <= 0.0 || bleeding_resistance <= 0.0 || arm_inductance <= 0.0 || switching_module_capacitance <= 0.0 || bleeding_resistance <= 0.0)
	{
		throw std::invalid_argument("ModularMultilevelConverter_HalfBridgeModules::setParameters(): parameters dt, bleeding_resistance, arm_inductance, switching_module_capacitance, and bleeding_resistance must be positive nonzero values");
	}

	if(number_of_levels < 2)
	{
		throw std::invalid_argument("ModularMultilevelConverter_HalfBridgeModules::setParameters(): parameter num_of_levels must be 2 or greater");
	}

	MMC_LEVELS = number_of_levels;
	DT = dt;
	RB = bleeding_resistance;
	RARM = arm_resistance;
	LARM = arm_inductance;
	SUBMOD_CAP = switching_module_capacitance;
	DTOC = dt/switching_module_capacitance;
	DTOL = dt/arm_inductance;
	LODT = arm_inductance / dt;
	INVRFC = 1.0/(bleeding_resistance*switching_module_capacitance);
	NUM_ARM_SUBMOD = number_of_levels - 1;
	CAP_SUBMOD_INIT = 2.0*initial_arm_voltage/double(number_of_levels-1);
}

void ModularMultilevelConverter_HalfBridgeModules::stampConductance(SystemConductanceGenerator& gen)
{
	//no conductances to stamp
}

void ModularMultilevelConverter_HalfBridgeModules::stampSources(SystemSourceVectorGenerator& gen)
{
	source_id_P = gen.insertSource(P,0);
	source_id_N = gen.insertSource(N,0);
	source_id_A = gen.insertSource(A,0);
	source_id_B = gen.insertSource(B,0);
	source_id_C = gen.insertSource(C,0);
}

std::string ModularMultilevelConverter_HalfBridgeModules::generateParameters()
{

	std::stringstream sstrm;
	sstrm <<
	std::setprecision(16) <<
	std::fixed <<
	std::scientific;

	generateTypedParameter<unsigned int>(sstrm, "unsigned int","MMC_LEVELS", MMC_LEVELS);
    generateParameter(sstrm, "DT", DT);
    generateParameter(sstrm, "RB", RB);
    generateParameter(sstrm, "INV_BLEEDING_RES", 1.0/RB);
    generateParameter(sstrm, "RARM", RARM);
	generateParameter(sstrm, "LARM", LARM);
    generateParameter(sstrm, "SUBMOD_CAP", SUBMOD_CAP);
    generateParameter(sstrm, "DTOC", DTOC);
    generateParameter(sstrm, "DTOL", DTOL);
    generateParameter(sstrm, "LODT", LODT);
    generateParameter(sstrm, "INVRFC", INVRFC);
    generateTypedParameter<unsigned int>(sstrm, "unsigned int","NUM_ARM_SUBMOD", NUM_ARM_SUBMOD);
    generateParameter(sstrm, "CAP_SUBMOD_INIT", CAP_SUBMOD_INIT);

	return sstrm.str();
}

std::string ModularMultilevelConverter_HalfBridgeModules::generateFields()
{
	std::stringstream sstrm;
	sstrm <<
	std::setprecision(16) <<
	std::fixed <<
	std::scientific;

    generateField(sstrm, "Rpre", 220.0);
    generateField(sstrm, "a", 0.0);
    generateTypedArrayField<double>(sstrm, "real", "mula", 2*NUM_ARM_SUBMOD);
    generateTypedArrayField<double>(sstrm, "real", "mulb", 2*NUM_ARM_SUBMOD);
    generateTypedArrayField<double>(sstrm, "real", "mulc", 2*NUM_ARM_SUBMOD);

#if 0
	generateTypedArrayField<double>(sstrm, "real", "Vca", 2*NUM_ARM_SUBMOD, CAP_SUBMOD_INIT);
	generateTypedArrayField<double>(sstrm, "real", "Vcb", 2*NUM_ARM_SUBMOD, CAP_SUBMOD_INIT);
	generateTypedArrayField<double>(sstrm, "real", "Vcc", 2*NUM_ARM_SUBMOD, CAP_SUBMOD_INIT);
#endif

	auto generateVcxArrayField = [&](const std::string& name)
	{
		if(NUM_ARM_SUBMOD < 1)
		{
			throw std::invalid_argument
			(
				"ModularMultilevelConverter_HalfBridgeModules::generateFields()::generateVcxArrayField() -- NUM_ARM_SUBMOD cannot be less than 1"
			);
		}

		sstrm
		<< "static real "
		<< appendName(name)
		<< "["
		<< 2*NUM_ARM_SUBMOD
		<< "] = { "
		<< CAP_SUBMOD_INIT
		;

		for(unsigned long i = 1; i < 2*NUM_ARM_SUBMOD; i++)
		{
			double init_value = 0.0;

			if(i < NUM_ARM_SUBMOD )
			{
				init_value = CAP_SUBMOD_INIT;
			}
			else
			{
				init_value = -CAP_SUBMOD_INIT;
			}

			sstrm << "," << init_value;
		}

		sstrm << " };\n";

	};

	generateVcxArrayField("Vca");
	generateVcxArrayField("Vcb");
	generateVcxArrayField("Vcc");

	generateTypedTemporary<double>(sstrm, "real", "upa", 0.0);
	generateTypedTemporary<double>(sstrm, "real", "upb", 0.0);
	generateTypedTemporary<double>(sstrm, "real", "upc", 0.0);
	generateTypedTemporary<double>(sstrm, "real", "lowa", 0.0);
	generateTypedTemporary<double>(sstrm, "real", "lowb", 0.0);
	generateTypedTemporary<double>(sstrm, "real", "lowc", 0.0);

	generateField(sstrm, "Ilupapast"    , 0.0);
	generateField(sstrm, "Ilupbpast"    , 0.0);
	generateField(sstrm, "Ilupcpast"    , 0.0);
	generateField(sstrm, "Illowapast"   , 0.0);
	generateField(sstrm, "Illowbpast"   , 0.0);
	generateField(sstrm, "Illowcpast"	, 0.0);
	generateField(sstrm, "Ilupa"        , 0.0);
	generateField(sstrm, "Ilupb"        , 0.0);
	generateField(sstrm, "Ilupc"        , 0.0);
	generateField(sstrm, "Illowa"       , 0.0);
	generateField(sstrm, "Illowb"       , 0.0);
	generateField(sstrm, "Illowc"		, 0.0);
	generateField(sstrm, "Ic_upa"       , 0.0);
	generateField(sstrm, "Ic_upb"       , 0.0);
	generateField(sstrm, "Ic_upc"       , 0.0);
	generateField(sstrm, "Ic_lowa"      , 0.0);
	generateField(sstrm, "Ic_lowb"      , 0.0);
	generateField(sstrm, "Ic_lowc"		, 0.0);

	generateField(sstrm, "Vup"   , 0.0);
	generateField(sstrm, "Vlow"  , 0.0);
	generateField(sstrm, "Vouta" , 0.0);
	generateField(sstrm, "Voutb" , 0.0);
	generateField(sstrm, "Voutc" , 0.0);

	return sstrm.str();

}

std::string ModularMultilevelConverter_HalfBridgeModules::generateInputs()
{
	std::stringstream sstrm;
	sstrm <<
	std::setprecision(16) <<
	std::fixed <<
	std::scientific;

	Object     swp("bool", appendName("swp"), "");
	ArrayObject Sa("bool", appendName("Sa"), "", {2*NUM_ARM_SUBMOD});
	ArrayObject Sb("bool", appendName("Sb"), "", {2*NUM_ARM_SUBMOD});
	ArrayObject Sc("bool", appendName("Sc"), "", {2*NUM_ARM_SUBMOD});

	sstrm <<
	swp.generateArgument() << ",\n" <<
	Sa.generateArgument() << ",\n" <<
	Sb.generateArgument() << ",\n" <<
	Sc.generateArgument();

	return sstrm.str();
}

std::string ModularMultilevelConverter_HalfBridgeModules::generateOutputs(std::string output)
{
	std::stringstream sstrm;
	sstrm <<
	std::setprecision(16) <<
	std::fixed <<
	std::scientific;

	if(output == "ALL")
	{
		Object la_up_current  ("real *", appendName("la_up_current") , "");   //*Ilupaout
		Object lb_up_current  ("real *", appendName("lb_up_current") , "");   //*Ilupbout
		Object lc_up_current  ("real *", appendName("lc_up_current") , "");   //*Ilupcout
		Object la_low_current ("real *", appendName("la_low_current"), "");    //*Illowaout
		Object lb_low_current ("real *", appendName("lb_low_current"), "");    //*Illowbout
		Object lc_low_current ("real *", appendName("lc_low_current"), "");    //*Illowcout
		ArrayObject cap_a_voltages ("real", appendName("cap_a_voltages"), "", {2*NUM_ARM_SUBMOD}); //Vcaout[i]
		ArrayObject cap_b_voltages ("real", appendName("cap_b_voltages"), "", {2*NUM_ARM_SUBMOD}); //Vcbout[i]
		ArrayObject cap_c_voltages ("real", appendName("cap_c_voltages"), "", {2*NUM_ARM_SUBMOD}); //Vccout[i]

		sstrm <<
		la_up_current.generateArgument()  << ",\n" <<
		lb_up_current.generateArgument()  << ",\n" <<
		lc_up_current.generateArgument()  << ",\n" <<
		la_low_current.generateArgument() << ",\n" <<
		lb_low_current.generateArgument() << ",\n" <<
		lc_low_current.generateArgument() << ",\n" <<
		cap_a_voltages.generateArgument() << ",\n" <<
		cap_b_voltages.generateArgument() << ",\n" <<
		cap_c_voltages.generateArgument();

		return sstrm.str();
	}
	else
	{
		throw std::runtime_error("ModularMultilevelConverter_HalfBridgeModules::generateOutputs() -- given output is not supported; only \"ALL\" is supported");
	}

	return sstrm.str();
}

std::string ModularMultilevelConverter_HalfBridgeModules::generateOutputsUpdateBody(std::string output)
{
	std::stringstream sstrm;
	sstrm <<
	std::setprecision(16) <<
	std::fixed <<
	std::scientific;

	std::string output_update_body =
R"(
//update outputs
	*la_up_current  = Ilupa;
	*lb_up_current  = Ilupb;
	*lc_up_current  = Ilupc;
	*la_low_current = Illowa;
	*lb_low_current = Illowb;
	*lc_low_current = Illowc;

	for(unsigned int i = 0; i < 2*NUM_ARM_SUBMOD; i++)
	{
		#pragma LBLMC_UNROLL

		cap_a_voltages[i] = Vca[i];
		cap_b_voltages[i] = Vcb[i];
		cap_c_voltages[i] = Vcc[i];
	}
)";

    if(output == "ALL")
	{
		appendNameToWords
		(
			output_update_body,
			{
				"la_up_current",
				"lb_up_current",
				"lc_up_current",
				"la_low_current",
				"lb_low_current",
				"lc_low_current",
				"Ilupa",
				"Ilupb",
				"Ilupc",
				"Illowa",
				"Illowb",
				"Illowc",
				"cap_a_voltages",
				"cap_b_voltages",
				"cap_c_voltages",
				"Vca",
				"Vcb",
				"Vcc",
				"NUM_ARM_SUBMOD"
			}
		);


		sstrm <<
		output_update_body;
	}
	else
	{
		throw std::runtime_error("ModularMultilevelConverter_HalfBridgeModules::generateOutputsUpdateBody() -- given output is not supported; only \"ALL\" is supported");
	}

	return sstrm.str();
}

static const std::string MODULARMULTILEVELCONVERTER_HALFBRIDGEMODULES_GENERATEUPDATEBODY_BASE_STRING =
R"(
	Vup   = x[P];
	Vlow  = x[N];
	Vouta = x[A];
	Voutb = x[B];
	Voutc = x[C];

	Ilupapast = Ilupa;
    Ilupbpast = Ilupb;
    Ilupcpast = Ilupc;
    Illowapast = Illowa;
    Illowbpast = Illowb;
    Illowcpast = Illowc;


    ///**********MULTIPLEXING of Capacitors Voltages Equations ***************************************************
	for(unsigned int i = 0; i < 2*NUM_ARM_SUBMOD; i++)
	{
		#pragma LBLMC_UNROLL

        //****CAPACITORS VOLTAGES EQUATIONS MULTIPLEXING:

		//********************PHASE A************************************/
	    if(Sa[i])  //if Sa[i]==1
		{
          if(i<NUM_ARM_SUBMOD)	         //for the upper arm SM voltages use Ilupapast
             a = Ilupapast;

          else if(i>=NUM_ARM_SUBMOD)
		  {
             a = Illowapast;

		  }
        Vca[i] = Vca[i] + DTOC*(a);
		}

        else  //when SM is in  bypassed state
	        {
				a = 0.0;
						Vca[i] = Vca[i] *(real(1.0) - real(DT)*real(INVRFC));
			}

		//********************PHASE B************************************/
	    if(Sb[i])
		{
          if(i<NUM_ARM_SUBMOD)	         //for the upper arm SM voltages use Ilupapast
             a = Ilupbpast;

          else  if(i>=NUM_ARM_SUBMOD)
		  {
             a = Illowbpast;
		  }
         Vcb[i] = Vcb[i] + DTOC*(a);
		}

        else
	        {
				a = 0.0;
					Vcb[i] = Vcb[i]*(real(1.0) - real(DT)*real(INVRFC));
				}

		//********************PHASE C************************************/
	    if(Sc[i])
		{
          if(i<NUM_ARM_SUBMOD)	         //for the upper arm SM voltages use Ilupapast
             a = Ilupcpast;

          else  if(i>=NUM_ARM_SUBMOD)
		  {
             a = Illowcpast;
		  }
		  Vcc[i] = Vcc[i] + DTOC*(a);
		}
        else
	        {
			a = 0.0;

			Vcc[i] = Vcc[i] *(real(1.0) - real(DT)*real(INVRFC));

			}

        //*********MULTIPLEXING of Inductors Currents Equations
		if(Sa[i])
		   mula[i] = Vca[i];
	    else
		   mula[i] = 0;

	    if(Sb[i])
		   mulb[i] = Vcb[i];
	    else
		   mulb[i] = 0;

	    if(Sc[i])
		   mulc[i] = Vcc[i];
	    else
		   mulc[i] = 0;

	} //end of the for loop.

	for(unsigned int i = 0; i < NUM_ARM_SUBMOD; i++)
	{
		#pragma LBLMC_UNROLL

		upa += mula[i];
		upb += mulb[i];
		upc += mulc[i];

		lowa += mula[i+NUM_ARM_SUBMOD];
		lowb += mulb[i+NUM_ARM_SUBMOD];
		lowc += mulc[i+NUM_ARM_SUBMOD];

	}

	// *********PRE-CHARGER ON/OFF:

	if(swp)          //means : swp = true; then the pre-charger resistance is bypassed
		Rpre = real(0.0);   //  <----leading to that value od the parallel equivalent resistance betweeen 220 Ohm (pre-charger resistance) and 0.1 Ohm (arm resistance)
	else
	{
		Rpre = real(220.0);
	}

//update state difference equations
	// USE THE FOLLOWING 6 EQUATIONS ACCORDING TO THE VALUE OF "sw" CAN SIMULATE OR NOT THE PRE-CHARGER
	//the pre-charger is deactivated as default because "sw" is set to "TRUE"

	// *****PHASE A****************************************************************
  	Ilupa = Ilupapast + DTOL * (Vup -upa - (RARM+Rpre) * Ilupapast   - Vouta );

  	Illowa = Illowapast + DTOL * (Vlow - lowa - (RARM+Rpre)* Illowapast  - Vouta );

  	// *****PHASE B****************************************************************
    Ilupb = Ilupbpast + DTOL * (Vup - upb - (RARM+Rpre) * Ilupbpast  - Voutb);

  	Illowb = Illowbpast + DTOL  * (Vlow -lowb - (RARM+Rpre) * Illowbpast   - Voutb );

  	// *****PHASE C****************************************************************
  	Ilupc = Ilupcpast + DTOL  * (Vup - upc - (RARM+Rpre) * Ilupcpast - Voutc);

  	Illowc = Illowcpast + DTOL * (Vlow - lowc - (RARM+Rpre) *  Illowcpast - Voutc);





// Reinitialize for the next loop iteration:
    upa  = 0;
    upb  = 0;
	upc  = 0;
	lowa = 0;
	lowb = 0;
	lowc = 0;

//update source contributions
	*bpos=   Ilupa + Ilupb + Ilupc;
	*bneg = -(Illowa + Illowb + Illowc);
	*bout1 = Ilupa + Illowa ;
	*bout2 = Ilupb + Illowb ;
	*bout3 = Ilupc + Illowc ;
)";

std::string ModularMultilevelConverter_HalfBridgeModules::generateUpdateBody()
{

	std::stringstream sstrm;
	sstrm <<
	std::setprecision(16) <<
	std::fixed <<
	std::scientific;

		//specialize converter update body code for component instance

	std::string body = MODULARMULTILEVELCONVERTER_HALFBRIDGEMODULES_GENERATEUPDATEBODY_BASE_STRING;
	StringProcessor str_proc(body);

		//specialize data type

	str_proc.replaceWordAll("real", "real");

		//specialize constant parameters

	appendNameToWords
	(
		body,
		{
			"MMC_LEVELS",
			"DT",
			"RB",
			"INV_BLEEDING_RES",
			"RARM",
			"LARM",
			"SUBMOD_CAP",
			"DTOC",
			"DTOL",
			"LODT",
			"INVRFC",
			"NUM_ARM_SUBMOD",
			"CAP_SUBMOD_INIT"
		}
	);


		//specialize internal temp parameters

	appendNameToWords
	(
		body,
		{
			"upa",
			"upb",
			"upc",
			"lowa",
			"lowb",
			"lowc",
		}
	);

		//specialize states and fields

	appendNameToWords
	(
		body,
		{
			"Rpre",
			"a",
			"mula",
			"mulb",
			"mulc",
			"Vca",
			"Vcb",
			"Vcc",
			"Ilupapast" ,
			"Ilupbpast" ,
			"Ilupcpast" ,
			"Illowapast",
			"Illowbpast",
			"Illowcpast",
			"Ilupa" ,
			"Ilupb" ,
			"Ilupc" ,
			"Illowa",
			"Illowb",
			"Illowc",
			"Ic_upa",
			"Ic_upb",
			"Ic_upc",
			"Ic_lowa",
			"Ic_lowb",
			"Ic_lowc",
			"Vup",
			"Vlow",
			"Vouta",
			"Voutb",
			"Voutc"
		}
	);

		//specialize solution inputs and outputs

	str_proc.replaceWordAll("P", std::to_string(P));
	str_proc.replaceWordAll("N", std::to_string(N));
	str_proc.replaceWordAll("A", std::to_string(A));
	str_proc.replaceWordAll("B", std::to_string(B));
	str_proc.replaceWordAll("C", std::to_string(C));

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

	appendNameToWords
	(
		body,
		{
			"swp",
			"Sa",
			"Sb",
			"Sc"
		}
	);

	return body;
}

} //namespace lblmc

