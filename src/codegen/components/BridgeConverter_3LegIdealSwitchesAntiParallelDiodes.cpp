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

#include "codegen/components/BridgeConverter_3LegIdealSwitchesAntiParallelDiodes.hpp"
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



BridgeConverter_3LegIdealSwitchesAntiParallelDiodes::BridgeConverter_3LegIdealSwitchesAntiParallelDiodes(std::string comp_name) :
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
	P(0), G(0), N(0), A(0), B(0), Ct(0),
	source_id_P(0), source_id_N(0),
	source_id_A(0), source_id_B(0), source_id_C(0)
{
	if(comp_name.empty())
	{
		throw std::invalid_argument("BridgeConverter_3LegIdealSwitchesAntiParallelDiodes::constructor(): comp_name must be a valid, non-empty C++ label");
	}
}

BridgeConverter_3LegIdealSwitchesAntiParallelDiodes::BridgeConverter_3LegIdealSwitchesAntiParallelDiodes
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
	P(0), G(0), N(0), A(0), B(0), Ct(0),
	source_id_P(0), source_id_N(0),
	source_id_A(0) ,source_id_B(0), source_id_C(0)
{
	if(comp_name.empty())
	{
		throw std::invalid_argument("BridgeConverter_3LegIdealSwitchesAntiParallelDiodes::constructor(): comp_name must be a valid, non-empty C++ label");
	}

	if(DT <= 0.0 || C <= 0.0 || L <= 0.0)
	{
		throw std::invalid_argument("BridgeConverter_3LegIdealSwitchesAntiParallelDiodes::constructor(): parameters dt, dc_filter_capacitance, and leg_inductance must be positive nonzero values");
	}
}

BridgeConverter_3LegIdealSwitchesAntiParallelDiodes::BridgeConverter_3LegIdealSwitchesAntiParallelDiodes(const BridgeConverter_3LegIdealSwitchesAntiParallelDiodes& base) :
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
	P(base.P), G(base.G), N(base.N),
	A(base.A), B(base.B), Ct(base.Ct),
	source_id_P(base.source_id_P), source_id_N(base.source_id_N),
	source_id_A(base.source_id_A), source_id_B(base.source_id_B), source_id_C(base.source_id_C)
{}

void BridgeConverter_3LegIdealSwitchesAntiParallelDiodes::getSourceIds(std::vector<unsigned int>& ids) const
{
	ids.clear();
	ids.push_back(source_id_P);
	ids.push_back(source_id_N);
	ids.push_back(source_id_A);
	ids.push_back(source_id_B);
	ids.push_back(source_id_C);
}

void BridgeConverter_3LegIdealSwitchesAntiParallelDiodes::getResistiveCompanionElements(std::vector<ResistiveCompanionElement>& elements) const
{
    elements.clear();

    RCE rce_p{ appendName(std::string("bpos")),  P, G, 0.0, GIN, source_id_P, false, false, false };
    RCE rce_n{ appendName(std::string("bneg")),  N, G, 0.0, GIN, source_id_N, false, false, false };
    RCE rce_a{ appendName(std::string("bouta")), A, G, 0.0, 0.0, source_id_A, false, false, false };
	RCE rce_b{ appendName(std::string("boutb")), B, G, 0.0, 0.0, source_id_B, false, false, false };
    RCE rce_c{ appendName(std::string("boutc")), Ct, G, 0.0, 0.0, source_id_C, false, false, false };

    elements.push_back(rce_p);
    elements.push_back(rce_n);
    elements.push_back(rce_a);
    elements.push_back(rce_b);
    elements.push_back(rce_c);
}

void BridgeConverter_3LegIdealSwitchesAntiParallelDiodes::setParameters
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

void BridgeConverter_3LegIdealSwitchesAntiParallelDiodes::stampConductance(SystemConductanceGenerator& gen)
{

	gen.stampConductance(GIN, P, G);
	gen.stampConductance(GIN, N, G);

}

void BridgeConverter_3LegIdealSwitchesAntiParallelDiodes::stampSources(SystemSourceVectorGenerator& gen)
{
	source_id_P = gen.insertSource(P,G);
	source_id_N = gen.insertSource(N,G);
	source_id_A = gen.insertSource(A,G);
	source_id_B = gen.insertSource(B,G);
	source_id_C = gen.insertSource(Ct,G);
}

std::string BridgeConverter_3LegIdealSwitchesAntiParallelDiodes::generateParameters()
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

std::string BridgeConverter_3LegIdealSwitchesAntiParallelDiodes::generateFields()
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
	generatePersistentField(sstrm, "ilb_past", 0.0);
	generatePersistentField(sstrm, "ilc_past", 0.0);
	generatePersistentField(sstrm, "ila_der_past" , 0.0);
	generatePersistentField(sstrm, "ilb_der_past" , 0.0);
	generatePersistentField(sstrm, "ilc_der_past" , 0.0);
	generatePersistentField(sstrm, "conduct_upper_a_past", 0.0);
	generatePersistentField(sstrm, "conduct_upper_b_past", 0.0);
	generatePersistentField(sstrm, "conduct_upper_c_past", 0.0);
	generatePersistentField(sstrm, "conduct_lower_a_past", 0.0);
	generatePersistentField(sstrm, "conduct_lower_b_past", 0.0);
	generatePersistentField(sstrm, "conduct_lower_c_past", 0.0);

		//generate temporaries

	generateTemporaryField(sstrm, "vp"           , 0.0);
	generateTemporaryField(sstrm, "vn"           , 0.0);
	generateTemporaryField(sstrm, "vg"           , 0.0);
	generateTemporaryField(sstrm, "va"           , 0.0);
	generateTemporaryField(sstrm, "vb"           , 0.0);
	generateTemporaryField(sstrm, "vc"           , 0.0);
	generateTemporaryField(sstrm, "vcp"          , 0.0);
	generateTemporaryField(sstrm, "vcn"          , 0.0);
	generateTemporaryField(sstrm, "ila"          , 0.0);
	generateTemporaryField(sstrm, "ilb"          , 0.0);
	generateTemporaryField(sstrm, "ilc"          , 0.0);
	generateTemporaryField(sstrm, "vcpg_past"    , 0.0);
	generateTemporaryField(sstrm, "vcng_past"    , 0.0);
	generateTemporaryField(sstrm, "vla_past"     , 0.0);
	generateTemporaryField(sstrm, "vlb_past"     , 0.0);
	generateTemporaryField(sstrm, "vlc_past"     , 0.0);
	generateTemporaryField(sstrm, "vstar_a"      , 0.0);
	generateTemporaryField(sstrm, "vstar_b"      , 0.0);
	generateTemporaryField(sstrm, "vstar_c"      , 0.0);
	generateTemporaryField(sstrm, "vstar_a_past" , 0.0);
	generateTemporaryField(sstrm, "vstar_b_past" , 0.0);
	generateTemporaryField(sstrm, "vstar_c_past" , 0.0);
	generateTemporaryField(sstrm, "ila_der"      , 0.0);
	generateTemporaryField(sstrm, "ilb_der"      , 0.0);
	generateTemporaryField(sstrm, "ilc_der"      , 0.0);

		//switching functional elements seen by DC filter capacitors
	generateTemporaryField(sstrm, "sfi_pa", 0.0);
	generateTemporaryField(sstrm, "sfi_pb", 0.0);
	generateTemporaryField(sstrm, "sfi_pc", 0.0);
	generateTemporaryField(sstrm, "sfi_na", 0.0);
	generateTemporaryField(sstrm, "sfi_nb", 0.0);
	generateTemporaryField(sstrm, "sfi_nc", 0.0);

		//switching function elements seen by by leg filter inductors
	generateTemporaryField(sstrm, "sfvg_a"     , 0.0);
	generateTemporaryField(sstrm, "sfvcp_a"    , 0.0);
	generateTemporaryField(sstrm, "sfvcn_a"    , 0.0);
	generateTemporaryField(sstrm, "sfvstar_a"  , 0.0);
	generateTemporaryField(sstrm, "sfrswrol_a" , 0.0);
	generateTemporaryField(sstrm, "sfvg_b"     , 0.0);
	generateTemporaryField(sstrm, "sfvcp_b"    , 0.0);
	generateTemporaryField(sstrm, "sfvcn_b"    , 0.0);
	generateTemporaryField(sstrm, "sfvstar_b"  , 0.0);
	generateTemporaryField(sstrm, "sfrswrol_b" , 0.0);
	generateTemporaryField(sstrm, "sfvg_c"     , 0.0);
	generateTemporaryField(sstrm, "sfvcp_c"    , 0.0);
	generateTemporaryField(sstrm, "sfvcn_c"    , 0.0);
	generateTemporaryField(sstrm, "sfvstar_c"  , 0.0);
	generateTemporaryField(sstrm, "sfrswrol_c" , 0.0);

	generateTemporaryBooleanField(sstrm, "conduct_upper_a", false);
	generateTemporaryBooleanField(sstrm, "conduct_lower_a", false);
	generateTemporaryBooleanField(sstrm, "gate_upper_a"   , false);
	generateTemporaryBooleanField(sstrm, "gate_lower_a"   , false);
	generateTemporaryBooleanField(sstrm, "conduct_upper_b", false);
	generateTemporaryBooleanField(sstrm, "conduct_lower_b", false);
	generateTemporaryBooleanField(sstrm, "gate_upper_b"   , false);
	generateTemporaryBooleanField(sstrm, "gate_lower_b"   , false);
	generateTemporaryBooleanField(sstrm, "conduct_upper_c", false);
	generateTemporaryBooleanField(sstrm, "conduct_lower_c", false);
	generateTemporaryBooleanField(sstrm, "gate_upper_c"   , false);
	generateTemporaryBooleanField(sstrm, "gate_lower_c"   , false);

	return sstrm.str();

}

std::string BridgeConverter_3LegIdealSwitchesAntiParallelDiodes::generateInputs()
{
	std::stringstream sstrm;
	sstrm <<
	std::setprecision(16) <<
	std::fixed <<
	std::scientific;

	ArrayObject switch_gates("bool", appendName("switch_gates"), "", {6});

	sstrm <<
	switch_gates.generateArgument();

	return sstrm.str();
}

std::string BridgeConverter_3LegIdealSwitchesAntiParallelDiodes::generateOutputs(std::string output)
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
		Object leg_b_inductor_current("real&", appendName("leg_b_inductor_current"), "");
		Object leg_c_inductor_current("real&", appendName("leg_c_inductor_current"), "");

		sstrm <<
		positive_capacitor_voltage.generateArgument() << ",\n" <<
		negative_capacitor_voltage.generateArgument() << ",\n" <<
		leg_a_inductor_current.generateArgument() << ",\n" <<
		leg_b_inductor_current.generateArgument() << ",\n" <<
		leg_c_inductor_current.generateArgument();

		return sstrm.str();
	}
	else
	{
		return std::string();
	}

	return sstrm.str();
}

std::string BridgeConverter_3LegIdealSwitchesAntiParallelDiodes::generateOutputsUpdateBody(std::string output)
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
	leg_b_inductor_current     = ilb;
	leg_c_inductor_current     = ilc;

)";

	appendNameToWords
	(
		output_update_body,
		{
			"positive_capacitor_voltage",
			"negative_capacitor_voltage",
			"leg_a_inductor_current",
			"leg_b_inductor_current",
			"leg_c_inductor_current",
			"vcp",
			"vcn",
			"ila",
			"ilb",
			"ilc"
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

static const std::string BRIDGECONVERTER3LEGIDEALSWITCHESANTIPARALLELDIODES_GENERATEUPDATEBODY_BASE_STRING =
R"(
	//update terminal voltages and inputs

	vp = x[P];
	vn = x[N];
	vg = x[G];
	va = x[A];
	vb = x[B];
	vc = x[Ct];
	gate_upper_a = switch_gates[0];
	gate_lower_a = switch_gates[1];
	gate_upper_b = switch_gates[2];
	gate_lower_b = switch_gates[3];
	gate_upper_c = switch_gates[4];
	gate_lower_c = switch_gates[5];


		//update intermediate variables

	vla_past = L * ila_der_past;
	vlb_past = L * ilb_der_past;
	vlc_past = L * ilc_der_past;
	vcpg_past = vcp_past + vg;
	vcng_past = vcn_past + vg;
	vstar_a_past = vla_past + (ila_past*R) + va;
	vstar_b_past = vlb_past + (ilb_past*R) + vb;
	vstar_c_past = vlc_past + (ilc_past*R) + vc;
	vstar_a = real(1.0/2.0)*(vcp_past + vcn_past) + vg - (RSW/real(2.0))*ila_past;
	vstar_b = real(1.0/2.0)*(vcp_past + vcn_past) + vg - (RSW/real(2.0))*ilb_past;
	vstar_c = real(1.0/2.0)*(vcp_past + vcn_past) + vg - (RSW/real(2.0))*ilc_past;

		//determine conduction of switches+diodes
	if(conduct_upper_a_past)
	{
		conduct_upper_a = gate_upper_a || (ila_past < ITH);
	}
	else
	{
		conduct_upper_a = gate_upper_a || (vstar_a_past-vcpg_past >= VTH);
	}

	if(conduct_lower_a_past)
	{
		conduct_lower_a = gate_lower_a || (ila_past > ITH);
	}
	else
	{
		conduct_lower_a = gate_lower_a || (vcng_past-vstar_a_past >= VTH);
	}

	if(conduct_upper_b_past)
	{
		conduct_upper_b = gate_upper_b || (ilb_past < ITH);
	}
	else
	{
		conduct_upper_b = gate_upper_b || (vstar_b_past-vcpg_past >= VTH);
	}

	if(conduct_lower_b_past)
	{
		conduct_lower_b = gate_lower_b || (ilb_past > ITH);
	}
	else
	{
		conduct_lower_b = gate_lower_b || (vcng_past-vstar_b_past >= VTH);
	}

	if(conduct_upper_c_past)
	{
		conduct_upper_c = gate_upper_c || (ilc_past < ITH);
	}
	else
	{
		conduct_upper_c = gate_upper_c || (vstar_c_past-vcpg_past >= VTH);
	}

	if(conduct_lower_c_past)
	{
		conduct_lower_c = gate_lower_c || (ilc_past > ITH);
	}
	else
	{
		conduct_lower_c = gate_lower_c || (vcng_past-vstar_c_past >= VTH);
	}


		//leg A
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

		//leg B
	if ( (conduct_upper_b==true) && (conduct_lower_b==false) )  // upper conducting
	{
		sfi_pb     = ilb_past;
		sfi_nb     = real(0.0);

		sfvg_b     = vg;
		sfvcp_b    = vcp_past;
		sfvcn_b    = real(0.0);
		sfvstar_b  = real(0.0);
		sfrswrol_b = (RSW/L + R/L);
	}
	else if ( (conduct_upper_b==false) && (conduct_lower_b==true) )  // lower conducting
	{
		sfi_pb = real(0.0);
		sfi_nb = ilb_past;

		sfvg_b     = vg;
		sfvcp_b    = real(0.0);
		sfvcn_b    = vcn_past;
		sfvstar_b  = real(0.0);
		sfrswrol_b = (RSW/L + R/L);
	}
	else if ( (conduct_upper_b==false) && (conduct_lower_b==false) ) // none conducting (deadtime)
	{
		sfi_pb = real(0.0);
		sfi_nb = real(0.0);

		sfvg_b     = real(0.0); //vg;
		sfvcp_b    = real(0.0);
		sfvcn_b    = real(0.0);
		sfvstar_b  = vb; //real(0.0);
		sfrswrol_b = real(0.0); //(R/L);
		ilb_past = real(0.0);
	}
	else // ( (conduct_upper_b==true) && (conduct_lower_b==true) )   // both conducting (short)
	{
		sfi_pb = (real(1.0)/RSW)*(vcp_past + vg - vstar_b);
		sfi_nb = (real(1.0)/RSW)*(vcn_past + vg - vstar_b);

		sfvg_b     = real(0.0);
		sfvcp_b    = real(0.0);
		sfvcn_b    = real(0.0);
		sfvstar_b  = vstar_b;
		sfrswrol_b = (R/L);
	}

		//leg C
	if ( (conduct_upper_c==true) && (conduct_lower_c==false) )  // upper conducting
	{
		sfi_pc     = ilc_past;
		sfi_nc     = real(0.0);

		sfvg_c     = vg;
		sfvcp_c    = vcp_past;
		sfvcn_c    = real(0.0);
		sfvstar_c  = real(0.0);
		sfrswrol_c = (RSW/L + R/L);
	}
	else if ( (conduct_upper_c==false) && (conduct_lower_c==true) )  // lower conducting
	{
		sfi_pc = real(0.0);
		sfi_nc = ilc_past;

		sfvg_c     = vg;
		sfvcp_c    = real(0.0);
		sfvcn_c    = vcn_past;
		sfvstar_c  = real(0.0);
		sfrswrol_c = (RSW/L + R/L);
	}
	else if ( (conduct_upper_c==false) && (conduct_lower_c==false) ) // none conducting (deadtime)
	{
		sfi_pc = real(0.0);
		sfi_nc = real(0.0);

		sfvg_c     = real(0.0); //vg;
		sfvcp_c    = real(0.0);
		sfvcn_c    = real(0.0);
		sfvstar_c  = vc; //real(0.0);
		sfrswrol_c = real(0.0); //(R/L);
		ilc_past = real(0.0);
	}
	else // ( (conduct_upper_c==true) && (conduct_lower_c==true) )   // both conducting (short)
	{
		sfi_pc = (real(1.0)/RSW)*(vcp_past + vg - vstar_c);
		sfi_nc = (real(1.0)/RSW)*(vcn_past + vg - vstar_c);

		sfvg_c     = real(0.0);
		sfvcp_c    = real(0.0);
		sfvcn_c    = real(0.0);
		sfvstar_c  = vstar_c;
		sfrswrol_c = (R/L);
	}

	vcp = vcp_past + DT*( (real(1.0)/C/RIN)*(vp-vcp_past-vg) + (real(1.0)/C)*(- sfi_pa - sfi_pb - sfi_pc) );
	vcn = vcn_past + DT*( (real(1.0)/C/RIN)*(vn-vcn_past-vg) + (real(1.0)/C)*(- sfi_na - sfi_nb - sfi_nc) );
	ila_der = (real(1.0)/L)*(sfvg_a + sfvcp_a + sfvcn_a + sfvstar_a - va) - sfrswrol_a*ila_past;
	ilb_der = (real(1.0)/L)*(sfvg_b + sfvcp_b + sfvcn_b + sfvstar_b - vb) - sfrswrol_b*ilb_past;
	ilc_der = (real(1.0)/L)*(sfvg_c + sfvcp_c + sfvcn_c + sfvstar_c - vc) - sfrswrol_c*ilc_past;
	ila = ila_past + DT*ila_der;
	ilb = ilb_past + DT*ilb_der;
	ilc = ilc_past + DT*ilc_der;

		//update state registers for next time step

	vcp_past = vcp;
	vcn_past = vcn;
	ila_past = ila;
	ilb_past = ilb;
	ilc_past = ilc;
	ila_der_past = ila_der;
	ilb_der_past = ilb_der;
	ilc_der_past = ilc_der;

	conduct_upper_a_past = conduct_upper_a;
    conduct_lower_a_past = conduct_lower_a;
    conduct_upper_b_past = conduct_upper_b;
    conduct_lower_b_past = conduct_lower_b;
    conduct_upper_c_past = conduct_upper_c;
    conduct_lower_c_past = conduct_lower_c;

		//update resistive companion source contributions of component

	bpos = vcp*GIN;
	bneg = vcn*GIN;
	bouta = ila;
	boutb = ilb;
	boutc = ilc;
)";

std::string BridgeConverter_3LegIdealSwitchesAntiParallelDiodes::generateUpdateBody()
{
	std::stringstream sstrm;
	sstrm <<
	std::setprecision(16) <<
	std::fixed <<
	std::scientific;

		//specialize converter update body code for component instance

	std::string body = BRIDGECONVERTER3LEGIDEALSWITCHESANTIPARALLELDIODES_GENERATEUPDATEBODY_BASE_STRING;
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
			"ilb_past",
			"ilc_past",
			"vp"           ,
			"vn"           ,
			"vg"           ,
			"va"           ,
			"vb"           ,
			"vc"           ,
			"vcp"          ,
			"vcn"          ,
			"ila"          ,
			"ilb"          ,
			"ilc"          ,
			"ila_der_past" ,
			"ilb_der_past" ,
			"ilc_der_past" ,
			"ila_der"      ,
			"ilb_der"      ,
			"ilc_der"      ,
			"vcpg_past"    ,
			"vcng_past"    ,
			"vla_past"     ,
			"vlb_past"     ,
			"vlc_past"     ,
			"vstar_a"      ,
			"vstar_b"      ,
			"vstar_c"      ,
			"vstar_a_past" ,
			"vstar_b_past" ,
			"vstar_c_past" ,
			"conduct_upper_a",
            "conduct_lower_a",
            "conduct_upper_b",
            "conduct_lower_b",
            "conduct_upper_c",
            "conduct_lower_c",
            "gate_upper_a"   ,
            "gate_lower_a"   ,
            "gate_upper_b"   ,
            "gate_lower_b"   ,
            "gate_upper_c"   ,
            "gate_lower_c"   ,
            "sfi_pa"         ,
            "sfi_pb"         ,
            "sfi_pc"         ,
            "sfi_na"         ,
            "sfi_nb"         ,
            "sfi_nc"         ,
            "sfvg_a"     ,
            "sfvcp_a"    ,
            "sfvcn_a"    ,
            "sfvstar_a"  ,
            "sfrswrol_a" ,
            "sfvg_b"     ,
            "sfvcp_b"    ,
            "sfvcn_b"    ,
            "sfvstar_b"  ,
            "sfrswrol_b" ,
            "sfvg_c"     ,
            "sfvcp_c"    ,
            "sfvcn_c"    ,
            "sfvstar_c"  ,
            "sfrswrol_c" ,
            "conduct_upper_a_past" ,
            "conduct_upper_b_past" ,
            "conduct_upper_c_past" ,
            "conduct_lower_a_past" ,
            "conduct_lower_b_past" ,
            "conduct_lower_c_past"
		}
	);

		//specialize solution and source contribution access

    replaceTerminalConnectionNameWithIndex(body, "P", P);
    replaceTerminalConnectionNameWithIndex(body, "G", G);
    replaceTerminalConnectionNameWithIndex(body, "N", N);
    replaceTerminalConnectionNameWithIndex(body, "A", A);
    replaceTerminalConnectionNameWithIndex(body, "B", B);
    replaceTerminalConnectionNameWithIndex(body, "Ct", Ct);

	replaceSourceNameWithSourceContributionVector(body, "bpos", source_id_P);
	replaceSourceNameWithSourceContributionVector(body, "bneg", source_id_N);
	replaceSourceNameWithSourceContributionVector(body, "bouta", source_id_A);
	replaceSourceNameWithSourceContributionVector(body, "boutb", source_id_B);
	replaceSourceNameWithSourceContributionVector(body, "boutc", source_id_C);

		//specialize signal inputs and outputs

	appendNameToWords
	(
		body,
		{
			"switch_gates",
			"positive_capacitor_voltage",
			"negative_capacitor_voltage",
			"leg_a_inductor_current",
			"leg_b_inductor_current",
			"leg_c_inductor_current"
		}
	);

	return body;
}

} //namespace lblmc


