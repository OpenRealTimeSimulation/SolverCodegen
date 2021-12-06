/*

Copyright (C) 2019-2021 Matthew Milton, Michele Difronzo, Dhiman Chowdhury

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

#include "codegen/components/ModularMultilevelConverter_1LegHalfBridgeAntiParallelDiodes.hpp"
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
#include <utility>

namespace lblmc
{

//==============================================================================================================================
//==============================================================================================================================

void
ModularMultilevelConverter_1LegHalfBridgeAntiParallelDiodes::setupFields()
{
	std::vector<Component::Field> default_parameters
	{
		{Field::PARAMETER, Field::REAL, "real", "DT", 1, dt},

		{Field::PARAMETER, Field::INTEGER, "unsigned int", "NUM_SM", 1, num_sm_arm},

		{Field::PARAMETER, Field::REAL, "real", "CSM", 1, csm},

		{Field::PARAMETER, Field::REAL, "real", "LARM", 1, larm},

		{Field::PARAMETER, Field::REAL, "real", "RARM", 1, rarm},

		{Field::PARAMETER, Field::REAL, "real", "VCAP_INIT", 1, vcap_init},

		{Field::PARAMETER, Field::REAL, "real", "IIND_INIT", 1, iind_init},

		{Field::PARAMETER, Field::REAL, "real", "VTH", 1, vth},

		{Field::PARAMETER, Field::REAL, "real", "ITH", 1, ith}
	};
	parameters = std::move(default_parameters);

	std::vector<Component::Field> default_constants
	{
		//enumerations

		{Field::CONSTANT, Field::INTEGER, "unsigned char", "OPEN", 1, OPEN},

		{Field::CONSTANT, Field::INTEGER, "unsigned char", "BYPASSED", 1, BYPASSED},

		{Field::CONSTANT, Field::INTEGER, "unsigned char", "INSERTED", 1, INSERTED},

		{Field::CONSTANT, Field::INTEGER, "unsigned char", "SHORT", 1, SHORT},

		{Field::CONSTANT, Field::INTEGER, "unsigned char", "UNDECIDED", 1, UNDECIDED},

		{Field::CONSTANT, Field::INTEGER, "unsigned char", "NO_DIODES_CONDUCTING", 1, NO_DIODES_CONDUCTING},

		{Field::CONSTANT, Field::INTEGER, "unsigned char", "TOP_DIODES_CONDUCTING", 1, TOP_DIODES_CONDUCTING},

		{Field::CONSTANT, Field::INTEGER, "unsigned char", "BOTTOM_DIODES_CONDUCTING", 1, BOTTOM_DIODES_CONDUCTING},

		//internal constants

		{Field::CONSTANT, Field::REAL, "real", "ONE_OVER_LARM", 1, 1.0/larm},

		{Field::CONSTANT, Field::REAL, "real", "DT_OVER_CSM", 1, dt/csm}
	};
	constants = std::move(default_constants);

	std::vector<Component::Field> default_persistents
	{
		//state variables and derivatives

		{Field::PERSISTENT, Field::REAL, "real", "vc_upper_past", num_sm_arm, vcap_init},

		{Field::PERSISTENT, Field::REAL, "real", "vc_lower_past", num_sm_arm, vcap_init},

		{Field::PERSISTENT, Field::REAL, "real", "il_upper_past", 1, iind_init},

		{Field::PERSISTENT, Field::REAL, "real", "il_lower_past", 1, iind_init},

		{Field::PERSISTENT, Field::REAL, "real", "der_il_upper_past", 1, 0.0},

		{Field::PERSISTENT, Field::REAL, "real", "der_il_lower_past", 1, 0.0},

		//diode conduction

		{Field::PERSISTENT, Field::INTEGER, "unsigned char", "diode_conduction_upper_last", 1, NO_DIODES_CONDUCTING},

		{Field::PERSISTENT, Field::INTEGER, "unsigned char", "diode_conduction_lower_last", 1, NO_DIODES_CONDUCTING}
	};
	persistents = std::move(default_persistents);

	std::vector<Component::Field> default_temporaries
	{
		//upper arm

		{Field::TEMPORARY, Field::REAL, "real", "vp_past_upper", 1, 0.0},

		{Field::TEMPORARY, Field::REAL, "real", "va_past_upper", 1, 0.0},

		{Field::TEMPORARY, Field::BOOLEAN, "bool", "arm_is_open_upper", 1, 0},

		{Field::TEMPORARY, Field::INTEGER, "unsigned char", "conduction_sm_upper", num_sm_arm, 0},

		{Field::TEMPORARY, Field::REAL, "real", "vsm_upper", num_sm_arm, 0.0},

		{Field::TEMPORARY, Field::REAL, "real", "vsm_sum_upper", 1, 0.0},

		{Field::TEMPORARY, Field::REAL, "real", "vsm_known_sum_upper", 1, 0.0},

		{Field::TEMPORARY, Field::REAL, "real", "vc_undecided_upper", num_sm_arm, 0.0},

		{Field::TEMPORARY, Field::REAL, "real", "vc_undecided_sum_upper", 1, 0.0},

		{Field::TEMPORARY, Field::INTEGER, "unsigned int", "num_undecided_sm_upper", 1, 0},

		{Field::TEMPORARY, Field::REAL, "real", "vstar_upper", 1, 0.0},

		{Field::TEMPORARY, Field::REAL, "real", "vl_past_upper", 1, 0.0},

		{Field::TEMPORARY, Field::REAL, "real", "vr_past_upper", 1, 0.0},

		{Field::TEMPORARY, Field::REAL, "real", "vc_now_upper", num_sm_arm, 0.0},

		{Field::TEMPORARY, Field::REAL, "real", "il_now_upper", 1, 0.0},

		{Field::TEMPORARY, Field::REAL, "real", "der_il_now_upper", 1, 0.0},

		//lower arm

		{Field::TEMPORARY, Field::REAL, "real", "vp_past_lower", 1, 0.0},

		{Field::TEMPORARY, Field::REAL, "real", "va_past_lower", 1, 0.0},

		{Field::TEMPORARY, Field::BOOLEAN, "bool", "arm_is_open_lower", 1, 0},

		{Field::TEMPORARY, Field::INTEGER, "unsigned char", "conduction_sm_lower", num_sm_arm, 0},

		{Field::TEMPORARY, Field::REAL, "real", "vsm_lower", num_sm_arm, 0.0},

		{Field::TEMPORARY, Field::REAL, "real", "vsm_sum_lower", 1, 0.0},

		{Field::TEMPORARY, Field::REAL, "real", "vsm_known_sum_lower", 1, 0.0},

		{Field::TEMPORARY, Field::REAL, "real", "vc_undecided_lower", num_sm_arm, 0.0},

		{Field::TEMPORARY, Field::REAL, "real", "vc_undecided_sum_lower", 1, 0.0},

		{Field::TEMPORARY, Field::INTEGER, "unsigned int", "num_undecided_sm_lower", 1, 0},

		{Field::TEMPORARY, Field::REAL, "real", "vstar_lower", 1, 0.0},

		{Field::TEMPORARY, Field::REAL, "real", "vl_past_lower", 1, 0.0},

		{Field::TEMPORARY, Field::REAL, "real", "vr_past_lower", 1, 0.0},

		{Field::TEMPORARY, Field::REAL, "real", "vc_now_lower", num_sm_arm, 0.0},

		{Field::TEMPORARY, Field::REAL, "real", "il_now_lower", 1, 0.0},

		{Field::TEMPORARY, Field::REAL, "real", "der_il_now_lower", 1, 0.0}

	};
	temporaries = std::move(default_temporaries);

	std::vector<Component::Field> default_signal_inputs
	{
		{Field::SIGNAL_IN, Field::BOOLEAN, "bool", "gates_upper", num_sm_arm*2, 0},
		{Field::SIGNAL_IN, Field::BOOLEAN, "bool", "gates_lower", num_sm_arm*2, 0}
	};
	signal_inputs = std::move(default_signal_inputs);

	std::vector<Component::Field> default_signal_outputs
	{
		{Field::SIGNAL_OUT, Field::REAL, "real", "lega_inductor_currents", 2, 0.0},
		{Field::SIGNAL_OUT, Field::REAL, "real", "lega_capacitor_voltages", 2*num_sm_arm, 0.0}
	};
	signal_outputs = std::move(default_signal_outputs);

}

//==============================================================================================================================

ModularMultilevelConverter_1LegHalfBridgeAntiParallelDiodes::ModularMultilevelConverter_1LegHalfBridgeAntiParallelDiodes
(
	std::string comp_name
)
	: Component(comp_name),
	dt(1.0e-6),
	csm(10.0e-3),
	larm(1.0e-3),
	rarm(0.01),
	vcap_init(0.0),
	iind_init(0.0),
	vth(0.01),
	ith(0.0),
	num_sm_arm(1),
	terminals{{"node_p",0},{"node_n",0},{"node_a",0}},
	sources{{"bupper",Source::THROUGH,0},{"blower",Source::THROUGH,0}},
	parameters(),
	constants(),
	persistents(),
	temporaries(),
	signal_inputs(),
	signal_outputs()
{
	if(comp_name.empty())
	{
		throw std::invalid_argument("ModularMultilevelConverter_1LegHalfBridgeAntiParallelDiodes::constructor(.): comp_name must be a valid, non-empty C++ label");
	}

	setupFields();
}

ModularMultilevelConverter_1LegHalfBridgeAntiParallelDiodes::ModularMultilevelConverter_1LegHalfBridgeAntiParallelDiodes
(
	std::string comp_name,
	double dt,
	unsigned int num_sm_arm,
	double csm,
	double larm,
	double rarm,
	double vcap_init,
	double iind_init,
	double vth,
	double ith
)
	: Component(comp_name),
	dt(dt),
	csm(csm),
	larm(larm),
	rarm(rarm),
	vcap_init(vcap_init),
	iind_init(iind_init),
	vth(vth),
	ith(ith),
	num_sm_arm(num_sm_arm),
	terminals{{"node_p",0},{"node_n",0},{"node_a",0}},
	sources{{"bupper",Source::THROUGH,0},{"blower",Source::THROUGH,0}},
	parameters(),
	constants(),
	persistents(),
	temporaries(),
	signal_inputs(),
	signal_outputs()
{
	if(comp_name.empty())
	{
		throw std::invalid_argument("ModularMultilevelConverter_1LegHalfBridgeAntiParallelDiodes::constructor(.): comp_name must be a valid, non-empty C++ label");
	}

	if
	(
		dt <= 0.0 ||
		num_sm_arm <= 0 ||
		csm <= 0.0 ||
		larm <= 0.0
	)
	{
		throw
		std::invalid_argument
		(
			"Component " +
			comp_name +
			": "
			"ModularMultilevelConverter_1LegHalfBridgeAntiParallelDiodes::constructor(.): parameters dt, num_sm_arm, csm, and larm cannot be <= 0"
		);
	}

	setupFields();
}

ModularMultilevelConverter_1LegHalfBridgeAntiParallelDiodes::ModularMultilevelConverter_1LegHalfBridgeAntiParallelDiodes
(
	const ModularMultilevelConverter_1LegHalfBridgeAntiParallelDiodes& base
)
	: Component(base),
	dt(base.dt),
	csm(base.csm),
	larm(base.larm),
	rarm(base.rarm),
	vcap_init(base.vcap_init),
	iind_init(base.iind_init),
	vth(base.vth),
	ith(base.ith),
	num_sm_arm(base.num_sm_arm),
	terminals(base.terminals),
	sources(base.sources),
	parameters(base.parameters),
	constants(base.constants),
	persistents(base.persistents),
	temporaries(base.temporaries),
	signal_inputs(base.signal_inputs),
	signal_outputs(base.signal_outputs)
{}

//==============================================================================================================================

void
ModularMultilevelConverter_1LegHalfBridgeAntiParallelDiodes::stampConductance(SystemConductanceGenerator& gen)
{
	gen.stampConductance(conductance, terminals[0].node_index, terminals[2].node_index);
	gen.stampConductance(conductance, terminals[2].node_index, terminals[1].node_index);
}

void
ModularMultilevelConverter_1LegHalfBridgeAntiParallelDiodes::stampSources(SystemSourceVectorGenerator& gen)
{
	sources[0].id = gen.insertSource(terminals[0].node_index, terminals[2].node_index);
	sources[1].id = gen.insertSource(terminals[2].node_index, terminals[1].node_index);
}

//==============================================================================================================================

std::string
ModularMultilevelConverter_1LegHalfBridgeAntiParallelDiodes::generateParameters()
{
	std::stringstream sstrm;
	sstrm <<
	std::setprecision(16) <<
	std::fixed <<
	std::scientific;

	generateFieldStrings(sstrm, parameters);

	generateFieldStrings(sstrm, constants);

	return sstrm.str();
}

std::string
ModularMultilevelConverter_1LegHalfBridgeAntiParallelDiodes::generateFields()
{
	std::stringstream sstrm;
	sstrm <<
	std::setprecision(16) <<
	std::fixed <<
	std::scientific;

	generateFieldStrings(sstrm, persistents);

	generateFieldStrings(sstrm, temporaries);

	return sstrm.str();
}

std::string
ModularMultilevelConverter_1LegHalfBridgeAntiParallelDiodes::generateInputs()
{
	std::stringstream sstrm;
	sstrm <<
	std::setprecision(16) <<
	std::fixed <<
	std::scientific;

	generateSignalFieldStrings(sstrm, signal_inputs);

	return sstrm.str();
}

std::string
ModularMultilevelConverter_1LegHalfBridgeAntiParallelDiodes::generateOutputs(std::string output)
{
	std::stringstream sstrm;
	sstrm <<
	std::setprecision(16) <<
	std::fixed <<
	std::scientific;

	generateSignalFieldStrings(sstrm, signal_outputs);

	return sstrm.str();
}

std::string
ModularMultilevelConverter_1LegHalfBridgeAntiParallelDiodes::generateOutputsUpdateBody(std::string output)
{
	std::string output_body =
R"(
lega_inductor_currents[0] = il_upper_past;
lega_inductor_currents[1] = il_lower_past;

for(int i = 0; i < NUM_SM; i++)
{
	//#unroll

	lega_capacitor_voltages[i] = vc_upper_past[i];
	lega_capacitor_voltages[i+NUM_SM] = vc_lower_past[i];
}

)";

	appendNameToFields(output_body, parameters);

	appendNameToFields(output_body, constants);

	appendNameToFields(output_body, persistents);

	appendNameToFields(output_body, temporaries);

	appendNameToFields(output_body, signal_inputs);

	appendNameToFields(output_body, signal_outputs);

	return output_body;
}

//==============================================================================================================================

const static
std::string
MODULARMULTILEVELCONVERTER_1LEGHALFBRIDGEANTIPARALLELDIODES_MODELBODY_BASE_STRING =
R"(
	//a ModularMultilevelConverter_1LegHalfBridgeAntiParallelDiodes component

	// upper MMC arm

vp_past_upper = x[node_p];

va_past_upper = x[node_a];

vl_past_upper = LARM * der_il_upper_past;

vr_past_upper = il_upper_past * RARM;

for(int i = 0; i< NUM_SM; i++) //determine conduction from switches
{
	//#unroll

	const bool stop = gates_upper[i*2+0];
	const bool sbot = gates_upper[i*2+1];

	if(stop == 0 && sbot == 0) //undecided
	{
		conduction_sm_upper[i] = UNDECIDED;
		num_undecided_sm_upper = num_undecided_sm_upper + 1;
		vc_undecided_upper[i] = vc_upper_past[i];
	}
	else if(stop == 0 && sbot == 1) //bypassed
	{
		conduction_sm_upper[i] = BYPASSED;
		vsm_upper[i] = real(0.0);
	}
	else if(stop == 1 && sbot == 0) //inserted
	{
		conduction_sm_upper[i] = INSERTED;
		vsm_upper[i] = vc_upper_past[i];
	}
	else // short
	{
		conduction_sm_upper[i] = SHORT;
		vsm_upper[i] = real(0.0);
	}
}

if( num_undecided_sm_upper > real(0.0)) //determine conduction from diodes if necessary
{

	if( (diode_conduction_upper_last == TOP_DIODES_CONDUCTING) && (il_upper_past > ITH) )
	{
		diode_conduction_upper_last = TOP_DIODES_CONDUCTING;

		for(int i = 0; i < NUM_SM; i++)
		{
			//#unroll

			if(conduction_sm_upper[i] == UNDECIDED)
			{
				conduction_sm_upper[i] = INSERTED;
				vsm_upper[i] = vc_upper_past[i];
			}
		}
	}
	else if( (diode_conduction_upper_last == BOTTOM_DIODES_CONDUCTING) && (il_upper_past < ITH) )
	{
		diode_conduction_upper_last = BOTTOM_DIODES_CONDUCTING;

		for(int i = 0; i < NUM_SM; i++)
		{
			//#unroll

			if(conduction_sm_upper[i] == UNDECIDED)
			{
				conduction_sm_upper[i] = BYPASSED;
				vsm_upper[i] = real(0.0);
			}
		}
	}
	else
	{
		vsm_known_sum_upper = real(0.0);
		for(int i = 0; i < NUM_SM; i++)
		{
			//#unroll

			vsm_known_sum_upper += vsm_upper[i];
		}

		vc_undecided_sum_upper = real(0.0);
		for(int i = 0; i < NUM_SM; i++)
		{
			//#unroll

			vc_undecided_sum_upper += vc_undecided_upper[i];
		}

		const real vdtop = vp_past_upper - vsm_known_sum_upper - vc_undecided_sum_upper - vl_past_upper - vr_past_upper - va_past_upper;

		const real vdbot = vp_past_upper - vsm_known_sum_upper - vl_past_upper - va_past_upper;

		if( vdtop >= (num_undecided_sm_upper*VTH) )
		{
			diode_conduction_upper_last = TOP_DIODES_CONDUCTING;

			for(int i = 0; i < NUM_SM; i++)
			{
				//#unroll

				if(conduction_sm_upper[i] == UNDECIDED)
				{
					conduction_sm_upper[i] = INSERTED;
					vsm_upper[i] = vc_upper_past[i];
				}
			}
		}
		else if( vdbot <= -(num_undecided_sm_upper*VTH) )
		{
			diode_conduction_upper_last = BOTTOM_DIODES_CONDUCTING;

			for(int i = 0; i < NUM_SM; i++)
			{
				//#unroll

				if(conduction_sm_upper[i] == UNDECIDED)
				{
					conduction_sm_upper[i] = BYPASSED;
					vsm_upper[i] = real(0.0);
				}
			}
		}
		else
		{
			diode_conduction_upper_last = NO_DIODES_CONDUCTING;

			arm_is_open_upper = 1;
		}
	}
}
else
{
	diode_conduction_upper_last = NO_DIODES_CONDUCTING;
}

if(arm_is_open_upper == 1) //solve for new states
{
	vstar_upper = va_past_upper;

	der_il_now_upper = ONE_OVER_LARM * ( vstar_upper - il_upper_past*RARM - va_past_upper );

	il_now_upper = 0.0;

	for(int i = 0; i < NUM_SM; i++)
	{
		//#unroll

		if(conduction_sm_upper[i] == SHORT)
		{
			vc_now_upper[i] = real(0.0);
		}
		else
		{
			vc_now_upper[i] = vc_upper_past[i];
		}
	}
}
else
{
	vsm_sum_upper = real(0.0);
	for(int i = 0; i < NUM_SM; i++)
	{
		//#unroll

		vsm_sum_upper += vsm_upper[i];
	}

	vstar_upper = vp_past_upper - vsm_sum_upper;

	der_il_now_upper = ONE_OVER_LARM * ( vstar_upper - il_upper_past*RARM - va_past_upper );

	il_now_upper = il_upper_past + DT*der_il_now_upper;

	for(int i = 0; i < NUM_SM; i++)
	{
		//#unroll

		if(conduction_sm_upper[i] == INSERTED)
		{
			vc_now_upper[i] = vc_upper_past[i] + DT_OVER_CSM*il_upper_past;
		}
		else if( conduction_sm_upper[i] == SHORT )
		{
			vc_now_upper[i] = real(0.0);
		}
		else
		{
			vc_now_upper[i] = vc_upper_past[i];
		}
	}
}

	//lower MMC arm

vp_past_lower = x[node_a];

va_past_lower = x[node_n];

vl_past_lower = LARM * der_il_lower_past;

vr_past_lower = il_lower_past * RARM;

for(int i = 0; i< NUM_SM; i++) //determine conduction from switches
{
	//#unroll

	const bool stop = gates_lower[i*2+0];
	const bool sbot = gates_lower[i*2+1];

	if(stop == 0 && sbot == 0) //undecided
	{
		conduction_sm_lower[i] = UNDECIDED;
		num_undecided_sm_lower = num_undecided_sm_lower + 1;
		vc_undecided_lower[i] = vc_lower_past[i];
	}
	else if(stop == 0 && sbot == 1) //bypassed
	{
		conduction_sm_lower[i] = BYPASSED;
		vsm_lower[i] = real(0.0);
	}
	else if(stop == 1 && sbot == 0) //inserted
	{
		conduction_sm_lower[i] = INSERTED;
		vsm_lower[i] = vc_lower_past[i];
	}
	else // short
	{
		conduction_sm_lower[i] = SHORT;
		vsm_lower[i] = real(0.0);
	}
}

if( num_undecided_sm_lower > real(0.0)) //determine conduction from diodes if necessary
{

	if( (diode_conduction_lower_last == TOP_DIODES_CONDUCTING) && (il_lower_past > ITH) )
	{
		diode_conduction_lower_last = TOP_DIODES_CONDUCTING;

		for(int i = 0; i < NUM_SM; i++)
		{
			//#unroll

			if(conduction_sm_lower[i] == UNDECIDED)
			{
				conduction_sm_lower[i] = INSERTED;
				vsm_lower[i] = vc_lower_past[i];
			}
		}
	}
	else if( (diode_conduction_lower_last == BOTTOM_DIODES_CONDUCTING) && (il_lower_past < ITH) )
	{
		diode_conduction_lower_last = BOTTOM_DIODES_CONDUCTING;

		for(int i = 0; i < NUM_SM; i++)
		{
			//#unroll

			if(conduction_sm_lower[i] == UNDECIDED)
			{
				conduction_sm_lower[i] = BYPASSED;
				vsm_lower[i] = real(0.0);
			}
		}
	}
	else
	{
		vsm_known_sum_lower = real(0.0);
		for(int i = 0; i < NUM_SM; i++)
		{
			//#unroll

			vsm_known_sum_lower += vsm_lower[i];
		}

		vc_undecided_sum_lower = real(0.0);
		for(int i = 0; i < NUM_SM; i++)
		{
			//#unroll

			vc_undecided_sum_lower += vc_undecided_lower[i];
		}

		const real vdtop = vp_past_lower - vsm_known_sum_lower - vc_undecided_sum_lower - vl_past_lower - vr_past_lower - va_past_lower;

		const real vdbot = vp_past_lower - vsm_known_sum_lower - vl_past_lower - va_past_lower;

		if( vdtop >= (num_undecided_sm_lower*VTH) )
		{
			diode_conduction_lower_last = TOP_DIODES_CONDUCTING;

			for(int i = 0; i < NUM_SM; i++)
			{
				//#unroll

				if(conduction_sm_lower[i] == UNDECIDED)
				{
					conduction_sm_lower[i] = INSERTED;
					vsm_lower[i] = vc_lower_past[i];
				}
			}
		}
		else if( vdbot <= -(num_undecided_sm_lower*VTH) )
		{
			diode_conduction_lower_last = BOTTOM_DIODES_CONDUCTING;

			for(int i = 0; i < NUM_SM; i++)
			{
				//#unroll

				if(conduction_sm_lower[i] == UNDECIDED)
				{
					conduction_sm_lower[i] = BYPASSED;
					vsm_lower[i] = real(0.0);
				}
			}
		}
		else
		{
			diode_conduction_lower_last = NO_DIODES_CONDUCTING;

			arm_is_open_lower = 1;
		}
	}
}
else
{
	diode_conduction_lower_last = NO_DIODES_CONDUCTING;
}

if(arm_is_open_lower == 1) //solve for new states
{
	vstar_lower = va_past_lower;

	der_il_now_lower = ONE_OVER_LARM * ( vstar_lower - il_lower_past*RARM - va_past_lower );

	il_now_lower = 0.0;

	for(int i = 0; i < NUM_SM; i++)
	{
		//#unroll

		if(conduction_sm_lower[i] == SHORT)
		{
			vc_now_lower[i] = real(0.0);
		}
		else
		{
			vc_now_lower[i] = vc_lower_past[i];
		}
	}
}
else
{
	vsm_sum_lower = real(0.0);
	for(int i = 0; i < NUM_SM; i++)
	{
		//#unroll

		vsm_sum_lower += vsm_lower[i];
	}

	vstar_lower = vp_past_lower - vsm_sum_lower;

	der_il_now_lower = ONE_OVER_LARM * ( vstar_lower - il_lower_past*RARM - va_past_lower );

	il_now_lower = il_lower_past + DT*der_il_now_lower;

	for(int i = 0; i < NUM_SM; i++)
	{
		//#unroll

		if(conduction_sm_lower[i] == INSERTED)
		{
			vc_now_lower[i] = vc_lower_past[i] + DT_OVER_CSM*il_lower_past;
		}
		else if( conduction_sm_lower[i] == SHORT )
		{
			vc_now_lower[i] = real(0.0);
		}
		else
		{
			vc_now_lower[i] = vc_lower_past[i];
		}
	}
}

	//save MMC states

for(int i = 0; i < NUM_SM; i++)
{
	//#unroll

	vc_upper_past[i] = vc_now_upper[i];
}

il_upper_past = il_now_upper;

der_il_upper_past = der_il_now_upper;

for(int i = 0; i < NUM_SM; i++)
{
	//#unroll

	vc_lower_past[i] = vc_now_lower[i];
}

il_lower_past = il_now_lower;

der_il_lower_past = der_il_now_lower;

bupper = -il_now_upper;

blower = -il_now_lower;
)";

std::string
ModularMultilevelConverter_1LegHalfBridgeAntiParallelDiodes::generateUpdateBody()
{
	std::stringstream sstrm;
	sstrm <<
	std::setprecision(16) <<
	std::fixed <<
	std::scientific;

	std::string body = MODULARMULTILEVELCONVERTER_1LEGHALFBRIDGEANTIPARALLELDIODES_MODELBODY_BASE_STRING;

	appendNameToFields(body, parameters);

	appendNameToFields(body, constants);

	appendNameToFields(body, persistents);

	appendNameToFields(body, temporaries);

	appendNameToFields(body, signal_inputs);

	appendNameToFields(body, signal_outputs);

	replaceTerminalConnectionNames(body, terminals);

	replaceSourceNames(body, sources);

	return body;
}

//==============================================================================================================================
//==============================================================================================================================

} //namespace lblmc
