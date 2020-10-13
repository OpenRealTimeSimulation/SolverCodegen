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

#ifndef LBLMC_USERDEFINEDCOMPONENT_HPP
#define LBLMC_USERDEFINEDCOMPONENT_HPP

#include <vector>
#include <string>
#include <memory>

namespace lblmc
{

/**
	\brief defines a User Defined Component (UDC) for LB-LMC Solver Code Generation

	\author Matthew Milton

	\date Created July 1, 2020

    Instances of this class defines a User Defined Component (UDC) whose model can be incorporated
    into code-generated LB-LMC solvers.
    These UDCs can be constructed and put together either during compiler or run time, unlike
    the hardcoded component generators are put together only before compile time.
    UDCs can be also be assembled from definitions in plain-text files, using
    UserDefinedComponentFileParser.
    These UDC objects can be passed to UserDefinedComponentGenerator objects to generate code
    for specific instances of an UDC for a LB-LMC solver.

	A UDC is defined as a discrete state space (SS) model with corresponding resistive companion
	(RC) circuit topology for LB-LMC solvers.
	This discrete SS model is defined in C++ code and is functional to model parameters, constants,
	persistent (state) or temporary (scoped) variables, input signals (s), and system node
	voltages (x).
	The SS model can also have output signals (y).
	The solved states or outputs of these models are mapped to RC topology sources (b), while
	constant coefficients can be mapped to RC topology (trans)conductances (G).
	The model of a SS is defined to be solved for single time step.

	In general, each UDC SS model, with states w, is defined as:

	dw(t)/dt = Aw(t) + Bu(t)
	y(t) = Cw(t) + Du(t)

	To be mapped to RC model (Gx(t+dt)=b(t)), elements of the SS model can be functionally mapped
	accordingly:

	u(t) <= f{x(t), s(t)}
	y(t) <= f{w(t), u(t)}
	b(t) <= f{w(t), s(t)}
	G <= f{A, B}
	x(t+dt) <= f{G, b(t)}

	Each UDC consists of several elements that make up its complete SS and RC model for the LB-LMC
	solvers.
	The following elements define the SS model of the component.
	First, there is the model parameters that are fundamental to the model.
	These parameters include capacitance, inductance, number of switches, etc.
	Next, there is the constant coefficients for the model.
	These constants are typically hard-coded literal values or expressions functional to parameters.
	Some constants can serve as terms for the system conductance matrix G.
	In following is the persistent variables that persist between time steps (memory terms).
	These persistent variables can include state variables and unit delayed variables.
	The counterpart to the persistent variables are the temporary variables which only exist within
	a single time step (memoryless terms).
    Afterward, there is the input and output signal ports for the UDC model.
    Input ports are read into the model and outputs are solved from the model.

    The next elements of the UDC define the RC model of the component.
    The Through sources define current-like sources of the model.
    The states of the UDC are typically mapped to these through sources.
    An alternative source is across sources which define ideal voltage-like sources of the model.
    Like through sources, these across sources are mapped to states of a model.
    However, across sources require computationally more expensive modified nodal analysis for
    the LB-LMC solver, so across sources should not be used in RC models unless it is unavoidable.
    Finally, there are the conductance and transconductance elements for the RC model.
    These conductances are typically mapped to the constant terms of a SS model.
    Transconductance can model Voltage Controlled Current Source (VCCS) type topologies like seen
    in multi-port components.

    The last element of a UDC is the model solving definition itself.
    This model update code is written in C++ and refers to the SS and RC model elements to evaluate
    the model.
    The model update code updates the RC source contributions (b) from the UDC instance, as well as
    output signals (y), using past system solutions (x), input signals (s), and past states (w).
    Different UDC will have different model code, and will have different code based on used
    integration/discretization method for SS model.


**/
class UserDefinedComponent
{

public:

//==================================================================================================
// CONSTANTS

	/**
		\brief enumerations of supported data types in UDCs
	**/
	enum class DataType : char
	{
        UNDEFINED = -1, ///< undefined data type

        CUSTOM    =  0, ///< custom data type

        BOOL,           ///< bool: binary boolean type

        CHAR,           ///< char: 8-bit signed character or integral type

        UCHAR,          ///< uchar (unsigned char): 8-bit unsigned character or integral type

        INT,            ///< int: 32-bit signed integral type

        UINT,           ///< uint (unsigned int): 32-bit unsigned integral type

        LONG,           ///< long (long int): 64-bit signed integral type

        ULONG,          ///< ulong (unsigned long int): 32 or 64-bit unsigned integral type

        DOUBLE,         ///< double: 64-bit floating point type

        REAL            ///< real: real valued numerical type (actual type left to solver implementation)
	};

		// strings indicating data types supported for data elements by UDC

	const static std::string TYPE_UNDEFINED; ///< undefined data type

	const static std::string TYPE_CUSTOM; ///< custom data type

	const static std::string TYPE_BOOL;   ///< bool: binary boolean type

	const static std::string TYPE_CHAR;   ///< char: 8-bit signed character or integral type

	const static std::string TYPE_UCHAR;  ///< uchar (unsigned char): 8-bit unsigned character or integral type

	const static std::string TYPE_INT;    ///< int: 32-bit signed integral type

	const static std::string TYPE_UINT;   ///< uint (unsigned int): 32-bit unsigned integral type

	const static std::string TYPE_LONG;   ///< long (long int): 32 or 64-bit signed integral type

	const static std::string TYPE_ULONG;  ///< ulong (unsigned long int): 64-bit unsigned integral type

	const static std::string TYPE_DOUBLE; ///< double: 64-bit floating point type

	const static std::string TYPE_REAL;   ///< real: real valued numerical type (actual type left to solver implementation)

	const static std::string TYPE_CPP_BOOL;   ///< bool: binary boolean type as c++ equivalent

	const static std::string TYPE_CPP_CHAR;   ///< char: 8-bit signed character or integral type as c++ equivalent

	const static std::string TYPE_CPP_UCHAR;  ///< uchar (unsigned char): 8-bit unsigned character or integral type as c++ equivalent

	const static std::string TYPE_CPP_INT;    ///< int: 32-bit signed integral type as c++ equivalent

	const static std::string TYPE_CPP_UINT;   ///< uint (unsigned int): 32-bit unsigned integral type as c++ equivalent

	const static std::string TYPE_CPP_LONG;   ///< long (long int): 32 or 64-bit signed integral type as c++ equivalent

	const static std::string TYPE_CPP_ULONG;  ///< ulong (unsigned long int): 64-bit unsigned integral type as c++ equivalent

	const static std::string TYPE_CPP_DOUBLE; ///< double: 64-bit floating point type as c++ equivalent

	const static std::string TYPE_CPP_REAL;   ///< real: real valued numerical type (actual type left to solver implementation);  as c++ equivalent



	/**
		\brief enumerations of supported element types in UDCs
	**/
	enum class ElementType : char
	{
		UNDEFINED = -1,   ///< undefined element type

		CUSTOM    =  0,   ///< custom element type

		PARAMETER,        ///< model parameter

		CONSTANT,         ///< model constant coefficient

		PERSISTENT,       ///< persistent variable (between time steps)

		TEMPORARY,        ///< temporary scoped variable (only in time step)

		SIGNAL_PORT,      ///< I/O signal port for UDC in solver

		THROUGH_SOURCE,   ///< resistive companion through (current) source

		ACROSS_SOURCE,    ///< resistive companion across (ideal voltage) source

		CONDUCTANCE,      ///< resistive companion conductance

		TRANSCONDUCTANCE, ///< resistive companion transconductance

		TERMINAL,         ///< model network/circuit terminal

		MODEL_CODE        ///< model update code
	};

		// strings indicating types of elements that make up an UDC

	const static std::string ELEMENT_UNDEFINED;           ///< undefined element type

	const static std::string ELEMENT_CUSTOM;              ///< custom element type

	const static std::string ELEMENT_PARAMETER;        ///< model parameter

	const static std::string ELEMENT_CONSTANT;         ///< model constant coefficient

	const static std::string ELEMENT_PERSISTENT;       ///< persistent variable (between time steps)

	const static std::string ELEMENT_TEMPORARY;        ///< temporary scoped variable (only in time step)

	const static std::string ELEMENT_SIGNAL_PORT;      ///< I/O signal port for UDC in solver

	const static std::string ELEMENT_THROUGH_SOURCE;   ///< resistive companion through (current) source

	const static std::string ELEMENT_ACROSS_SOURCE;    ///< resistive companion across (ideal voltage) source

	const static std::string ELEMENT_CONDUCTANCE;      ///< resistive companion conductance

	const static std::string ELEMENT_TRANSCONDUCTANCE; ///< resistive companion transconductance

	const static std::string ELEMENT_TERMINAL;         ///< model network/circuit terminal

	const static std::string ELEMENT_MODEL_CODE;       ///< model update code

		// other constants

	const static unsigned int NULLID; ///< null id or index

//==================================================================================================
// SUBCLASSES AND TYPE ALIASES

	/**
		\brief defines a data element of the component

		This type is used to define component parameters, variables, and signal I/O ports
	**/
	struct DataElement
	{
		std::string label; ///< label of the data element

		UserDefinedComponent::DataType type; ///< data type of data element

		unsigned int array_size; ///< size of element if array; 0, 1 for scalar non-array, greater than 1 for array

		std::string value; ///< assigned value of the data element; either fixed or functional
	};

	/**
		\brief defines a through (current) source for resistive companion model of UDC
	**/
	struct ThroughSource
	{
		std::string label; ///< label of source

		std::string p_terminal; ///< label of the positive terminal of source

		std::string n_terminal; ///< label of the negative terminal of source

		std::string value; ///< initial/default value of the source; either fixed or functional
	};

	/**
		\brief defines an across (ideal voltage/potential) source for resistive companion model of UDC
	**/
	struct AcrossSource
	{
		std::string label; ///< label of source

		std::string p_terminal; ///< label of the positive terminal of source

		std::string n_terminal; ///< label of the negative terminal of source

		std::string value; ///< initial/default value of the source; either fixed or functional
	};

	/**
		\brief defines a conductance for resistive companion model of UDC
	**/
	struct Conductance
	{
		std::string label; ///< label of conductance

		std::string p_terminal; ///< label of the positive terminal of conductance

		std::string n_terminal; ///< label of the negative terminal of conductance

		std::string value; ///< fixed or functional/expression of conductance (evaluated at codegen time)
	};

	/**
		\brief defines a transconductance (voltage controlled current source) for resistive companion model of UDC
	**/
	struct Transconductance
	{
		std::string label; ///< label of transconductance

		std::string voltage_p_terminal; ///< label of the positive voltage side terminal of transconductance

		std::string voltage_n_terminal; ///< label of the negative voltage side terminal of transconductance

		std::string current_p_terminal; ///< label of the positive current side terminal of transconductance

		std::string current_n_terminal; ///< label of the negative current side terminal of transconductance

		std::string value; ///< fixed or functional/expression of transconductance (evaluated at codegen time)
	};

	/**
		\brief defines a network/circuit terminal of UDC
	**/
	struct Terminal
	{
		std::string label; ///< label of terminal
	};

	typedef unsigned int Id; ///< numerical id type; -1 indicates invalid id value

	typedef unsigned int Node; ///< terminal node connection id type; 0 is common/ground; -1 indicates invalid value



//==================================================================================================
// FIELDS

private:

    std::string type; ///< type of the UDC (i.e. Capacitor, SwitchingConverter, etc.)

    std::string model_label; ///< label of model used for the component

    std::vector<UserDefinedComponent::DataElement> parameters; ///< model parameters

    std::vector<UserDefinedComponent::DataElement> constants; ///< constant coefficients

    std::vector<UserDefinedComponent::DataElement> persistents; ///< persistent variables (across time steps)

    std::vector<UserDefinedComponent::DataElement> temporaries; ///< temporary variables (scoped in one time step)

    std::vector<UserDefinedComponent::DataElement> input_signal_ports; ///< input signal ports

    std::vector<UserDefinedComponent::DataElement> output_signal_ports; ///< output signal ports

    std::vector<UserDefinedComponent::Terminal> terminals; ///< network/circuit terminals

    std::vector<UserDefinedComponent::ThroughSource> through_sources; ///< resistive companion through (current) sources

    std::vector<UserDefinedComponent::AcrossSource> across_sources; ///< resistive companion across (ideal voltage) sources

    std::vector<UserDefinedComponent::Conductance> conductances; ///< resistive companion conductance

    std::vector<UserDefinedComponent::Transconductance> transconductances; ///< resistive companion transconductance

    std::string model_update_code; ///< C++ source code to update UDC model for single time step

//==================================================================================================
//CONSTRUCTORS

public:

	/**
		\brief default constructor
	**/
	UserDefinedComponent();

	/**
		\brief constructs default UDC with given type name

		\throw invalid_argument if type is not valid C++ label
	**/
	explicit
	UserDefinedComponent(const std::string& type, const std::string& model_label);

	/**
		\brief copy constructor (deleted)
	**/
	UserDefinedComponent(const UserDefinedComponent& base) = delete;

	/**
		\brief move constructor
	**/
	UserDefinedComponent(UserDefinedComponent&& base);

//==================================================================================================
// COPY METHODS

	/**
		\brief creates a copy of the UDC object and its members

		\return unique scoped pointer to created object.  The UDC copy will persist as long as the
		pointer does.

		\note Due to cost of resource allocation and initialization for a copy of an UDC object,
		this method should only be used sparingly.
	**/
	std::unique_ptr<UserDefinedComponent>
	clone() const;


//==================================================================================================
// ACCESSOR METHODS

	void
	setType(const std::string& type);

	const std::string&
	getType() const;

	void
	setModelLabel(const std::string& label);

	const std::string&
	getModelLabel() const;

	//element addition

	/**
		\brief adds new parameter element to UDC

		If UDC already has parameter with same label as given element x, then this parameter will be
		replaced by given x
	**/
	void
	addParameter(const UserDefinedComponent::DataElement& x);

	/**
		\brief adds new constant element to UDC

		If UDC already has constant with same label as given element x, then this constant will be
		replaced by given x
	**/
	void
	addConstant(const UserDefinedComponent::DataElement& x);

	/**
		\brief adds new persistent variable element to UDC

		If UDC already has persistent variable with same label as given element x, then this
		persistent variable will be replaced by given x
	**/
	void
	addPersistent(const UserDefinedComponent::DataElement& x);

	/**
		\brief adds new temporary element to UDC

		If UDC already has temporary with same label as given element x, then this temporary will be
		replaced by given x
	**/
	void
	addTemporary(const UserDefinedComponent::DataElement& x);

	/**
		\brief adds new input signal port element to UDC

		If UDC already has input signal port with same label as given element x, then this port
		will be replaced by given x
	**/
	void
	addInputSignalPort(const UserDefinedComponent::DataElement& x);

	/**
		\brief adds new output signal port element to UDC

		If UDC already has output port with same label as given element x, then this port will be
		replaced by given x
	**/
	void
	addOutputSignalPort(const UserDefinedComponent::DataElement& x);

	/**
		\brief adds new terminal element to UDC

		If UDC already has terminal with same label as given element x, then this terminal will be
		replaced by given x
	**/
	void
	addTerminal(const UserDefinedComponent::Terminal& x);

	/**
		\brief adds new through source element to UDC

		If UDC already has through source with same label as given element x, then this source will
		be replaced by given x
	**/
	void
	addThroughSource(const UserDefinedComponent::ThroughSource& x);

	/**
		\brief adds new across source element to UDC

		If UDC already has across source with same label as given element x, then this source will
		be replaced by given x
	**/
	void
	addAcrossSource(const UserDefinedComponent::AcrossSource& x);

	/**
		\brief adds new conductance element to UDC

		If UDC already has conductance with same label as given element x, then this conductance
		will be replaced by given x
	**/
	void
	addConductance(const UserDefinedComponent::Conductance& x);

	/**
		\brief adds new transconductance element to UDC

		If UDC already has transconductance with same label as given element x, then this
		transconductance will be replaced by given x
	**/
	void
	addTransconductance(const UserDefinedComponent::Transconductance& x);

//==================================================================================================

	//safe element fetch

	const std::vector<UserDefinedComponent::DataElement>&
	getParameters() const;

	const std::vector<UserDefinedComponent::DataElement>&
	getConstants() const;

	const std::vector<UserDefinedComponent::DataElement>&
	getPersistents() const;

	const std::vector<UserDefinedComponent::DataElement>&
	getTemporaries() const;

	const std::vector<UserDefinedComponent::DataElement>&
	getInputSignalPorts() const;

	const std::vector<UserDefinedComponent::DataElement>&
	getOutputSignalPorts() const;

	const std::vector<UserDefinedComponent::Terminal>&
	getTerminals() const;

	const std::vector<UserDefinedComponent::ThroughSource>&
	getThroughSources() const;

	const std::vector<UserDefinedComponent::AcrossSource>&
	getAcrossSources() const;

	const std::vector<UserDefinedComponent::Conductance>&
	getConductances() const;

	const std::vector<UserDefinedComponent::Transconductance>&
	getTransconductances() const;

//==================================================================================================

	const UserDefinedComponent::DataElement&
	getParameter(const std::string& label) const;

	const UserDefinedComponent::DataElement&
	getConstant(const std::string& label) const;

	const UserDefinedComponent::DataElement&
	getPersistent(const std::string& label) const;

	const UserDefinedComponent::DataElement&
	getTemporary(const std::string& label) const;

	const UserDefinedComponent::DataElement&
	getInputSignalPort(const std::string& label) const;

	const UserDefinedComponent::DataElement&
	getOutputSignalPort(const std::string& label) const;

	const UserDefinedComponent::Terminal&
	getTerminal(const std::string& label) const;

	const UserDefinedComponent::ThroughSource&
	getThroughSource(const std::string& label) const;

	const UserDefinedComponent::AcrossSource&
	getAcrossSource(const std::string& label) const;

	const UserDefinedComponent::Conductance&
	getConductance(const std::string& label) const;

	const UserDefinedComponent::Transconductance&
	getTransconductance(const std::string& label) const;

//==================================================================================================

	//element lookup

	const UserDefinedComponent::DataElement*
	findParameter(const std::string& label) const;

	const UserDefinedComponent::DataElement*
	findConstant(const std::string& label) const;

	const UserDefinedComponent::DataElement*
	findPersistent(const std::string& label) const;

	const UserDefinedComponent::DataElement*
	findTemporary(const std::string& label) const;

	const UserDefinedComponent::DataElement*
	findInputSignalPort(const std::string& label) const;

	const UserDefinedComponent::DataElement*
	findOutputSignalPort(const std::string& label) const;

	const UserDefinedComponent::Terminal*
	findTerminal(const std::string& label) const;

	const UserDefinedComponent::ThroughSource*
	findThroughSource(const std::string& label) const;

	const UserDefinedComponent::AcrossSource*
	findAcrossSource(const std::string& label) const;

	const UserDefinedComponent::Conductance*
	findConductance(const std::string& label) const;

	const UserDefinedComponent::Transconductance*
	findTransconductance(const std::string& label) const;

//==================================================================================================

	//model code set

	void
	setModelUpdateCode(const std::string& x);

	const std::string&
	getModelUpdateCode() const;

//==================================================================================================
// ENUMERATION CONVERSION METHODS

	/**
		\brief gets name of data type corresponding to given type enumeration

		\param type type enumeration which to get name

		\return name of type as string; returns "undefined" if no enumeration matches
	**/
	static
	const std::string&
	getDataTypeName(const UserDefinedComponent::DataType& type);

	/**
		\brief gets enumeration corresponding to given data type name

		\param name string containing name

		\return enumeration of type; returns UserDefinedComponent::DataType::UNDEFINED if no name
		matches
	**/
	static
	UserDefinedComponent::DataType
	getDataTypeEnum(const std::string& name);

	static
	const std::string&
	getCppDataTypeName(const UserDefinedComponent::DataType& type);

	static
	UserDefinedComponent::DataType
	getCppDataTypeEnum(const std::string& name);

	/**
		\brief gets name of element type corresponding to given type enumeration

		\param type type enumeration which to get name

		\return name of type as string
	**/
	static
	const std::string&
	getElementTypeName(const UserDefinedComponent::ElementType& type);

	/**
		\brief gets enumeration corresponding to given element type name

		\param name string containing name

		\return enumeration of type; returns UserDefinedComponent::ElementType::UNDEFINED if no name
		matches
	**/
	static
	UserDefinedComponent::ElementType
	getElementTypeEnum(const std::string& name);


//==================================================================================================
// VALIDATION/ASSERTATION METHODS

	unsigned int
	containsLabel(const std::string& label) const;

//	bool
//	isTypeValid(const std::string& type) const;

	bool
	isLabelValid(const std::string& x) const;

	bool
	isParameterValid(const UserDefinedComponent::DataElement& x) const;

	bool
	isConstantValid(const UserDefinedComponent::DataElement& x) const;

	bool
	isPersistentValid(const UserDefinedComponent::DataElement& x) const;

	bool
	isTemporaryValid(const UserDefinedComponent::DataElement& x) const;

	bool
	isInputSignalPortValid(const UserDefinedComponent::DataElement& x) const;

	bool
	isOutputSignalPortValid(const UserDefinedComponent::DataElement& x) const;

	bool
	isTerminalValid(const UserDefinedComponent::Terminal& x) const;

	bool
	isThroughSourceValid(const UserDefinedComponent::ThroughSource& x) const;

	bool
	isAcrossSourceValid(const UserDefinedComponent::AcrossSource& x) const;

	bool
	isConductanceValid(const UserDefinedComponent::Conductance& x) const;

	bool
	isTransconductanceValid(const UserDefinedComponent::Transconductance& x) const;


	/**
		\brief checks if the UDC definition is valid

		This method goes through all elements of the UDC and determines if the definition is valid
		for code generation.  This method does not check if an element's value expression is valid
		C++ expression.  That is left up to codegen tools or resultant solver's compiler or
		synthesizer.

		\warning This method is computationally costly, so it should not be called frequently; only
		once after UDC is constructed and elements are added to it.

		\return true if UDC is valid; false otherwise
	**/
	bool
	isValid() const;

//==================================================================================================

//	void
//	assertTypeValid(const std::string& type) const;

	void
	assertParameterValid(const UserDefinedComponent::DataElement& x) const;

	void
	assertConstantValid(const UserDefinedComponent::DataElement& x) const;

	void
	assertPersistentValid(const UserDefinedComponent::DataElement& x) const;

	void
	assertTemporaryValid(const UserDefinedComponent::DataElement& x) const;

	void
	assertInputSignalPortValid(const UserDefinedComponent::DataElement& x) const;

	void
	assertOutputSignalPortValid(const UserDefinedComponent::DataElement& x) const;

	void
	assertTerminalValid(const UserDefinedComponent::Terminal& x) const;

	void
	assertThroughSourceValid(const UserDefinedComponent::ThroughSource& x) const;

	void
	assertAcrossSourceValid(const UserDefinedComponent::AcrossSource& x) const;

	void
	assertConductanceValid(const UserDefinedComponent::Conductance& x) const;

	void
	assertTransconductanceValid(const UserDefinedComponent::Transconductance& x) const;

	/**
		\brief asserts that UDC definition is valid

		This method goes through all elements of the UDC and determines if the definition is valid
		for code generation.  This method does not check if an element's value expression is valid
		C++ expression.  That is left up to codegen tools or resultant solver's compiler or
		synthesizer.

		\warning This method is computationally costly, so it should not be called frequently; only
		once after UDC is constructed and elements are added to it.

		\throw invalid_argument if UDC is not valid

		\return true if UDC is valid; false otherwise
	**/
	void
	assertValid() const;

//==================================================================================================


};

} //namespace lblmc

#endif // LBLMC_USERDEFINEDCOMPONENT_HPP
