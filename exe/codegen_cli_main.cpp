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

/**
	@ file main file for CLI application for LB-LMC solver code generator
	@author Matthew Milton
	@date 2019-2020
**/

#include <iostream>
#include <string>
#include <utility>

#include "codegen/netlist/Netlist.hpp"
#include "codegen/netlist/NetlistLoader.hpp"
#include "codegen/netlist/ComponentFactory.hpp"
#include "codegen/SolverEngineGenerator.hpp"

#define STRINGFY(x) #x
#define TOSTRING(x) STRINGFY(x)

const static std::string PROGRAM_TITLE =
"ORTiS Solver C++ Code Generator";

const static std::string PROGRAM_VERSION =
"Built: " __DATE__ " " __TIME__
;

const static std::string COPYRIGHT =
"Copyright (c) 2019-2020 Matthew Milton and others";

const static std::string PROGRAM_DESCRIPTION =
R"(

Simple usage: codegen netlist_file

For help, use codegen -help
To learn more about this tool, use codegen -about
)";

const static std::string HELP_TEXT =
R"(

Simple usage: codegen netlist_file

To see this help text, use codegen -help
To learn more about this tool, use codegen -about

For more detailed information, see the manual/user guide.

NETLIST FORMAT:

Only 1 command, comment, or component listing can be placed in each line.
White space is ignored in netlist.
Labels must start with and contain only 'a-z', 'A-Z', and '_'; '0-9' can be used after the start.  No other characters or space are allowed.
Indices must be positive integers (0 and up) and cannot contain exponents (e,E).
Math expressions are not currently supported.
IdealVoltageSource component is not supported yet, though VoltageSource with series resistance is supported.

	commands:
#name model_label -- (mandatory) name/label of system model
#const const_label const_value -- (optional) define constant to use in netlist

	comments:
% some comment goes here -- (optional) a comment to be ignored

	component listing:
ComponentType label (param1, ..., paramP) {node_index1, ..., node_indexN} -- (mandatory) define a component

	Example Netlist:

#name RLC_Circuit
#const DT 50.0e-9
#const R  10.0
#const L  25.0e-3
#const C  47.0e-3
#const V  100.0
#const RV 0.001
% here is a comment
VoltageSource vg (V, RV) {1, 0}
Inductor ind (DT, L) {1, 2}
Capacitor cap (DT, C) {2, 0}
Resistor  res (R) {2, 0}
)";

const static std::string ABOUT_TEXT =
R"(

This tool generates C++ source code for solvers of multi-physics networked systems such as
electrical, power electronic, and energy conversion systems.  These systems are defined with a
netlist file which is input to this tool.  The solver algorithm used is the
Latency-Based Linear Multi-step Compound (LB-LMC) method.

ORTiS Solver C++ Code Generator uses Eigen 3 Linear Algebra C++ Template Library
<http://eigen.tuxfamily.org/index.php?title=Main_Page>

Acknowledgements:

Matthew Milton   -- ORTiS Code Generation Library and Tool Creator, Lead Developer and Director
Michele Difronzo -- Component Model Developer, Software Tester, Feature Requester
Mark Vygoder     -- Component Model Developer, Software Tester, Feature Requester
Dhiman Chowdhury -- Component Model Developer
Andrea Benigni   -- Original LB-LMC Solver Algorithm Creator

)";

using namespace lblmc;

int main(int argc, char* argv[])
{
	if(argc == 1)
	{
		std::cout << PROGRAM_TITLE + "\n" + COPYRIGHT + "\n" + PROGRAM_VERSION + PROGRAM_DESCRIPTION << std::endl;
		return 0;
	}

	if(argc == 2)
	{
		if(std::string(argv[1]) == std::string("-help") )
		{
			std::cout << PROGRAM_TITLE + "\n" + COPYRIGHT + "\n" + PROGRAM_VERSION + HELP_TEXT << std::endl;
			return 0;
		}
		else if(std::string(argv[1]) == std::string("-about") )
		{
			std::cout << PROGRAM_TITLE + "\n" + COPYRIGHT + "\n" + PROGRAM_VERSION + ABOUT_TEXT << std::endl;
			return 0;
		}
		else if(std::string(argv[1])[0] == '-' )
		{
			std::cout << "Unsupported switch/option given.\n" << std::endl;
			return 0;
		}
		else
		{
			std::string netlist_filename(argv[1]);

			ComponentFactory factory;
			factory.registerBuiltinComponentProducers();

			NetlistLoader netlist_loader;
			Netlist netlist;

			try
			{
				netlist = std::move(netlist_loader.loadFromFile(netlist_filename));
			}
			catch(std::exception& e)
			{
				std::cerr<<
				"Error occurred during loading netlist:\n" <<
				e.what() << std::endl;

				return 1;
			}

			std::string model_name = netlist.getModelName();
			std::string model_solver_src_filename = model_name+std::string(".hpp");
			unsigned int num_solutions = netlist.getNumberOfNodes();

			std::vector< ComponentFactory::ComponentPtr > component_generators;

			SolverEngineGenerator seg(model_name, num_solutions);
			SolverEngineGeneratorParameters seg_params;
			seg_params.codegen_solver_templated_function_enable = true;
			seg_params.codegen_solver_templated_real_type_enable = true;
				seg.setParameters(seg_params);

			try
			{
				for(const auto& comp_listing : netlist.getComponents())
				{
					component_generators.push_back( factory.produceComponent(comp_listing) );
				}

				for(const auto& comp_gen_ptr : component_generators)
				{
					comp_gen_ptr->stampSystem(seg);
				}

				seg.generateCFunctionAndExport(model_solver_src_filename);
			}
			catch(const std::exception& e)
			{
				std::cerr<<
				"Error occurred during generation of solver code:\n" <<
				e.what() << std::endl;

				return 1;
			}

			std::cout <<"\'"<< model_solver_src_filename << "\' generated from netlist \'" << netlist_filename <<"\'"<< std::endl;

			return 0;
		}
	}

	if(argc > 2)
	{
		std::cout << "More than 1 argument is currently not supported.\n" << std::endl;
		return 0;
	}

	return 0;
}
