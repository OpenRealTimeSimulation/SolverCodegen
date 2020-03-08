/*

Copyright (C) 2018-2020 Matthew Milton

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
 *
 *
 * @author Matthew Milton
 * @date Spring 2018
 *
 */
#ifndef SYSTEMSOLVERGENERATOR_HPP
#define SYSTEMSOLVERGENERATOR_HPP

#include <vector>
#include <string>

namespace lblmc
{

class SystemSolverGenerator
{
private:
	const double* A; ///< the inverted conductance matrix ( A = G^-1 of Gx=b )
	unsigned int dimension; ///< number of solutions in the system Gx=b
	unsigned int num_components; ///< number of components in system to contribute to vector b of Gx=b
	double zero_bound; ///< range from zero when determining whether Aij*bi=xi is close to zero to be ignored; defaults to 1e-12.

public:

	SystemSolverGenerator();

	/**
	 * parameter constructor
	 * @param A the inverted conductance matrix ( A = G^-1 of Gx=b )
	 * @param dimension number of solutions in the system Gx=b
	 * @param num_components number of components in system to contribute to vector b of Gx=b
	 * @param zero_bound range from zero when determining whether Aij*bi=xi is close to zero to be ignored; defaults to 1e-12.
	 */
	SystemSolverGenerator(const double* A, unsigned int dimension, unsigned int num_components, double zero_bound = 1.0e-12);
	SystemSolverGenerator(const SystemSolverGenerator& base);

	void reset(const double* A, unsigned int dimension, unsigned int num_components, double zero_bound = 1.0e-12);
	void reset(const SystemSolverGenerator& base);

	/**
		\brief generates C/C++ inline-able code that includes only the solver for x=(G^-1)*b

		Unlike code produced by generateCFunction, this method produces code that does not include
		the source vector aggregation function.  Only code for x=(G^-1)*b is generated.

		Input of the inline code is the source vector NumType b[<num_nodes>] and the output is NumType x[<num_nodes>].

		\param buffer the string that will store the generated code
		\param invg_name name of the inverted conductance matrix G^-1; default is inv_g
		\deprecated This method is to be replaced by std::string generateCInlineCode(std::string invg_name) const;
	**/
	void generateCInlineCode(std::string& buffer, const char* invg_name = "inv_g");

	/**
		\brief generates C/C++ code for system solver function to solve x=(G^-1)*b

		This method generates a function definition for the system solver.  This solver takes as input
		the unaggregated component source contributions NumType b_components[num_components]
		and outputs system solution vector NumType x[num_nodes].

		The generated function calls a function to aggregate source contributions into single
		vector NumType b[num_nodes].

		\param buffer the string that will store the generated code
		\param solver_name name of the generated function; default is "solve"
		\param A_name name of the global inverted conductance matrix G^-1 generated function refers to
		\param b_func_name name of the source contribution aggregation function to be called to compute source vector b from component sources
		\deprecated This method is to be replaced by std::string generateCFunction(std::string solver_name, std::string A_name, std::string b_func_name) const;
	**/
	void generateCFunction(std::string& buffer, const char* solver_name = "solve",
			const char* A_name = "inv_g", const char* b_func_name = "agg_b") const;

	/**
		\brief generates C/C++ code for system solver function to solve x=(G^-1)*b

		This method generates a function definition for the system solver.  This solver takes as input
		the unaggregated component source contributions NumType b_components[num_components]
		and outputs system solution vector NumType x[num_nodes].

		The generated function calls a function to aggregate source contributions into single
		vector NumType b[num_nodes].

		\param solver_name name of the generated function; default is "solve"
		\param A_name name of the global inverted conductance matrix G^-1 generated function refers to
		\param b_func_name name of the source contribution aggregation function to be called to compute source vector b from component sources
		\return string that will store the generated code
	**/
	std::string generateCFunction(std::string solver_name = "solve",
			std::string A_name = "inv_g", std::string b_func_name = "agg_b") const;

	/**
		\param filename name of the HPP/CPP source files, without extension
		\param solver_name name of the generated function; default is "solve"
		\param A_name name of the global inverted conductance matrix G^-1 generated function refers to
		\param b_func_name name of the source contribution aggregation function to be called to compute source vector b from component sources
		\deprecated This method might be replaced or removed in future versions of the library
	**/
	void generateCFunctionAndExport(std::string filename, std::string solver_name = "solve",
			std::string A_name = "inv_g", std::string b_func_name = "agg_b") const;
//	void generateCFunctionAndExport(const char* filename, const char* solver_name = "solve",
//			const char* A_name = "inv_g", const char* b_func_name = "agg_b");
};

} //namespace lblmc

#endif //SYSTEMSOLVERGENERATOR_HPP
