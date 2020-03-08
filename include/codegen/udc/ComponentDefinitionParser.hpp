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

#ifndef LBLMC_COMPONENTDEFINITIONPARSER_HPP
#define LBLMC_COMPONENTDEFINITIONPARSER_HPP

#include <string>

#include "codegen/components/ComponentDefinition.hpp"

namespace lblmc
{

/**
	\brief creates LB-LMC model components from text-based definitions
**/
class ComponentDefinitionParser
{
public:

	ComponentDefinition& createComponentDefinition(ComponentDefinition& component, std::string& definition);

private:
	void extractName(std::string& name, std::string& definition);
	void extractNumberOfTerminals(unsigned int& num_terminals, std::string& definition);
	void extractNumberOfSources(unsigned int& num_terminals, std::string& definition);
	void extractParameters(std::vector<std::string>& parameters, std::string& definition);
	void extractFields(std::vector<std::string>& fields, std::string& definition);
	void extractInputs(std::vector<std::string>& inputs, std::string& definition);
	void extractOutputs(std::vector<std::string>& outputs, std::string& definition);
	void extractUpdateBody(std::string& update_body, std::string& definition);

	void specializeBodyVariables(std::string& specialized_code, std::string& general_code, ComponentDefinition& component);
};

} //namespace lblmc

#endif // LBLMC_COMPONENTDEFINITIONPARSER_HPP
