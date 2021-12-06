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

#ifndef LBLMC_CAPACITOR_HPP
#define LBLMC_CAPACITOR_HPP

#include <string>
#include <vector>
#include "codegen/components/Component.hpp"

namespace lblmc
{

class Capacitor : public Component
{

private:

	double DT;
	double CAP;
	double HOC2;
	unsigned int P, N;
	unsigned int source_id;

public:

	Capacitor(std::string comp_name);
	Capacitor(std::string comp_name, double dt, double cap);
	Capacitor(const Capacitor& base);

	inline std::string getType() const { return std::string("Capacitor");}

	inline unsigned int getNumberOfTerminals() const { return 2; }
	inline unsigned int getNumberOfSources() const { return 1; }

	void getSourceIds(std::vector<unsigned int>& ids) const;
	void getResistiveCompanionElements(std::vector<ResistiveCompanionElement>& elements) const;

	void setTerminalConnections(unsigned int p, unsigned int n);
	inline void getTerminalConnections(std::vector<unsigned int>& term_ids) const
	{
        term_ids = {P, N};
	}

	inline void setParameters(double dt, double cap) { DT = dt; CAP = cap; }
	inline const double& getDT() const { return DT; }
	inline const double& getCapacitance() const { return CAP; }

	inline void setIntegrationMethod(std::string method) {}
	inline std::string getIntegrationMethod() const { return std::string("tustin"); }

	void stampConductance(SystemConductanceGenerator& gen);
	void stampSources(SystemSourceVectorGenerator& gen);
	std::string generateParameters();
	std::string generateFields();
	std::string generateInputs() { return std::string(""); }
	std::string generateOutputs(std::string output = "ALL") { return std::string(""); }
	std::string generateUpdateBody();
};

} //namespace lblmc

#endif // LBLMC_CAPACITOR_HPP
