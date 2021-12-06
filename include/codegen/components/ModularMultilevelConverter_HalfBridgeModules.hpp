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

#ifndef LBLMC_MODULARMULTILEVELCONVERTER_HALFBRIDGEMODULES_HPP
#define LBLMC_MODULARMULTILEVELCONVERTER_HALFBRIDGEMODULES_HPP

#include <string>
#include <vector>

#include "codegen/components/Component.hpp"

namespace lblmc
{

/**
	\brief Component code generator for a Modular Multilevel Converter (MMC) with half-bridge
	switching modules

**/
class ModularMultilevelConverter_HalfBridgeModules : public Component
{

private:

	unsigned int MMC_LEVELS;
	double DT;
	double RB;
	double RARM;
	double LARM;
	double SUBMOD_CAP;
	double DTOC;
	double DTOL;
	double LODT;
	double INVRFC;
	unsigned int NUM_ARM_SUBMOD;
	double CAP_SUBMOD_INIT;

	unsigned int P, N, A, B, C;
	unsigned int source_id_P, source_id_N, source_id_A, source_id_B, source_id_C;

public:

	ModularMultilevelConverter_HalfBridgeModules(std::string comp_name);
	ModularMultilevelConverter_HalfBridgeModules
	(
		std::string comp_name,
		double dt,
		double bleeding_resistance,
		double arm_inductance,
		double arm_resistance,
		double switching_module_capacitance,
		double initial_cap_voltage,
		unsigned int number_of_levels
	);
	ModularMultilevelConverter_HalfBridgeModules(const ModularMultilevelConverter_HalfBridgeModules& base);

	inline std::string getType() const { return std::string("ModularMultilevelConverter_HalfBridgeModules");}

	inline unsigned int getNumberOfTerminals() const { return 5; }
	inline unsigned int getNumberOfSources() const { return 5; }

	void getSourceIds(std::vector<unsigned int>& ids) const;
	void getResistiveCompanionElements(std::vector<ResistiveCompanionElement>& elements) const;

	inline void setTerminalConnections
	(
		unsigned int p,
		unsigned int n,
		unsigned int a,
		unsigned int b,
		unsigned int c
	) { P = p; N = n; A = a; B = b; C = c; }

	inline void getTerminalConnections(std::vector<unsigned int>& term_ids) const
	{
        term_ids = {P, N, A, B, C};
	}

	void setParameters
	(
		double dt,
		double bleeding_resistance,
		double arm_inductance,
		double arm_resistance,
		double switching_module_capacitance,
		double initial_arm_voltage,
		unsigned int number_of_levels
	);

	inline void setIntegrationMethod(std::string method) {}
	inline std::string getIntegrationMethod() const { return std::string("euler_forward"); }

	inline std::vector<std::string> getSupportedOutputs() const
	{
		std::vector<std::string> ret
		{
			"la_up_current",
			"lb_up_current",
			"lc_up_current",
			"la_low_current",
			"lb_low_current",
			"lc_low_current",
			"cap_a_voltages",
			"cap_b_voltages",
			"cap_c_voltages"
		};

		return ret;
	}

	void stampConductance(SystemConductanceGenerator& gen);
	void stampSources(SystemSourceVectorGenerator& gen);
	std::string generateParameters();
	std::string generateFields();
	std::string generateInputs();
	std::string generateOutputs(std::string output = "ALL");
	std::string generateOutputsUpdateBody(std::string output="ALL");
	std::string generateUpdateBody();

};

} //namespace lblmc

#endif // LBLMC_MODULARMULTILEVELCONVERTER_HALFBRIDGEMODULES_HPP


