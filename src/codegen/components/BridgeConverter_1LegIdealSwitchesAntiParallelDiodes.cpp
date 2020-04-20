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

#include "codegen/components/BridgeConverter_1LegIdealSwitchesAntiParallelDiodes.hpp"
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



BridgeConverter_1LegIdealSwitchesAntiParallelDiodes::BridgeConverter_1LegIdealSwitchesAntiParallelDiodes(std::string comp_name) :
	Component(comp_name),
	DT(1.0),
	RIN(100.0e-6),
	GIN(1.0/100.0e-6),
	RSW(100.0e-6),
	R(1.0),
	C(1.0),
	L(1.0),
	VTH(1.0),
	ITH(0.0),
	P(0), G(0), N(0), A(0),
	source_id_P(0), source_id_N(0),
	source_id_A(0)
{
	if(comp_name.empty())
	{
		throw std::invalid_argument("BridgeConverter_1LegIdealSwitchesAntiParallelDiodes::constructor(): comp_name must be a valid, non-empty C++ label");
	}
}

BridgeConverter_1LegIdealSwitchesAntiParallelDiodes::BridgeConverter_1LegIdealSwitchesAntiParallelDiodes
(
	std::string comp_name,
	double dt,
	double dc_filter_capacitance,
	double leg_inductance,
	double leg_resistance,
	double diode_threshold_voltage
) :
	Component(comp_name),
	DT(dt),
	RIN(100.0e-6),
	GIN(1.0/100.0e-6),
	RSW(100.0e-6),
	R(leg_resistance),
	C(dc_filter_capacitance),
	L(leg_inductance),
	VTH(diode_threshold_voltage),
	ITH(0.0),
	P(0), G(0), N(0), A(0),
	source_id_P(0), source_id_N(0),
	source_id_A(0)
{
	if(comp_name.empty())
	{
		throw std::invalid_argument("BridgeConverter_1LegIdealSwitchesAntiParallelDiodes::constructor(): comp_name must be a valid, non-empty C++ label");
	}

	if(DT <= 0.0 || C <= 0.0 || L <= 0.0)
	{
		throw std::invalid_argument("BridgeConverter_1LegIdealSwitchesAntiParallelDiodes::constructor(): parameters dt, dc_filter_capacitance, and leg_inductance must be positive nonzero values");
	}
}

BridgeConverter_1LegIdealSwitchesAntiParallelDiodes::BridgeConverter_1LegIdealSwitchesAntiParallelDiodes(const BridgeConverter_1LegIdealSwitchesAntiParallelDiodes& base) :
	Component(base),
	DT(base.DT),
	RIN(base.RIN),
	GIN(base.GIN),
	RSW(base.RSW),
	R(base.R),
	C(base.C),
	L(base.L),
	VTH(base.VTH),
	ITH(base.ITH),
	P(base.P), G(base.G), N(base.N), A(base.A),
	source_id_P(base.source_id_P), source_id_N(base.source_id_N),
	source_id_A(base.source_id_A)
{}

void BridgeConverter_1LegIdealSwitchesAntiParallelDiodes::getSourceIds(std::vector<unsigned int>& ids) const
{
	ids.clear();
	ids.push_back(source_id_P);
	ids.push_back(source_id_N);
	ids.push_back(source_id_A);
}

void BridgeConverter_1LegIdealSwitchesAntiParallelDiodes::getResistiveCompanionElements(std::vector<ResistiveCompanionElement>& elements) const
{
    elements.clear();

    RCE rce_p{ appendName(std::string("bpos")),  P, G, 0.0, GIN, source_id_P, false, false, false };
    RCE rce_n{ appendName(std::string("bneg")),  N, G, 0.0, GIN, source_id_N, false, false, false };
    RCE rce_a{ appendName(std::string("bouta")), A, G, 0.0, 0.0, source_id_A, false, false, false };

    elements.push_back(rce_p);
    elements.push_back(rce_n);
    elements.push_back(rce_a);
}

void BridgeConverter_1LegIdealSwitchesAntiParallelDiodes::setParameters
(
		double dt,
		double dc_filter_capacitance,
		double leg_inductance,
		double leg_resistance,
		double diode_threshold_voltage
)
{
    DT = dt;
	R = leg_resistance;
	C = dc_filter_capacitance;
	L = leg_inductance;
	VTH = diode_threshold_voltage;
}

void BridgeConverter_1LegIdealSwitchesAntiParallelDiodes::stampConductance(SystemConductanceGenerator& gen)
{

	gen.stampConductance(GIN, P, G);
	gen.stampConductance(GIN, N, G);

}

void BridgeConverter_1LegIdealSwitchesAntiParallelDiodes::stampSources(SystemSourceVectorGenerator& gen)
{
	source_id_P = gen.insertSource(P,G);
	source_id_N = gen.insertSource(N,G);
	source_id_A = gen.insertSource(A,G);
}

std::string BridgeConverter_1LegIdealSwitchesAntiParallelDiodes::generateParameters()
{

	std::stringstream sstrm;
	sstrm <<
	std::setprecision(16) <<
	std::fixed <<
	std::scientific;

    generateParameter(sstrm, "DT"  , DT);
    generateParameter(sstrm, "RIN" , RIN);
	generateParameter(sstrm, "GIN" , GIN);
    generateParameter(sstrm, "RSW" , RSW);
    generateParameter(sstrm, "R"   , R);
    generateParameter(sstrm, "C"   , C);
    generateParameter(sstrm, "L"   , L);
    generateParameter(sstrm, "VTH" , VTH);
    generateParameter(sstrm, "ITH" , ITH);

	return sstrm.str();
}

std::string BridgeConverter_1LegIdealSwitchesAntiParallelDiodes::generateFields()
{
	std::stringstream sstrm;
	sstrm <<
	std::setprecision(16) <<
	std::fixed <<
	std::scientific;

		//generate persistent variables

	generatePersistentField(sstrm, "vcp_past", 0.0);
	generatePersistentField(sstrm, "vcn_past", 0.0);
	generatePersistentField(sstrm, "ila_past", 0.0);
	generatePersistentField(sstrm, "ila_der_past" , 0.0);
	generatePersistentField(sstrm, "conduct_upper_a_past", 0.0);
	generatePersistentField(sstrm, "conduct_lower_a_past", 0.0);
	generatePersistentField(sstrm, "diode_conduct_upper_a_past", 0.0);
	generatePersistentField(sstrm, "diode_conduct_lower_a_past", 0.0);

		//generate temporaries

	generateTemporaryField(sstrm, "vp"           , 0.0);
	generateTemporaryField(sstrm, "vn"           , 0.0);
	generateTemporaryField(sstrm, "vg"           , 0.0);
	generateTemporaryField(sstrm, "va"           , 0.0);
	generateTemporaryField(sstrm, "vcp"          , 0.0);
	generateTemporaryField(sstrm, "vcn"          , 0.0);
	generateTemporaryField(sstrm, "ila"          , 0.0);
	generateTemporaryField(sstrm, "vcpg_past"    , 0.0);
	generateTemporaryField(sstrm, "vcng_past"    , 0.0);
	generateTemporaryField(sstrm, "vla_past"     , 0.0);
	generateTemporaryField(sstrm, "vstar_a"      , 0.0);
	generateTemporaryField(sstrm, "vstar_a_past" , 0.0);
	generateTemporaryField(sstrm, "ila_der"      , 0.0);

		//switching functional elements seen by DC filter capacitors
	generateTemporaryField(sstrm, "sfi_pa", 0.0);
	generateTemporaryField(sstrm, "sfi_na", 0.0);

		//switching function elements seen by leg filter inductors
	generateTemporaryField(sstrm, "sfvg_a"     , 0.0);
	generateTemporaryField(sstrm, "sfvcp_a"    , 0.0);
	generateTemporaryField(sstrm, "sfvcn_a"    , 0.0);
	generateTemporaryField(sstrm, "sfvstar_a"  , 0.0);
	generateTemporaryField(sstrm, "sfrswrol_a" , 0.0);

	generateTemporaryBooleanField(sstrm, "conduct_upper_a", false);
	generateTemporaryBooleanField(sstrm, "conduct_lower_a", false);
	generateTemporaryBooleanField(sstrm, "gate_upper_a"   , false);
	generateTemporaryBooleanField(sstrm, "gate_lower_a"   , false);

	generateTemporaryBooleanField(sstrm, "diode_conduct_upper_a", false);
	generateTemporaryBooleanField(sstrm, "diode_conduct_lower_a", false);

	return sstrm.str();

}

std::string BridgeConverter_1LegIdealSwitchesAntiParallelDiodes::generateInputs()
{
	std::stringstream sstrm;
	sstrm <<
	std::setprecision(16) <<
	std::fixed <<
	std::scientific;

	ArrayObject switch_gates("bool", appendName("switch_gates"), "", {2});

	sstrm <<
	switch_gates.generateArgument();

	return sstrm.str();
}

std::string BridgeConverter_1LegIdealSwitchesAntiParallelDiodes::generateOutputs(std::string output)
{
	std::stringstream sstrm;
	sstrm <<
	std::setprecision(16) <<
	std::fixed <<
	std::scientific;

	if(output == "ALL")
	{
		Object positive_capacitor_voltage("real&", appendName("positive_capacitor_voltage"), "");
		Object negative_capacitor_voltage("real&", appendName("negative_capacitor_voltage"), "");
		Object leg_a_inductor_current("real&", appendName("leg_a_inductor_current"), "");

		sstrm <<
		positive_capacitor_voltage.generateArgument() << ",\n" <<
		negative_capacitor_voltage.generateArgument() << ",\n" <<
		leg_a_inductor_current.generateArgument();

		return sstrm.str();
	}
	else
	{
		return std::string();
	}

	return sstrm.str();
}

std::string BridgeConverter_1LegIdealSwitchesAntiParallelDiodes::generateOutputsUpdateBody(std::string output)
{
	std::stringstream sstrm;
	sstrm <<
	std::setprecision(16) <<
	std::fixed <<
	std::scientific;

	std::string output_update_body =
R"(
	positive_capacitor_voltage = vcp;
	negative_capacitor_voltage = vcn;
	leg_a_inductor_current     = ila;

)";

	appendNameToWords
	(
		output_update_body,
		{
			"positive_capacitor_voltage",
			"negative_capacitor_voltage",
			"leg_a_inductor_current",
			"vcp",
			"vcn",
			"ila"
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

static const std::string BRIDGECONVERTER1LEGIDEALSWITCHESANTIPARALLELDIODES_GENERATEUPDATEBODY_BASE_STRING =
R"(
	//update terminal voltages and inputs

	vp = x[P];
	vn = x[N];
	vg = x[G];
	va = x[A];
	gate_upper_a = switch_gates[0];
	gate_lower_a = switch_gates[1];

		//update intermediate variables

	vla_past = L * ila_der_past;
	vcpg_past = vcp_past + vg;
	vcng_past = vcn_past + vg;
	vstar_a_past = vla_past + (ila_past*R) + va;
	vstar_a = real(1.0/2.0)*(vcp_past + vcn_past) + vg - (RSW/real(2.0))*ila_past;

		//determine conduction of switches+diodes

	if(diode_conduct_upper_a_past)
	{
		diode_conduct_upper_a = (ila_past <= ITH);
	}
	else
	{
		diode_conduct_upper_a = (vstar_a_past-vcpg_past >= VTH);
	}

	if(diode_conduct_lower_a_past)
	{
		diode_conduct_lower_a = (ila_past >= ITH);
	}
	else
	{
		diode_conduct_lower_a = (vcng_past-vstar_a_past >= VTH);
	}

	conduct_upper_a = gate_upper_a || diode_conduct_upper_a;
	conduct_lower_a = gate_lower_a || diode_conduct_lower_a;

		//update states of component based on conduction

	if ( (conduct_upper_a==true) && (conduct_lower_a==false) )  // upper conducting
	{

		sfi_pa     = ila_past;
		sfi_na     = real(0.0);

		sfvg_a     = vg;
		sfvcp_a    = vcp_past;
		sfvcn_a    = real(0.0);
		sfvstar_a  = real(0.0);
		sfrswrol_a = (RSW/L + R/L);

	}

    else if ( (conduct_upper_a==false) && (conduct_lower_a==true) )  // lower conducting
	{

		sfi_pa = real(0.0);
		sfi_na = ila_past;

		sfvg_a     = vg;
		sfvcp_a    = real(0.0);
		sfvcn_a    = vcn_past;
		sfvstar_a  = real(0.0);
		sfrswrol_a = (RSW/L + R/L);

	}

    else if ( (conduct_upper_a==false) && (conduct_lower_a==false) ) // none conducting (deadtime)
	{

		sfi_pa = real(0.0);
		sfi_na = real(0.0);

		sfvg_a     = real(0.0); //vg;
		sfvcp_a    = real(0.0);
		sfvcn_a    = real(0.0);
		sfvstar_a  = va;        //real(0.0);
		sfrswrol_a = real(0.0); //(R/L);
		ila_past = real(0.0);

	}

    else // ( (conduct_upper_a==true) && (conduct_lower_a==true) )   // both conducting (short)
	{

		sfi_pa = (real(1.0)/RSW)*(vcp_past + vg - vstar_a);
		sfi_na = (real(1.0)/RSW)*(vcn_past + vg - vstar_a);

		sfvg_a     = real(0.0);
		sfvcp_a    = real(0.0);
		sfvcn_a    = real(0.0);
		sfvstar_a  = vstar_a;
		sfrswrol_a = (R/L);

	}

	vcp = vcp_past + DT*( (real(1.0)/C/RIN)*(vp-vcp_past-vg) + (real(1.0)/C)*(- sfi_pa ) );
	vcn = vcn_past + DT*( (real(1.0)/C/RIN)*(vn-vcn_past-vg) + (real(1.0)/C)*(- sfi_na ) );
	ila_der = (real(1.0)/L)*(sfvg_a + sfvcp_a + sfvcn_a + sfvstar_a - va) - sfrswrol_a*ila_past;
	ila = ila_past + DT*ila_der;

		//update state registers for next time step

	vcp_past = vcp;
	vcn_past = vcn;
	ila_past = ila;
	ila_der_past = ila_der;

	conduct_upper_a_past = conduct_upper_a;
    conduct_lower_a_past = conduct_lower_a;

    diode_conduct_upper_a_past = diode_conduct_upper_a;
    diode_conduct_lower_a_past = diode_conduct_lower_a;

		//update resistive companion source contributions of component

	bpos = vcp*GIN;
	bneg = vcn*GIN;
	bouta = ila;
)";

std::string BridgeConverter_1LegIdealSwitchesAntiParallelDiodes::generateUpdateBody()
{
	std::stringstream sstrm;
	sstrm <<
	std::setprecision(16) <<
	std::fixed <<
	std::scientific;

		//specialize converter update body code for component instance

	std::string body = BRIDGECONVERTER1LEGIDEALSWITCHESANTIPARALLELDIODES_GENERATEUPDATEBODY_BASE_STRING;
	StringProcessor str_proc(body);

		//specialize constant parameters

	appendNameToWords
	(
		body,
		{
			"DT"  ,
			"RIN" ,
			"GIN" ,
			"RSW" ,
			"R"   ,
			"C"   ,
			"L"   ,
			"VTH" ,
			"ITH"
		}
	);

		//specialize fields

	appendNameToWords
	(
		body,
		{
			"vcp_past",
			"vcn_past",
			"ila_past",
			"vp"           ,
			"vn"           ,
			"vg"           ,
			"va"           ,
			"vcp"          ,
			"vcn"          ,
			"ila"          ,
			"ila_der_past" ,
			"ila_der"      ,
			"vcpg_past"    ,
			"vcng_past"    ,
			"vla_past"     ,
			"vstar_a"      ,
			"vstar_a_past" ,
			"conduct_upper_a",
            "conduct_lower_a",
            "gate_upper_a"   ,
            "gate_lower_a"   ,
            "sfi_pa"         ,
            "sfi_na"         ,
            "sfvg_a"     ,
            "sfvcp_a"    ,
            "sfvcn_a"    ,
            "sfvstar_a"  ,
            "sfrswrol_a" ,
            "conduct_upper_a_past" ,
            "conduct_lower_a_past" ,
            "diode_conduct_upper_a_past" ,
            "diode_conduct_lower_a_past" ,
            "diode_conduct_upper_a" ,
            "diode_conduct_lower_a"
		}
	);

		//specialize solution and source contribution access

    replaceTerminalConnectionNameWithIndex(body, "P", P);
    replaceTerminalConnectionNameWithIndex(body, "G", G);
    replaceTerminalConnectionNameWithIndex(body, "N", N);
    replaceTerminalConnectionNameWithIndex(body, "A", A);

	replaceSourceNameWithSourceContributionVector(body, "bpos", source_id_P);
	replaceSourceNameWithSourceContributionVector(body, "bneg", source_id_N);
	replaceSourceNameWithSourceContributionVector(body, "bouta", source_id_A);

		//specialize signal inputs and outputs

	appendNameToWords
	(
		body,
		{
			"switch_gates",
			"positive_capacitor_voltage",
			"negative_capacitor_voltage",
			"leg_a_inductor_current"
		}
	);

	return body;
}

} //namespace lblmc

