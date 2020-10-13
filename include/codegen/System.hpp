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

#ifndef LBLMC_SYSTEM_HPP
#define LBLMC_SYSTEM_HPP

#include <vector>
#include <memory>
#include <string>
#include "codegen/components/Component.hpp"
#include "codegen/SolverEngineGenerator.hpp"

namespace lblmc
{

class Component;
class SolverEngineGenerator;

/**
	\brief Defines a System composed of Component Objects which from a C++ LB-LMC
	solver can be code generated to simulate

	Objects of this class store Component objects that define a system network model using LB-LMC
	method.  From these components and their network connections, a System object can generate
	C++ solver code, using SimulationEngineGenerator, to compute solutions and simulate for the
	model.

	System objects require components to all have unique names in same model for code
	generation, and these names must be valid C++ labels (no operators, no spaces, and
	no reserved words; only alphanumerical and underscore characters allowed).

	\author Matthew Milton
	\date 2019
**/
class System
{
private:
	std::string name;
	unsigned int num_nodes;
	unsigned int num_ideal_voltage_sources;
	std::vector<std::unique_ptr<Component>> components;

public:

	System() = delete;

	/**
		\brief parameter constructor
		\param model_name name/label of the model; must be valid C++ label
		\param num_solutions number of solutions (across and through) to solve for model
	**/
	explicit System(std::string name);

	System(const System& base) = delete;

	/**
		\brief move constructor
		\param base object to move contents from into this object
	**/
	System(System&& base);

	/**
		\brief move assignment operator
		\param base object to move contents from into this object
	**/
	System& operator=(System&& base);

	/**
		\return name/label of system
	**/
	const std::string& getName() const;

	/**
		\brief gets number of network nodes in system

		The system's number of nodes is updated when a component is added.
	**/
	const unsigned int& getNumberOfNodes() const;

	/**
		\brief gets number of ideal voltage sources in system

		The system's number of ideal voltage sources is updated when a component is added.
	**/
	const unsigned int& getNumberOfIdealVoltageSources() const;

	/**
		\brief gets number of solutions (across and through) to solve for model

		The system's number of solutions is updated when a component is added.

		\return getNumberOfNodes() + getNumberOfIdealVoltageSources()
	**/
	unsigned int getNumberOfSolutions() const;

	/**
		\return number of components in model
	**/
	unsigned int getNumberOfComponents() const;

	/**
		\brief inserts a component into the system model

		The system model will take full ownership of the component object inserted.  As such, the
		component object should NOT be deleted or destroyed by anything other than the system model
		owning the component.  Also, the component added to the system model should not be a
		temporary object that can go out of scope (in other words, should not reside in stack
		memory).

		The system's number of nodes and ideal voltage sources is updated when a component is added.

		\param component pointer to component to insert
		\return reference to component now owned by system model

	**/
	Component& addComponent(Component* component);

	/**
		\brief inserts a component into the system model

		The system model will take full ownership of the component object inserted.  As such, the
		component object should NOT be deleted or destroyed by anything other than the system model
		owning the component.  Also, the component added to the system model should not be a
		temporary object that can go out of scope (in other words, should not reside in stack
		memory).

		The system's number of nodes and ideal voltage sources is updated when a component is added.

		\param component unique pointer to component to insert
		\return reference to component now owned by system model

	**/
	Component& addComponent(std::unique_ptr<Component>& component);

	/**
		\brief inserts a component into the system model

		The system model will take full ownership of the component object inserted.  As such, the
		component object should NOT be deleted or destroyed by anything other than the system model
		owning the component.  Also, the component added to the system model should not be a
		temporary object that can go out of scope (in other words, should not reside in stack
		memory).

		The system's number of nodes and ideal voltage sources is updated when a component is added.

		\param component unique pointer to component to insert
		\return reference to component now owned by system model

	**/
	Component& addComponent(std::unique_ptr<Component>&& component);

	/**
		\brief gets a component in system model by name

		This method returns first found component with given name, regardless of how many components
		have same name.

		The returned pointer is an observing raw pointer that should NOT be used to delete the
		component.

		\param name name of the component to get
		\return raw observing pointer to component; nullptr if no component of name is found
	**/
	Component* getComponent(std::string name);

	/**
		\brief gets a component in system model by name

		This method returns first found component with given name, regardless of how many components
		have same name.

		The returned pointer is an observing raw pointer that should NOT be used to delete the
		component.  The component object is treated as constant.

		\param name name of the component to get
		\return constant raw observing pointer to component; nullptr if no component of name is
		found
	**/
	const Component* getComponent(std::string name) const;

	/**
		\brief gets all components in the system model
		\return constant reference to internal vector of unique pointers that keep the components
	**/
	const std::vector<std::unique_ptr<Component>>& getComponents() const;

	/**
		\brief checks if components of system model all have unique names

		System objects require components to all have unique names in same model for code
		generation, and these names must be valid C++ labels (no operators, no spaces, and
		no reserved words; only alphanumerical and underscore characters allowed).

		\return true if component names are all unique; false otherwise
	**/
	bool areComponentNamesUnique() const;

};

} //namespace lblmc

#endif // LBLMC_SYSTEM_HPP
