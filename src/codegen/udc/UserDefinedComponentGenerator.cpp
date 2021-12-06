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

#include "codegen/udc/UserDefinedComponentGenerator.hpp"
#include "codegen/SystemConductanceGenerator.hpp"
#include "codegen/SystemSourceVectorGenerator.hpp"
#include "codegen/SolverEngineGenerator.hpp"
#include "codegen/Object.hpp"
#include "codegen/ArrayObject.hpp"
#include "codegen/StringProcessor.hpp"

#include <exprpar/exprpar.hpp>

#include <vector>
#include <map>
#include <string>
#include <memory>
#include <stdexcept>
#include <cstddef>

namespace lblmc
{

UserDefinedComponentGenerator::UserDefinedComponentGenerator
(
	std::string comp_name,
	std::shared_ptr<const UserDefinedComponent> component_def
)
	: Component(comp_name),
	component_definition(component_def),
	parameter_value_assignments(),
	terminal_node_assignments(),
	through_source_id_assignments(),
	across_source_id_assignments(),
	across_source_solution_id_assignments()
{

}

UserDefinedComponentGenerator::UserDefinedComponentGenerator
(
	std::string comp_name,
	std::shared_ptr<const UserDefinedComponent> component_def,
	const std::map<std::string, double>& parameters
)
	: Component(comp_name),
	component_definition(component_def),
	parameter_value_assignments(parameters),
	terminal_node_assignments(),
	through_source_id_assignments(),
	across_source_id_assignments(),
	across_source_solution_id_assignments()
{

}

UserDefinedComponentGenerator::UserDefinedComponentGenerator
(
	UserDefinedComponentGenerator&& base
)
	: Component(std::move(base)),
	component_definition(std::move(base.component_definition)),
	parameter_value_assignments(std::move(base.parameter_value_assignments)),
	terminal_node_assignments(std::move(base.terminal_node_assignments)),
	through_source_id_assignments(std::move(base.through_source_id_assignments)),
	across_source_id_assignments(std::move(base.across_source_id_assignments)),
	across_source_solution_id_assignments(std::move(base.across_source_solution_id_assignments))
{

}

//==================================================================================================

std::unique_ptr<UserDefinedComponentGenerator>
UserDefinedComponentGenerator::clone() const
{
	std::unique_ptr<UserDefinedComponentGenerator> new_udcg =
	std::make_unique<UserDefinedComponentGenerator>();

	new_udcg->component_definition = component_definition;
	new_udcg->parameter_value_assignments = parameter_value_assignments;
	new_udcg->terminal_node_assignments = terminal_node_assignments;
	new_udcg->through_source_id_assignments = through_source_id_assignments;
	new_udcg->across_source_id_assignments = across_source_id_assignments;
	new_udcg->across_source_solution_id_assignments = across_source_solution_id_assignments;

	return new_udcg;
}

//==================================================================================================

std::string
UserDefinedComponentGenerator::getType() const
{
	if(component_definition)
	{
		return component_definition->getType();
	}
	else
	{
		return std::string();
	}
}

unsigned int
UserDefinedComponentGenerator::getNumberOfTerminals() const
{
	if(component_definition)
	{
		return component_definition->getTerminals().size();
	}
	else
	{
		return 0;
	}
}

unsigned int
UserDefinedComponentGenerator::getNumberOfSources() const
{
	if(component_definition)
	{
		return component_definition->getThroughSources().size();
	}
	else
	{
		return 0;
	}
}

unsigned int
UserDefinedComponentGenerator::getNumberOfIdealVoltageSources() const
{
	if(component_definition)
	{
		return component_definition->getAcrossSources().size();
	}
	else
	{
		return 0;
	}
}

//==================================================================================================

void
UserDefinedComponentGenerator::setComponentDefinition(std::shared_ptr<UserDefinedComponent> cd)
{
	component_definition = cd;
}

const UserDefinedComponent*
UserDefinedComponentGenerator::getComponentDefinition() const
{
	return component_definition.get();
}

void
UserDefinedComponentGenerator::setParameterValue(const std::string& label, const double& value)
{
	parameter_value_assignments[label] = value;
}

void
UserDefinedComponentGenerator::setParameterValues(const std::map<std::string, double>& values)
{
	for(const auto& elem : values)
	{
		parameter_value_assignments[elem.first] = elem.second;
	}
}

void
UserDefinedComponentGenerator::setParameterValues(const std::vector<double>& values)
{
	if(component_definition)
	{
		const auto& parameters = component_definition->getParameters();

		if(values.size() != parameters.size())
		{
			throw std::length_error
			(
				"void UserDefinedComponentGenerator::setParameterValues(const std::vector<double>& values)"
				" -- "
				"number of values must equal number of parameters in assigned UDC definition"
			);
		}

		for(std::size_t i = 0; i < parameters.size(); i++)
		{
			const auto& parameter = parameters[i];

			parameter_value_assignments[parameter.label] = values[i];
		}
	}
	else
	{
		throw std::runtime_error
		(
			"void UserDefinedComponentGenerator::setParameterValues(const std::vector<double>& values)"
			" -- "
			"generator does not have an UserDefinedComponent definition assigned to it"
		);
	}
}

void
UserDefinedComponentGenerator::setTerminalConnection(const std::string& label, const unsigned int& node)
{
	terminal_node_assignments[label] = node;
}

void
UserDefinedComponentGenerator::setTerminalConnections(const std::map<std::string, unsigned int>& nodes)
{
	for(const auto& elem : nodes)
	{
		terminal_node_assignments[elem.first] = elem.second;
	}
}

void
UserDefinedComponentGenerator::setTerminalConnections(const std::vector<unsigned int>& nodes)
{
	if(component_definition)
	{
		const auto& terminals = component_definition->getTerminals();

		if(nodes.size() != terminals.size())
		{
			throw std::length_error
			(
				"void UserDefinedComponentGenerator::setTerminalConnections(const std::vector<unsigned int>& nodes)"
				" -- "
				"number of nodes must equal number of terminals in assigned UDC definition"
			);
		}

		for(std::size_t i = 0; i < terminals.size(); i++)
		{
			const auto& terminal = terminals[i];

			terminal_node_assignments[terminal.label] = nodes[i];
		}
	}
	else
	{
		throw std::runtime_error
		(
			"void UserDefinedComponentGenerator::setTerminalConnections(const std::vector<unsigned int>& nodes)"
			" -- "
			"generator does not have an UserDefinedComponent definition assigned to it"
		);
	}
}

void
UserDefinedComponentGenerator::getTerminalConnections(std::vector<unsigned int>& term_ids) const
{
	term_ids.clear();

	if(component_definition)
	{
		const auto& terminals = component_definition->getTerminals();


		for(std::size_t i = 0; i < terminals.size(); i++)
		{
			const auto& terminal = terminals[i];

			term_ids.push_back(terminal_node_assignments.at(terminal.label));
		}
	}
	else
	{
		throw std::runtime_error
		(
			"void UserDefinedComponentGenerator::getTerminalConnections(std::vector<unsigned int>& term_ids) const"
			" -- "
			"generator does not have an UserDefinedComponent definition assigned to it"
		);
	}
}

std::vector<unsigned int>
UserDefinedComponentGenerator::getTerminalConnections() const
{
	std::vector<unsigned int> term_ids;

	if(component_definition)
	{
		const auto& terminals = component_definition->getTerminals();


		for(std::size_t i = 0; i < terminals.size(); i++)
		{
			const auto& terminal = terminals[i];

			term_ids.push_back(terminal_node_assignments.at(terminal.label));
		}
	}
	else
	{
		throw std::runtime_error
		(
			"std::vector<unsigned int> UserDefinedComponentGenerator::getTerminalConnections() const"
			" -- "
			"generator does not have an UserDefinedComponent definition assigned to it"
		);
	}

	return term_ids;
}

void
UserDefinedComponentGenerator::setThroughSourceId(const std::string& label, const unsigned int& id)
{
	through_source_id_assignments[label] = id;
}

void
UserDefinedComponentGenerator::setAcrossSourceId(const std::string& label, const unsigned int& id)
{
	across_source_id_assignments[label] = id;
}

void
UserDefinedComponentGenerator::setAcrossSourceSolutionId(const std::string& label, const unsigned int& id)
{
	across_source_solution_id_assignments[label] = id;
}

void
UserDefinedComponentGenerator::getSourceIds(std::vector<unsigned int>& ids) const
{
	ids.clear();

	if(component_definition)
	{
		const auto& through_sources = component_definition->getThroughSources();


		for(std::size_t i = 0; i < through_sources.size(); i++)
		{
			const auto& source = through_sources[i];

			ids.push_back(terminal_node_assignments.at(source.label));
		}
	}
	else
	{
		throw std::runtime_error
		(
			"void UserDefinedComponentGenerator::getSourceIds(std::vector<unsigned int>& ids) const"
			" -- "
			"generator does not have an UserDefinedComponent definition assigned to it"
		);
	}
}

std::vector<unsigned int>
UserDefinedComponentGenerator::getSourceIds() const
{
	std::vector<unsigned int> ids;

	if(component_definition)
	{
		const auto& through_sources = component_definition->getThroughSources();


		for(std::size_t i = 0; i < through_sources.size(); i++)
		{
			const auto& source = through_sources[i];

			ids.push_back(terminal_node_assignments.at(source.label));
		}
	}
	else
	{
		throw std::runtime_error
		(
			"void UserDefinedComponentGenerator::getSourceIds(std::vector<unsigned int>& ids) const"
			" -- "
			"generator does not have an UserDefinedComponent definition assigned to it"
		);
	}

	return ids;
}

std::vector<std::string>
UserDefinedComponentGenerator::getSupportedInputs() const
{
	std::vector<std::string> inputs;

	if(component_definition)
	{
		for(const auto& input : component_definition->getInputSignalPorts())
		{
			inputs.push_back(input.label);
		}
	}
	else
	{
		throw std::runtime_error
		(
			"std::vector<std::string> "
			"UserDefinedComponentGenerator::getSupportedInputs() const"
			" -- "
			"generator does not have an UserDefinedComponent definition assigned to it"
		);
	}

	return inputs;
}

std::vector<std::string>
UserDefinedComponentGenerator::getSupportedOutputs() const
{
	std::vector<std::string> outputs;

	if(component_definition)
	{
		for(const auto& output : component_definition->getOutputSignalPorts())
		{
			outputs.push_back(output.label);
		}
	}
	else
	{
		throw std::runtime_error
		(
			"std::vector<std::string> "
			"UserDefinedComponentGenerator::getSupportedOutputs() const"
			" -- "
			"generator does not have an UserDefinedComponent definition assigned to it"
		);
	}

	return outputs;
}

//==================================================================================================

void
UserDefinedComponentGenerator::stampConductance(SystemConductanceGenerator& gen)
{
	assertUdcAlive
	(
		"void "
		"UserDefinedComponentGenerator::stampConductance(SystemConductanceGenerator& gen)"
		" -- "
		"generator does not have an UserDefinedComponent definition assigned to it"
	);

	ortis::ExpressionParser parser;

	const auto& conductances = component_definition->getConductances();
	const auto& transconductances = component_definition->getTransconductances();
	const auto& ideal_voltage_srcs = component_definition->getAcrossSources();

	for(const auto& conduct : conductances)
	{
		unsigned int p = terminal_node_assignments.at(conduct.p_terminal);
		unsigned int n = terminal_node_assignments.at(conduct.n_terminal);
		double g = parser.parse(conduct.value).evaluate(parameter_value_assignments);

		gen.stampConductance(g, p, n);
	}

	for(const auto& xconduct : transconductances)
	{
		unsigned int vp = terminal_node_assignments.at(xconduct.voltage_p_terminal);
		unsigned int vn = terminal_node_assignments.at(xconduct.voltage_n_terminal);
		unsigned int ip = terminal_node_assignments.at(xconduct.current_p_terminal);
		unsigned int in = terminal_node_assignments.at(xconduct.current_n_terminal);
		double xg = parser.parse(xconduct.value).evaluate(parameter_value_assignments);

		gen.stampTransconductance(xg, vp, vn, ip, in);
	}

	for(const auto& src : ideal_voltage_srcs)
	{
		unsigned int p = terminal_node_assignments.at(src.p_terminal);
		unsigned int n = terminal_node_assignments.at(src.n_terminal);
		unsigned int soln_id = across_source_solution_id_assignments.at(src.label);

		gen.stampIdealVoltageSourceIncidence(soln_id, p, n);
	}

}

void
UserDefinedComponentGenerator::stampSources(SystemSourceVectorGenerator& gen)
{
	assertUdcAlive
	(
		"void "
		"UserDefinedComponentGenerator::stampSources(SystemSourceVectorGenerator& gen)"
		" -- "
		"generator does not have an UserDefinedComponent definition assigned to it"
	);

	const auto& through_sources = component_definition->getThroughSources();
	const auto& across_sources  = component_definition->getAcrossSources();

	for(const auto& src : through_sources)
	{
		unsigned int p = terminal_node_assignments.at(src.p_terminal);
		unsigned int n = terminal_node_assignments.at(src.n_terminal);

		unsigned int src_id = gen.insertSource(p, n);

		setThroughSourceId(src.label, src_id);
	}

	for(const auto& src : across_sources)
	{
		unsigned int soln_id = across_source_solution_id_assignments.at(src.label);

		unsigned int src_id = gen.insertIdealVoltageSource(soln_id);

		setAcrossSourceId(src.label, src_id);
	}

}

void
UserDefinedComponentGenerator::stampSystem(SolverEngineGenerator& gen, std::vector<std::string> outputs)
{
	Component::stampSystem(gen, outputs);
}

std::string
UserDefinedComponentGenerator::generateParameters()
{
	assertUdcAlive
	(
		"void "
		"UserDefinedComponentGenerator::stampSources(SystemSourceVectorGenerator& gen)"
		" -- "
		"generator does not have an UserDefinedComponent definition assigned to it"
	);

	std::stringstream sstrm;
	sstrm <<
	std::setprecision(16) <<
	std::fixed <<
	std::scientific;

	ortis::ExpressionParser parser;

	const auto& parameters = component_definition->getParameters();
	const auto& constants = component_definition->getConstants();

	for(const auto& elem : parameters)
	{
		generateParameter
		(
			sstrm,
			elem.label,
			parser.parse(elem.value).evaluate(parameter_value_assignments)
		);
	}

	for(const auto& elem : constants)
	{
		generateTypedParameter<long double>
		(
			sstrm,
			UserDefinedComponent::getCppDataTypeName(elem.type),
			elem.label,
			parser.parse(elem.value).evaluate(parameter_value_assignments)
		);
	}

	return sstrm.str();

}

std::string
UserDefinedComponentGenerator::generateFields()
{
	assertUdcAlive
	(
		"void "
		"UserDefinedComponentGenerator::stampSources(SystemSourceVectorGenerator& gen)"
		" -- "
		"generator does not have an UserDefinedComponent definition assigned to it"
	);

	std::stringstream sstrm;
	sstrm <<
	std::setprecision(16) <<
	std::fixed <<
	std::scientific;

	ortis::ExpressionParser parser;

	const auto& persistents = component_definition->getPersistents();
	const auto& temporaries = component_definition->getTemporaries();

	for(const auto& elem : persistents)
	{
		if(elem.array_size <= 1)
		{
			sstrm
			<< "static "
			<< UserDefinedComponent::getCppDataTypeName(elem.type)
			<< " "
			<< appendName(elem.label)
			<< " = "
			<< elem.value
			<< ";\n"
			;
		}
		else
		{
			sstrm
			<< "static "
			<< UserDefinedComponent::getCppDataTypeName(elem.type)
			<< " "
			<< appendName(elem.label)
			<< "[" << elem.array_size << "]"
			<< " = "
			<< elem.value
			<< ";\n"
			;
		}
	}

	for(const auto& elem : temporaries)
	{
		if(elem.array_size <= 1)
		{
			sstrm
			<< UserDefinedComponent::getCppDataTypeName(elem.type)
			<< " "
			<< appendName(elem.label)
			<< " = "
			<< elem.value
			<< ";\n"
			;
		}
		else
		{
			sstrm
			<< UserDefinedComponent::getCppDataTypeName(elem.type)
			<< " "
			<< appendName(elem.label)
			<< "[" << elem.array_size << "]"
			<< " = "
			<< elem.value
			<< ";\n"
			;
		}
	}

	return sstrm.str();
}

std::string
UserDefinedComponentGenerator::generateInputs()
{
	std::stringstream sstrm;
	sstrm <<
	std::setprecision(16) <<
	std::fixed <<
	std::scientific;

	const auto& ports = component_definition->getInputSignalPorts();

	for(std::size_t i = 0; i < ports.size(); i++)
	{
		const auto& elem = ports[i];

		if(i != 0)
		{
			sstrm << ",\n";
		}

		if(elem.array_size <= 1)
		{
			sstrm
			<< "const "
			<< UserDefinedComponent::getCppDataTypeName(elem.type)
			<< "& "
			<< appendName(elem.label)
			;
		}
		else
		{
			sstrm
			<< "const "
			<< UserDefinedComponent::getCppDataTypeName(elem.type)
			<< " "
			<< appendName(elem.label)
			<< "[" << elem.array_size << "]"
			;
		}
	}

	return sstrm.str();
}

std::string
UserDefinedComponentGenerator::generateOutputs(std::string output)
{
	std::stringstream sstrm;
	sstrm <<
	std::setprecision(16) <<
	std::fixed <<
	std::scientific;

	const auto& ports = component_definition->getOutputSignalPorts();

	for(std::size_t i = 0; i < ports.size(); i++)
	{
		const auto& elem = ports[i];

		if(i != 0)
		{
			sstrm << ",\n";
		}

		if(elem.array_size <= 1)
		{
			sstrm
			<< UserDefinedComponent::getCppDataTypeName(elem.type)
			<< "& "
			<< appendName(elem.label)
			;
		}
		else
		{
			sstrm
			<< UserDefinedComponent::getCppDataTypeName(elem.type)
			<< " "
			<< appendName(elem.label)
			<< "[" << elem.array_size << "]"
			;
		}
	}

	return sstrm.str();
}

std::string
UserDefinedComponentGenerator::generateOutputsUpdateBody(std::string output)
{
	return std::string(); //do nothing; outputs done in model update code
}

std::string
UserDefinedComponentGenerator::generateUpdateBody()
{
	std::stringstream sstrm;
	sstrm <<
	std::setprecision(16) <<
	std::fixed <<
	std::scientific;

	std::string body = component_definition->getModelUpdateCode();

	StringProcessor str_proc(body);

	//append component label to variables in model code

	for(const auto& elem : component_definition->getParameters())
	{
		str_proc.replaceWordAll
		(
			elem.label,
			appendName(elem.label)
		);
	}

	for(const auto& elem : component_definition->getConstants())
	{
		str_proc.replaceWordAll
		(
			elem.label,
			appendName(elem.label)
		);
	}

	for(const auto& elem : component_definition->getPersistents())
	{
		str_proc.replaceWordAll
		(
			elem.label,
			appendName(elem.label)
		);
	}

	for(const auto& elem : component_definition->getTemporaries())
	{
		str_proc.replaceWordAll
		(
			elem.label,
			appendName(elem.label)
		);
	}

	for(const auto& elem : component_definition->getInputSignalPorts())
	{
		str_proc.replaceWordAll
		(
			elem.label,
			appendName(elem.label)
		);
	}

	for(const auto& elem : component_definition->getOutputSignalPorts())
	{
		str_proc.replaceWordAll
		(
			elem.label,
			appendName(elem.label)
		);
	}

	for(const auto& elem : terminal_node_assignments)
	{
		str_proc.replaceWordAll
		(
			elem.first,
			std::to_string(elem.second)
		);
	}

	for(const auto& elem : through_source_id_assignments)
	{
		sstrm.str("");
		sstrm.clear();
		sstrm << "b_components["<< (elem.second - 1) <<"]";

		str_proc.replaceWordAll
		(
			elem.first,
			sstrm.str()
		);
	}

	for(const auto& elem : across_source_id_assignments)
	{
		sstrm.str("");
		sstrm.clear();
		sstrm << "b_components["<< (elem.second - 1) <<"]";

		str_proc.replaceWordAll
		(
			elem.first,
			sstrm.str()
		);
	}

	return body;
}

//==================================================================================================

void
UserDefinedComponentGenerator::assertUdcAlive(const std::string& error_message)
{
	if(!component_definition)
	{
		throw
		std::runtime_error(error_message);
	}
}

} //namespace lblmc
