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

#ifndef LBLMC_RESISTOR_HPP
#define LBLMC_RESISTOR_HPP

#include <string>
#include <vector>
#include "codegen/components/Component.hpp"

namespace lblmc
{

class Resistor : public Component
{

private:

	double RES;
	unsigned int P, N;

public:

	Resistor(std::string comp_name);
	Resistor(std::string comp_name, double res);
	Resistor(const Resistor& base);

	inline std::string getType() const { return std::string("Resistor");}

	inline unsigned int getNumberOfTerminals() const { return 2; }
	inline unsigned int getNumberOfSources() const { return 0; }

	void getResistiveCompanionElements(std::vector<ResistiveCompanionElement>& elements) const;

	void setTerminalConnections(unsigned int p, unsigned int n);
	inline void getTerminalConnections(std::vector<unsigned int>& term_ids) const
	{
        term_ids = {P, N};
	}

	inline void setParameters(double res) { RES = res; }
	inline const double& getResistance() const { return RES; }
	inline const double getConductance() const { return 1.0/RES; }

	void stampConductance(SystemConductanceGenerator& gen);
	inline void stampSources(SystemSourceVectorGenerator& gen) {}
	inline std::string generateParameters() { return std::string(""); }
	inline std::string generateFields() { return std::string(""); }
	inline std::string generateInputs() { return std::string(""); }
	inline std::string generateOutputs(std::string output = "ALL") { return std::string(""); }
	inline std::string generateUpdateBody() { return std::string(""); }
};

} //namespace lblmc

#endif // LBLMC_RESISTOR_HPP

