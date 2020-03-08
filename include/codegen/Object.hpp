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

#ifndef CODEGEN_OBJECT_HPP
#define CODEGEN_OBJECT_HPP

#include <string>
#include <vector>
#include <stdexcept>
#include <sstream>

namespace lblmc
{

/**
	\brief describes C++ objects such as variables, constants, arguments/parameters,
	object instances, etc., and generate appropriate C++ code for the objects

	Each instance of the lblmc::Object class describes a single C++ object that can exist in
	generated C++ code. The qualifiers, type, label, and default/literal value of the object is
	described.  From the description, lblmc::Object can generate appropriate C++ declarations,
	definitions, assignments, and argument-declarations stored in character strings that can be
	stamped into generated C++ source code.

	Note that the data type, label, and even value of the described object are all strings.  This
	allows robust and generic code generation for any valid C++ object that could exist in C++
	source code.  It is up to the user of this class to provide these strings and make sure they
	are valid C++ code so lblmc::Object can generate valid C++ code.

	This class cannot describe reference objects such as pointers, references, and arrays.
	Moreover, this class cannot describe definitions for classes, functions or templates.

	\see ArrayObject for class to describe array objects
	\see ReferenceObject for class to describe pointer and reference objects

	\author Matthew Milton
	\date 2019
**/
class Object
{

public:

	/**
		\brief enumeration of object qualifiers, such as const, static, volatile, extern, etc.
	**/
	enum class Qualifier: int
	{
		NONE = 0,	///< no qualifier specified; default
		CONST,		///< constant (non-changing) object; or constant integral/enumeration class field
		CONSTEXPR,	///< constant expression object; C++11 and up
		VOLATILE,	///< volatile (change unexpectedly) object
		MUTABLE,	///< mutable class field object
		AUTO,		///< automatic qualifier determined for object
		REGISTER,	///< registered (frequently used) object
		STATIC,		///< persistent object between owner's instances/calls, or object exists only in translation unit
		EXTERN,		///< externally defined object
		THREAD_LOCAL	///< object that exists only for the thread it is declared; C++11 and up
	};

protected:

	std::vector<Qualifier> qualifiers; ///< qualifiers of the described object
	std::string type;                  ///< data type of the described object
	std::string label;                 ///< C++ valid label of the described object
	std::string value;                 ///< value that is assigned to the described object in definition or assignment

public:

	/**
		\brief default constructor
	**/
	Object() :
		qualifiers(),
		type(),
		label(),
		value()
	{}

	/**
		\brief parameter constructor
		\param type data type of the described object
		\param label C++ valid label of the described object
		\param value value that is assigned to the described object in definition or assignment
	**/
	Object(std::string type, std::string label, std::string value) :
		qualifiers(),
		type(type),
		label(label),
		value(value)
	{
		if(type.empty()) throw std::invalid_argument("lblmc::Object constructor(): type cannot be empty or null");
		if(label.empty()) throw std::invalid_argument("lblmc::Object constructor(): label cannot be empty or null");
	}

	/**
		\brief copy constructor
	**/
	Object(const Object& base) :
		qualifiers(base.qualifiers),
		type(base.type),
		label(base.label),
		value(base.value)
	{}

	/**
		\brief inserts a qualifier for the described object

		If object already has the given qualifier, this method does nothing.

		\param qual qualifier to attach to described object
	**/
	inline void insertQualifier(Qualifier qual)
	{
		for(auto q : qualifiers)
		{
			if(q == qual) return;
		}

		qualifiers.push_back(qual);
	}

	/**
		\return constant container storing all qualifiers for the described object
	**/
	inline const std::vector<Qualifier>& getQualifiers() const
	{
		return qualifiers;
	}

	/**
		\brief sets data type of the described object
		\param type type of the object
	**/
	inline void setType(std::string type)
	{
		if(this->type.empty()) throw std::invalid_argument("lblmc::Object::setType(): type cannot be empty or null");

		this->type = type;
	}

	/**
		\return data type of the described object
	**/
	inline const std::string& getType() const
	{
		return type;
	}

	/**
		\brief sets label for the described object
		\param label C++ valid label for the object
	**/
	inline void setLabel(std::string label)
	{
		if(this->label.empty()) throw std::invalid_argument("lblmc::Object::setLabel(): label cannot be empty or null");

		this->label = label;
	}

	/**
		\return label of the described object
	**/
	inline const std::string& getLabel() const
	{
		return label;
	}

	/**
		\brief sets default value for the described object
		\param value value that is assigned to the described object in definition or assignment
	**/
	inline void setValue(std::string value)
	{
		this->value = value;
	}

	/**
		\return default value for the described object
	**/
	inline const std::string& getValue() const
	{
		return value;
	}

	/**
		\brief generates a string for a C++ declaration of the described object

		A declaration declares the qualifier, type, and label of an instanced object only.  Examples
		include:\n
		volatile double some_object;\n
		extern const int some_ext_def_object;\n
		static SomeClass some_class_object;\n
		int temp;\n

		The qualifier are optional in declarations and initial/default value are normally not stated in declaration.

		\return string containing generated string

		\see generateDefinition() if initial/default value of described object needs to be defined.

	**/
	virtual std::string generateDeclaration()
	{
		if(type.empty()) throw std::runtime_error("lblmc::Object::generateDeclaration(): type cannot be empty or null");
		if(label.empty()) throw std::runtime_error("lblmc::Object::generateDeclaration(): label cannot be empty or null");

		std::stringstream sstrm;

		for(auto q : qualifiers)
		{

			if(qualiferAsString(q).empty()) continue;

			sstrm << qualiferAsString(q) << " ";
		}

		sstrm << type << " " << label << ";";

		return sstrm.str();
	}

	/**
		\brief generates a string for a C++ definition of the described object

		A definition defines the qualifier, type, label, and the default/initial value of an
		instanced object.  Examples include:\n
		volatile double some_object = 25.0;\n
		const int some_ext_def_object = 1000;\n
		static SomeClass some_class_object = SomeClass(some_arguments);\n
		int temp = 3;\n

		The default/initial value and qualifier are optional in definitions.

		\return string containing generated string

		\see generateDeclaration() for forward declarations of the described object

	**/
	virtual std::string generateDefinition()
	{
		if(type.empty()) throw std::runtime_error("lblmc::Object::generateDefinition(): type cannot be empty or null");
		if(label.empty()) throw std::runtime_error("lblmc::Object::generateDefinition(): label cannot be empty or null");

		std::stringstream sstrm;

		for(auto q : qualifiers)
		{
			if(qualiferAsString(q).empty()) continue;

			sstrm << qualiferAsString(q) << " ";
		}

		sstrm << type << " " << label ;

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


	/**
		\brief generates a string for a C++ assignment of the described object

		Assignments assume the described object has already been declared or defined in generated
		C++ code.  Examples include:\n
		some_object = 25.0;\n
		some_ext_def_object = 1000;\n
		some_class_object = SomeClass(some_arguments);\n
		temp = 3;\n

		This method uses the value of the described object for the assignment.
		\see generateAssignment(std::string) for assigning value different than described object's
		default value.

		\return string containing generated string

	**/
	virtual std::string generateAssignment()
	{
		if(label.empty()) throw std::runtime_error("lblmc::Object::generateAssignment(): label cannot be empty or null");
		if(value.empty()) throw std::runtime_error("lblmc::Object::generateAssignment(): value cannot be empty or null");

		std::stringstream sstrm;

		sstrm << label ;
		sstrm << " = " << value << ";";

		return sstrm.str();
	}

	/**
		\brief generates a string for a C++ assignment of the described object

		Assignments assume the described object has already been declared or defined in generated
		C++ code.  Examples include:\n
		some_object = 25.0;\n
		some_ext_def_object = 1000;\n
		some_class_object = SomeClass(some_arguments);\n
		temp = 3;\n

		This method uses the passed argument for the assignment.
		\see generateAssignment(void) for assigning the described object's default value.

		\param v the value that will be assigned to the described object
		\return string containing generated string

	**/
	virtual std::string generateAssignment(std::string v)
	{
		if(label.empty()) throw std::runtime_error("lblmc::Object::generateAssignment(): label cannot be empty or null");
		if(v.empty()) throw std::invalid_argument("lblmc::Object::generateAssignment(): argument v cannot be empty or null");

		std::stringstream sstrm;

		sstrm << label ;
		sstrm << " = " << v << ";";

		return sstrm.str();
	}

	/**
		\brief generates a string for C++ declaration of the described object as an argument
		parameter for a function, method, class, or template.

		\return string containing generated string
	**/
	virtual std::string generateArgument()
	{
		if(type.empty()) throw std::runtime_error("lblmc::Object::generateArgument(): type cannot be empty or null");
		if(label.empty()) throw std::runtime_error("lblmc::Object::generateArgument(): label cannot be empty or null");

		std::stringstream sstrm;

		for(auto q : qualifiers)
		{
			if(qualiferAsString(q).empty()) continue;

			sstrm << qualiferAsString(q) << " ";
		}

		sstrm << type << " " << label;

		if(!value.empty())
		{
			sstrm << " = " << value;
		}

		return sstrm.str();
	}

protected:

	/**
		\brief protected method that returns valid C++ keyword as string for given qualifier enumeration
		\param qual qualifier enumeration to convert to C++ keyword string
		\return C++ keyword string for given qualifier enumeration
	**/
	inline std::string qualiferAsString(Qualifier qual)
	{
		switch (qual)
		{
			case Qualifier::NONE :
				return std::string("");
				break;
			case Qualifier::CONST :
				return std::string("const");
				break;
			case Qualifier::CONSTEXPR :
				return std::string("constexpr");
				break;
			case Qualifier::VOLATILE :
				return std::string("volatile");
				break;
			case Qualifier::MUTABLE :
				return std::string("mutable");
				break;
			case Qualifier::AUTO :
				return std::string("auto");
				break;
			case Qualifier::REGISTER :
				return std::string("register");
				break;
			case Qualifier::STATIC :
				return std::string("static");
				break;
			case Qualifier::EXTERN :
				return std::string("extern");
				break;
			case Qualifier::THREAD_LOCAL :
				return std::string("thread_local");
				break;
			default :
				return std::string("");
				break;

		}
	}

};

} //namespace codegen

#endif // CODEGEN_OBJECT_HPP
