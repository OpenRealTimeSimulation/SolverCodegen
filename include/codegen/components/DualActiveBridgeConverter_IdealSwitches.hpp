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

#ifndef LBLMC_DUALACTIVEBRIDGECONVERTER_IDEALSWITCHES_HPP
#define LBLMC_DUALACTIVEBRIDGECONVERTER_IDEALSWITCHES_HPP

#include <string>
#include <vector>

#include "codegen/components/Component.hpp"

namespace lblmc
{

/**
	\brief component code generator for a Dual Active Bridge power electronic converter

	This currently a placeholder for the final component generator and as such, is not operational.
**/
class DualActiveBridgeConverter_IdealSwitches : public Component
{

private:

	double DT;
	double R11;
	double R22;
	double R1;
	double R2;
	double RM;
	double LM;
	double C1;
	double C2;
	double L1;
	double L2;
	double N;

	unsigned int P1, N1, P2, N2;
	unsigned int source_id1, source_id2;

public:


    DualActiveBridgeConverter_IdealSwitches(std::string comp_name);
    DualActiveBridgeConverter_IdealSwitches
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
	);
	DualActiveBridgeConverter_IdealSwitches(const DualActiveBridgeConverter_IdealSwitches& base);

	inline std::string getType() const { return std::string("DualActiveBridgeConverter_IdealSwitches");}

	inline unsigned int getNumberOfTerminals() const { return 4; }
	inline unsigned int getNumberOfSources() const { return 2; }

	void getSourceIds(std::vector<unsigned int>& ids) const;
	std::vector<unsigned int> getSourceIds() const;

	inline void setTerminalConnections
	(
		unsigned int p1,
		unsigned int n1,
		unsigned int p2,
		unsigned int n2
	) { P1 = p1; N1 = n1; P2 = p2; N2 = n2; }

	inline void getTerminalConnections(std::vector<unsigned int>& term_ids) const
	{
        term_ids = {P1,N1,P2,N2};
	}

	inline std::vector<unsigned int> getTerminalConnections() const
	{
		return std::vector<unsigned int> {P1,N1,P2,N2};
	}

	void setParameters
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
	);

	inline void setIntegrationMethod(std::string method) {}
	inline std::string getIntegrationMethod() const { return std::string("euler_forward"); }

	std::vector<std::string> getSupportedOutputs() const;

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

#endif // LBLMC_DUALACTIVEBRIDGECONVERTER_HPP

