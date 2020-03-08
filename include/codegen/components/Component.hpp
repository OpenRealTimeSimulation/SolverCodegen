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

#ifndef LBLMC_COMPONENT_HPP
#define LBLMC_COMPONENT_HPP

#include <vector>
#include <string>
#include <sstream>
#include <iomanip>
#include <stdexcept>

#include "codegen/ResistiveCompanionElements.hpp"

namespace lblmc
{

class SystemConductanceGenerator;
class SystemSourceVectorGenerator;
class SimulationEngineGenerator;
class StringProcessor;

/**
	\brief base class for LB-LMC component model generators for simulation engine code generation

	Unlike ComponentDefinition which provides for more general generation of component model code,
	descendants of Component provide specific, hard-coded generation of code for component models.
	The descendants of Component act as a placeholder until the general component code generation
	is completed.

	Component and its descendants cannot be used for simulation directly, nor for HDL synthesis.

	\author Matthew Milton
	\date 2019
**/
class Component
{

protected:

	std::string comp_name;

public:

	//struct ResistiveCompanionElement;
	//typedef ResistiveCompanionElement RCE; ///< typedef alias for Component::ResistiveCompanionElement type

		//string constants for common integration methods used by components
	const static std::string INTEGRATION_NONE; //no integration method used
	const static std::string INTEGRATION_EULER_FORWARD;
	const static std::string INTEGRATION_EULER_BACKWARD;
	const static std::string INTEGRATION_TRAPEZOIDAL; //a.k.a. Tustin
	const static std::string INTEGRATION_GEAR;
	const static std::string INTEGRATION_RUNGE_KUTTA_4; //Runge Kutta 4th order

	/**
		\brief default constructor
		\param comp_name name/label for the component generated.  Inheritors of this class should
		throw error if comp_name is null ("").  This name should be unique for all components
		generated.
	**/
	explicit Component(std::string comp_name = "") : comp_name(comp_name) {}

	/**
		\brief copy constructor
	**/
	Component(const Component& base) : comp_name(base.comp_name) {}

	/**
		\return type of component
	**/
	virtual inline std::string getType() const { return std::string("Component");}

	/**
		\brief sets name/label for the generated component
		\param name name/label for the generated component.  Inheritors of this class should
		throw error if comp_name is null ("").  This name should be unique for all components
		generated.
	**/
	inline void setName(std::string name)
	{
		if(name == "")
		{
			throw std::invalid_argument("Component::setName(): comp_name must be a valid, non-empty C++ label");
		}

		comp_name = name;
	}

	/**
		\return name/label of the generated component
	**/
	inline const std::string& getName() const { return comp_name; }

	/**
		\return number of terminals supported by generated component
	**/
	inline virtual unsigned int getNumberOfTerminals() const { return 0; }

	/**
		\return number of companion model sources in generated component
	**/
	inline virtual unsigned int getNumberOfSources() const { return 0; }

	/**
		\return number of ideal voltage sources contained in generated component
	**/
	inline virtual unsigned int getNumberOfIdealVoltageSources() const { return 0; }

	/**
		\brief gets ids for companion model sources in generated component
		\param ids vector to store the ids
		\deprecated This method is being deprecated in favor of getResistiveCompanionElements()
	**/
	inline virtual void getSourceIds(std::vector<unsigned int>& ids) const { ids.clear(); }

	/**
		\brief gets basic resistive companion model elements (source+id+conductance) in generated component
		\param vector that will store the elements
	**/
	inline virtual void getResistiveCompanionElements(std::vector<ResistiveCompanionElement>& elements) const { elements.clear(); }

	/**
		\brief gets controlled source resistive companion model elements in generated component
		\param vector that will store the elements
	**/
	inline virtual void getResistiveCompanionControlledSourceElements
		(std::vector<ResistiveCompanionControlledSourceElement>& elements) const { elements.clear(); }

	/**
		\brief sets terminal connections of generated component by given node indices

		It is up to the children classes to check number of given terminal/node indices is correct

		\param term_ids vector of node indices of the terminal connections
	**/
	inline virtual void setTerminalConnections(const std::vector<unsigned int>& term_ids) {}

	inline virtual void getTerminalConnections(std::vector<unsigned int>& term_ids) const { term_ids.clear(); }

	inline virtual std::vector<unsigned int> getTerminalConnections() const
	{
		std::vector<unsigned int> term_ids;
		getTerminalConnections(term_ids);

		return term_ids;
	}

	inline virtual unsigned int getLargestTerminalConnectionIndex() const
	{
		std::vector<unsigned int> term_ids = getTerminalConnections();
		unsigned int largest_id = 0;

		for(const auto& id : term_ids)
		{
			if(id >= largest_id)
			{
				largest_id = id;
			}
		}

		return largest_id;
	}

	/**
		\brief sets the integration method used by the generated component
		\param method name of the integration method used; the method selected must be supported
		by the component
	**/
	inline virtual void setIntegrationMethod(std::string method) {}

	/**
		\return the name of integration method currently used by generated component
	**/
	inline virtual std::string getIntegrationMethod() const { return INTEGRATION_NONE; }

	/**
		\return vector storing names of supported inputs to generated component
	**/
	inline virtual std::vector<std::string> getSupportedInputs() const { return std::vector<std::string>(); }

	/**
		\return vector storing names of supported outputs from generated component
	**/
	inline virtual std::vector<std::string> getSupportedOutputs() const { return std::vector<std::string>(); }

	/**
		\brief stamps generated component conductances/incidences into the conductance generator
		\param gen the conductance generator that creates conductance matrix for system generated component will reside
	**/
	virtual void stampConductance(SystemConductanceGenerator& gen) {}

	/**
		\brief stamps generated companion model sources of generated component into the source vector generator
		\param gen source vector generator that creates source vector function for system generated component will reside
	**/
	virtual void stampSources(SystemSourceVectorGenerator& gen) {}

	/**
		\brief stamps generated component elements into the simulation solver engine generator
		\param gen the simulation solver engine generator that creates solver code for the system generated component resides
		\param outputs vector of supported outputs that generated component will have; default is ALL outputs supported
	**/
	virtual void stampSystem(SimulationEngineGenerator& gen, std::vector<std::string> outputs = {"ALL"});

	/**
		\brief generates constant static parameters body code for generated component
	**/
	virtual std::string generateParameters() { return std::string(""); }

	/**
		\brief generates fields body code for generated component

		The fields for generated component will include temporary variables, static/state variables,
		etc.
	**/
	virtual std::string generateFields() { return std::string(""); }

	/**
		\brief generates solver function parameter list for inputs of the generated component
	**/
	virtual std::string generateInputs() { return std::string(""); }

	/**
		\brief generates solver function parameter list for outputs of the generated component
	**/
	virtual std::string generateOutputs(std::string output = "ALL") { return std::string(""); }

	/**
		\brief generates update body code for the outputs of the generated component
	**/
	virtual std::string generateOutputsUpdateBody(std::string output = "ALL") { return std::string(""); }

	/**
		\brief generates the main update body for the behavior of the generated component
	**/
	virtual std::string generateUpdateBody() { return std::string(""); }

protected:

	/**
		\brief appends component name to given string
	**/
	inline std::string& appendName(std::string& var) const
	{
		var += "_";
		var += comp_name;
		return var;
	}

	/**
		\brief appends component name to given string
	**/
	inline std::string& appendName(std::string&& var) const
	{
		var += "_";
		var += comp_name;
		return var;
	}

	/**
		\brief appends component name to copy of given string
	**/
	inline std::string appendName(const std::string& var) const
	{
		std::string ret = var;
		ret += "_";
		ret += comp_name;
		return ret;
	}

	/**
		\brief appends component name to given word strings found in given body string
		\param body string whose words will have component name appended to
		\param words words to find in body that will have component name appended to
		\return reference to body string after its words have been appended with component name
	**/
	std::string& appendNameToWords(std::string& body, const std::vector<std::string>& words) const;

	/**
		\brief replaces source name in given code body with source contribution vector b_components[id]
		\param body string the contains code body to modify
		\param src_name name of the source that will be replaced
		\param source_id source contribution index of the source
		\return reference to body string after the modification
	**/
	std::string& replaceSourceNameWithSourceContributionVector(std::string& body, const std::string& src_name, unsigned int source_id );

	/**
		\brief replaces terminal connection names with given id
		\param body string the contains code body to modify
		\param term_name name of the terminal connection that will be replaced with index
		\param index index value to replace name of terminal connection
		\return reference to body string after the modification
	**/
	std::string& replaceTerminalConnectionNameWithIndex(std::string& body, const std::string& term_name, unsigned int index);

	/**
		\brief generates string for a parameter
	**/
	inline void generateParameter(std::stringstream& sstrm, std::string var, double value)
	{

		sstrm << "const static "<<"real "<<appendName(var)<<" = "<<value<<";\n";
	}

	template<typename T>
	inline void generateTypedParameter(std::stringstream& sstrm, std::string type, std::string var, T value)
	{
		sstrm << "const static "<<type<<" "<<appendName(var)<<" = "<<value<<";\n";
	}

	/**
		\brief generates string for a real number persistent field (static/state var, etc.)
		\deprecated being replaced by generatedPersistentField
	**/
	inline void generateField(std::stringstream& sstrm, std::string var, double value)
	{

		sstrm << "static "<<"real "<<appendName(var)<<" = "<<value<<";\n";
	}

	/**
		\brief generates string for a real number persistent field (static/state var, etc.)
	**/
	inline void generatePersistentField(std::stringstream& sstrm, std::string var, double value)
	{
		sstrm << "static "<<"real "<<appendName(var)<<" = "<<value<<";\n";
	}

	/**
		\brief generates string for a real number temporary field (intermediate var, signals/wires, etc.)
	**/
	inline void generateTemporaryField(std::stringstream& sstrm, std::string var, double value)
	{
		sstrm <<"real "<<appendName(var)<<" = "<<value<<";\n";
	}

	/**
		\brief generates string for a boolean temporary field (intermediate var, signals/wires, etc.)
	**/
	inline void generateTemporaryBooleanField(std::stringstream& sstrm, std::string var, bool value)
	{
		sstrm <<"bool "<<appendName(var)<<" = "<< (value?"true":"false") <<";\n";
	}

	template<typename T>
	inline void generateTypedField(std::stringstream& sstrm, std::string type, std::string var, T value)
	{

		sstrm << "static "<<type<<" "<<appendName(var)<<" = "<<value<<";\n";
	}

	template<typename T>
	inline void generateTypedArrayField(std::stringstream& sstrm, std::string type, std::string var, long size)
	{
		if(size < 1)
		{
			throw std::invalid_argument("Component::generateTypedArrayField() -- size cannot be less than 1");
		}

		sstrm << "static "<<type<<" "<<appendName(var)<<"["<<size<<"]"<<" = { "<<T(0);
		for(long i = 1; i<size; i++)
		{
			sstrm << ","<<T(0);
		}
		sstrm << " };\n";
	}

	/**
		\brief generates string for a boolean typed field
		\deprecated Being replaced by generatePersistentBooleanField
	**/
	inline void generateBoolField(std::stringstream& sstrm, std::string var, bool value)
	{
		sstrm << "static "<<"bool "<<appendName(var)<<" = "<< (value?"true":"false") <<";\n";
	}

	/**
		\brief generates string for a boolean typed field
	**/
	inline void generatePersistentBooleanField(std::stringstream& sstrm, std::string var, bool value)
	{
		sstrm << "static "<<"bool "<<appendName(var)<<" = "<< (value?"true":"false") <<";\n";
	}

	template<typename T>
	inline void generateTypedTemporary(std::stringstream& sstrm, std::string type, std::string var, T value)
	{
		sstrm <<type<<" "<<appendName(var)<<" = "<<value<<";\n";
	}

};

} //namespace lblmc

#endif // LBLMC_COMPONENT_HPP
