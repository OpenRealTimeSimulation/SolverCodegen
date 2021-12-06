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


#include "codegen/SystemSolverGenerator.hpp"
#include <string>
#include <sstream>
#include <fstream>
#include <stdexcept>

namespace lblmc
{

SystemSolverGenerator::SystemSolverGenerator() :
	A(nullptr), dimension(0), num_components(0), zero_bound(1.0e-12)
{}

SystemSolverGenerator::SystemSolverGenerator(const double* A, unsigned int dimension, unsigned int num_components, double zero_bound) :
	A(A), dimension(dimension), num_components(num_components), zero_bound(zero_bound)
{
	//do nothing else
}

SystemSolverGenerator::SystemSolverGenerator(const SystemSolverGenerator& base) :
	A(base.A), dimension(base.dimension), num_components(base.num_components), zero_bound(base.zero_bound)
{
	//do nothing else
}

void SystemSolverGenerator::reset(const double* A, unsigned int dimension, unsigned int num_components, double zero_bound)
{
	this->A = A;
	this->dimension = dimension;
	this->num_components = num_components;
	this->zero_bound = zero_bound;
}

void SystemSolverGenerator::reset(const SystemSolverGenerator& base)
{
	A = base.A;
	dimension = base.dimension;
	num_components = base.num_components;
	zero_bound = base.zero_bound;
}

void SystemSolverGenerator::generateCInlineCode(std::string& buffer, const char* A_name)
{
	if(A == nullptr || dimension == 0)
		throw std::runtime_error("SystemSolverGenerator::generateCInlineCode(): cannot generate code without conductance matrix and dimension set");

	std::stringstream sstrm;

	sstrm << "x[0] = 0.0;\n";

	for(int r = 0; r < dimension; r++)
	{
		sstrm << "x[" << r+1 << "] = ";
		if( !(A[dimension*r+0] < zero_bound && A[dimension*r+0] > -zero_bound) )
			sstrm << A_name << "[" << r << "][" << int(0) <<"]*b[" << int(0) << "] ";
		else
			sstrm << "real(0.0) ";
		for(int c = 1; c < dimension; c++)
		{
			if( A[dimension*r+c] < zero_bound && A[dimension*r+c] > -zero_bound )
				continue; // A[r,c] is close to zero, so ignore the term.

			sstrm << "+ " << A_name << "[" << r << "][" << c <<"]*b[" << c << "] ";
		}

		sstrm << ";\n";
	}

	buffer = sstrm.str();
}

void SystemSolverGenerator::generateCFunction(std::string& buffer, const char* solver_name,const char* A_name, const char* b_func_name) const
{
	if(A == nullptr || dimension == 0)
		throw std::runtime_error("SystemSolverGenerator::generateCFunction(): cannot generate code without conductance matrix and dimension set");

	std::stringstream sstrm;

	sstrm <<
	"void " << solver_name << "(real x["<<dimension<<"], real b_components["<<num_components<<"])\n"
	"{\n\t"
		"real b[" << dimension << "];\n\n";

	sstrm << b_func_name << "(b, b_components);\n\n";

	for(int r = 0; r < dimension; r++)
	{
		sstrm << "x[" << r << "] = ";
		if( !(A[dimension*r+0] < zero_bound && A[dimension*r+0] > -zero_bound) )
			sstrm << A_name << "[" << r << "][" << int(0) <<"]*b[" << int(0) << "] ";
		else
			sstrm << "real(0.0) ";
		for(int c = 1; c < dimension; c++)
		{
			if( A[dimension*r+c] < zero_bound && A[dimension*r+c] > -zero_bound )
				continue; // A[r,c] is close to zero, so ignore the term.

			sstrm << "+ " << A_name << "[" << r << "][" << c <<"]*b[" << c << "] ";
		}

		sstrm << ";\n";
	}

	sstrm << "\n}";

	buffer = sstrm.str();
}

std::string SystemSolverGenerator::generateCFunction(std::string solver_name, std::string A_name, std::string b_func_name) const
{
	if(A == nullptr || dimension == 0)
		throw std::runtime_error("SystemSolverGenerator::generateCFunction(): cannot generate code without conductance matrix and dimension set");

	std::stringstream sstrm;

	sstrm <<
	"void " << solver_name << "(real x["<<dimension<<"], real b_components["<<num_components<<"])\n"
	"{\n\t"
		"real b[" << dimension << "];\n\n";

	sstrm << b_func_name << "(b, b_components);\n\n";

	for(int r = 0; r < dimension; r++)
	{
		sstrm << "x[" << r << "] = ";
		if( !(A[dimension*r+0] < zero_bound && A[dimension*r+0] > -zero_bound) )
			sstrm << A_name << "[" << r << "][" << int(0) <<"]*b[" << int(0) << "] ";
		else
			sstrm << "real(0.0) ";
		for(int c = 1; c < dimension; c++)
		{
			if( A[dimension*r+c] < zero_bound && A[dimension*r+c] > -zero_bound )
				continue; // A[r,c] is close to zero, so ignore the term.

			sstrm << "+ " << A_name << "[" << r << "][" << c <<"]*b[" << c << "] ";
		}

		sstrm << ";\n";
	}

	sstrm << "\n}";

	return sstrm.str();
}

void SystemSolverGenerator::generateCFunctionAndExport(std::string filename, std::string solver_name, std::string A_name, std::string b_func_name) const
{
	if(A == nullptr || dimension == 0)
		throw std::runtime_error("SystemSolverGenerator::generateCFunctionAndExport(): cannot generate code without conductance matrix and dimension set");

	std::fstream header;
	std::fstream source;

	std::string hname = filename; hname += ".hpp";
	std::string sname = filename; sname += ".cpp";

	try
	{
		header.open((hname).c_str(), std::fstream::out | std::fstream::trunc);
		source.open((sname).c_str(), std::fstream::out | std::fstream::trunc);
	}
	catch(...)
	{
		header.close();
		source.close();
		throw std::runtime_error("SystemSolverGenerator::generateFunctionAndExportC(): failed to open or create source files");
	}

	header <<
			"/**\n"
			" *\n"
			" * LBLMC Vivado HLS Simulation Engine for FPGA Designs\n"
			" *\n"
			" * Auto-generated by SystemSolverGenerator Object\n"
			" *\n"
			" */\n\n";

	header << "#ifndef " << solver_name << "_HPP\n";
	header << "#define " << solver_name << "_HPP\n\n";
	header << "\n#include \"LBLMC/DataTypes.hpp\"\n";
	header << "#include \""<< A_name << ".hpp\"\n";
	header << "#include \""<< b_func_name << ".hpp\"\n\n";
	header << "void " << solver_name << "(real x["<<dimension<<"], real b_components["<<num_components<<"]);\n\n";
	header << "#endif";
	header.close();

	source << "#include \"" << filename << ".hpp" << "\"\n\n";

	source << generateCFunction(solver_name,A_name,b_func_name);
	source.close();
}

} // namespace lblmc
