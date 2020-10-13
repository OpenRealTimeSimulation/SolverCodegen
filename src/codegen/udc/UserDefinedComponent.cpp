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

#include "codegen/udc/UserDefinedComponent.hpp"

#include "codegen/Cpp.hpp"

#include <utility>
#include <vector>
#include <string>
#include <memory>
#include <stdexcept>

namespace lblmc
{

//==================================================================================================

const std::string UserDefinedComponent::TYPE_UNDEFINED("undefined");

const std::string UserDefinedComponent::TYPE_CUSTOM("custom");

const std::string UserDefinedComponent::TYPE_BOOL("bool");

const std::string UserDefinedComponent::TYPE_CHAR("char");

const std::string UserDefinedComponent::TYPE_UCHAR("uchar");

const std::string UserDefinedComponent::TYPE_INT("int");

const std::string UserDefinedComponent::TYPE_UINT("uint");

const std::string UserDefinedComponent::TYPE_LONG("long");

const std::string UserDefinedComponent::TYPE_ULONG("ulong");

const std::string UserDefinedComponent::TYPE_DOUBLE("double");

const std::string UserDefinedComponent::TYPE_REAL("real");

const std::string UserDefinedComponent::TYPE_CPP_BOOL(UserDefinedComponent::TYPE_BOOL);

const std::string UserDefinedComponent::TYPE_CPP_CHAR(UserDefinedComponent::TYPE_CHAR);

const std::string UserDefinedComponent::TYPE_CPP_UCHAR("unsigned char");

const std::string UserDefinedComponent::TYPE_CPP_INT(UserDefinedComponent::TYPE_INT);

const std::string UserDefinedComponent::TYPE_CPP_UINT("unsigned int");

const std::string UserDefinedComponent::TYPE_CPP_LONG(UserDefinedComponent::TYPE_LONG);

const std::string UserDefinedComponent::TYPE_CPP_ULONG("unsigned long");

const std::string UserDefinedComponent::TYPE_CPP_DOUBLE(UserDefinedComponent::TYPE_DOUBLE);

const std::string UserDefinedComponent::TYPE_CPP_REAL(UserDefinedComponent::TYPE_REAL);

const std::string UserDefinedComponent::ELEMENT_UNDEFINED("undefined");

const std::string UserDefinedComponent::ELEMENT_CUSTOM("custom");

const std::string UserDefinedComponent::ELEMENT_PARAMETER("Parameter");

const std::string UserDefinedComponent::ELEMENT_CONSTANT("Constant");

const std::string UserDefinedComponent::ELEMENT_PERSISTENT("Persistent");

const std::string UserDefinedComponent::ELEMENT_TEMPORARY("Temporary");

const std::string UserDefinedComponent::ELEMENT_SIGNAL_PORT("SignalPort");

const std::string UserDefinedComponent::ELEMENT_THROUGH_SOURCE("ThroughSource");

const std::string UserDefinedComponent::ELEMENT_ACROSS_SOURCE("AcrossSource");

const std::string UserDefinedComponent::ELEMENT_CONDUCTANCE("Conductance");

const std::string UserDefinedComponent::ELEMENT_TRANSCONDUCTANCE("Transconductance");

const std::string UserDefinedComponent::ELEMENT_TERMINAL("Terminal");

const std::string UserDefinedComponent::ELEMENT_MODEL_CODE("ModelCode");

const unsigned int UserDefinedComponent::NULLID(-1);

//==================================================================================================

UserDefinedComponent::UserDefinedComponent() :
	type("udc"),
	model_label("undefined"),
	parameters(),
	constants(),
	persistents(),
	temporaries(),
    input_signal_ports(),
    output_signal_ports(),
    terminals(),
    through_sources(),
    across_sources(),
    conductances(),
    transconductances(),
    model_update_code()
{}

UserDefinedComponent::UserDefinedComponent(const std::string& type, const std::string& model_label) :
	type(type),
	model_label(model_label),
	parameters(),
	constants(),
	persistents(),
	temporaries(),
    input_signal_ports(),
    output_signal_ports(),
    terminals(),
    through_sources(),
    across_sources(),
    conductances(),
    transconductances(),
    model_update_code()
{
    if(!Cpp::isNameValid(type) || !Cpp::isNameValid(model_label))
	{
		throw
		std::invalid_argument
		(
			"UserDefinedComponent::UserDefinedComponent(const std::string& type, const std::string& model_label) -- type and model_label must be non-empty and valid C++ labels"
		);
	}
}

UserDefinedComponent::UserDefinedComponent(UserDefinedComponent&& base) :
	type(std::move(base.type)),
	model_label(std::move(base.model_label)),
	parameters(std::move(base.parameters)),
	constants(std::move(base.constants)),
	persistents(std::move(base.persistents)),
	temporaries(std::move(base.temporaries)),
    input_signal_ports(std::move(base.input_signal_ports)),
    output_signal_ports(std::move(base.output_signal_ports)),
    terminals(std::move(base.terminals)),
    through_sources(std::move(base.through_sources)),
    across_sources(std::move(base.across_sources)),
    conductances(std::move(base.conductances)),
    transconductances(std::move(base.transconductances)),
    model_update_code(std::move(base.model_update_code))
{}

//==================================================================================================

std::unique_ptr<UserDefinedComponent>
UserDefinedComponent::clone() const
{
	std::unique_ptr<UserDefinedComponent> new_udc = std::make_unique<UserDefinedComponent>();

	new_udc->type = type;
	new_udc->model_label = model_label;
	new_udc->parameters = parameters;
	new_udc->constants = constants;
	new_udc->persistents = persistents;
	new_udc->temporaries = temporaries;
    new_udc->input_signal_ports = input_signal_ports;
    new_udc->output_signal_ports = output_signal_ports;
    new_udc->terminals = terminals;
    new_udc->through_sources = through_sources;
    new_udc->across_sources = across_sources;
    new_udc->conductances = conductances;
    new_udc->transconductances = transconductances;
    new_udc->model_update_code = model_update_code;

    return new_udc;
}

//==================================================================================================

void
UserDefinedComponent::setType(const std::string& type)
{
	if(!Cpp::isNameValid(type))
	{
		throw
		std::invalid_argument
		(
			"UserDefinedComponent::setType(const std::string& type) -- type must be non-empty and valid C++ label"
		);
	}

	this->type = type;
}

const std::string&
UserDefinedComponent::getType() const
{
	return type;
}

void
UserDefinedComponent::setModelLabel(const std::string& label)
{
	if(!Cpp::isNameValid(model_label))
	{
		throw
		std::invalid_argument
		(
			"UserDefinedComponent::setModelLabel(const std::string& label) -- method must be non-empty and valid C++ labels"
		);
	}

	this->model_label = label;
}

const std::string&
UserDefinedComponent::getModelLabel() const
{
	return model_label;
}

void
UserDefinedComponent::addParameter(const UserDefinedComponent::DataElement& x)
{
	for(auto& e : parameters)
	{
        if(e.label == x.label)
		{
			e = x;
			return;
		}
	}

	parameters.push_back(x);
}

void
UserDefinedComponent::addConstant(const UserDefinedComponent::DataElement& x)
{
	for(auto& e : constants)
	{
        if(e.label == x.label)
		{
			e = x;
			return;
		}
	}

	constants.push_back(x);
}

void
UserDefinedComponent::addPersistent(const UserDefinedComponent::DataElement& x)
{
	for(auto& e : persistents)
	{
        if(e.label == x.label)
		{
			e = x;
			return;
		}
	}

	persistents.push_back(x);
}

void
UserDefinedComponent::addTemporary(const UserDefinedComponent::DataElement& x)
{
	for(auto& e : temporaries)
	{
        if(e.label == x.label)
		{
			e = x;
			return;
		}
	}

	temporaries.push_back(x);
}

void
UserDefinedComponent::addInputSignalPort(const UserDefinedComponent::DataElement& x)
{
	for(auto& e : input_signal_ports)
	{
        if(e.label == x.label)
		{
			e = x;
			return;
		}
	}

	input_signal_ports.push_back(x);
}

void
UserDefinedComponent::addOutputSignalPort(const UserDefinedComponent::DataElement& x)
{
	for(auto& e : output_signal_ports)
	{
        if(e.label == x.label)
		{
			e = x;
			return;
		}
	}

	output_signal_ports.push_back(x);
}

void
UserDefinedComponent::addTerminal(const UserDefinedComponent::Terminal& x)
{
	for(auto& e : terminals)
	{
        if(e.label == x.label)
		{
			e = x;
			return;
		}
	}

	terminals.push_back(x);
}

void
UserDefinedComponent::addThroughSource(const UserDefinedComponent::ThroughSource& x)
{
	for(auto& e : through_sources)
	{
        if(e.label == x.label)
		{
			e = x;
			return;
		}
	}

	through_sources.push_back(x);
}

void
UserDefinedComponent::addAcrossSource(const UserDefinedComponent::AcrossSource& x)
{
	for(auto& e : across_sources)
	{
        if(e.label == x.label)
		{
			e = x;
			return;
		}
	}

	across_sources.push_back(x);
}

void
UserDefinedComponent::addConductance(const UserDefinedComponent::Conductance& x)
{
	for(auto& e : conductances)
	{
        if(e.label == x.label)
		{
			e = x;
			return;
		}
	}

	conductances.push_back(x);
}

void
UserDefinedComponent::addTransconductance(const UserDefinedComponent::Transconductance& x)
{
	for(auto& e : transconductances)
	{
        if(e.label == x.label)
		{
			e = x;
			return;
		}
	}

	transconductances.push_back(x);
}


//==================================================================================================

const std::vector<UserDefinedComponent::DataElement>&
UserDefinedComponent::getParameters() const
{
	return parameters;
}

const std::vector<UserDefinedComponent::DataElement>&
UserDefinedComponent::getConstants() const
{
	return constants;
}

const std::vector<UserDefinedComponent::DataElement>&
UserDefinedComponent::getPersistents() const
{
	return persistents;
}

const std::vector<UserDefinedComponent::DataElement>&
UserDefinedComponent::getTemporaries() const
{
	return temporaries;
}

const std::vector<UserDefinedComponent::DataElement>&
UserDefinedComponent::getInputSignalPorts() const
{
	return input_signal_ports;
}

const std::vector<UserDefinedComponent::DataElement>&
UserDefinedComponent::getOutputSignalPorts() const
{
	return output_signal_ports;
}

const std::vector<UserDefinedComponent::Terminal>&
UserDefinedComponent::getTerminals() const
{
	return terminals;
}

const std::vector<UserDefinedComponent::ThroughSource>&
UserDefinedComponent::getThroughSources() const
{
	return through_sources;
}

const std::vector<UserDefinedComponent::AcrossSource>&
UserDefinedComponent::getAcrossSources() const
{
	return across_sources;
}

const std::vector<UserDefinedComponent::Conductance>&
UserDefinedComponent::getConductances() const
{
	return conductances;
}

const std::vector<UserDefinedComponent::Transconductance>&
UserDefinedComponent::getTransconductances() const
{
	return transconductances;
}

//==================================================================================================

const UserDefinedComponent::DataElement&
UserDefinedComponent::getParameter(const std::string& label) const
{
	for(const auto& e : parameters)
	{
        if(e.label == label)
		{
			return e;
		}
	}

	throw std::out_of_range
	(
		"UserDefinedComponent::getParameter(const std::string& label) const -- element with given label does not exist"
	);
}

const UserDefinedComponent::DataElement&
UserDefinedComponent::getConstant(const std::string& label) const
{
	for(const auto& e : constants)
	{
        if(e.label == label)
		{
			return e;
		}
	}

	throw std::out_of_range
	(
		"UserDefinedComponent::getConstant(const std::string& label) const -- element with given label does not exist"
	);
}

const UserDefinedComponent::DataElement&
UserDefinedComponent::getPersistent(const std::string& label) const
{
	for(const auto& e : persistents)
	{
        if(e.label == label)
		{
			return e;
		}
	}

	throw std::out_of_range
	(
		"UserDefinedComponent::getPersistent(const std::string& label) const -- element with given label does not exist"
	);
}

const UserDefinedComponent::DataElement&
UserDefinedComponent::getTemporary(const std::string& label) const
{
	for(const auto& e : temporaries)
	{
        if(e.label == label)
		{
			return e;
		}
	}

	throw std::out_of_range
	(
		"UserDefinedComponent::getTemporary(const std::string& label) const -- element with given label does not exist"
	);
}

const UserDefinedComponent::DataElement&
UserDefinedComponent::getInputSignalPort(const std::string& label) const
{
	for(const auto& e : input_signal_ports)
	{
        if(e.label == label)
		{
			return e;
		}
	}

	throw std::out_of_range
	(
		"UserDefinedComponent::getInputSignalPort(const std::string& label) const -- element with given label does not exist"
	);
}

const UserDefinedComponent::DataElement&
UserDefinedComponent::getOutputSignalPort(const std::string& label) const
{
	for(const auto& e : output_signal_ports)
	{
        if(e.label == label)
		{
			return e;
		}
	}

	throw std::out_of_range
	(
		"UserDefinedComponent::getOutputSignalPort(const std::string& label) const -- element with given label does not exist"
	);
}

const UserDefinedComponent::Terminal&
UserDefinedComponent::getTerminal(const std::string& label) const
{
	for(const auto& e : terminals)
	{
        if(e.label == label)
		{
			return e;
		}
	}

	throw std::out_of_range
	(
		"UserDefinedComponent::getTerminal(const std::string& label) const -- element with given label does not exist"
	);
}

const UserDefinedComponent::ThroughSource&
UserDefinedComponent::getThroughSource(const std::string& label) const
{
	for(const auto& e : through_sources)
	{
        if(e.label == label)
		{
			return e;
		}
	}

	throw std::out_of_range
	(
		"UserDefinedComponent::getThroughSource(const std::string& label) const -- element with given label does not exist"
	);
}

const UserDefinedComponent::AcrossSource&
UserDefinedComponent::getAcrossSource(const std::string& label) const
{
	for(const auto& e : across_sources)
	{
        if(e.label == label)
		{
			return e;
		}
	}

	throw std::out_of_range
	(
		"UserDefinedComponent::getAcrossSource(const std::string& label) const -- element with given label does not exist"
	);
}

const UserDefinedComponent::Conductance&
UserDefinedComponent::getConductance(const std::string& label) const
{
	for(const auto& e : conductances)
	{
        if(e.label == label)
		{
			return e;
		}
	}

	throw std::out_of_range
	(
		"UserDefinedComponent::getConductance(const std::string& label) const -- element with given label does not exist"
	);
}

const UserDefinedComponent::Transconductance&
UserDefinedComponent::getTransconductance(const std::string& label) const
{
	for(const auto& e : transconductances)
	{
        if(e.label == label)
		{
			return e;
		}
	}

	throw std::out_of_range
	(
		"UserDefinedComponent::getTransconductance(const std::string& label) const -- element with given label does not exist"
	);
}

//==================================================================================================

const UserDefinedComponent::DataElement*
UserDefinedComponent::findParameter(const std::string& label) const
{
	for(const auto& e : parameters)
	{
        if(e.label == label)
		{
			return &e;
		}
	}

	return nullptr;
}

const UserDefinedComponent::DataElement*
UserDefinedComponent::findConstant(const std::string& label) const
{
	for(const auto& e : constants)
	{
        if(e.label == label)
		{
			return &e;
		}
	}

	return nullptr;
}

const UserDefinedComponent::DataElement*
UserDefinedComponent::findPersistent(const std::string& label) const
{
	for(const auto& e : persistents)
	{
        if(e.label == label)
		{
			return &e;
		}
	}

	return nullptr;
}

const UserDefinedComponent::DataElement*
UserDefinedComponent::findTemporary(const std::string& label) const
{
	for(const auto& e : temporaries)
	{
        if(e.label == label)
		{
			return &e;
		}
	}

	return nullptr;
}

const UserDefinedComponent::DataElement*
UserDefinedComponent::findInputSignalPort(const std::string& label) const
{
	for(const auto& e : input_signal_ports)
	{
        if(e.label == label)
		{
			return &e;
		}
	}

	return nullptr;
}

const UserDefinedComponent::DataElement*
UserDefinedComponent::findOutputSignalPort(const std::string& label) const
{
	for(const auto& e : output_signal_ports)
	{
        if(e.label == label)
		{
			return &e;
		}
	}

	return nullptr;
}

const UserDefinedComponent::Terminal*
UserDefinedComponent::findTerminal(const std::string& label) const
{
	for(const auto& e : terminals)
	{
        if(e.label == label)
		{
			return &e;
		}
	}

	return nullptr;
}

const UserDefinedComponent::ThroughSource*
UserDefinedComponent::findThroughSource(const std::string& label) const
{
	for(const auto& e : through_sources)
	{
        if(e.label == label)
		{
			return &e;
		}
	}

	return nullptr;
}

const UserDefinedComponent::AcrossSource*
UserDefinedComponent::findAcrossSource(const std::string& label) const
{
	for(const auto& e : across_sources)
	{
        if(e.label == label)
		{
			return &e;
		}
	}

	return nullptr;
}

const UserDefinedComponent::Conductance*
UserDefinedComponent::findConductance(const std::string& label) const
{
	for(const auto& e : conductances)
	{
        if(e.label == label)
		{
			return &e;
		}
	}

	return nullptr;
}

const UserDefinedComponent::Transconductance*
UserDefinedComponent::findTransconductance(const std::string& label) const
{
	for(const auto& e : transconductances)
	{
        if(e.label == label)
		{
			return &e;
		}
	}

	return nullptr;
}

//==================================================================================================

void
UserDefinedComponent::setModelUpdateCode(const std::string& x)
{
    model_update_code = x;
}

const std::string&
UserDefinedComponent::getModelUpdateCode() const
{
	return model_update_code;
}

//==================================================================================================
// ENUMERATION CONVERSION METHODS

const std::string&
UserDefinedComponent::getDataTypeName(const UserDefinedComponent::DataType& type)
{
	switch(type)
	{
		case DataType::CUSTOM :
			return TYPE_CUSTOM;
		break;

		case DataType::BOOL :
			return TYPE_BOOL;
		break;

		case DataType::CHAR :
			return TYPE_CHAR;
		break;

		case DataType::UCHAR :
			return TYPE_UCHAR;
		break;

		case DataType::INT :
			return TYPE_INT;
		break;

		case DataType::UINT :
			return TYPE_UINT;
		break;

		case DataType::LONG :
			return TYPE_LONG;
		break;

		case DataType::ULONG :
			return TYPE_ULONG;
		break;

		case DataType::DOUBLE :
			return TYPE_DOUBLE;
		break;

		case DataType::REAL :
			return TYPE_REAL;
		break;

		default:
			return TYPE_UNDEFINED;
		break;
	}
}

UserDefinedComponent::DataType
UserDefinedComponent::getDataTypeEnum(const std::string& name)
{

	if(name == TYPE_CUSTOM)
	{
		return DataType::CUSTOM;
	}

	else if(name == TYPE_BOOL)
	{
		return DataType::BOOL;
	}

	else if(name == TYPE_CHAR)
	{
		return DataType::CHAR;
	}

	else if(name == TYPE_UCHAR)
	{
		return DataType::UCHAR;
	}

	else if(name == TYPE_INT)
	{
		return DataType::INT;
	}

	else if(name == TYPE_UINT)
	{
		return DataType::UINT;
	}

	else if(name == TYPE_LONG)
	{
		return DataType::LONG;
	}

	else if(name == TYPE_ULONG)
	{
		return DataType::ULONG;
	}

	else if(name == TYPE_DOUBLE)
	{
		return DataType::DOUBLE;
	}

	else if(name == TYPE_REAL)
	{
		return DataType::REAL;
	}

	else
	{
		return DataType::UNDEFINED;
	}
}

const std::string&
UserDefinedComponent::getCppDataTypeName(const UserDefinedComponent::DataType& type)
{
	switch(type)
	{
		case DataType::CUSTOM :
			return TYPE_CUSTOM;
		break;

		case DataType::BOOL :
			return TYPE_CPP_BOOL;
		break;

		case DataType::CHAR :
			return TYPE_CPP_CHAR;
		break;

		case DataType::UCHAR :
			return TYPE_CPP_UCHAR;
		break;

		case DataType::INT :
			return TYPE_CPP_INT;
		break;

		case DataType::UINT :
			return TYPE_CPP_UINT;
		break;

		case DataType::LONG :
			return TYPE_CPP_LONG;
		break;

		case DataType::ULONG :
			return TYPE_CPP_ULONG;
		break;

		case DataType::DOUBLE :
			return TYPE_CPP_DOUBLE;
		break;

		case DataType::REAL :
			return TYPE_CPP_REAL;
		break;

		default:
			return TYPE_UNDEFINED;
		break;
	}
}

UserDefinedComponent::DataType
UserDefinedComponent::getCppDataTypeEnum(const std::string& name)
{
	if(name == TYPE_CUSTOM)
	{
		return DataType::CUSTOM;
	}

	else if(name == TYPE_CPP_BOOL)
	{
		return DataType::BOOL;
	}

	else if(name == TYPE_CPP_CHAR)
	{
		return DataType::CHAR;
	}

	else if(name == TYPE_CPP_UCHAR)
	{
		return DataType::UCHAR;
	}

	else if(name == TYPE_CPP_INT)
	{
		return DataType::INT;
	}

	else if(name == TYPE_CPP_UINT)
	{
		return DataType::UINT;
	}

	else if(name == TYPE_CPP_LONG)
	{
		return DataType::LONG;
	}

	else if(name == TYPE_CPP_ULONG)
	{
		return DataType::ULONG;
	}

	else if(name == TYPE_CPP_DOUBLE)
	{
		return DataType::DOUBLE;
	}

	else if(name == TYPE_CPP_REAL)
	{
		return DataType::REAL;
	}

	else
	{
		return DataType::UNDEFINED;
	}
}

const std::string&
UserDefinedComponent::getElementTypeName(const UserDefinedComponent::ElementType& type)
{
	switch(type)
	{
		case ElementType::CUSTOM :
			return ELEMENT_CUSTOM;
		break;

		case ElementType::PARAMETER :
			return ELEMENT_PARAMETER;
		break;

		case ElementType::CONSTANT :
			return ELEMENT_CONSTANT;
		break;

		case ElementType::PERSISTENT :
			return ELEMENT_PERSISTENT;
		break;

		case ElementType::TEMPORARY :
			return ELEMENT_TEMPORARY;
		break;

		case ElementType::SIGNAL_PORT :
			return ELEMENT_SIGNAL_PORT;
		break;

		case ElementType::THROUGH_SOURCE :
			return ELEMENT_THROUGH_SOURCE;
		break;

		case ElementType::ACROSS_SOURCE :
			return ELEMENT_ACROSS_SOURCE;
		break;

		case ElementType::CONDUCTANCE :
			return ELEMENT_CONDUCTANCE;
		break;

		case ElementType::TRANSCONDUCTANCE :
			return ELEMENT_TRANSCONDUCTANCE;
		break;

		case ElementType::TERMINAL :
			return ELEMENT_TERMINAL;
		break;

		case ElementType::MODEL_CODE :
			return ELEMENT_MODEL_CODE;
		break;

		default:
			return ELEMENT_UNDEFINED;
		break;
	}
}

UserDefinedComponent::ElementType
UserDefinedComponent::getElementTypeEnum(const std::string& name)
{

	if(name == ELEMENT_CUSTOM)
	{
		return ElementType::CUSTOM;
	}

	else if(name == ELEMENT_PARAMETER)
	{
		return ElementType::PARAMETER;
	}

	else if(name == ELEMENT_CONSTANT)
	{
		return ElementType::CONSTANT;
	}

	else if(name == ELEMENT_PERSISTENT)
	{
		return ElementType::PERSISTENT;
	}

	else if(name == ELEMENT_TEMPORARY)
	{
		return ElementType::TEMPORARY;
	}

	else if(name == ELEMENT_SIGNAL_PORT)
	{
		return ElementType::SIGNAL_PORT;
	}

	else if(name == ELEMENT_THROUGH_SOURCE)
	{
		return ElementType::THROUGH_SOURCE;
	}

	else if(name == ELEMENT_ACROSS_SOURCE)
	{
		return ElementType::ACROSS_SOURCE;
	}

	else if(name == ELEMENT_CONDUCTANCE)
	{
		return ElementType::CONDUCTANCE;
	}

	else if(name == ELEMENT_TRANSCONDUCTANCE)
	{
		return ElementType::TRANSCONDUCTANCE;
	}

	else if(name == ELEMENT_TERMINAL)
	{
		return ElementType::TERMINAL;
	}

	else if(name == ELEMENT_MODEL_CODE)
	{
		return ElementType::MODEL_CODE;
	}

	else
	{
		return ElementType::UNDEFINED;
	}

}

//==================================================================================================

unsigned int
UserDefinedComponent::containsLabel(const std::string& label) const
{
	unsigned int count = 0;

	for(const auto& e : parameters)
	{
		if(e.label == label)
		{
			count++;
		}
	}

	for(const auto& e : constants)
	{
		if(e.label == label)
		{
			count++;
		}
	}

	for(const auto& e : persistents)
	{
		if(e.label == label)
		{
			count++;
		}
	}

	for(const auto& e : temporaries)
	{
		if(e.label == label)
		{
			count++;
		}
	}

	for(const auto& e : input_signal_ports)
	{
		if(e.label == label)
		{
			count++;
		}
	}

	for(const auto& e : output_signal_ports)
	{
		if(e.label == label)
		{
			count++;
		}
	}

	for(const auto& e : terminals)
	{
		if(e.label == label)
		{
			count++;
		}
	}

	for(const auto& e : through_sources)
	{
		if(e.label == label)
		{
			count++;
		}
	}

	for(const auto& e : across_sources)
	{
		if(e.label == label)
		{
			count++;
		}
	}

	for(const auto& e : conductances)
	{
		if(e.label == label)
		{
			count++;
		}
	}

	for(const auto& e : transconductances)
	{
		if(e.label == label)
		{
			count++;
		}
	}

	return count;
}

bool
UserDefinedComponent::isLabelValid(const std::string& x) const
{
	return Cpp::isNameValid(x);
}

bool
UserDefinedComponent::isParameterValid(const UserDefinedComponent::DataElement& x) const
{
	return
	(
		( isLabelValid(x.label)  ) &&
		//( isTypeValid(x.type)    ) &&
		( x.type != DataType::REAL    ) &&
		( x.array_size != NULLID )
	);
}

bool
UserDefinedComponent::isConstantValid(const UserDefinedComponent::DataElement& x) const
{
	return
	(
		( isLabelValid(x.label)  ) &&
		//( isTypeValid(x.type)    ) &&
		( x.type != DataType::DOUBLE  ) &&
		( x.array_size != NULLID ) &&
		( !x.value.empty() )
	);
}

bool
UserDefinedComponent::isPersistentValid(const UserDefinedComponent::DataElement& x) const
{
	return
	(
		( isLabelValid(x.label)  ) &&
		//( isTypeValid(x.type)    ) &&
		( x.type != DataType::DOUBLE  ) &&
		( x.array_size != NULLID ) &&
		( !x.value.empty() )
	);
}

bool
UserDefinedComponent::isTemporaryValid(const UserDefinedComponent::DataElement& x) const
{
	return
	(
		( isLabelValid(x.label)  ) &&
		//( isTypeValid(x.type)    ) &&
		( x.type != DataType::DOUBLE  ) &&
		( x.array_size != NULLID )
	);
}

bool
UserDefinedComponent::isInputSignalPortValid(const UserDefinedComponent::DataElement& x) const
{
	return
	(
		( isLabelValid(x.label)  ) &&
		//( isTypeValid(x.type)    ) &&
		( x.type != DataType::DOUBLE  ) &&
		( x.array_size != NULLID )
	);
}

bool
UserDefinedComponent::isOutputSignalPortValid(const UserDefinedComponent::DataElement& x) const
{
	return
	(
		( isLabelValid(x.label)  ) &&
		//( isTypeValid(x.type)    ) &&
		( x.type != DataType::DOUBLE  ) &&
		( x.array_size != NULLID )
	);
}

bool
UserDefinedComponent::isTerminalValid(const UserDefinedComponent::Terminal& x) const
{
	return
	(
		isLabelValid(x.label)
	);
}

bool
UserDefinedComponent::isThroughSourceValid(const UserDefinedComponent::ThroughSource& x) const
{
	return
	(
		( isLabelValid(x.label) ) &&
		( !x.p_terminal.empty() ) &&
		( !x.n_terminal.empty() )
	);
}

bool
UserDefinedComponent::isAcrossSourceValid(const UserDefinedComponent::AcrossSource& x) const
{
	return
	(
		( isLabelValid(x.label) ) &&
		( !x.p_terminal.empty() ) &&
		( !x.n_terminal.empty() )
	);
}

bool
UserDefinedComponent::isConductanceValid(const UserDefinedComponent::Conductance& x) const
{
	return
	(
		( isLabelValid(x.label) ) &&
		( !x.p_terminal.empty() ) &&
		( !x.n_terminal.empty() ) &&
		( !x.value.empty()      )
	);
}

bool
UserDefinedComponent::isTransconductanceValid(const UserDefinedComponent::Transconductance& x) const
{
	return
	(
		( isLabelValid(x.label)  ) &&
		( !x.voltage_p_terminal.empty() ) &&
		( !x.voltage_n_terminal.empty() ) &&
		( !x.current_p_terminal.empty() ) &&
		( !x.current_n_terminal.empty() ) &&
		( !x.value.empty()       )
	);
}

bool
UserDefinedComponent::isValid() const
{
	for(const auto& e : parameters)
	{
		if(!isParameterValid(e))
		{
			return false;
		}

		if(containsLabel(e.label) > 1)
		{
			return false;
		}
	}

	for(const auto& e : constants)
	{
		if(!isConstantValid(e))
		{
			return false;
		}

		if(containsLabel(e.label) > 1)
		{
			return false;
		}
	}

	for(const auto& e : persistents)
	{
		if(!isPersistentValid(e))
		{
			return false;
		}

		if(containsLabel(e.label) > 1)
		{
			return false;
		}
	}

	for(const auto& e : temporaries)
	{
		if(!isTemporaryValid(e))
		{
			return false;
		}

		if(containsLabel(e.label) > 1)
		{
			return false;
		}
	}

	for(const auto& e : input_signal_ports)
	{
		if(!isInputSignalPortValid(e))
		{
			return false;
		}

		if(containsLabel(e.label) > 1)
		{
			return false;
		}
	}

	for(const auto& e : output_signal_ports)
	{
		if(!isOutputSignalPortValid(e))
		{
			return false;
		}

		if(containsLabel(e.label) > 1)
		{
			return false;
		}
	}

	for(const auto& e : terminals)
	{
		if(!isTerminalValid(e))
		{
			return false;
		}

		if(containsLabel(e.label) > 1)
		{
			return false;
		}
	}

	for(const auto& e : through_sources)
	{
		if(!isThroughSourceValid(e))
		{
			return false;
		}

		if( !findTerminal(e.p_terminal) || !findTerminal(e.n_terminal) )
		{
			return false;
		}

		if(containsLabel(e.label) > 1)
		{
			return false;
		}
	}

	for(const auto& e : across_sources)
	{
		if(!isAcrossSourceValid(e))
		{
			return false;
		}

		if( !findTerminal(e.p_terminal) || !findTerminal(e.n_terminal) )
		{
			return false;
		}

		if(containsLabel(e.label) > 1)
		{
			return false;
		}
	}

	for(const auto& e : conductances)
	{
		if(!isConductanceValid(e))
		{
			return false;
		}

		if( !findTerminal(e.p_terminal) || !findTerminal(e.n_terminal) )
		{
			return false;
		}

		if(containsLabel(e.label) > 1)
		{
			return false;
		}
	}

	for(const auto& e : transconductances)
	{
		if(!isTransconductanceValid(e))
		{
			return false;
		}

		if
		(
			!findTerminal(e.voltage_p_terminal) ||
			!findTerminal(e.voltage_n_terminal) ||
			!findTerminal(e.current_p_terminal) ||
			!findTerminal(e.current_n_terminal)
		)
		{
			return false;
		}

		if(containsLabel(e.label) > 1)
		{
			return false;
		}
	}

	return true;
}

//==================================================================================================

void
UserDefinedComponent::assertParameterValid(const UserDefinedComponent::DataElement& x) const
{
	if(!isParameterValid(x))
	{
		throw
		std::invalid_argument
		(
			"UserDefinedComponent::assertParameterValid(const UserDefinedComponent::DataElement& x) const -- x is not valid parameter"
		);
	}
}

void
UserDefinedComponent::assertConstantValid(const UserDefinedComponent::DataElement& x) const
{
	if(!isConstantValid(x))
	{
		throw
		std::invalid_argument
		(
			"UserDefinedComponent::assertConstantValid(const UserDefinedComponent::DataElement& x) const -- x is not valid constant"
		);
	}
}

void
UserDefinedComponent::assertPersistentValid(const UserDefinedComponent::DataElement& x) const
{
	if(!isPersistentValid(x))
	{
		throw
		std::invalid_argument
		(
			"UserDefinedComponent::assertPersistentValid(const UserDefinedComponent::DataElement& x) const -- x is not valid persistent"
		);
	}
}

void
UserDefinedComponent::assertTemporaryValid(const UserDefinedComponent::DataElement& x) const
{
	if(!isTemporaryValid(x))
	{
		throw
		std::invalid_argument
		(
			"UserDefinedComponent::assertTemporaryValid(const UserDefinedComponent::DataElement& x) const -- x is not valid temporary"
		);
	}
}

void
UserDefinedComponent::assertInputSignalPortValid(const UserDefinedComponent::DataElement& x) const
{
	if(!isInputSignalPortValid(x))
	{
		throw
		std::invalid_argument
		(
			"UserDefinedComponent::assertInputSignalPortValid(const UserDefinedComponent::DataElement& x) const -- x is not valid input signal port"
		);
	}
}

void
UserDefinedComponent::assertOutputSignalPortValid(const UserDefinedComponent::DataElement& x) const
{
	if(!isOutputSignalPortValid(x))
	{
		throw
		std::invalid_argument
		(
			"UserDefinedComponent::assertOutputSignalPortValid(const UserDefinedComponent::DataElement& x) const -- x is not valid output signal port"
		);
	}
}

void
UserDefinedComponent::assertTerminalValid(const UserDefinedComponent::Terminal& x) const
{
	if(!isTerminalValid(x))
	{
		throw
		std::invalid_argument
		(
			"UserDefinedComponent::assertTerminalValid(const UserDefinedComponent::DataElement& x) const -- x is not valid terminal"
		);
	}
}

void
UserDefinedComponent::assertThroughSourceValid(const UserDefinedComponent::ThroughSource& x) const
{
	if(!isThroughSourceValid(x))
	{
		throw
		std::invalid_argument
		(
			"UserDefinedComponent::assertThroughSourceValid(const UserDefinedComponent::DataElement& x) const -- x is not valid through source"
		);
	}
}

void
UserDefinedComponent::assertAcrossSourceValid(const UserDefinedComponent::AcrossSource& x) const
{
	if(!isAcrossSourceValid(x))
	{
		throw
		std::invalid_argument
		(
			"UserDefinedComponent::assertAcrossSourceValid(const UserDefinedComponent::DataElement& x) const -- x is not valid across source"
		);
	}
}

void
UserDefinedComponent::assertConductanceValid(const UserDefinedComponent::Conductance& x) const
{
	if(!isConductanceValid(x))
	{
		throw
		std::invalid_argument
		(
			"UserDefinedComponent::assertConductanceValid(const UserDefinedComponent::DataElement& x) const -- x is not valid conductance"
		);
	}
}

void
UserDefinedComponent::assertTransconductanceValid(const UserDefinedComponent::Transconductance& x) const
{
	if(!isTransconductanceValid(x))
	{
		throw
		std::invalid_argument
		(
			"UserDefinedComponent::assertTransconductanceValid(const UserDefinedComponent::DataElement& x) const -- x is not valid transconductance"
		);
	}
}

void
UserDefinedComponent::assertValid() const
{
	if(!isValid())
	{
		throw
		std::invalid_argument
		(
			std::string("UserDefinedComponent::assertValid() const -- UDC of type \"") +
			type +
			std::string("\" is not valid")
		);
	}
}

//==================================================================================================
} //namespace lblmc
