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


#include "codegen/components/Component.hpp"
#include "codegen/SystemConductanceGenerator.hpp"
#include "codegen/SystemSolverGenerator.hpp"
#include "codegen/SolverEngineGenerator.hpp"
#include "codegen/StringProcessor.hpp"

#include <sstream>

namespace lblmc
{

const std::string Component::INTEGRATION_NONE = "";
const std::string Component::INTEGRATION_EULER_FORWARD  = "euler_forward";
const std::string Component::INTEGRATION_EULER_BACKWARD = "euler_backward";
const std::string Component::INTEGRATION_TRAPEZOIDAL    = "trapezoidal";
const std::string Component::INTEGRATION_GEAR           = "gear";
const std::string Component::INTEGRATION_RUNGE_KUTTA_4  = "runge_kutta_4";

void Component::stampSystem(SolverEngineGenerator& gen, const std::vector<std::string>& outputs)
{
	std::string buf;
	SystemConductanceGenerator& scg = gen.getConductanceGenerator();
	SystemSourceVectorGenerator& ssvg = gen.getSourceVectorGenerator();

	stampConductance(scg);
	stampSources(ssvg);
	buf = generateParameters();
	gen.insertComponentParametersCode(buf);

	buf = generateFields();
	gen.insertComponentFieldsCode(buf);

	buf = generateInputs();
	gen.insertComponentInputsCode(buf);

	for(auto output : outputs)
	{
		buf = generateOutputs(output);
		gen.insertComponentOutputsCode(buf);

		buf = generateOutputsUpdateBody(output);
		gen.insertComponentOutputsUpdateBody(buf);
	}

	buf = generateUpdateBody();
	gen.insertComponentUpdateBody(buf);
}

std::string& Component::appendNameToWords(std::string& body, const std::vector<std::string>& words) const
{
	StringProcessor str_proc(body);

	for(const auto& word : words)
	{
		str_proc.replaceWordAll(word, appendName(word));
	}

	return body;
}

std::string& Component::replaceSourceNameWithSourceContributionVector(std::string& body, const std::string& src_name, unsigned int source_id )
{
	StringProcessor str_proc(body);
	std::stringstream sstrm;
	sstrm << "b_components["<<source_id-1<<"]";
	str_proc.replaceWordAll(src_name, sstrm.str());

	return body;
}

std::string& Component::replaceTerminalConnectionNameWithIndex(std::string& body, const std::string& term_name, unsigned int index)
{
	StringProcessor str_proc(body);
	str_proc.replaceWordAll(term_name, std::to_string(index));

	return body;
}



} //namespace lblmc
