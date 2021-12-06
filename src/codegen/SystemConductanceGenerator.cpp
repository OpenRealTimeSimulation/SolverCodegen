/*

Copyright (C) 2017-2020 Matthew Milton

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


#include "codegen/SystemConductanceGenerator.hpp"

#include <vector>
#include <fstream>
#include <iomanip>
#include <stdexcept>

#include <Eigen/Dense>

namespace lblmc
{

//SystemConductanceGenerator::SystemConductanceGenerator() {}

SystemConductanceGenerator::SystemConductanceGenerator(unsigned int dimension):
	matrix(MatrixRMXd::Zero(dimension,dimension)), dimension(dimension)
{
	if(dimension == 0)
		throw std::invalid_argument("SystemConductanceGenerator constructor(): dimension must be nonzero");
}

SystemConductanceGenerator::SystemConductanceGenerator(unsigned int dimension, const MatrixRMXd& base):
		matrix(base), dimension(dimension)
{
	if(dimension == 0)
		throw std::invalid_argument("SystemConductanceGenerator constructor(): dimension must be nonzero");
}

SystemConductanceGenerator::SystemConductanceGenerator(const SystemConductanceGenerator& base) :
		matrix(base.matrix), dimension(base.dimension)
{
	//do nothing else
}

void SystemConductanceGenerator::reset(unsigned int dimension)
{
	if(dimension == 0)
		throw std::invalid_argument("SystemConductanceGenerator::reset(): dimension must be nonzero");

	this->dimension = dimension;
	this->matrix.setZero();
}

void SystemConductanceGenerator::reset(unsigned int dimension, const MatrixRMXd& base)
{
	if(dimension == 0)
		throw std::invalid_argument("SystemConductanceGenerator::reset(): dimension must be nonzero");

	this->dimension = dimension;
	this->matrix = base;
}

void SystemConductanceGenerator::reset(const SystemConductanceGenerator& base)
{
	dimension = base.dimension;
	matrix = base.matrix;
}

double* SystemConductanceGenerator::asPointer()
{
	return matrix.data();
}

double* SystemConductanceGenerator::asArray()
{
	return matrix.data();
}

MatrixRMXd& SystemConductanceGenerator::asEigen3Matrix()
{
	return matrix;
}

const MatrixRMXd& SystemConductanceGenerator::asEigen3Matrix() const
{
	return matrix;
}

unsigned int SystemConductanceGenerator::getDimension() const
{
	return dimension;
}

void SystemConductanceGenerator::stampConductance(double conductance, unsigned int p, unsigned int n)
{
    if(dimension < p || dimension < n)
	{
		throw std::invalid_argument("SystemConductanceGenerator::stampConductance(): given node index/indices are outside dimension of conductance matrix");
	}

	if(p == n) return;

	if( p != 0 && n != 0)
	{
		matrix(p-1,p-1) +=  conductance;
		matrix(p-1,n-1) += -conductance;
		matrix(n-1,p-1) += -conductance;
		matrix(n-1,n-1) +=  conductance;
	}
	else if (p != 0)
		matrix(p-1,p-1) += conductance;
	else if (n != 0)
		matrix(n-1,n-1) += conductance;
}

void SystemConductanceGenerator::stampTransconductance(double transconductance, unsigned int m, unsigned int n, unsigned int p, unsigned int q)
{
	if( (dimension < m) || (dimension < n) || (dimension < p) || (dimension < q) )
	{
		throw std::invalid_argument("SystemConductanceGenerator::stampTransconductance(): given node index/indices are outside dimension of conductance matrix");
	}

	if( (m == n) && (m == p) && (m == q) ) return; //component ports are shorted out, so do nothing

	if( (m != 0) && (p != 0) )
	{
		matrix(p-1,m-1) += transconductance;
	}

	if( (m != 0) && (q != 0) )
	{
		matrix(q-1,m-1) -= transconductance;
	}

	if( (n != 0) && (p != 0) )
	{
		matrix(p-1,n-1) -= transconductance;
	}

	if( (n != 0) && (q != 0) )
	{
		matrix(q-1,n-1) += transconductance;
	}

}

void SystemConductanceGenerator::stampTransconductance2(double transconductance12, double transconductance21, unsigned int m, unsigned int n, unsigned int p, unsigned int q)
{
	if( (dimension < m) || (dimension < n) || (dimension < p) || (dimension < q) )
	{
		throw std::invalid_argument("SystemConductanceGenerator::stampTransconductance2(): given node index/indices are outside dimension of conductance matrix");
	}

	if( (m == n) && (m == p) && (m == q) ) return; //component ports are shorted out, so do nothing

	if( (m != 0) && (p != 0) )
	{
		matrix(m-1,p-1) += transconductance12;
		matrix(p-1,m-1) += transconductance21;
	}

	if( (m != 0) && (q != 0) )
	{
		matrix(m-1,q-1) -= transconductance12;
		matrix(q-1,m-1) -= transconductance21;
	}

	if( (n != 0) && (p != 0) )
	{
		matrix(n-1,p-1) -= transconductance12;
		matrix(p-1,n-1) -= transconductance21;
	}

	if( (n != 0) && (q != 0) )
	{
		matrix(n-1,q-1) += transconductance12;
		matrix(q-1,n-1) += transconductance21;
	}
}

void SystemConductanceGenerator::stampPartialConductance(double conductance, unsigned int r, unsigned int c)
{
	if( (dimension < r) || (dimension < c) )
	{
		throw std::invalid_argument("SystemConductanceGenerator::stampPartialConductance(): given matrix index/indices are outside dimension of conductance matrix");
	}

	if( r != 0 && c != 0)
		matrix(r-1,c-1) += conductance;
}

void SystemConductanceGenerator::stampIdealVoltageSourceIncidence(unsigned int s, unsigned int p, unsigned int n)
{


	if(dimension < p || dimension < n || dimension < s || s==0)
	{
		throw std::invalid_argument("SystemConductanceGenerator::stampIdealVoltageSourceIncidence(): given solution/node index/indices are outside dimension of conductance matrix");
	}

	if(s==p || s==n)
	{
		throw std::invalid_argument("SystemConductanceGenerator::stampIdealVoltageSourceIncidence(): given solution index cannot equal the given node indices p, n (second and last arguments)");
	}

	if(p == n) return;

	if( p != 0 && n != 0)
	{
		matrix(s-1,p-1) =  1;
		matrix(s-1,n-1) = -1;
		matrix(n-1,s-1) = -1;
		matrix(p-1,s-1) =  1;
	}
	else if (p != 0)
	{
		matrix(s-1,p-1) = 1;
		matrix(p-1,s-1) = 1;
	}
	else if (n != 0)
	{
		matrix(s-1,n-1) = -1;
		matrix(n-1,s-1) = -1;
	}
}

bool SystemConductanceGenerator::isInvertible() const
{
	return matrix.fullPivLu().isInvertible();
}

void SystemConductanceGenerator::invertSelf()
{
	if(!matrix.fullPivLu().isInvertible())
	{
		throw std::runtime_error("SystemConductanceGenerator::invertSelf(): cannot invert conductance matrix as it is singular");
	}

	matrix = matrix.fullPivLu().inverse();
}

SystemConductanceGenerator SystemConductanceGenerator::invert() const
{
	SystemConductanceGenerator ret(*this);
	ret.invertSelf();
	return ret;
}

std::string SystemConductanceGenerator::spy() const
{
	std::string buffer;

	for(unsigned int r = 0; r < dimension; r++)
	{
		for(unsigned int c = 0; c < dimension; c++)
		{
			if( matrix(r,c)  == 0.0 )
				buffer += ". ";
			else
				buffer += "X ";
		}
		buffer += '\n';
	}

	return buffer;
}

void SystemConductanceGenerator::exportSpy(std::string filename) const
{
	std::fstream file;

	try
	{
		file.open(filename, std::fstream::out | std::fstream::trunc);
	}
	catch(...)
	{
		throw std::runtime_error("SystemConductanceGenerator::exportSpy(): failed to open or create file");
	}

	for(unsigned int r = 0; r < dimension; r++)
	{
		for(unsigned int c = 0; c < dimension; c++)
		{
			if( matrix(r,c)  == 0.0 )
				file << '.';
			else
				file << 'X';
		}
		file << '\n';
	}
	file << std::flush;

	file.close();
}

std::string SystemConductanceGenerator::asString() const
{
	std::stringstream str;

	str << std::setprecision(16);
	str << std::fixed;
	str << std::scientific;

	for(unsigned int r = 0; r < dimension; r++)
	{
		for(unsigned int c = 0; c < dimension; c++)
		{
			str << "   " << matrix(r,c);
		}
		str << "\n";
	}

	std::string buffer = str.str();
	return buffer;
}

void SystemConductanceGenerator::exportAsASCIIMatlab(std::string filename) const
{
	std::fstream file;

	try
	{
		file.open(filename, std::fstream::out | std::fstream::trunc);
	}
	catch(...)
	{
		throw std::runtime_error("SystemConductanceGenerator::exportAsASCIIMatlab(): failed to open or create file");
	}

	file << std::setprecision(16);
	file << std::fixed;
	file << std::scientific;

	for(unsigned int r = 0; r < dimension; r++)
	{
		for(unsigned int c = 0; c < dimension; c++)
		{
			file << "   " << matrix(r,c);
		}
		file << "\n";
	}
	file << std::flush;

	file.close();

}

void SystemConductanceGenerator::exportAsCSV(std::string filename) const
{
	std::fstream file;

	try
	{
		file.open(filename, std::fstream::out | std::fstream::trunc);
	}
	catch(...)
	{
		throw std::runtime_error("SystemConductanceGenerator::exportAsCSV(): failed to open or create file");
	}

	file << std::setprecision(16);
	file << std::fixed;
	file << std::scientific;

	for(unsigned int r = 0; r < dimension; r++)
	{
		file << matrix(r,0);

		for(unsigned int c = 1; c < dimension; c++)
		{
			file << ", " << matrix(r,c);
		}
		file << "\n";
	}
	file << std::flush;

	file.close();
}

void SystemConductanceGenerator::exportAsCHeader(std::string filename, std::string mat_name) const
{
	std::fstream file;

	std::string fname = filename;
	fname += ".hpp";

	try
	{
		file.open(fname.c_str(), std::fstream::out | std::fstream::trunc);
	}
	catch(...)
	{
		throw std::runtime_error("SystemConductanceGenerator::exportAsCHeader(): failed to open or create file");
	}

	file << std::setprecision(16);
	file << std::fixed;
	file << std::scientific;

	file <<
			"/**\n"
			" *\n"
			" * LBLMC Vivado HLS Simulation Engine for FPGA Designs\n"
			" *\n"
			" * Auto-generated by SystemConductanceGenerator Object\n"
			" *\n"
			" * NOTE: For this header, do not include outside the system solver to avoid linkage/compilation issues\n"
			" *\n"
			" */\n\n";

	file << "#ifndef " << mat_name << "_HPP" << "\n";
	file << "#define " << mat_name << "_HPP" << "\n";

	file << "\n#include \"LBLMC/DataTypes.hpp\"\n\n";

	file << "const LBLMC::numType " << mat_name << "[" << dimension << "][" << dimension << "] =\n{";

	for(unsigned int r = 0; r < dimension; r++)
	{
		file << "{" << matrix(r,0);

		for(unsigned int c = 1; c < dimension; c++)
		{
			file << "," << matrix(r,c);
		}
		file << "}";

		if(r != dimension-1) file << ",";

		file << "\n";
	}

	file << "};\n";

	file << "\n#endif";

	file << std::flush;

	file.close();
}

std::string SystemConductanceGenerator::asCLiteral(std::string mat_name) const
{
	if( mat_name.empty() )
		throw std::invalid_argument("SystemConductanceGenerator::exportAsCCodeLiteral(): mat_name cannot be empty or null");

	std::stringstream mat;

	mat << std::setprecision(16);
	mat << std::fixed;
	mat << std::scientific;

	mat << "const static real " << mat_name << "[" << dimension << "][" << dimension << "] =\n{";

	for(unsigned int r = 0; r < dimension; r++)
	{
		mat << "{" << matrix(r,0);

		for(unsigned int c = 1; c < dimension; c++)
		{
			mat << "," << matrix(r,c);
		}
		mat << "}";

		if(r != dimension-1) mat << ",";

		mat << "\n";
	}

	mat << "};\n";

	std::string buf = mat.str();
	return buf;
}

void SystemConductanceGenerator::importFromASCIIMatlab(std::string filename)
{
	std::fstream file;

	try
	{
		file.open(filename, std::fstream::in);
	}
	catch(...)
	{
		throw std::runtime_error("SystemConductanceGenerator::importFromASCIIMatlab(): failed to open or read file");
	}

	reset(dimension);

	//in a lazy way, we are not checking the matlab file for proper formatting here, so beware!

	for(unsigned int i = 0; i < dimension*dimension; i++)
	{
		file >> (matrix.data())[i];
	}

	file.close();
}

} //namespace lblmc
