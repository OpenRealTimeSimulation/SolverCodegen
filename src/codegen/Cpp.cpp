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

#include "codegen/Cpp.hpp"
#include <string>
#include <stdexcept>
#include <vector>

namespace lblmc
{


// valid characters for type names, names, and labels

const std::string Cpp::WHITESPACE_CHARS(" \n\r\t\f\v");
const std::string Cpp::BAD_START_CHARS("1234567890.!@$^&*()-+=\\|;:\"'<>,?/~`[]{}");
const std::string Cpp::VALID_NAME_CHARS("1234567890_abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ");
const std::string Cpp::VALID_NUMBER_CHARS("1234567890.eE+-");

// storage (class) specifiers

const std::string Cpp::STATIC("static");
const std::string Cpp::CONST("const");
const std::string Cpp::CONSTEXPR("constexpr");
const std::string Cpp::VOLATILE("volatile");
const std::string Cpp::REGISTER("register");
const std::string Cpp::EXTERN("extern");
const std::string Cpp::THREAD_LOCAL("thread_local");
const std::string Cpp::MUTABLE("mutable");
const std::string Cpp::AUTO("auto");
const std::string Cpp::CONST_STATIC("const static");
const std::string Cpp::STATIC_CONST("static const");

const std::vector<std::string> Cpp::ALL_STORAGE_QUALIFIERS
{
	"static",
	"const",
	"constexpr",
	"volatile",
	"register",
	"extern",
	"thread_local",
	"mutable",
	"auto",
	"const static",
	"static const"
};

// C++ type modifiers

const std::string Cpp::SIGNED("signed");
const std::string Cpp::UNSIGNED("unsigned");
const std::string Cpp::SHORT("short");
const std::string Cpp::LONG("long");

const std::vector<std::string> Cpp::ALL_TYPE_MODIFIERS
{
	"signed",
	"unsigned",
	"short",
	"long",
};

// intrinsic primitive C++ data types

const std::string Cpp::VOID("void");
const std::string Cpp::BOOL("bool");
const std::string Cpp::CHAR("char");
const std::string Cpp::INT("int");
const std::string Cpp::FLOAT("float");
const std::string Cpp::DOUBLE("double");

const std::string Cpp::SHORT_INT("short int");
const std::string Cpp::LONG_INT("long");
const std::string Cpp::LONG_DOUBLE("long double");

const std::string Cpp::UNSIGNED_CHAR("unsigned char");
const std::string Cpp::UNSIGNED_INT("unsigned int");
const std::string Cpp::UNSIGNED_SHORT_INT("unsigned short int");
const std::string Cpp::UNSIGNED_LONG_INT("unsigned long");

const std::vector<std::string> Cpp::ALL_INTRINSIC_TYPES
{
	"void",
	"bool",
	"char",
	"int",
	"float",
	"double",
	"short int",
	"long",
	"long double",
	"unsigned char",
	"unsigned int",
	"unsigned short int",
	"unsigned long"
};

//	non-standard C++ types used by codegen tools

const std::string Cpp::REAL("real");
const std::string Cpp::FIXED("fixed");

const std::vector<std::string> Cpp::ALL_CODEGEN_TYPES
{
	"real",
	"fixed"
};

// C++ reference specifiers

const std::string Cpp::POINTER("*");
const std::string Cpp::LVALUE_REFERENCE("&");
const std::string Cpp::RVALUE_REFERENCE("&&");
const std::string Cpp::POINTER_TO_POINTER("**");
const std::string Cpp::POINTER_TO_POINTER_TO_POINTER("***");
const std::string Cpp::CONST_POINTER("*const");

const std::vector<std::string> Cpp::ALL_REFERENCE_SPECIFIERS
{
	"*",
	"&",
	"&&",
	"**",
	"***",
	"*const"
};

// C++ array decorations

const std::string Cpp::ARRAY_BRACKETS("[]");

// C++ brackets and parentheses

const std::string Cpp::SQUARE_BRACKETS("[]");
const std::string Cpp::CURLY_BRACKETS("{}");
const std::string Cpp::ANGULAR_BRACKETS("<>");
const std::string Cpp::PARENTHESES("()");

// C++ user defined type and related keywords

const std::string Cpp::CLASS("class");
const std::string Cpp::STRUCT("struct");
const std::string Cpp::UNION("union");
const std::string Cpp::ENUM("enum");
const std::string Cpp::PUBLIC("public");
const std::string Cpp::PRIVATE("private");
const std::string Cpp::PROTECTED("protected");
const std::string Cpp::VIRTUAL("virtual");
const std::string Cpp::FRIEND("friend");
const std::string Cpp::DELETE("delete");
const std::string Cpp::INLINE("inline");
const std::string Cpp::TEMPLATE("template");

// C++ loop keywords

const std::string Cpp::WHILE("while");
const std::string Cpp::DO("do");
const std::string Cpp::FOR("for");

// C++ branch keywords

const std::string Cpp::IF("if");
const std::string Cpp::ELSE("else");
const std::string Cpp::GOTO("goto");
const std::string Cpp::SWITCH("switch");
const std::string Cpp::CASE("case");
const std::string Cpp::CONTINUE("continue");
const std::string Cpp::BREAK("break");

// all C++11/14 keywords together

const std::vector<std::string> Cpp::ALL_CPP14_KEYWORDS
{
	"alignas",
	"alignof",
	"and",
	"and_eq",
	"asm",
	"auto",
	"bitand",
	"bitor",
	"bool",
	"break",
	"case",
	"catch",
	"char",
	"char16_t",
	"char32_t",
	"class",
	"compl",
	"const",
	"constexpr",
	"const_cast",
	"continue",
	"decltype",
	"default",
	"delete",
	"do",
	"double",
	"dynamic_cast",
	"else",
	"enum",
	"explicit",
	"export",
	"extern",
	"false",
	"float",
	"for",
	"friend",
	"goto",
	"if",
	"inline",
	"int",
	"long",
	"mutable",
	"namespace",
	"new",
	"noexcept",
	"not",
	"not_eq",
	"nullptr",
	"operator",
	"or",
	"or_eq",
	"private",
	"protected",
	"public",
	"register",
	"reinterpret_cast",
	"return",
	"short",
	"signed",
	"sizeof",
	"static",
	"static_assert",
	"static_cast",
	"struct",
	"switch",
	"template",
	"this",
	"thread_local",
	"throw",
	"true",
	"try",
	"typedef",
	"typeid",
	"typename",
	"union",
	"unsigned",
	"using",
	"virtual",
	"void",
	"volatile",
	"wchar_t",
	"while",
	"xor",
	"xor_eq"
};

// Methods

bool Cpp::isNameValid(const std::string& name)
{
	bool valid = false;

	valid =
	(name.size() != 0) &&
	(name.find_first_of(WHITESPACE_CHARS) == std::string::npos) &&
	(name.find_first_of(BAD_START_CHARS) != 0) &&
	(name.find_first_not_of(VALID_NAME_CHARS) == std::string::npos)
	;

	return valid;
}

bool Cpp::isCpp14Keyword(const std::string& word)
{

	for(const auto& keyword : ALL_CPP14_KEYWORDS)
	{
		if(word == keyword) return true;
	}

	return false;
}

bool Cpp::containsCpp14Keyword(const std::string& code)
{
	for(const auto& keyword : ALL_CPP14_KEYWORDS)
	{
		if(code.find(keyword) != std::string::npos) return true;
	}

	return false;
}

bool Cpp::isStorageQualifier(const std::string& word)
{
	for(const auto& qualifier : ALL_STORAGE_QUALIFIERS)
	{
		if(word == qualifier) return true;
	}

	return false;
}

bool Cpp::isTypeModifier(const std::string& word)
{
	for(const auto& modifier : ALL_TYPE_MODIFIERS)
	{
		if(word == modifier) return true;
	}

	return false;
}

bool Cpp::isIntrinsicType(const std::string& word)
{
	for(const auto& type : ALL_INTRINSIC_TYPES)
	{
		if(word == type) return true;
	}

	return false;
}

bool Cpp::isCodeGenType(const std::string& word)
{
	for(const auto& type : ALL_CODEGEN_TYPES)
	{
		if(word == type) return true;
	}

	return false;
}

bool Cpp::isReferenceSpecifier(const std::string& word)
{
	for(const auto& refspec : ALL_REFERENCE_SPECIFIERS)
	{
		if(word == refspec) return true;
	}

	return false;
}

std::string Cpp::convertEnumerationToString(const Cpp::StorageQualifier& e)
{
	switch (e)
	{
		case StorageQualifier::NONE :
			return std::string();
		case StorageQualifier::CONST :
			return CONST;
		case StorageQualifier::CONSTEXPR :
			return CONSTEXPR;
		case StorageQualifier::VOLATILE :
			return VOLATILE;
		case StorageQualifier::MUTABLE :
			return MUTABLE;
		case StorageQualifier::AUTO :
			return AUTO;
		case StorageQualifier::REGISTER :
			return REGISTER;
		case StorageQualifier::STATIC :
			return STATIC;
		case StorageQualifier::EXTERN :
			return EXTERN;
		case StorageQualifier::THREAD_LOCAL :
			return THREAD_LOCAL;
		case StorageQualifier::CONST_STATIC :
			return CONST_STATIC;
		default :
			return std::string();
	}
}

std::string Cpp::convertEnumerationToString(const Cpp::TypeModifier& e)
{
	switch (e)
	{
		case TypeModifier::NONE :
			return std::string();
		case TypeModifier::SIGNED :
			return SIGNED;
		case TypeModifier::UNSIGNED :
			return UNSIGNED;
		case TypeModifier::SHORT :
			return SHORT;
		case TypeModifier::LONG :
			return LONG;
		default :
			return std::string();
	}
}

std::string Cpp::convertEnumerationToString(const Cpp::IntrinsicType& e)
{
	switch (e)
	{
		case IntrinsicType::VOID :
			return VOID;
		case IntrinsicType::BOOL :
			return BOOL;
		case IntrinsicType::CHAR :
			return CHAR;
		case IntrinsicType::INT :
			return INT;
		case IntrinsicType::FLOAT :
			return FLOAT;
		case IntrinsicType::DOUBLE :
			return DOUBLE;
		case IntrinsicType::SHORT_INT :
			return SHORT_INT;
		case IntrinsicType::LONG_INT :
			return LONG_INT;
		case IntrinsicType::LONG_DOUBLE :
			return LONG_DOUBLE;
		case IntrinsicType::UNSIGNED_CHAR :
			return UNSIGNED_CHAR;
		case IntrinsicType::UNSIGNED_INT :
			return UNSIGNED_INT;
		case IntrinsicType::UNSIGNED_SHORT_INT :
			return UNSIGNED_SHORT_INT;
		case IntrinsicType::UNSIGNED_LONG_INT :
			return UNSIGNED_LONG_INT;
		default :
			return std::string();
	}
}

std::string Cpp::convertEnumerationToString(const Cpp::CodeGenType& e)
{
	switch (e)
	{
		case CodeGenType::REAL :
			return REAL;
		case CodeGenType::FIXED :
			return FIXED;
		default :
			return std::string();
	}
}

std::string Cpp::convertEnumerationToString(const Cpp::ReferenceSpecifier& e)
{
	switch (e)
	{
		case ReferenceSpecifier::NONE :
			return std::string();
		case ReferenceSpecifier::POINTER :
			return POINTER;
		case ReferenceSpecifier::LVALUE_REFERENCE :
			return LVALUE_REFERENCE;
		case ReferenceSpecifier::RVALUE_REFERENCE :
			return RVALUE_REFERENCE;
		case ReferenceSpecifier::POINTER_TO_POINTER :
			return POINTER_TO_POINTER;
		case ReferenceSpecifier::POINTER_TO_POINTER_TO_POINTER :
			return POINTER_TO_POINTER_TO_POINTER;
		case ReferenceSpecifier::CONST_POINTER :
			return CONST_POINTER;
		default :
			return std::string();
	}
}

Cpp::StorageQualifier Cpp::convertStorageQualifierStringToEnumeration(const std::string& s)
{
	if(s == std::string())
		return StorageQualifier::NONE;
	if(s == STATIC)
		return StorageQualifier::STATIC;
	if(s == CONST)
		return StorageQualifier::CONST;
	if(s == CONSTEXPR)
		return StorageQualifier::CONSTEXPR;
	if(s == VOLATILE)
		return StorageQualifier::VOLATILE;
	if(s == REGISTER)
		return StorageQualifier::REGISTER;
	if(s == EXTERN)
		return StorageQualifier::EXTERN;
	if(s == THREAD_LOCAL)
		return StorageQualifier::THREAD_LOCAL;
	if(s == MUTABLE)
		return StorageQualifier::MUTABLE;
	if(s == AUTO)
		return StorageQualifier::AUTO;
	if(s == CONST_STATIC)
		return StorageQualifier::CONST_STATIC;
	if(s == STATIC_CONST)
		return StorageQualifier::CONST_STATIC;

	return StorageQualifier::NONE;
}

Cpp::TypeModifier Cpp::convertTypeModifierStringToEnumeration(const std::string& s)
{
	if(s == std::string())
		return TypeModifier::NONE;
	if(s == SIGNED)
		return TypeModifier::SIGNED;
	if(s == UNSIGNED)
		return TypeModifier::UNSIGNED;
	if(s == SHORT)
		return TypeModifier::SHORT;
	if(s == LONG)
		return TypeModifier::LONG;

	return TypeModifier::NONE;
}

Cpp::IntrinsicType Cpp::convertIntrinsicTypeStringToEnumeration(const std::string& s)
{
	if(s == VOID)
		return IntrinsicType::VOID;
	if(s == BOOL)
		return IntrinsicType::BOOL;
	if(s == CHAR)
		return IntrinsicType::CHAR;
	if(s == INT)
		return IntrinsicType::INT;
	if(s == FLOAT)
		return IntrinsicType::FLOAT;
	if(s == DOUBLE)
		return IntrinsicType::DOUBLE;
	if(s == SHORT_INT)
		return IntrinsicType::SHORT_INT;
	if(s == LONG_INT)
		return IntrinsicType::LONG_INT;
	if(s == LONG_DOUBLE)
		return IntrinsicType::LONG_DOUBLE;
	if(s == UNSIGNED_CHAR)
		return IntrinsicType::UNSIGNED_CHAR;
	if(s == UNSIGNED_INT)
		return IntrinsicType::UNSIGNED_INT;
	if(s == UNSIGNED_SHORT_INT)
		return IntrinsicType::UNSIGNED_SHORT_INT;
	if(s == UNSIGNED_LONG_INT)
		return IntrinsicType::UNSIGNED_LONG_INT;

	return IntrinsicType::NA;
}

Cpp::CodeGenType Cpp::convertCodeGenTypeStringToEnumeration(const std::string& s)
{
	if(s == REAL)
		return CodeGenType::REAL;
	if(s == FIXED)
		return CodeGenType::FIXED;

	return CodeGenType::NA;
}

Cpp::ReferenceSpecifier Cpp::convertReferenceSpecifierStringToEnumeration(const std::string& s)
{
	if(s == std::string())
		return ReferenceSpecifier::NONE;
	if(s == POINTER)
		return ReferenceSpecifier::POINTER;
	if(s == LVALUE_REFERENCE)
		return ReferenceSpecifier::LVALUE_REFERENCE;
	if(s == RVALUE_REFERENCE)
		return ReferenceSpecifier::RVALUE_REFERENCE;
	if(s == POINTER_TO_POINTER)
		return ReferenceSpecifier::POINTER_TO_POINTER;
	if(s == POINTER_TO_POINTER_TO_POINTER)
		return ReferenceSpecifier::POINTER_TO_POINTER_TO_POINTER;
	if(s == CONST_POINTER)
		return ReferenceSpecifier::CONST_POINTER;

	return ReferenceSpecifier::NONE;
}

} //namespace lblmc
