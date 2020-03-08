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

#ifndef LBLMC_SERIESRLIDEALSWITCH_HPP
#define LBLMC_SERIESRLIDEALSWITCH_HPP

#include <vector>
#include <string>
#include <sstream>
#include <stdexcept>

#include "codegen/components/Component.hpp"

namespace lblmc
{

class SeriesRLIdealSwitch : public Component
{

private:

	double DT;
	double L;
	double R;

	unsigned int P, N;
	unsigned int source_id;

	std::string integration_method;

public:

	SeriesRLIdealSwitch(std::string comp_name);
	SeriesRLIdealSwitch(std::string comp_name, double dt, double l, double r);
	SeriesRLIdealSwitch(const SeriesRLIdealSwitch& base);

	inline std::string getType() const { return std::string("SeriesRLIdealSwitch");}

	inline unsigned int getNumberOfTerminals() const { return 2; }
	inline unsigned int getNumberOfSources() const { return 1; }

	inline void getSourceIds(std::vector<unsigned int>& ids) const { ids.clear(); ids.push_back(source_id); }
	void getResistiveCompanionElements(std::vector<ResistiveCompanionElement>& elements) const;

	inline void setTerminalConnections
	(
		unsigned int p,
		unsigned int n
	) { P = p; N = n; }

	inline void getTerminalConnections(std::vector<unsigned int>& term_ids) const
	{
        term_ids = {P,N};
	}

	inline void setParameters
	(
		double dt,
		double l,
		double r
	) { DT = dt; L = l; R = r; }

	inline const double& getDT() const { return DT; }
	inline const double& getInductance() const { return L; }
	inline const double& getResistance() const { return R; }

	inline void setIntegrationMethod(std::string method)
	{
		if(method == INTEGRATION_EULER_FORWARD || method == INTEGRATION_RUNGE_KUTTA_4)
			integration_method = method;
		else throw std::invalid_argument("SeriesRLIdealSwitch::setIntegrationMethod(std::string) -- selected integration method is not supported");
	}
	inline std::string getIntegrationMethod() const
	{
		return integration_method;
	}

	inline std::vector<std::string> getSupportedInputs() const { return std::vector<std::string>{"sw"}; }

	inline std::vector<std::string> getSupportedOutputs() const
	{
		std::vector<std::string> ret
		{
			"l_current"
		};

		return ret;
	}

	void stampConductance(SystemConductanceGenerator& gen);
	void stampSources(SystemSourceVectorGenerator& gen);
	std::string generateParameters();
	std::string generateFields();
	std::string generateInputs();
	std::string generateOutputs(std::string output = "ALL");
	std::string generateOutputsUpdateBody(std::string output = "ALL");
	std::string generateUpdateBody();

private:
	std::string generateUpdateBodyEulerForward();
	std::string generateUpdateBodyRungeKutta4();
};

} //namespace lblmc

#endif // LBLMC_SERIESRLIDEALSWITCH_HPP
