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

#ifndef CODEGEN_REFERENCEOBJECT_HPP
#define CODEGEN_REFERENCEOBJECT_HPP

#include <string>
#include <vector>
#include "codegen/Object.hpp"

namespace lblmc
{

/**
	\brief describes C++ pointer, reference, and r-value reference objects and generate appropriate
	C++ code for the objects

	This class cannot describe arrays, containers, or smart pointers
	(std::unique_ptr, std::shared_ptr, etc.)

	\author Matthew Milton
	\date 2019
**/
class ReferenceObject : public Object
{

public:

	enum class ReferenceType : int
	{
		NONE = 0,
		POINTER,
		CONSTANT_POINTER,
		REFERENCE,
		RVALUE_REFERENCE,
		POINTER_REFERENCE,
		CONSTANT_POINTER_REFERENCE
	};


};

} //namespace codegen

#endif // CODEGEN_REFERENCEOBJECT_HPP
