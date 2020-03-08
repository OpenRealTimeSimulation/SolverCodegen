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

#ifndef LBLMC_LABELVALUEPAIR_HPP
#define LBLMC_LABELVALUEPAIR_HPP

#include <string>

/**
	\brief defines a general-purpose label and value pair
	\tparam TYPE the data type of the value

	This pair can be used to define new fields in open-ended definitions.
**/
template<typename TYPE>
struct LabelValuePair
{
	std::string label;
	TYPE value;
};

/**
	\brief defines a label and value pair where the value is a variant of 2 different types
	\tparam TYPE the data type of the value

	This pair can be used to define new fields in open-ended definitions.

	The value of the pair is assigned to either value1 or value2, depending on desired
	type for the value.  Fields value1 and value2 share same memory space.
**/
template<typename TYPE1, typename TYPE2>
struct LabelValuePair2
{
	std::string label;
	union
	{
		TYPE1 value1;  ///< value as type TYPE1 associated with label
		TYPE2 value2;  ///< value as type TYPE2 associated with label
	};

};

/**
	\brief defines a label and value pair where the value is a variant of 3 different types
	\tparam TYPE the data type of the value

	This pair can be used to define new fields in open-ended definitions.

	The value of the pair is assigned to either value1, value2, or value3 depending on desired
	type for the value.  Fields value1 through value3 share same memory space.
**/
template<typename TYPE1, typename TYPE2, typename TYPE3>
struct LabelValuePair3
{
	std::string label;
	union
	{
		TYPE1 value1; ///< value as type TYPE1 associated with label
		TYPE2 value2; ///< value as type TYPE2 associated with label
		TYPE3 value3; ///< value as type TYPE3 associated with label
	};

};

/**
	\brief defines a label and value pair where the value is a variant of 4 different types
	\tparam TYPE the data type of the value

	This pair can be used to define new fields in open-ended definitions.

	The value of the pair is assigned to either value1, value2, value3, or value4 depending on
	desired type for the value.  Fields value1 through value4 share same memory space.
**/
template<typename TYPE1, typename TYPE2, typename TYPE3, typename TYPE4>
struct LabelValuePair4
{
	std::string label;
	union
	{
		TYPE1 value1; ///< value as type TYPE1 associated with label
		TYPE2 value2; ///< value as type TYPE2 associated with label
		TYPE3 value3; ///< value as type TYPE3 associated with label
		TYPE4 value4; ///< value as type TYPE4 associated with label
	};

};

#endif // LBLMC_LABELVALUEPAIR_HPP

