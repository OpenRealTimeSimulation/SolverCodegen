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


#ifndef LBLMC_NONIDEALFUNCTIONALCURRENTSOURCE_HPP
#define LBLMC_NONIDEALFUNCTIONALCURRENTSOURCE_HPP

#include <string>
#include <vector>
#include "codegen/components/Component.hpp"

namespace lblmc
{

/**
	\brief generates code for a functional current source with parallel conductance

	This component can be used to recreate resistive companion elements (source+conductance) of
	components as components themselves.  As such, this component generator has a constructor that
	takes as argument a ResistiveCompanionElement object that describes the element.
**/
class NonidealFunctionalCurrentSource : public Component
{

private:

	double CONDUCTANCE;
	unsigned int P, N;
	unsigned int source_id;

public:

	explicit NonidealFunctionalCurrentSource(std::string comp_name);
	NonidealFunctionalCurrentSource(std::string comp_name, double parallel_conductance);
	explicit NonidealFunctionalCurrentSource(const ResistiveCompanionElement& rce);
	NonidealFunctionalCurrentSource(const NonidealFunctionalCurrentSource& base);

	inline std::string getType() const { return std::string("NonidealFunctionalCurrentSource");}

	inline unsigned int getNumberOfTerminals() const { return 2; }
	inline unsigned int getNumberOfSources() const { return 1; }

	inline void setTerminalConnections(unsigned int p, unsigned int n) { P = p; N = n; }
	inline void getTerminalConnections(std::vector<unsigned int>& term_ids) const
	{
        term_ids = {P, N};
	}

	inline void setParameters(double para_cond) { CONDUCTANCE = para_cond; }
	inline const double getParallelResistance() const { return 1.0/CONDUCTANCE; }
	inline const double& getParallelConductance() const { return CONDUCTANCE; }


	void getSourceIds(std::vector<unsigned int>& ids) const;
	void getResistiveCompanionElements(std::vector<ResistiveCompanionElement>& elements) const;

	void stampConductance(SystemConductanceGenerator& gen);
	void stampSources(SystemSourceVectorGenerator& gen);
	std::string generateParameters();
	inline std::string generateFields() { return std::string(""); }
	inline std::string generateInputs();
	inline std::string generateOutputs(std::string output = "ALL") { return std::string(""); }
	std::string generateUpdateBody();
};

} //namespace lblmc

#endif // LBLMC_NONIDEALFUNCTIONALCURRENTSOURCE_HPP




