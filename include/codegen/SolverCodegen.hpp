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

#ifndef ORTIS_SOLVERCODEGEN_HPP
#define ORTIS_SOLVERCODEGEN_HPP

#include <string>
#include <memory>

#include "codegen/netlist/Netlist.hpp"
#include "codegen/netlist/NetlistLoader.hpp"
#include "codegen/netlist/ComponentFactory.hpp"
#include "codegen/SolverEngineGenerator.hpp"
#include "codegen/System.hpp"

/**
	\brief spawns a solver engine code generator initialized for a given circuit defined by a netlist string

	\param netlist_str string containing the circuit netlist text

	\param seg_params default parameters used by generator to generate solver code

	\return unique pointer to the spawned solver engine generator
**/
inline
std::unique_ptr<SolverEngineGenerator>
produceSolverEngineGeneratorFromNetlistString
(
	const std::string& netlist_str,
	const SolverEngineGeneratorParameters& seg_params
)
{
	NetlistLoader netlist_loader;
	Netlist netlist = netlist_loader.loadFromString(netlist_str);

	System system(netlist.getModelName());

	ComponentFactory factory;
	factory.registerBuiltinComponentProducers();

	for(const auto& comp_listing : netlist.getComponents())
	{
		system.addComponent( factory.produceComponent(comp_listing) );
	}

	std::unique_ptr<SolverEngineGenerator> seg_ptr
	(
		new SolverEngineGenerator(system.getName(), system.getNumberOfSolutions() )
	);

	seg_ptr->setParameters(seg_params);

	for(const auto& comp_gen_ptr : system.getComponents() )
	{
		comp_gen_ptr->stampSystem(*seg_ptr);
	}

	return seg_ptr;

}

/**
	\brief spawns a solver engine code generator initialized for a given circuit defined by a netlist string

	\param netlist_str string containing the circuit netlist text

	\return unique pointer to the spawned solver engine generator

	Similar to other overload of this function, but uses default parameters where generated solver function
	is implemented as template with real type parameter.
**/
inline
std::unique_ptr<SolverEngineGenerator>
produceSolverEngineGeneratorFromNetlistString(const std::string& netlist_str)
{
	SolverEngineGeneratorParameters seg_params;
	seg_params.codegen_solver_templated_function_enable = true;
	seg_params.codegen_solver_templated_real_type_enable = true;

	return produceSolverEngineGeneratorFromNetlistString(netlist_str, seg_params);
}

/**
	\brief generates circuit solver code source file from given circuit netlist

	\param netlist_str string containing the circuit netlist text

	\param filename full filename (<directory/><name>.hpp) of the generated source code file
**/
inline
void
generateSolverSourceCodeFileFromNetlistString(const std::string& netlist_str, const std::string& filename)
{
	auto seg = produceSolverEngineGenerator(netlist_str);

	seg->generateCFunctionAndExport(filename);
}

#endif // ORTIS_SOLVERCODEGEN_HPP

