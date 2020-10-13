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


#include "codegen/SystemSourceVectorGenerator.hpp"

#include <cstdlib>
#include <vector>
#include <fstream>
#include <string>
#include <sstream>
#include <stdexcept>

namespace lblmc
{

SystemSourceVectorGenerator::SystemSourceVectorGenerator(unsigned int dimension) :
	vector(dimension, std::vector<long>()), source_nodes(), dimension(dimension), src_index(0)
{
	if(dimension == 0)
		throw std::invalid_argument("SystemSourceVectorGenerator::constructor(): dimension must be nonzero");
}

SystemSourceVectorGenerator::SystemSourceVectorGenerator(const SystemSourceVectorGenerator& base) :
	vector(base.vector), source_nodes(base.source_nodes), dimension(base.dimension),
	src_index(base.src_index)
{
	//do nothing else
}

void SystemSourceVectorGenerator::reset(unsigned int dimension)
{
	if(dimension == 0)
		throw std::invalid_argument("SystemSourceVectorGenerator::reset(): dimension must be nonzero");

	vector = std::vector<std::vector<long> >(dimension, std::vector<long>());
	source_nodes.clear();
	this->dimension = dimension;
	src_index = 0;
}

void SystemSourceVectorGenerator::reset(const SystemSourceVectorGenerator& base)
{
	vector = base.vector;
	source_nodes = base.source_nodes;
	dimension = base.dimension;
	src_index = base.src_index;
}

std::vector<long>& SystemSourceVectorGenerator::asVector(unsigned int n)
{
	if(n >= vector.size())
		throw std::invalid_argument("SystemSourceVectorGenerator::asVector(): index n is out of bounds in source vector");

	return vector[n-1];
}

std::map<long, std::vector<long> >& SystemSourceVectorGenerator::asMap()
{
	return source_nodes;
}

unsigned int SystemSourceVectorGenerator::getDimension() const
{
	return dimension;
}

unsigned int SystemSourceVectorGenerator::getNumSources() const
{
	return src_index;
}

const std::vector<long>& SystemSourceVectorGenerator::getSourceNodesById(long source_id) const
{
    auto nodes_iter = source_nodes.find(source_id);

    if( nodes_iter == source_nodes.end() )
	{
		throw std::out_of_range("SystemSourceVectorGenerator::getSourceNodesById(source_id) -- source does not exist for given source_id");
	}

    return (nodes_iter->second);
}

unsigned int SystemSourceVectorGenerator::insertSource(unsigned int npos, unsigned int nneg)
{
	if(npos == nneg) return 0;

	++src_index;

	if(npos != 0)
	{
		vector[npos-1].push_back(+src_index);
	}
	if(nneg != 0)
	{
		vector[nneg-1].push_back(-src_index);
	}

	source_nodes[src_index].push_back(npos);
	source_nodes[src_index].push_back(nneg);

	return src_index;
}

unsigned int SystemSourceVectorGenerator::insertIdealVoltageSource(unsigned int solution_id)
{
	if(solution_id==0) return 0;

    return insertSource(solution_id, 0);
}

std::vector<unsigned int> SystemSourceVectorGenerator::insertComponents(std::vector<unsigned int> nodes)
{
    if(nodes.empty() || (nodes.size()%2 != 0) ) return std::vector<unsigned int>();

        //check if any source is shorted (npos == nneg)
    for(unsigned int i = 0; i < nodes.size(); i+=2)
    {
        if( nodes[i] == nodes[i+1] ) return std::vector<unsigned int>();
    }

    std::vector<unsigned int> indices = std::vector<unsigned int>();

        //insert the sources, via their nodes, into the source vector
    for(unsigned int i = 0; i < nodes.size(); i+=2)
    {
        indices.push_back( insertSource( nodes[i], nodes[i+1] ) );
    }

    return indices;
}

void SystemSourceVectorGenerator::asString(std::string& buffer) const
{
	std::stringstream sstrm;

	for(unsigned int i = 0; i < dimension; i++)
	{
		if(vector[i].empty())
		{
			sstrm << i+1 << ": 0\n";
			continue;
		}

		sstrm << i+1 << ": ";

		std::vector<long>::const_iterator iter = vector[i].begin();
		std::vector<long>::const_iterator end  = vector[i].end();
		for(iter; iter != end; iter++)
		{
			sstrm << *iter << " ";
		}

		sstrm << "\n";
	}

	buffer = sstrm.str();
}

std::string SystemSourceVectorGenerator::asString() const
{
	std::stringstream sstrm;

	for(unsigned int i = 0; i < dimension; i++)
	{
		if(vector[i].empty())
		{
			sstrm << i+1 << ": 0\n";
			continue;
		}

		sstrm << i+1 << ": ";

		std::vector<long>::const_iterator iter = vector[i].begin();
		std::vector<long>::const_iterator end  = vector[i].end();
		for(iter; iter != end; iter++)
		{
			sstrm << *iter << " ";
		}

		sstrm << "\n";
	}

	return sstrm.str();
}

void SystemSourceVectorGenerator::asCFunction(std::string& buffer, const char* func_name) const
{
	std::stringstream sstrm;

	sstrm <<
	"void " << func_name << "(real b["<<dimension<<"], real b_components["<<src_index<<"])\n"
	"{\n\t";

	for(unsigned int i = 0; i < dimension; i++)
	{
		if(vector[i].empty())
		{
			sstrm << "b[" << i << "] = 0.0;\n\t";
			continue;
		}

		sstrm << "b[" << i << "] = ";

		std::vector<long>::const_iterator iter = vector[i].begin();
		std::vector<long>::const_iterator end  = vector[i].end();
		for(iter; iter != end; iter++)
		{
			if( (*iter) >= 0)
			{
				sstrm << " b_components[" << long(abs(*iter)-1) << "] ";
			}
			else
			{
				sstrm << " -b_components[" << long(abs(*iter)-1) << "] ";
			}

			if((iter+1) == end) sstrm << ";\n\t";
			else sstrm << "+";
		}
	}

	sstrm << "\n}";

	buffer = sstrm.str();
}

void SystemSourceVectorGenerator::asCInlineCode(std::string& buffer) const
{
	std::stringstream sstrm;

	for(unsigned int i = 0; i < dimension; i++)
	{
		if(vector[i].empty())
		{
			sstrm << "b[" << i << "] = 0.0;\n";
			continue;
		}

		sstrm << "b[" << i << "] = ";

		std::vector<long>::const_iterator iter = vector[i].begin();
		std::vector<long>::const_iterator end  = vector[i].end();
		for(iter; iter != end; iter++)
		{
			if( (*iter) >= 0)
			{
				sstrm << " b_components[" << long(abs(*iter)-1) << "] ";
			}
			else
			{
				sstrm << " -b_components[" << long(abs(*iter)-1) << "] ";
			}

			if((iter+1) == end) sstrm << ";\n";
			else sstrm << "+";
		}
	}

	buffer = sstrm.str();
}

std::string SystemSourceVectorGenerator::asCInlineCode() const
{
	std::stringstream sstrm;

	for(unsigned int i = 0; i < dimension; i++)
	{
		if(vector[i].empty())
		{
			sstrm << "b[" << i << "] = 0.0;\n";
			continue;
		}

		sstrm << "b[" << i << "] = ";

		std::vector<long>::const_iterator iter = vector[i].begin();
		std::vector<long>::const_iterator end  = vector[i].end();
		for(iter; iter != end; iter++)
		{
			if( (*iter) >= 0)
			{
				sstrm << " b_components[" << long(abs(*iter)-1) << "] ";
			}
			else
			{
				sstrm << " -b_components[" << long(abs(*iter)-1) << "] ";
			}

			if((iter+1) == end) sstrm << ";\n";
			else sstrm << "+";
		}
	}

	return sstrm.str();
}

void SystemSourceVectorGenerator::exportAsCFunctionSource(const char* filename, const char* func_name) const
{
	std::fstream file;
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

		throw std::runtime_error("SystemSourceVectorGenerator::exportAsCFunctionSource(): failed to open or create source files");
	}

	header <<
			"/**\n"
			" *\n"
			" * LBLMC Vivado HLS Simulation Engine for FPGA Designs\n"
			" *\n"
			" * Auto-generated by SystemSourceVectorGenerator Object\n"
			" *\n"
			" */\n\n";

	header << "#ifndef " << func_name << "_HPP\n";
	header << "#define " << func_name << "_HPP\n\n";
	header << "\n#include \"LBLMC/DataTypes.hpp\"\n\n";
	header << "using namespace lblmc;\n\n";
	header << "void " << func_name << "(real b["<<dimension<<"], real b_components["<<src_index<<"]);\n\n";
	header << "#endif";
	header.close();

	source << "#include \"" << filename<< ".hpp" << "\"\n\n";

	std::string buf;
	asCFunction(buf,func_name);
	source << buf;
	source.close();
}

} //namespace lblmc
