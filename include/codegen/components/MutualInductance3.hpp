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

#ifndef LBLMC_MUTUALINDUCTANCE3_HPP
#define LBLMC_MUTUALINDUCTANCE3_HPP

#include <vector>
#include <string>
#include <stdexcept>

#include "codegen/components/Component.hpp"

namespace lblmc
{

class MutualInductance3 : public Component
{
private:

	double DT;   ///< simulation time step
	double L1;   ///< inductance of 1st inductor
	double L2;   ///< inductance of 2nd inductor
	double L3;   ///< inductance of 3rd inductor
	double M12;  ///< mutual inductance between 1st and 2nd inductors
	double M23;  ///< mutual inductance between 2nd and 3rd inductors
	double M31;  ///< mutual inductance between 3rd and 1st inductors
	double D;    ///< internal constant
	double K1;   ///< internal constant
	double K2;   ///< internal constant
	double K3;   ///< internal constant
	double K4;   ///< internal constant
	double K5;   ///< internal constant
	double K6;   ///< internal constant
	double K7;   ///< internal constant
	double K8;   ///< internal constant
	double K9;   ///< internal constant

	unsigned int PA, NA, PB, NB, PC, NC;
	unsigned int source_id_A, source_id_B, source_id_C;

public:

	MutualInductance3(std::string comp_name);

	MutualInductance3
	(
		std::string comp_name,
		double dt,
		double l1,
		double l2,
		double l3,
		double m12,
		double m23,
		double m31
	);

	MutualInductance3(const MutualInductance3& base);

	inline std::string getType() const { return std::string("MutualInductance3");}

	inline unsigned int getNumberOfTerminals() const { return 6; }
	inline unsigned int getNumberOfSources() const { return 3; }

	void getSourceIds(std::vector<unsigned int>& ids) const;
	void getResistiveCompanionElements(std::vector<ResistiveCompanionElement>& elements) const;

	inline void setTerminalConnections
	(
		unsigned int pa,
		unsigned int na,
		unsigned int pb,
		unsigned int nb,
		unsigned int pc,
		unsigned int nc
	) { PA = pa; NA = na; PB = pb; NB = nb; PC = pc; NC = nc; }

	inline void getTerminalConnections(std::vector<unsigned int>& term_ids) const
	{
        term_ids = {PA, NA, PB, NB, PC, NC};
	}

	inline void setTerminalConnections(std::vector<unsigned int> term_ids)
	{
		if(term_ids.size() != getNumberOfTerminals() )
			throw std::invalid_argument("MutualInductance3::setTerminalConnections(vector): number of given terminal ids must equal 6");

		PA = term_ids[0];
		NA = term_ids[1];
		PB = term_ids[2];
		NB = term_ids[3];
		PC = term_ids[4];
		NC = term_ids[5];
	}

	void setParameters
	(
		double dt,
		double l1,
		double l2,
		double l3,
		double m12,
		double m23,
		double m31
	);

	inline const double& getDT()  const { return DT; }
	inline const double& getL1()  const { return L1; }
	inline const double& getL2()  const { return L2; }
	inline const double& getL3()  const { return L3; }
	inline const double& getM12() const { return M12; }
	inline const double& getM23() const { return M23; }
	inline const double& getM31() const { return M31; }

	inline void setIntegrationMethod(std::string method) {}
	inline std::string getIntegrationMethod() const { return std::string("euler_forward"); }

	void stampConductance(SystemConductanceGenerator& gen);
	void stampSources(SystemSourceVectorGenerator& gen);
	std::string generateParameters();
	std::string generateFields();
	std::string generateUpdateBody();
};

} //namespace lblmc

#endif // LBLMC_MUTUALINDUCTANCE3_HPP

