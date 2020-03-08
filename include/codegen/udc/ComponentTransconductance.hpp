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

#ifndef LBLMC_COMPONENTTRANSCONDUCTANCE_HPP
#define LBLMC_COMPONENTTRANSCONDUCTANCE_HPP

#include "codegen/udc/ComponentElement.hpp"

namespace lblmc
{

/**
	\brief defines transconductance elements in a component of a system

	This element type defines transconductances, single ports, and voltage controlled current
	sources for electrical systems.
**/
class ComponentTransconductance : public ComponentElement
{
private:

	std::string measure_p;	///< references by name the terminal element for the positive measure (across) terminal of the transconductance
	std::string measure_n;  ///< references by name the terminal element for the negative measure (across) terminal of the transconductance
	std::string source_p;	///< references by name the terminal element for the positive source (through) terminal of the transconductance
	std::string source_n;   ///< references by name the terminal element for the positive source (through) terminal of the transconductance
	std::string value;      ///< numerical value or expression for the transconductance

public:

	ComponentTransconductance();
	ComponentTransconductance
	(
		const std::string& label,
		const std::string& p = std::string("p"),
		const std::string& n = std::string("n"),
		const std::string& value = std::string("1.0")
	);
	ComponentTransconductance(const ComponentTransconductance& base);
	ComponentTransconductance(ComponentTransconductance&& base);

	void setTerminals
	(
		const std::string& measure_p,
		const std::string& measure_n,
		const std::string& source_p,
		const std::string& source_n
	);
	void setPositiveMeasureTerminal(const std::string& p);
	void setNegativeMeasureTerminal(const std::string& n);
	void setPositiveSourceTerminal(const std::string& p);
	void setNegativeSourceTerminal(const std::string& n);
	const std::string& getPositiveMeasureTerminal() const;
	const std::string& getNegativeMeasureTerminal() const;
	const std::string& getPositiveSourceTerminal() const;
	const std::string& getNegativeSourceTerminal() const;
	void setValue(const std::string& value);
	const std::string& getValue() const;

	std::string asString() const;
	explicit operator std::string() const;

};

} //namespace lblmc

#endif // LBLMC_COMPONENTCONDUCTANCE_HPP



