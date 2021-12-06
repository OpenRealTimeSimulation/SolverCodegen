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

#ifndef CODEGEN_ARRAYOBJECT_HPP
#define CODEGEN_ARRAYOBJECT_HPP

#include <string>
#include <vector>
#include <stdexcept>
#include <sstream>
#include "Object.hpp"

namespace lblmc
{

/**
	\brief describes C++ array objects and generates appropriate C++ code for the objects

	This class cannot describe containers such as std::vector, std::list, or even std::array.

	\author Matthew Milton
	\date 2019
**/
class ArrayObject : public Object
{
protected:

	std::vector<unsigned int> dimensions;

public:

	ArrayObject() :
		Object(),
		dimensions()
	{}

	ArrayObject(std::string type, std::string label, std::string value, std::vector<unsigned int> dimensions):
		Object(type, label, value),
		dimensions(dimensions)
	{
		if(dimensions.size() < 1) throw std::invalid_argument("lblmc::ArrayObject::constructor(): dimensions must have size of 1 or higher");
	}

	ArrayObject(const ArrayObject& base) :
		Object(base),
		dimensions(base.dimensions)
	{}

	inline void insertDimension(unsigned int dim)
	{
		dimensions.push_back(dim);
	}

	inline const std::vector<unsigned int>& getDimensions() const
	{
		return dimensions;
	}

	inline unsigned int getDimensionSize() const
	{
		return dimensions.size();
	}

	std::string generateDeclaration()
	{
		if(type.empty()) throw std::runtime_error("lblmc::ArrayObject::generateDeclaration(): type cannot be empty or null");
		if(label.empty()) throw std::runtime_error("lblmc::ArrayObject::generateDeclaration(): label cannot be empty or null");
		if(dimensions.empty()) throw std::runtime_error("lblmc::ArrayObject::generateDeclaration(): dimension size cannot be zero");

		std::stringstream sstrm;

		for(auto q : qualifiers)
		{

			if(qualiferAsString(q).empty()) continue;

			sstrm << qualiferAsString(q) << " ";
		}

		sstrm << type << " " << label;

		for(auto d : dimensions)
		{
			sstrm << "[" << d << "]";
		}

		sstrm << ";";

		return sstrm.str();
	}

	std::string generateDefinition()
	{
		if(type.empty()) throw std::runtime_error("lblmc::ArrayObject::generateDefinition(): type cannot be empty or null");
		if(label.empty()) throw std::runtime_error("lblmc::ArrayObject::generateDefinition(): label cannot be empty or null");
		if(dimensions.empty()) throw std::runtime_error("lblmc::ArrayObject::generateDefinition(): dimension size cannot be zero");

		std::stringstream sstrm;

		for(auto q : qualifiers)
		{

			if(qualiferAsString(q).empty()) continue;

			sstrm << qualiferAsString(q) << " ";
		}

		sstrm << type << " " << label;

		for(auto d : dimensions)
		{
			sstrm << "[" << d << "]";
		}

		if(!value.empty())
		{
			sstrm << " = " << value << ";";
		}
		else
		{
			sstrm << ";";
		}

		return sstrm.str();
	}

	std::string generateAssignment()
	{
		throw std::logic_error("lblmc::ArrayObject::generateAssignment(void): assignment generation cannot be performed without specifying indices for assignment");

		return std::string("");
	}

	template <typename T>
	std::string generateAssignment(std::vector<T>&& indices, std::string v)
	{
		if(indices.size() != dimensions.size())
			throw std::invalid_argument("lblmc::ArrayObject::generateAssignment(std::vector<T>&&): number of specified indices must equal array dimension size");
		if(v.empty()) throw std::invalid_argument("lblmc::ArrayObject::generateAssignment(): argument v cannot be empty or null");

		if(label.empty()) throw std::runtime_error("lblmc::ArrayObject::generateAssignment(): label cannot be empty or null");

		std::stringstream sstrm;

		sstrm << label;

		for(auto i : indices)
		{
			sstrm << "[" << i << "]";
		}

		sstrm << " = " << v << ";";

		return sstrm.str();
	}

	std::string generateArgument()
	{
		if(type.empty()) throw std::runtime_error("lblmc::ArrayObject::generateArgument(): type cannot be empty or null");
		if(label.empty()) throw std::runtime_error("lblmc::ArrayObject::generateArgument(): label cannot be empty or null");
		if(dimensions.empty()) throw std::runtime_error("lblmc::ArrayObject::generateArgument(): dimension size cannot be zero");

		std::stringstream sstrm;

		for(auto q : qualifiers)
		{

			if(qualiferAsString(q).empty()) continue;

			sstrm << qualiferAsString(q) << " ";
		}

		sstrm << type << " " << label;

		for(auto d : dimensions)
		{
			sstrm << "[" << d << "]";
		}

		return sstrm.str();
	}


};

} //namespace codegen

#endif // CODEGEN_ARRAYOBJECT_HPP
