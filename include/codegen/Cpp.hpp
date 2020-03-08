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

#ifndef LBLMC_CPP_HPP
#define LBLMC_CPP_HPP

#include <string>
#include <vector>

namespace lblmc
{

/**
	\brief defines standard C++11/14 features for code generation and validates C++ code

	This class provides public string constants for C++ standard keywords and characters/operators
**/
class Cpp
{

public:

// valid characters for type names, names, and labels

	const static std::string WHITESPACE_CHARS; ///< characters for whitespace
	const static std::string BAD_START_CHARS;  ///< characters that are invalid for start of labels/names
	const static std::string VALID_NAME_CHARS; ///< valid characters for labels and names
	const static std::string VALID_NUMBER_CHARS; ///< valid characters for number literal expression

// storage (class) qualifiers

	/**
		\brief enumeration of object qualifiers, such as const, static, volatile, extern, etc.
	**/
	enum class StorageQualifier: int
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
		THREAD_LOCAL,	///< object that exists only for the thread it is declared; C++11 and up
		CONST_STATIC ///< constant static object
	};

	const static std::string STATIC;       ///< static (persistent) or file-local qualifier
	const static std::string CONST;        ///< constant (read only) qualifier
	const static std::string CONSTEXPR;    ///< constant (read only) expression qualifier
	const static std::string VOLATILE;     ///< volatile (unpredictably changing) qualifier
	const static std::string REGISTER;     ///< register-occupied qualifier
	const static std::string EXTERN;       ///< externally defined qualifier
	const static std::string THREAD_LOCAL; ///< thread only scoped qualifier
	const static std::string MUTABLE;      ///< explicit non-constant qualifier
	const static std::string AUTO;         ///< automatic (default) qualifier
	const static std::string CONST_STATIC; ///< constant static (true constant) combo qualifier; same as STATIC_CONST
	const static std::string STATIC_CONST; ///< constant static (true constant) combo qualifier; same as CONST_STATIC

	const static std::vector<std::string> ALL_STORAGE_QUALIFIERS; ///< vector of all storage qualifiers

// C++ type modifiers

	enum class TypeModifier: int
	{
		NONE = 0,
		SIGNED,
		UNSIGNED,
		SHORT,
		LONG
	};

	const static std::string SIGNED;       ///< signed (supports negative) data type
	const static std::string UNSIGNED;     ///< unsigned (nonnegative) data type
	const static std::string SHORT;        ///< shorter version of data type
	const static std::string LONG;         ///< longer version of data type

	const static std::vector<std::string> ALL_TYPE_MODIFIERS; ///< vector of all type modifiers

// intrinsic primitive C++ data types

	enum class IntrinsicType: int
	{
		NA   = 0,
		VOID,
		BOOL,
		CHAR,
		INT,
		FLOAT,
		DOUBLE,
		SHORT_INT,
		LONG_INT,
		LONG_DOUBLE,
		UNSIGNED_CHAR,
		UNSIGNED_INT,
		UNSIGNED_SHORT_INT,
		UNSIGNED_LONG_INT
	};

	const static std::string VOID;               ///< no data type; raw data
	const static std::string BOOL;               ///< boolean type
	const static std::string CHAR;               ///< 8-bit character/integral type
	const static std::string INT;                ///< 32-bit integral type
	const static std::string FLOAT;              ///< 32-bit floating point type
	const static std::string DOUBLE;             ///< 32/64-bit floating point type

	const static std::string SHORT_INT;          ///< 16-bit integral type
	const static std::string LONG_INT;           ///< 32/64-bit integral type
	const static std::string LONG_DOUBLE;        ///< 64/96/128-bit floating point type

	const static std::string UNSIGNED_CHAR;      ///< unsigned 8-bit character/integral type
	const static std::string UNSIGNED_INT;       ///< unsigned 32-bit integral type
	const static std::string UNSIGNED_SHORT_INT; ///< unsigned 16-bit integral type
	const static std::string UNSIGNED_LONG_INT;  ///< unsigned 32/64-bit integral type

	const static std::vector<std::string> ALL_INTRINSIC_TYPES; ///< vector of all intrinsic C++ types

//	non-standard C++ types used by codegen tools

	enum class CodeGenType : int
	{
		NA = 0,
		REAL,
		FIXED
	};

	const static std::string REAL; ///< arbitrarily-implemented real (decimal) numerical data type
	const static std::string FIXED; ///< arbitrarily-implemented but fixed point real (decimal) numerical data type

	const static std::vector<std::string> ALL_CODEGEN_TYPES; ///< vector of all custom types in the codegen tools

// C++ reference specifiers

	enum class ReferenceSpecifier : int
	{
		NONE = 0,
		POINTER,
		LVALUE_REFERENCE,
		RVALUE_REFERENCE,
		POINTER_TO_POINTER,
		POINTER_TO_POINTER_TO_POINTER,
		CONST_POINTER
	};

	const static std::string POINTER;                       ///< memory-address oriented pointer to object (*)
	const static std::string LVALUE_REFERENCE;              ///< alias reference to object (&) with memory assigned
	const static std::string RVALUE_REFERENCE;              ///< alias reference to object (&&) without memory (literals and inline temporaries).
	const static std::string POINTER_TO_POINTER;            ///< pointer to a pointer
	const static std::string POINTER_TO_POINTER_TO_POINTER; ///< pointer to a pointer to a pointer
	const static std::string CONST_POINTER;                 ///< constant pointer to (non-)const object (*const)

	const static std::vector<std::string> ALL_REFERENCE_SPECIFIERS; ///< vector of all reference specifiers

// C++ array decorations

	const static std::string ARRAY_BRACKETS; ///< brackets used for array declaration and indexing

// C++ brackets and parentheses

	const static std::string SQUARE_BRACKETS; ///< square [] brackets used for array declaration and indexing
	const static std::string CURLY_BRACKETS;  ///< curly {} brackets used for scopes/bodies, initialization, etc.
	const static std::string ANGULAR_BRACKETS; ///< angular <> brackets used for template parameters
	const static std::string PARENTHESES; ///< parentheses () used for function/method parameter/argument lists, etc.

// C++ user defined type and related keywords

	const static std::string CLASS;  ///< class definition for objects with private-defaulted members
	const static std::string STRUCT; ///< class definition for objects with public-defaulted members
	const static std::string UNION;  ///< class definition for objects with members that share same memory space
	const static std::string ENUM;   ///< class definition of enumerated values
	const static std::string PUBLIC; ///< defines members of class to be publicly accessible
	const static std::string PRIVATE; ///< defines members to be privy only to the class
	const static std::string PROTECTED; ///< defines members to be privy only to class and class derivatives.
	const static std::string VIRTUAL; ///< defines member can be overridden by derived class
	const static std::string FRIEND; ///< defines member can be accessed by friended classes
	const static std::string DELETE; ///< defines member is to be deleted from class definition
	const static std::string INLINE; ///< defines members is to be instanced inline to its use
	const static std::string TEMPLATE; ///< defines class to be generic/template

// C++ loop keywords

	const static std::string WHILE;     ///< while condition true loop
	const static std::string DO;        ///< do at least once while condition true loop
	const static std::string FOR;       ///< for over index range or for each element loop

// C++ branch keywords

	const static std::string IF;        ///< if condition true branch
	const static std::string ELSE;      ///< else condition false branch
	const static std::string GOTO;      ///< goto label branch
	const static std::string SWITCH;    ///< switch branching
	const static std::string CASE;      ///< case in switch branch
	const static std::string CONTINUE;  ///< continue to next loop iteration or branch
	const static std::string BREAK;     ///< break from current branch

// all C++11/14 keywords together

	const static std::vector<std::string> ALL_CPP14_KEYWORDS; ///< vector of all keywords in C++11/14

// Methods

	/**
		\return true if given name is a valid C++ name/label; false otherwise

		A valid C++ name or label must have no whitespace, start only with letters or underscore,
		and consist of only alphanumerical or underscore characters.
	**/
	static bool isNameValid(const std::string& name);

	//static bool isObjectDeclarationValid(const std::string& decl);
	//static bool isObjectDefinitionValid(const std::string& def);
	//static bool isParameterValid(const std::string& param);

	/**
		\return true if given word is a valid C++ keyword; false otherwise

		The given keyword must not have any whitespace in it.
	**/
	static bool isCpp14Keyword(const std::string& word);

	/**
		\return true if given code string contains a valid C++ keyword; false otherwise
	**/
	static bool containsCpp14Keyword(const std::string& code);

	/**
		\return true if word is a valid C++ storage qualifier

		The given word must not contain any whitespace in it.  If the qualifier is a combo one
		(const static, static const), then only one space is permitted between the words for this
		method even though C++ allows more than one space between words.
	**/
	static bool isStorageQualifier(const std::string& word);

	/**
		\return true if word is a valid C++ type modifier

		The given word must not contain any whitespace in it.
	**/
	static bool isTypeModifier(const std::string& word);

	/**
		\return true if word is a valid C++ intrinsic (primitive) type

		The given word must not contain any whitespace in it.  If the type is a combo one
		(unsigned int, for instance), then only one space is permitted between the words for this
		method even though C++ allows more than one space between words.
	**/
	static bool isIntrinsicType(const std::string& word);

	/**
		\return true if word is a valid custom codegen type used by the LB-LMC tools

		The custom types include only real and fixed.

		The given word must not contain any whitespace in it.
	**/
	static bool isCodeGenType(const std::string& word);

	/**
		\return true if word is a valid C++ reference specifier

		The given word must not contain any whitespace in it.
	**/
	static bool isReferenceSpecifier(const std::string& word);

	static std::string convertEnumerationToString(const StorageQualifier& e);
	static std::string convertEnumerationToString(const TypeModifier& e);
	static std::string convertEnumerationToString(const IntrinsicType& e);
	static std::string convertEnumerationToString(const CodeGenType& e);
	static std::string convertEnumerationToString(const ReferenceSpecifier& e);

	static StorageQualifier   convertStorageQualifierStringToEnumeration(const std::string& s);
	static TypeModifier       convertTypeModifierStringToEnumeration(const std::string& s);
	static IntrinsicType      convertIntrinsicTypeStringToEnumeration(const std::string& s);
	static CodeGenType        convertCodeGenTypeStringToEnumeration(const std::string& s);
	static ReferenceSpecifier convertReferenceSpecifierStringToEnumeration(const std::string& s);

};

} //namespace lblmc

#endif // LBLMC_CPPVALIDATOR_HPP

