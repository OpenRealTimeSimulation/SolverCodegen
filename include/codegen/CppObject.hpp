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

#ifndef LBLMC_CPPOBJECT_HPP
#define LBLMC_CPPOBJECT_HPP

#include "codegen/Cpp.hpp"
#include <vector>
#include <string>

namespace lblmc
{

/**
    \brief defines an object in C++, such as variables and parameters

    This class replaces lblmc::Object, lblmc::ReferenceObject, and lblmc::ArrayObject
**/
class CppObject
{

private:

	std::vector<Cpp::StorageQualifier> storage_qualifiers;     ///< storage qualifier of the object
	std::string type;                                          ///< data type of the object
	std::vector<Cpp::ReferenceSpecifier> reference_specifiers; ///< reference specifiers (pointer, reference) of object
	std::string label;                                         ///< label of the object
	std::vector<long> dimensions;                              ///< array dimension of object
	std::string value;                                         ///< assigned value of object

public:

	/**
		\brief default constructor
	**/
	CppObject();

	/**
		\brief constructs object as a plain scalar object
	**/
	CppObject
	(
		const std::string& type,
		const std::string& label,
		const std::string& value = std::string()
	);

	/**
		\brief constructs object as a pointer/reference
	**/
	CppObject
	(
		const std::string& type,
		std::vector<Cpp::ReferenceSpecifier> reference_specifiers,
		const std::string& label,
		const std::string& value = std::string()
	);

	/**
		\brief constructs object as a n-dimensional array
	**/
	CppObject
	(
		const std::string& type,
		const std::string& label,
		const std::vector<long>& dimensions,
		const std::string& value = std::string()
	);

	/**
		\brief full parameter constructor
	**/
	CppObject
	(
		const std::vector<Cpp::StorageQualifier>& storage_qualifiers,
		const std::string& type,
		const std::vector<Cpp::ReferenceSpecifier>& reference_specifiers,
		const std::string& label,
		const std::vector<long>& dimensions,
		const std::string& value = std::string()
	);

	/**
		\brief copy constructor
	**/
	CppObject(const CppObject& base);

	/**
		\brief move constructor
	**/
	CppObject(CppObject&& base);

	void setStorageQualifiers(const std::vector<Cpp::StorageQualifier>& q);
	void setType(const std::string& type);
	void setReferenceSpecifiers(const std::vector<Cpp::ReferenceSpecifier>& r);
	void setLabel(const std::string& label);
	void setArrayDimensions(const std::vector<long>& d);
	void setAssignedValue(const std::string& v);

	const std::vector<Cpp::StorageQualifier>& getStorageQualifiers() const;
	const std::string& getType() const;
	const std::vector<Cpp::ReferenceSpecifier>& getReferenceSpecifiers() const;
	const std::string& getLabel() const;
	const std::vector<long>& getArrayDimensions() const;
	const std::string& getAssignedValue() const;

	void insertStorageQualifier(Cpp::StorageQualifier q);
	void insertReferenceSpecifier(Cpp::ReferenceSpecifier r);
	void insertArrayDimension(long d);

	std::string generateStorageQualifiersAsString() const;
	std::string generateReferenceSpecifiersAsString() const;
	std::string generateArrayDimensionsAsString() const;

	std::string generateDeclaration() const;
	std::string generateDefinition() const;
	std::string generateAssignment() const;
	std::string generateAssignment(const std::string& value) const;
	std::string generateArgument() const;


};

} //namespace lblmc

#endif // LBLMC_CPPOBJECT_HPP

