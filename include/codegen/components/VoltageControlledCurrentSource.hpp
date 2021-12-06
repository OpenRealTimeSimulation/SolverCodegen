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


#ifndef LBLMC_VOLTAGECONTROLLEDCURRENTSOURCE_HPP
#define LBLMC_VOLTAGECONTROLLEDCURRENTSOURCE_HPP

#include <string>
#include <vector>
#include "codegen/components/Component.hpp"

namespace lblmc
{

/**
	\brief Code generator for Voltage Controlled Current Sources (VCCS)

	The VCCS is a source whose current is set by a measured voltage across two terminals times a
	transconductance gain.

	The VCCS can be used to model transconductance between two ports.

	\author Matthew Milton
	\date 2019
**/
class VoltageControlledCurrentSource : public Component
{

private:

	double TRANSCONDUCTANCE;
	unsigned int M, N, P, Q;

public:

	VoltageControlledCurrentSource(std::string comp_name);
	VoltageControlledCurrentSource(std::string comp_name, double transconductance);
	VoltageControlledCurrentSource(const VoltageControlledCurrentSource& base);

	inline std::string getType() const { return std::string("VoltageControlledCurrentSource");}

	inline unsigned int getNumberOfTerminals() const { return 4; }
	inline unsigned int getNumberOfSources() const { return 0; }

	inline void setTerminalConnections(
		unsigned int pv,
		unsigned int nv,
		unsigned int pi,
		unsigned int ni
	)
	{ M = pv; N = nv; P = pi; Q = ni; }

	inline void getTerminalConnections(std::vector<unsigned int>& term_ids) const
	{
        term_ids = {M, N, P, Q};
	}

	inline void setParameters(double tc) { TRANSCONDUCTANCE = tc; }
	inline const double& getTransconductance() const { return TRANSCONDUCTANCE; }

	void getSourceIds(std::vector<unsigned int>& ids) const { ids.clear(); }

	/**
		As VoltageControlledCurrentSource components do not follow normal resistive companion
		structure, this method gives back an empty vector.
	**/
	void getResistiveCompanionElements(std::vector<ResistiveCompanionElement>& elements) const { elements.clear(); }
	void getResistiveCompanionControlledSourceElements
		(std::vector<ResistiveCompanionControlledSourceElement>& elements) const;

	void stampConductance(SystemConductanceGenerator& gen);
	inline void stampSources(SystemSourceVectorGenerator& gen) {}
	inline std::string generateParameters() {return std::string(""); }
	inline std::string generateFields() { return std::string(""); }
	inline std::string generateInputs() { return std::string(""); }
	inline std::string generateOutputs(std::string output = "ALL") { return std::string(""); }
	inline std::string generateUpdateBody() {return std::string(""); }
};

} //namespace lblmc

#endif // LBLMC_VOLTAGECONTROLLEDCURRENTSOURCE_HPP



