/*

Copyright (C) 2019-2021 Matthew Milton

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

//==============================================================================================================================
//==============================================================================================================================

#include <vector>
#include <string>
#include <sstream>
#include <iomanip>
#include <stdexcept>
#include <tuple>
#include <functional>

#include "codegen/ResistiveCompanionElements.hpp"

//==============================================================================================================================
//==============================================================================================================================

namespace lblmc
{

//==============================================================================================================================
//==============================================================================================================================

class SystemConductanceGenerator;
class SystemSourceVectorGenerator;
class SolverEngineGenerator;
class StringProcessor;

//==============================================================================================================================
//==============================================================================================================================

/**
	\brief base class for LB-LMC component model generators for simulation engine code generation

	Unlike ComponentDefinition which provides for more general generation of component model code,
	descendants of Component provide specific, hard-coded generation of code for component models.
	The descendants of Component act as a placeholder until the general component code generation
	is completed.

	Component and its descendants cannot be used for simulation directly, nor for HDL synthesis.

	\author Matthew Milton
	\date Created 2019
**/
class Component
{

protected:

//==============================================================================================================================

	std::string comp_name;

public:

//==============================================================================================================================

	/**
		\brief Embodies a data field for component models
		\date Created 2021.10.19
	**/
	class Field
	{
		public:

		/**
			\brief enumeration of field type
		**/
		enum Type : int
		{
			PARAMETER=0, ///< model input parameter
			CONSTANT,  ///< model internal constant
			PERSISTENT, ///< persistent (state/registered) field
			TEMPORARY,  ///< temporary (intermediate) field
			SIGNAL_IN,  ///< model input signal
			SIGNAL_OUT  ///< model output signal
 		};

 		/**
			\brief enumeration of data type class
 		**/
 		enum DataClass : int
 		{
 			BOOLEAN,        ///< boolean type class.  Associated C++ types include bool or custom boolean types.
 			INTEGER,        ///< integer type class (signed or unsigned).  Associated C++ types include int, long, char, short, etc., or custom integer types.
 			REAL            ///< real-valued type class (signed or unsigned).  Associated C++ types include float, double, custom fixed-point types, etc.
 		};

		Type type; ///< field type

		DataClass data_class; ///< data type class

		std::string data_typename; ///< specific data typename of field.  Type name should be type that is within data type class.

		std::string label; ///< textual label of the field.  Should be valid non-empty C++ label.

		unsigned int length; ///< element length of the field.  0 or 1 for scalar, >1 for array.  Does not apply to parameters or constants that are always scalar.

		double value; ///< numerical value of the field element(s) when first initialized or assigned.  Must fit in range/precision of double-precision floating point.

		Field()
			: type(Type::PARAMETER),
			data_class(DataClass::REAL),
			data_typename("real"),
			label("param"),
			length(1),
			value(0.0)
		{}

		Field
		(
			Type type,
			DataClass data_class,
			const std::string& data_typename,
			const std::string& label,
			unsigned int length,
			double value
		)
			: type(type),
			data_class(data_class),
			data_typename(data_typename),
			label(label),
			length(length),
			value(value)
		{}
	};

	/**
		\brief Embodies a labeled terminal of a component
	**/
	class Terminal
	{
		public:

		std::string label; ///< textual label of the terminal

		unsigned int node_index; ///< nodal connection index of terminal into a system netlist. Value 0 is ground index, <0 are invalid.
	};

	/**
		\brief Embodies a resistive companion model independent source of the component

		These sources do not embody dependent sources (like voltage controlled current sources) which can be handled with
		conductance stamping instead.
	**/
	class Source
	{
		public:

		/**
			\brief type of the source
		**/
		enum Type : int
		{
			THROUGH = 0, ///< through source (like current sources)
			ACROSS ///< across source (like ideal voltage sources)
		};

		std::string label; ///< textual label of the source

		Type type; ///< source type

		unsigned int id;  ///< id number of the source

		Source()
			: label("source"),
			type(THROUGH),
			id(-1)
		{}

		Source(const std::string& label, Type type, unsigned int id)
			: label(label),
			type(type),
			id(id)
		{}


	};

//==============================================================================================================================

		//string constants for common integration methods used by components
	const static std::string INTEGRATION_NONE; //no integration method used
	const static std::string INTEGRATION_EULER_FORWARD;
	const static std::string INTEGRATION_EULER_BACKWARD;
	const static std::string INTEGRATION_TRAPEZOIDAL; //a.k.a. Tustin
	const static std::string INTEGRATION_GEAR;
	const static std::string INTEGRATION_RUNGE_KUTTA_4; //Runge Kutta 4th order

//==============================================================================================================================

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

//==============================================================================================================================

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

		\deprecated This method will be removed in future versions of the library
	**/
	inline virtual void getResistiveCompanionElements(std::vector<ResistiveCompanionElement>& elements) const { elements.clear(); }

	/**
		\brief gets controlled source resistive companion model elements in generated component
		\param vector that will store the elements

		\deprecated This method will be removed in future versions of the library
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

//==============================================================================================================================

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
	virtual void stampSystem(SolverEngineGenerator& gen, const std::vector<std::string>& outputs = {"ALL"});

//==============================================================================================================================

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

//==============================================================================================================================

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
		\brief appends component name to given fields found in given body string
	**/
	std::string& appendNameToFields(std::string& body, const std::vector<Component::Field>& fields) const;

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

	std::string&
	replaceTerminalConnectionNames(std::string& body, const std::vector<Component::Terminal>& terminals);

	std::string&
	replaceSourceNames(std::string& body, const std::vector<Component::Source>& sources);

//==============================================================================================================================

	inline
	void
	generateFieldStrings(std::stringstream& sstrm, const std::vector<std::reference_wrapper<const Component::Field>>& fields)
	{
		for(const auto& field: fields)
		{
			generateFieldString(sstrm, field);
		}
	}

	/**
		\brief generates code string for collection of general Field objects based on their type

		Each field's code definition will be generated each on a new line, delimited by semicolon.
	**/
	inline
	void
	generateFieldStrings(std::stringstream& sstrm, const std::vector<Component::Field>& fields)
	{
		for(const auto& field: fields)
		{
			generateFieldString(sstrm, field);
		}
	}

	/**
		\brief generates code string for collection of signal I/O Field objects based on their type

		Each field's code definition will be generated each on a new line.  Unlike generateFieldStrings(.), each element that is
		not last element will have a comma delimiter before the new line is inserted.

		If given fields have non-signal ones, they will be generated like regular fields, but will have commas and newlines
		added to them, potentially breaking proper C++ formatting of generated code.
	**/
	inline
	void
	generateSignalFieldStrings(std::stringstream& sstrm, const std::vector<Component::Field>& fields)
	{
		for(auto iter = fields.begin(); iter != fields.end(); iter++)
		{
			const auto& field = *iter;

			generateFieldString(sstrm, field);

			if((iter+1) != fields.end())
			{
				sstrm << ",";
			}

			sstrm << "\n";
		}
	}

	/**
		\brief generates code string for general Field object based on its type
	**/
	inline
	void
	generateFieldString(std::stringstream& sstrm, const Component::Field& field )
	{
		switch(field.type)
		{
			case Component::Field::Type::PARAMETER :

				generateParameterConstantFieldString(sstrm, field);

			break;

			case Component::Field::Type::CONSTANT :

				generateParameterConstantFieldString(sstrm, field);

			break;

			case Component::Field::Type::PERSISTENT :

				generatePersistentFieldString(sstrm, field);

			break;

			case Component::Field::Type::TEMPORARY :

				generateTemporaryFieldString(sstrm, field);

			break;

			case Component::Field::Type::SIGNAL_IN :

				generateSignalInFieldString(sstrm, field);

			break;

			case Component::Field::Type::SIGNAL_OUT :

				generateSignalOutFieldString(sstrm, field);

			break;

			default:

			break;
		}
	}

	inline
	void
	generateParameterConstantFieldString(std::stringstream& sstrm, const Component::Field& field)
	{
		switch(field.data_class)
		{
			case Component::Field::DataClass::BOOLEAN :

				generateTypedParameter<bool>
				(
					sstrm,
					field.data_typename,
					field.label,
					bool( field.value )
				);

			break;

			case Component::Field::DataClass::INTEGER :

				generateTypedParameter<long long>
				(
					sstrm,
					field.data_typename,
					field.label,
					(long long)( field.value )
				);

			break;

			case Component::Field::DataClass::REAL :

				generateTypedParameter<double>
				(
					sstrm,
					field.data_typename,
					field.label,
					double( field.value )
				);

			break;
		}
	}

	inline
	void
	generatePersistentFieldString(std::stringstream& sstrm, const Component::Field& field)
	{
		if(field.length <= 1)
		{
			switch(field.data_class)
			{
				case Component::Field::DataClass::BOOLEAN :

					generateTypedPersistentField<bool>
					(
						sstrm,
						field.data_typename,
						field.label,
						bool(field.value)
					);

				break;

				case Component::Field::DataClass::INTEGER :
					generateTypedPersistentField<long long>
					(
						sstrm,
						field.data_typename,
						field.label,
						(long long)(field.value)
					);
				break;

				case Component::Field::DataClass::REAL :
					generateTypedPersistentField<double>
					(
						sstrm,
						field.data_typename,
						field.label,
						double(field.value)
					);
				break;
			}
		}
		else
		{
			switch(field.data_class)
			{
				case Component::Field::DataClass::BOOLEAN :

					generateTypedArrayPersistentField<bool>
					(
						sstrm,
						field.data_typename,
						field.label,
						field.length,
						bool(field.value)
					);

				break;

				case Component::Field::DataClass::INTEGER :
					generateTypedArrayPersistentField<long long>
					(
						sstrm,
						field.data_typename,
						field.label,
						field.length,
						(long long)(field.value)
					);
				break;

				case Component::Field::DataClass::REAL :
					generateTypedArrayPersistentField<double>
					(
						sstrm,
						field.data_typename,
						field.label,
						field.length,
						double(field.value)
					);
				break;
			}
		}


	}

	inline
	void
	generateTemporaryFieldString(std::stringstream& sstrm, const Component::Field& field)
	{
		if(field.length <= 1)
		{
			switch(field.data_class)
			{
				case Component::Field::DataClass::BOOLEAN :

					generateTypedTemporary<bool>
					(
						sstrm,
						field.data_typename,
						field.label,
						bool(field.value)
					);

				break;

				case Component::Field::DataClass::INTEGER :

					generateTypedTemporary<long long>
					(
						sstrm,
						field.data_typename,
						field.label,
						(long long)(field.value)
					);

				break;

				case Component::Field::DataClass::REAL :
					generateTypedTemporary<double>
					(
						sstrm,
						field.data_typename,
						field.label,
						double(field.value)
					);
				break;
			}
		}
		else
		{
			switch(field.data_class)
			{

				case Component::Field::DataClass::BOOLEAN :

					generateTypedArrayTemporary<bool>
					(
						sstrm,
						field.data_typename,
						field.label,
						field.length,
						bool(field.value)
					);

				break;

				case Component::Field::DataClass::INTEGER :

					generateTypedArrayTemporary<long long>
					(
						sstrm,
						field.data_typename,
						field.label,
						field.length,
						(long long)(field.value)
					);

				break;

				case Component::Field::DataClass::REAL :
					generateTypedArrayTemporary<double>
					(
						sstrm,
						field.data_typename,
						field.label,
						field.length,
						double(field.value)
					);
				break;
			}
		}
	}

	inline
	void
	generateSignalInFieldString(std::stringstream& sstrm, const Component::Field& field)
	{
		generateTypedSignalInput
		(
			sstrm,
			field.data_typename,
			field.label,
			field.length
		);
	}

	inline
	void
	generateSignalOutFieldString(std::stringstream& sstrm, const Component::Field& field)
	{
		generateTypedSignalOutput
		(
			sstrm,
			field.data_typename,
			field.label,
			field.length
		);
	}

//==============================================================================================================================

	/**
		\brief generates code string for an input signal port of a component

		\param sstrm string stream which will store the generated string

		\param type type of the signal port in textual form

		\param var_label label of the signal port

		\param length length of the signal port in number of elements.  <=1 scalar, array otherwise.

		If the signal port is a scalar, it is realized as an argument in form of a pass-by-value object.
		If the port is an array, it is realized as an argument in form of fixed-size array ([]) of constants.

		The generated signal port code definition has no ending newline or delimiter.
	**/
	inline
	void
	generateTypedSignalInput(std::stringstream& sstrm, const std::string& type, const std::string& var_label, unsigned int length=1)
	{
		if(length <= 1)
		{
			sstrm
			<< type
			<< " "
			<< var_label
			<< "_" << comp_name
			;
		}
		else
		{
			sstrm
			<< "const "
			<< type
			<< " "
			<< var_label
			<< "_" << comp_name
			<< "["
			<< length
			<< "]"
			;
		}
	}

	/**
		\brief generates code string for an output signal port of a component

		\param sstrm string stream which will store the generated string

		\param type type of the signal port in textual form

		\param var_label label of the signal port

		\param length length of the signal port in number of elements.  <=1 scalar, array otherwise.

		If the signal port is a scalar, it is realized as an argument in form of reference (&).
		If the port is an array, it is realized as an argument in form of fixed-size array ([]).

		The generated signal port code definition has no ending newline or delimiter.
	**/
	inline
	void
	generateTypedSignalOutput(std::stringstream& sstrm, const std::string& type, const std::string& var_label, unsigned int length=1)
	{
		if(length <= 1)
		{
			sstrm
			<< type
			<< "& "
			<< var_label
			<< "_" << comp_name
			;
		}
		else
		{
			sstrm
			<< type
			<< " "
			<< var_label
			<< "_" << comp_name
			<< "["
			<< length
			<< "]"
			;
		}
	}


	/**
		\brief generates string for a parameter
	**/
	inline void generateParameter(std::stringstream& sstrm, std::string var, double value)
	{

		sstrm << "const static "<<"real "<<appendName(var)<<" = "<<value<<";\n";
	}

	/**
		\brief generates string for a set of parameters/constants

		\param sstrm string stream to write generated string to

		\param parameters collection of string-label and value pairs for the parameters
	**/
	inline
	void
	generateRealParameters(std::stringstream& sstrm, const std::vector<std::pair<const std::string&,double>>& parameters)
	{
		for(const auto& param : parameters)
		{
			generateParameter(sstrm, param.first, param.second);
		}
	}

	template<typename T>
	inline void generateTypedParameter(std::stringstream& sstrm, std::string type, std::string var, T value)
	{
		sstrm << "const static "<<type<<" "<<appendName(var)<<" = "<<value<<";\n";
	}

	template<typename TYPE>
	inline
	void
	generateTypedParameters(std::stringstream& sstrm, const std::vector<std::tuple<const std::string&, const std::string&, TYPE>>& parameters)
	{
		for(const auto& param : parameters)
		{
			generateTypedParameter<TYPE>(sstrm, std::get<0>(param), std::get<1>(param), std::get<2>(param));
		}
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
		\brief generates string for a collection of real number persistent fields (static/state var, etc.)
		\deprecated being replaced by generatedRealPersistentFields
	**/
	inline
	void
	generateRealFields(std::stringstream& sstrm, const std::vector<std::pair<const std::string&,double>>& fields)
	{
		for(const auto& field : fields)
		{
			generateField(sstrm, field.first, field.second);
		}
	}

	/**
		\brief generates string for a real number persistent field (static/state var, etc.)
	**/
	inline void generatePersistentField(std::stringstream& sstrm, std::string var, double value)
	{
		sstrm << "static "<<"real "<<appendName(var)<<" = "<<value<<";\n";
	}

	/**
		\brief generates a string for a collection of real-valued persistent fields (static/state var, etc.)

		\param sstrm string stream to write generated string to

		\param fields collection of string-label and value pairs for the fields
	**/
	inline
	void
	generateRealPersistentFields(std::stringstream& sstrm, const std::vector<std::pair<const std::string&,double>>& fields)
	{
		for(const auto& field : fields)
		{
			generatePersistentField(sstrm, field.first, field.second);
		}
	}

	/**
		\brief generates string for a real number temporary field (intermediate var, signals/wires, etc.)
	**/
	inline void generateTemporaryField(std::stringstream& sstrm, std::string var, double value)
	{
		sstrm <<"real "<<appendName(var)<<" = "<<value<<";\n";
	}

	/**
		\brief generates a string for a collection of real-valued temporary fields (intermediate var, signals/wires, etc.)

		\param sstrm string stream to write generated string to

		\param fields collection of string-label and value pairs for the fields
	**/
	inline
	void
	generateRealTemporaryFields(std::stringstream& sstrm, const std::vector<std::pair<const std::string&,double>>& fields)
	{
		for(const auto& field : fields)
		{
			generateTemporaryField(sstrm, field.first, field.second);
		}
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
	inline void generateTypedPersistentField(std::stringstream& sstrm, std::string type, std::string var, T value)
	{

		sstrm << "static "<<type<<" "<<appendName(var)<<" = "<<value<<";\n";
	}

	template<typename TYPE>
	inline
	void
	generateTypedPersistentFields(std::stringstream& sstrm, const std::vector<std::tuple<const std::string&, const std::string&, TYPE>>& fields)
	{
		for(const auto& field : fields)
		{
			generateTypedPersistentField<TYPE>(sstrm, std::get<0>(field), std::get<1>(field), std::get<2>(field));
		}
	}

	/**
		\deprecated for generateTypedArrayPersistentField(.) with same behavior and arguments
	**/
	template<typename T>
	inline void generateTypedArrayField(std::stringstream& sstrm, std::string type, std::string var, long size, T value = 0)
	{
		if(size < 1)
		{
			throw std::invalid_argument("Component::generateTypedArrayField() -- size cannot be less than 1");
		}

		sstrm << "static "<<type<<" "<<appendName(var)<<"["<<size<<"]"<<" = { "<<value;
		for(long i = 1; i<size; i++)
		{
			sstrm << ","<<value;
		}
		sstrm << " };\n";
	}

	template<typename T>
	inline void generateTypedArrayPersistentField(std::stringstream& sstrm, std::string type, std::string var, long size, T value = 0)
	{
		if(size < 1)
		{
			throw std::invalid_argument("Component::generateTypedArrayField() -- size cannot be less than 1");
		}

		sstrm << "static "<<type<<" "<<appendName(var)<<"["<<size<<"]"<<" = { "<<value;
		for(long i = 1; i<size; i++)
		{
			sstrm << ","<<value;
		}
		sstrm << " };\n";
	}

	/**
		\brief generates string for a collection of typed persistent array fields

		\tparam TYPE type of the values stored by the array fields

		\param sstrm string stream to store generated string

		\param array_fields collection of tuples containing each array field's type, label, array-size, and default value for each element in the array field
	**/
	template<typename TYPE>
	inline
	void
	generateTypedArrayPersistentFields
	(
		std::stringstream& sstrm,
		const std::vector< std::tuple<const std::string&, const std::string&, long, const TYPE&> >& array_fields
	)
	{
		for(const auto& field : array_fields)
		{
			generateTypedArrayPersistentField(sstrm, std::get<0>(field), std::get<1>(field), std::get<2>(field), std::get<3>(field));
		}
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

	template<typename T>
	inline void generateTypedArrayTemporary(std::stringstream& sstrm, std::string type, std::string var, long size, T value = 0)
	{
		if(size < 1)
		{
			throw std::invalid_argument("Component::generateTypedArrayField() -- size cannot be less than 1");
		}

		sstrm << type<<" "<<appendName(var)<<"["<<size<<"]"<<" = { "<<value;
		for(long i = 1; i<size; i++)
		{
			sstrm << ","<<value;
		}
		sstrm << " };\n";
	}

};

} //namespace lblmc

#endif // LBLMC_COMPONENT_HPP
