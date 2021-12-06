/*

Copyright (C) 2020 Matthew Milton

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

#ifndef LBLMC_USERDEFINEDCOMPONENTGENERATOR_HPP
#define LBLMC_USERDEFINEDCOMPONENTGENERATOR_HPP

#include <vector>
#include <map>
#include <string>
#include <memory>

#include "codegen/components/Component.hpp"
#include "codegen/udc/UserDefinedComponent.hpp"

namespace lblmc
{

/**
	\brief defines LB-LMC component code generators for User Defined Components (UDC)

	\author Matthew Milton

	\date Created July 3, 2020
**/
class UserDefinedComponentGenerator : public Component
{

private:

//==================================================================================================

	std::shared_ptr<const UserDefinedComponent> component_definition; ///< shared pointer to a UDC definition

	std::map<std::string, double> parameter_value_assignments; ///< assignments of values to UDC parameter by label

	std::map<std::string, unsigned int> terminal_node_assignments; ///< assignments of node ids to UDC terminal by label

	std::map<std::string, unsigned int> through_source_id_assignments; ///< assignments of source ids to UDC through source by label

	std::map<std::string, unsigned int> across_source_id_assignments; ///< assignments of source ids to UDC across source by label

	std::map<std::string, unsigned int> across_source_solution_id_assignments; ///< assignment of solution ids to UDC across sources by label

public:

//==================================================================================================

	/**
		\brief default constructor
	**/
	UserDefinedComponentGenerator
	(
		std::string comp_name = "",
		std::shared_ptr<const UserDefinedComponent> component_def = nullptr
	);

	UserDefinedComponentGenerator
	(
		std::string comp_name,
		std::shared_ptr<const UserDefinedComponent> component_def,
		const std::map<std::string, double>& parameters
	);

	/**
		\brief copy constructor (deleted)
	**/
	UserDefinedComponentGenerator(const UserDefinedComponentGenerator& base) = delete;

	/**
		\brief move constructor
	**/
	UserDefinedComponentGenerator(UserDefinedComponentGenerator&& base);

//==================================================================================================

	/**
		\brief creates a copy of the UDC generator object

		\return unique scoped pointer to created object.  The object copy will persist as long as
		the pointer does.

		\note Due to cost of resource allocation and initialization for a copy of these objects,
		this method should only be used sparingly.
	**/
	std::unique_ptr<UserDefinedComponentGenerator>
	clone() const;

//==================================================================================================

	/**
		\return type of the generated components.  This type will be that of the assigned UDC
		definition, or be empty string if UDC definition is not assigned.
	**/
	std::string
	getType() const;

	/**
		\return number of terminals of the generated components.  If a UDC definition is not
		assigned, then returns 0.
	**/
	unsigned int
	getNumberOfTerminals() const;

	/**
		\return number of through (current) sources of the generated components.  If a UDC
		definition is not assigned, then returns 0.
	**/
	unsigned int
	getNumberOfSources() const;

	/**
		\return number of across (ideal voltage) sources of the generated components.  If a UDC
		definition is not assigned, then returns 0.
	**/
	unsigned int
	getNumberOfIdealVoltageSources() const;

//==================================================================================================

	/**
		\brief assigns an existing UDC component definition to the generator

		\param cd shared scoped pointer to the UDC object.

		If the generator is the last object to reference the formerly assigned component definition
		before the set, then that definition object will be destroyed by its shared pointer in this
		method.
	**/
	void
	setComponentDefinition(std::shared_ptr<UserDefinedComponent> cd);

	/**
		\return observing raw pointer to UDC component definition object.
		Do not attempt to delete or take ownership of the UDC object through this pointer.
	**/
	const UserDefinedComponent*
	getComponentDefinition() const;

    void
    setParameterValue(const std::string& label, const double& value);

    void
    setParameterValues(const std::map<std::string, double>& values);

    void
    setParameterValues(const std::vector<double>& values);

    void
    setTerminalConnection(const std::string& label, const unsigned int& node);

    void
    setTerminalConnections(const std::map<std::string, unsigned int>& nodes);

	void
    setTerminalConnections(const std::vector<unsigned int>& nodes);

	void
	getTerminalConnections(std::vector<unsigned int>& term_ids) const;

	std::vector<unsigned int>
	getTerminalConnections() const;

    void
    setThroughSourceId(const std::string& label, const unsigned int& id);

    void
    setAcrossSourceId(const std::string& label, const unsigned int& id);

    void
    setAcrossSourceSolutionId(const std::string& label, const unsigned int& id);

    void
    getSourceIds(std::vector<unsigned int>& ids) const;

    std::vector<unsigned int>
    getSourceIds() const;

	std::vector<std::string>
	getSupportedInputs() const;

	std::vector<std::string>
	getSupportedOutputs() const;

//==================================================================================================

	void
	stampConductance(SystemConductanceGenerator& gen);

	void
	stampSources(SystemSourceVectorGenerator& gen);

	void
	stampSystem(SolverEngineGenerator& gen, std::vector<std::string> outputs = {"ALL"});

	std::string
	generateParameters();

	std::string
	generateFields();

	std::string
	generateInputs();

	std::string
	generateOutputs(std::string output = "ALL");

	std::string
	generateOutputsUpdateBody(std::string output = "ALL");

	std::string
	generateUpdateBody();

//==================================================================================================

private:

	void
	assertUdcAlive(const std::string& error_message);

//==================================================================================================

//==================================================================================================

};

} //namespace lblmc

#endif // LBLMC_USERDEFINEDCOMPONENTGENERATOR_HPP

