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

#ifndef LBLMC_COMPONENTDEFINITION_HPP
#define LBLMC_COMPONENTDEFINITION_HPP

#include <string>
#include <vector>

#include "codegen/udc/ComponentAcrossSource.hpp"
#include "codegen/udc/ComponentConductance.hpp"
#include "codegen/udc/ComponentField.hpp"
#include "codegen/udc/ComponentSignalPort.hpp"
#include "codegen/udc/ComponentTerminal.hpp"
#include "codegen/udc/ComponentThroughSource.hpp"
#include "codegen/udc/ComponentTransconductance.hpp"

namespace lblmc
{

/**
	\brief defines a custom user defined component (UDC) not hard coded into the codegen library
	\author Matthew Milton
	\date 2019

	\see UserDefinedComponent for code generator that generates component code based on these definitions.
**/
class ComponentDefinition
{

private:

	std::string type; ///< type name of the component; i.e. Resistor, Capacitor, BuckConverter, etc.
	std::string integration_method; ///< integration method used for the component model

	std::vector<ComponentField> parameters;   ///< parameters of the component
	std::vector<ComponentField> constants;    ///< internal constants of the component
	std::vector<ComponentField> persistents;  ///< persistent fields of the component
	std::vector<ComponentField> temporaries;  ///< temporaries of the component
	std::vector<ComponentSignalPort> input_ports;  ///< input signal ports of the component
	std::vector<ComponentSignalPort> output_ports; ///< output signal ports of the component

	std::vector<ComponentTerminal> terminals;                       ///< nodal terminals of the component
	std::vector<ComponentThroughSource> through_sources;            ///< resistive companion through sources of the component
	std::vector<ComponentConductance> conductances;                 ///< resistive companion conductances of the component
	std::vector<ComponentTransconductance> transconductances;       ///< resistive companion transconductances of the component
	std::vector<ComponentAcrossSource> across_sources;              ///< resistive companion ideal across sources of the component

	std::string output_body; ///< output body C++ code for updating the output signal ports
	std::string update_body; ///< update body C++ code for updating the component behavior during simulation

public:

	ComponentDefinition();
	ComponentDefinition(const std::string& type, const std::string& integration_method);
	ComponentDefinition(const ComponentDefinition& base);
	ComponentDefinition(ComponentDefinition&& base);

	void setType(const std::string& type);
	const std::string& getType() const;
	void setIntegrationMethod(const std::string& im);
	const std::string& getIntegrationMethod() const;

	void addParameter(const ComponentField& p);
	void addConstant(const ComponentField& c);
	void addPersistent(const ComponentField& p);
	void addTemporary(const ComponentField& t);
	void addInputPort(const ComponentSignalPort& ip);
	void addOutputPort(const ComponentSignalPort& op);
	void addTerminal(const ComponentTerminal& t);
	void addThroughSource(const ComponentThroughSource& s);
	void addConductance(const ComponentConductance& c);
	void addTransconductance(const ComponentTransconductance& tc);
	void addAcrossSource(const ComponentAcrossSource& s);

	const std::vector<ComponentField>& getParameters() const;
	const std::vector<ComponentField>& getConstants() const;
	const std::vector<ComponentField>& getPersistents() const;
	const std::vector<ComponentField>& getTemporaries() const;
	const std::vector<ComponentSignalPort>& getInputPorts() const;
	const std::vector<ComponentSignalPort>& getOutputPorts() const;

	const std::vector<ComponentTerminal>& getTerminals() const;
	const std::vector<ComponentThroughSource>& getThroughSources() const;
	const std::vector<ComponentConductance>& getConductances() const;
	const std::vector<ComponentTransconductance>& getTransconductances() const;
	const std::vector<ComponentAcrossSource>& getAcrossSources() const;

	void setOutputBody(const std::string& body);
	const std::string& getOutputBody() const;
	void setUpdateBody(const std::string& body);
	const std::string& getUpdateBody() const;

};

} //namespace lblmc

#endif // LBLMC_COMPONENTDEFINITION_HPP
