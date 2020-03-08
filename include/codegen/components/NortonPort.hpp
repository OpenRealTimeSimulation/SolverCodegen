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


#ifndef LBLMC_NORTONPORT_HPP
#define LBLMC_NORTONPORT_HPP

#include <string>
#include <vector>
#include "codegen/components/Component.hpp"

namespace lblmc
{

/**
	\brief Code generator for a Norton Equivalent Port for a multiport system

	This port component is placed across two terminals and consists of a functional current source,
	0 or more transconductance source, and 0 or 1 conductance across said terminals.

	The component can be used for Nodal decomposition

	\author Matthew Milton
	\date 2019
**/
class NortonPort : public Component
{

private:

	double CONDUCTANCE;
	std::vector<double> TRANSCONDUCTANCES;
	unsigned int P, N;
	std::vector<unsigned int> OTHER_PORT_TERMINALS;
	unsigned int source_id;

public:

	NortonPort(std::string comp_name);
	NortonPort(std::string comp_name, const std::vector<double>& all_conductances);
	NortonPort(std::string comp_name, double conductance, const std::vector<double>& transconductances);
	NortonPort(const NortonPort& base);

	inline std::string getType() const { return std::string("NortonPort");}

	inline unsigned int getNumberOfTerminals() const { return 2 + OTHER_PORT_TERMINALS.size(); }
	inline unsigned int getNumberOfSources() const { return 1; }

	void setTerminalConnections
	(
		unsigned int p_this_port,
		unsigned int n_this_port,
		const std::vector<unsigned int>& pn_other_ports
	);

	void setTerminalConnections(const std::vector<unsigned int>& all_terminals);

	void setParameters(double conduct, const std::vector<double>& xconduct);

	inline const double& getConductance() const { return CONDUCTANCE; }
	inline const std::vector<double>& getTransconductances() const { return TRANSCONDUCTANCES; }

	void getSourceIds(std::vector<unsigned int>& ids) const;

	/**
		As NortonPort components have transconductance and as such do not fully follow normal
		resistive companion structure, this method only gives back an element that describes the
		functional source and parallel conductance only; transconductance elements are not
		described.
	**/
	void getResistiveCompanionElements(std::vector<ResistiveCompanionElement>& elements) const;

	void stampConductance(SystemConductanceGenerator& gen);
	void stampSources(SystemSourceVectorGenerator& gen);
	std::string generateParameters();
	std::string generateFields();
	std::string generateInputs();
	std::string generateOutputs(std::string output = "ALL");
	std::string generateUpdateBody();
};

} //namespace lblmc

#endif // LBLMC_NORTONPORT_HPP




