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

#ifndef LBLMC_USERDEFINEDCOMPONENT_HPP
#define LBLMC_USERDEFINEDCOMPONENT_HPP

#include <string>
#include <vector>

#include "codegen/components/Component.hpp"
#include "codegen/udc/ComponentDefinition.hpp"
#include "codegen/LabelValuePair.hpp"

namespace lblmc
{


/**
	\brief Component generator for custom user defined components (UDC) not hard-coded into the codegen library
	\author Matthew Milton
	\date 2019
**/
class UserDefinedComponent : public Component
{

private:

	const ComponentDefinition* comp_def; ///< component definition via raw observing pointer used by the custom UDC
	std::vector<LabelValuePair<double>> parameter_assignments; ///< label-value pairs for parameter assignments
	std::vector<LabelValuePair<unsigned int>> terminal_assignments; ///< label-value pairs for terminal assignments
	std::vector<LabelValuePair<unsigned int>> through_source_assignments; ///< label-value pairs for through source assignments
	std::vector<LabelValuePair<unsigned int>> across_source_assignments; ///< label-value pairs for across source assignments

public:

	/**
		\brief constructor
		\param comp_name instance name of the component
	**/
	UserDefinedComponent(std::string comp_name);

	/**
		\brief constructor
		\param comp_name instance name of the component
		\param comp_def component definition used by the UDC; the
		definition should persist at least the lifetime of the constructed UDC object.  The
		constructed UDC object does *not* take ownership of the component definition
	**/
	UserDefinedComponent(std::string comp_name, const ComponentDefinition& comp_def);

	/**
		\brief move constructor
		\param base the component that will have its contents moved to this one; base will be
		modified in the process.
	**/
	UserDefinedComponent(UserDefinedComponent && base);

	/**
		\brief copy constructor
		\param base component that will be copied from

		Use the move constructor instead if base is a temporary object

	**/
	UserDefinedComponent(const UserDefinedComponent& base);

	/**
		\brief sets component definition used by the UDC
		\param comp_def component definition used by the UDC; the
		definition should persist at least the lifetime of the constructed UDC object.  The
		UDC object does *not* take ownership of the component definition
	**/
	void setComponentDefinition(const ComponentDefinition& comp_def);
	const ComponentDefinition& getComponentDefinition() const;
	const ComponentDefinition* const getComponentDefinitionPointer() const;

	void setParameter(const std::string& param, double value);
	void setParameter(const LabelValuePair<double>& param_value);
	void setTerminal(const std::string& term, unsigned int index);
	void setTerminal(const LabelValuePair<unsigned int>& term_index);
	void setThroughSource(const std::string& src, unsigned int index);
	void setThroughSource(const LabelValuePair<unsigned int>& src_index);
	void setAcrossSource(const std::string& src, unsigned int index);
	void setAcrossSource(const LabelValuePair<unsigned int>& src_index);

	/**
		\return type name of the component
	**/
	std::string getType() const;

	unsigned int getNumberOfTerminals() const;
	unsigned int getNumberOfSources() const;

	void getSourceIds(std::vector<unsigned int>& ids) const;

	void setTerminalConnections(const std::vector<unsigned int>& term_ids);
	void getTerminalConnections(std::vector<unsigned int>& term_ids) const;

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

#endif // LBLMC_USERDEFINEDCOMPONENT_HPP


