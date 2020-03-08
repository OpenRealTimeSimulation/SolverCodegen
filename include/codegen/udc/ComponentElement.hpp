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

#ifndef LBLMC_COMPONENTELEMENT_HPP
#define LBLMC_COMPONENTELEMENT_HPP

#include <string>

namespace lblmc
{

/**
	\brief defines the base class for elements that are used to assemble a component of a system
**/
class ComponentElement
{
protected:

	std::string label;        ///< unique label for the element; must be valid C++ label; default is "label"
	std::string element_type; ///< unique data type (class name) of the element

public:

	const static unsigned int null_index = -1; ///< indicates a null or invalid numerical index
	const static unsigned int null_id    = -1; ///< indicates a null or invalid numerical id

	ComponentElement();
	ComponentElement(const std::string& label);
	ComponentElement(const ComponentElement& base);
	ComponentElement(ComponentElement&& base);

	void setLabel(const std::string& label);
	const std::string& getLabel() const;
	const std::string& getElementType() const;

	/**
		\brief casts the element as a printable string for code generation

		This method is overridden by children classes of ComponentElement
	**/
	virtual std::string asString() const;

	/**
		\brief casts the element as a printable string for code generation

		This method is overridden by children classes of ComponentElement
	**/
	virtual explicit operator std::string() const;


};

} //namespace lblmc

#endif // LBLMC_COMPONENTELEMENT_HPP

