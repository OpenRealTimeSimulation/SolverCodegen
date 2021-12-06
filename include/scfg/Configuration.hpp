/*

Copyright (C) 2020 Matthew Milton

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


#ifndef SCFG_CONFIGURATION_HPP
#define SCFG_CONFIGURATION_HPP

#include <unordered_map>
#include <string>
#include <utility>
#include <stdexcept>
#include <memory>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <cstddef>

namespace ortis
{

/**
	\brief stores simple parameter-value pairs for use in configuration of applications

	\author Matthew Milton

	\date Created July 30, 2020

	In a Configuration object, a parameter is stored as a label-value pair where the label is a
	character string while value is variant type which can be boolean, integer, floating point, or
	character string type.
	Parameters can be easily added to Configuration via add methods and their values can be
	recalled via lookup	methods.
	Configuration objects are capable of loading their parameters from plain text files and storing
	their parameters into files as well.

	Configuration files loadable by Configuration objects are plain-text files with simple syntax.
	All comments in a configuration text file are single-line comments that start with // characters
	string and end with newline character(s).
	Each parameter is defined a label assigned to its value using this notation:\n
	label = value;\n
	Any amount of whitespace is acceptable between label, =, value, and ; strings.
	Labels have lax rules, supporting any combination of characters, so long as the label does not
	include whitespace and = characters.
	It is up to application using Configuration object to apply stricter rules to the labels.
	Value can either be true, false, integer literals, floating point literals, or character
	strings.
	Integer and floating point literals follow C/C++ literal definition rules with restrictions.
	Integers can be expressed in base10 or hex (prefixed with 0x) notation without decimal points
	nor exponential postfix forms (i.e. 1e6 or 5E2); sign/size characters (u, l, ll) are not
	allowed.
	Depending on range of the integer literal, it will be stored in smallest integer that fits it,
	from 32-bit integers to 64-bit long long integers.
	Floating points can be expressed in decimal form with or without exponential postfix notation
	, but type characters (f) are not allowed.
	All floating points are stored in 64-bit double-precision floating point (IEEE 754).
	Character strings are expressed as string of characters delimited on both ends by " character.
	Any characters in string that are special characters are started with \ (forward slash) escape
	character before actual character is entered, including " character (\") that does not delimit
	string.
	Boolean values are always either true or false in value.
	Examples of parameters defined in a config. file include the following:\n
	//some example parameters with comments
	bool_param = true;\n //bool parameter
	int_param = 10;\n    //integer parameter
	long_param = 25L;\n  //long integer parameter
	long_long_param = 33LL;\n //long long integer parameter
	double_param = 1234.56789;\n //double floating point parameter
	string_param = "a string value delimited by \\" characters ";\n //string parameter
	1aiosdfalskfjaslf.woriu!!! = false;\n //bool parameter with crazy label

**/
class Configuration
{

//==================================================================================================

public:

	/**
		\brief parameter value data types supported by configuration
	**/
	enum class ValueType : char
	{
		BOOL,           ///< boolean type
		INT,            ///< 32-bit integer type
		LONG,           ///< 32 or 64-bit integer type, depending on platform
		LONG_LONG,      ///< 64-bit integer type
		DOUBLE,         ///< 64-bit double-precision floating point type
		STRING          ///< string of 8-bit characters type
	};

//==================================================================================================

	/**
		\brief variant type to store a parameter value
	**/
	class Value
	{

	//----------------------------------------------------------------------------------------------

	private:


		ValueType type; ///< data type of the value

		union
		{
			bool as_bool;           ///< value as boolean
			int as_int;             ///< value as integer
			long as_long;           ///< value as long integer
			long long as_long_long; ///< value as long long integer
			double as_double;       ///< value as double-precision floating point
			std::string as_string;  ///< value as character string
		};

	//----------------------------------------------------------------------------------------------

	public:

		/**
			\brief default constructor

			Default constructed Value object will be of type BOOL with value of false

		**/
		Value()
			: type(ValueType::BOOL)
		{
			as_bool = false;
		}

		Value(bool v)
			: type(ValueType::BOOL)
		{
			as_bool = v;
		}

		Value(int v)
			: type(ValueType::INT)
		{
			as_int = v;
		}

		Value(long v)
			: type(ValueType::LONG)
		{
			as_long = v;
		}

		Value(long long v)
			: type(ValueType::LONG_LONG)
		{
			as_long_long = v;
		}

		Value(double v)
			: type(ValueType::DOUBLE)
		{
			as_double = v;
		}

		Value(const char* v)
			: type(ValueType::STRING)
		{
			new(&as_string) std::string(v);
		}

		Value(std::string v)
			: type(ValueType::STRING)
		{
			new(&as_string) std::string(v);
		}

		Value(std::string&& v)
			: type(ValueType::STRING)
		{
			new(&as_string) std::string(std::move(v));
		}

		Value(const Value& base)
			: type(base.type)
		{
			switch(type)
			{
				case ValueType::BOOL :
					as_bool = base.as_bool;
					break;
				case ValueType::INT :
					as_int = base.as_int;
					break;
				case ValueType::LONG :
					as_long = base.as_long;
					break;
				case ValueType::LONG_LONG :
					as_long_long = base.as_long_long;
					break;
				case ValueType::DOUBLE :
					as_double = base.as_double;
					break;
				case ValueType::STRING :
					new(&as_string) std::string(base.as_string);
					break;
			}
		}

		Value(Value&& base)
			: type(base.type)
		{
			switch(type)
			{
				case ValueType::BOOL :
					as_bool = base.as_bool;
					break;
				case ValueType::INT :
					as_int = base.as_int;
					break;
				case ValueType::LONG :
					as_long = base.as_long;
					break;
				case ValueType::LONG_LONG :
					as_long_long = base.as_long_long;
					break;
				case ValueType::DOUBLE :
					as_double = base.as_double;
					break;
				case ValueType::STRING :
					new(&as_string) std::string(std::move(base.as_string));
					break;
			}
		}

		~Value()
		{
			if(type == ValueType::STRING)
			{
				as_string.~basic_string<char>();
			}
		}

	//----------------------------------------------------------------------------------------------

		inline
		Value
		operator=(const Value& base)
		{
			if(type == ValueType::STRING && base.type == ValueType::STRING)
			{
				as_string = base.as_string;
				return *this;
			}

			if(type == ValueType::STRING)
			{
				as_string.~basic_string<char>();
			}

			switch(base.type)
			{
				case ValueType::BOOL :
					as_bool = base.as_bool;
					break;
				case ValueType::INT :
					as_int = base.as_int;
					break;
				case ValueType::LONG :
					as_long = base.as_long;
					break;
				case ValueType::LONG_LONG :
					as_long_long = base.as_long_long;
					break;
				case ValueType::DOUBLE :
					as_double = base.as_double;
					break;
				case ValueType::STRING :
					new(&as_string) std::string(base.as_string);
					break;
			}

			type = base.type;

			return *this;
		}

		inline
		Value
		operator=(Value&& base)
		{
			if(type == ValueType::STRING && base.type == ValueType::STRING)
			{
				as_string = std::move(base.as_string);
				return *this;
			}

			if(type == ValueType::STRING)
			{
				as_string.~basic_string<char>();
			}

			switch(base.type)
			{
				case ValueType::BOOL :
					as_bool = base.as_bool;
					break;
				case ValueType::INT :
					as_int = base.as_int;
					break;
				case ValueType::LONG :
					as_long = base.as_long;
					break;
				case ValueType::LONG_LONG :
					as_long_long = base.as_long_long;
					break;
				case ValueType::DOUBLE :
					as_double = base.as_double;
					break;
				case ValueType::STRING :
					new(&as_string) std::string(std::move(base.as_string));
					break;
			}

			type = base.type;

			return *this;
		}

	//----------------------------------------------------------------------------------------------

		inline
		operator std::string() const
		{
			switch(type)
			{
				case ValueType::STRING :

					return as_string;

				case ValueType::BOOL :

					return std::to_string(as_bool);

				case ValueType::INT :

					return std::to_string(as_int);

				case ValueType::LONG :

					return std::to_string(as_long);

				case ValueType::LONG_LONG :

					return std::to_string(as_long_long);

				case ValueType::DOUBLE :

					return std::to_string(as_double);
			}

			return std::string();
		}

		inline
		operator bool() const
		{
			if(type == ValueType::BOOL)
			{
				return as_bool;
			}

			throw
			std::runtime_error
			(
				"Configuration::Value::operator bool() const -- "
				"Value object is not a bool(ean)"
			);
		}

		inline
		operator int() const
		{
			if(type == ValueType::INT)
			{
				return as_int;
			}

			throw
			std::runtime_error
			(
				"Configuration::Value::operator int() const -- "
				"Value object is not an int(eger)"
			);
		}

		inline
		operator long() const
		{
			if(type == ValueType::LONG)
			{
				return as_long;
			}

			throw
			std::runtime_error
			(
				"Configuration::Value::operator long() const -- "
				"Value object is not a long integer"
			);
		}

		inline
		operator long long() const
		{
			if(type == ValueType::LONG_LONG)
			{
				return as_long_long;
			}

			throw
			std::runtime_error
			(
				"Configuration::Value::operator long long() const -- "
				"Value object is not a long long integer"
			);
		}

		inline
		operator double() const
		{
			if(type == ValueType::DOUBLE)
			{
				return as_double;
			}

			throw
			std::runtime_error
			(
				"Configuration::Value::operator double() const -- "
				"Value object is not a double floating point"
			);
		}

	//----------------------------------------------------------------------------------------------

		inline
		ValueType
		getType() const
		{
			return type;
		}

	//----------------------------------------------------------------------------------------------

	};

//==================================================================================================

	typedef std::unordered_map<std::string, Value> ParameterMap; ///< map type used to store parameter label-value pairs
	typedef std::unordered_map<std::string, Value>::value_type Parameter; ///< parameter label-value pair

	/**
		\brief gets constant static string of whitespace characters

		\return constant static string of whitespace characters

		This static method allows the nontrivial string to be initialized as constant static in
		header only design.
	**/
	static
	const std::string&
	getWhitespaceChars()
	{
		const static std::string WHITESPACE_CHARS(" \n\r\t\f\v");

		return WHITESPACE_CHARS;
	}

	const static char END_PARAMETER_CHAR = ';'; ///< semicolon ; character that ends parameter statements

	const static char ASSIGNMENT_CHAR = '='; ///< assignment = character that assigns parameter label to its value

	const static char STRING_DELIMITER_CHAR = '\"'; ///< quotes " character delimiter for string values

	const static char START_COMMENT_CHAR = '/'; ///< slash / character that is part of starting // string of comment statements

	const static char NEWLINE_CHAR = '\n'; ///< new end of line character \n

	const static char ESCAPE_CHAR = '\\'; ///< special slash escape character

//==================================================================================================

private:

	ParameterMap parameters; ///< map of parameters with their associated values in string form

//==================================================================================================

public:

	/**
		\brief default constructor
	**/
	Configuration()
		: parameters()
	{}

	/**
		\brief ParameterMap move constructor

		\param parameters Configuration::ParameterMap object whose contents will be moved to the
		constructed Configuration object.
	**/
	Configuration(ParameterMap&& parameters)
		: parameters(std::move(parameters))
	{}


	/**
		\brief copy constructor (deleted)
	**/
	Configuration(const Configuration& base) = delete;

	/**
		\brief move constructor

		\param base Configuration object whose contents will be moved to the constructed
		Configuration object.
	**/
	Configuration(Configuration&& base)
		: parameters(std::move(base.parameters))
	{}

//==================================================================================================

	/**
		\brief makes a copy of the Configuration object

		\return unique pointer to created copy of the this configuration object

		This method should be used sparingly due to cost of resource allocation and copying of
		Configuration objects, especially those with large number of parameters
	**/
	inline
	std::unique_ptr<Configuration>
	clone() const
	{
		std::unique_ptr<Configuration> clone_config = std::make_unique<Configuration>();

		clone_config->parameters = parameters;

		return clone_config;
	}

//==================================================================================================

	/**
		\brief adds a parameter to Configuration by moving label and value to it

		\param label string label of parameter, with label being moved to Configuration object

		\param value Configuration::Value value of the parameter, with value being moved to
		Configuration object
	**/
	inline
	void
	addParameter(std::string&& label, Value&& value)
	{
		parameters[label] = value;
	}

	/**
		\brief adds a parameter to Configuration by copying label and value to it

		\param label string label of parameter, with label copied to Configuration object

		\param value Configuration::Value value of the parameter, with value copied to
		Configuration object

		Use Configuration::addParameter(std::string&& label, Value&& value) if label and value
		are temporaries or initialized at usage (r-valued).
	**/
	inline
	void
	addParameter(const std::string& label, const Value& value)
	{
		parameters[label] = value;
	}

//==================================================================================================

	/**
		\brief checks if Configuration has a given parameter by its label

		\param label string label of parameter

		\return true if parameter exists, false otherwise
	**/
	inline
	bool
	containsParameter(const std::string& label) const
	{
		return bool(parameters.count(label));
	}

	/**
		\brief finds the value of a parameter by label, if parameter exists

		\param label string label of parameter

		\return observing raw pointer to value if parameter exists, otherwise nullptr.
		Check if returned pointer is not nullptr before dereferencing.
		Do not attempt to delete the value through returned pointer.
	**/
	inline
	const Value*
	findParameterValue(const std::string& label) const
	{
		if(containsParameter(label))
		{
			return &(parameters.at(label));
		}

		return nullptr;
	}

//==================================================================================================

	/**
		\brief gets value of parameter as a boolean

		\param label string label of parameter

		\param value object that will store the value

		\throw error if parameter does not exist or its value is not boolean
	**/
	inline
	void
	getValue(const std::string& label, bool& value) const
	{
		try
		{
			const auto& val = parameters.at(label);

			value = bool(val);
		}
		catch(const std::out_of_range& error)
		{
			throw
			std::out_of_range
			(
				"Configuration::getValue(const std::string& label, bool& value) -- "
				"a parameter does not exist for given label"
			);
		}
		catch(const std::runtime_error& error)
		{
			throw
			std::runtime_error
			(
				"Configuration::getValue(const std::string& label, bool& value) -- "
				"could not convert parameter value to bool"
			);
		}
	}

	/**
		\brief gets value of parameter as a 32-bit integer

		\param label string label of parameter

		\param value object that will store the value

		\throw error if parameter does not exist or its value is not integer
	**/
	inline
	void
	getValue(const std::string& label, int& value) const
	{
		try
		{
			const auto& val = parameters.at(label);

			value = int(val);
		}
		catch(const std::out_of_range& error)
		{
			throw
			std::out_of_range
			(
				"Configuration::getValue(const std::string& label, int& value) -- "
				"a parameter does not exist for given label"
			);
		}
		catch(const std::runtime_error& error)
		{
			throw
			std::runtime_error
			(
				"Configuration::getValue(const std::string& label, int& value) -- "
				"could not convert parameter value to int"
			);
		}
	}

	/**
		\brief gets value of parameter as a 32,64-bit long integer

		\param label string label of parameter

		\param value object that will store the value

		\throw error if parameter does not exist or its value is not long integer
	**/
	inline
	void
	getValue(const std::string& label, long& value) const
	{
		try
		{
			const auto& val = parameters.at(label);

			value = long(val);
		}
		catch(const std::out_of_range& error)
		{
			throw
			std::out_of_range
			(
				"Configuration::getValue(const std::string& label, long& value) -- "
				"a parameter does not exist for given label"
			);
		}
		catch(const std::runtime_error& error)
		{
			throw
			std::runtime_error
			(
				"Configuration::getValue(const std::string& label, long& value) -- "
				"could not convert parameter value to long"
			);
		}
	}

	/**
		\brief gets value of parameter as a 64-bit long long integer

		\param label string label of parameter

		\param value object that will store the value

		\throw error if parameter does not exist or its value is not long long integer
	**/
	inline
	void
	getValue(const std::string& label, long long& value) const
	{
		try
		{
			const auto& val = parameters.at(label);

			value = (long long)(val);
		}
		catch(const std::out_of_range& error)
		{
			throw
			std::out_of_range
			(
				"Configuration::getValue(const std::string& label, long long& value) -- "
				"a parameter does not exist for given label"
			);
		}
		catch(const std::runtime_error& error)
		{
			throw
			std::runtime_error
			(
				"Configuration::getValue(const std::string& label, long long& value) -- "
				"could not convert parameter value to long long"
			);
		}
	}

	/**
		\brief gets value of parameter as a 64-bit double-precision floating point

		\param label string label of parameter

		\param value object that will store the value

		\throw error if parameter does not exist or its value is not double floating point
	**/
	inline
	void
	getValue(const std::string& label, double& value) const
	{
		try
		{
			const auto& val = parameters.at(label);

			value = double(val);
		}
		catch(const std::out_of_range& error)
		{
			throw
			std::out_of_range
			(
				"Configuration::getValue(const std::string& label, double& value) -- "
				"a parameter does not exist for given label"
			);
		}
		catch(const std::runtime_error& error)
		{
			throw
			std::runtime_error
			(
				"Configuration::getValue(const std::string& label, double& value) -- "
				"could not convert parameter value to double"
			);
		}
	}

	/**
		\brief gets value of parameter as a character string

		\param label string label of parameter

		\param value object that will store the value.
		If value is a string type, it will be assigned as is.
		However, if value is another type, it will be converted into its string character
		representation before assignment.

		\throw error if parameter does not exist.
	**/
	inline
	void
	getValue(const std::string& label, std::string& value) const
	{
		try
		{
			const auto& val = parameters.at(label);

			value = std::string(val);
		}
		catch(const std::out_of_range& error)
		{
			throw
			std::out_of_range
			(
				"Configuration::getValue(const std::string& label, std::string& value) -- "
				"a parameter does not exist for given label"
			);
		}
	}

	/**
		\brief gets value of parameter as its raw variant representation

		\param label string label of parameter

		\param value object that will store the value

		\throw error if parameter does not exist
	**/
	inline
	void
	getValue(const std::string& label, Value& value) const
	{
		try
		{
			value = parameters.at(label);
		}
		catch(const std::out_of_range& error)
		{
			throw
			std::out_of_range
			(
				"Configuration::getValue(const std::string& label, Configuration::Value& value) -- "
				"a parameter does not exist for given label"
			);
		}
	}

//==================================================================================================

	/**
		\brief checks if given parameter label has correct format

		A parameter label is considered correct if it has no whitespace and no = characters
		in it.

		\param label parameter label to check

		\return true if label is syntax correct, false otherwise
	**/
	static inline
	bool
	isLabelCorrect(const std::string& label)
	{
		return (!label.empty())&&( label.find_first_of(" \n\r\t\f\v=") == std::string::npos);
	}


//==================================================================================================

	/**
		\brief stores configuration parameters to plain-text character string

		\return string containing plain-text expressions of configuration parameters
		Note that the parameters will not be stored in any particular order in string.

		\throw std::runtime_error if parameters have syntax errors
		in them.
	**/
	inline
	std::string
	storeToString()
	{
		std::stringstream strm;
		strm << std::setprecision(15);

		strm << "// Automatically Generated by config::Configuration\n\n";

		for(const auto& parameter : parameters)
		{
			if(!isLabelCorrect(parameter.first))
			{
				throw
				std::runtime_error
				(
					"Configuration::storeToString() -- "
					"a parameter label has incorrect format: "
					+ parameter.first
				);
			}

			strm << parameter.first << " = ";

			switch(parameter.second.getType())
			{
				case ValueType::BOOL :
					strm << ( bool(parameter.second) ? "true" : "false");
					break;
				case ValueType::INT :
					strm << int(parameter.second);
					break;
				case ValueType::LONG :
					strm << long(parameter.second);
					break;
				case ValueType::LONG_LONG :
					strm << (long long)(parameter.second);
					break;
				case ValueType::DOUBLE :
					strm << std::fixed;
					strm << double(parameter.second);
					strm.unsetf(std::stringstream::fixed);
					break;
				case ValueType::STRING :
					strm << ( "\"" + std::string(parameter.second) + "\"" );
					break;
			}

			strm << ";\n";
		}

		return strm.str();
	}

	/**
		\brief stores configuration parameters to plain-text file

		\param filename name of file where parameters will be stored as plain-text format.
		The file contents will be erased before being written into.
		Note that the parameters will not be stored in any particular order in file.

		\throw std::runtime_error if fails to open/write file, or parameters have syntax errors
		in them.
	**/
	inline
	void
	storeToFile(const std::string& filename)
	{
		std::ofstream file(filename, std::ofstream::out|std::ofstream::trunc);

		if(!file.good())
		{
			throw
			std::runtime_error
			(
				"Configuration::storeToFile(const std::string& filename) -- "
				"failed to open file"
			);
		}

		file << std::setprecision(15);

		file << "// Automatically Generated by config::Configuration\n\n";

		for(const auto& parameter : parameters)
		{
			if(!isLabelCorrect(parameter.first))
			{
				throw
				std::runtime_error
				(
					"Configuration::storeToFile(const std::string& filename) -- "
					"a parameter label has incorrect format: "
					+ parameter.first
				);
			}

			file << parameter.first << " = ";

			switch(parameter.second.getType())
			{
				case ValueType::BOOL :
					file << ( bool(parameter.second) ? "true" : "false");
					break;
				case ValueType::INT :
					file << int(parameter.second);
					break;
				case ValueType::LONG :
					file << long(parameter.second);
					break;
				case ValueType::LONG_LONG :
					file << (long long)(parameter.second);
					break;
				case ValueType::DOUBLE :
					file << std::fixed;
					file << double(parameter.second);
					file.unsetf(std::fstream::fixed);
					break;
				case ValueType::STRING :
					file << ( "\"" + std::string(parameter.second) + "\"" );
					break;
			}

			file << ";\n";
		}

		file.close();
	}

	/**
		\brief loads configuration parameters from character string containing plain-text expression
		of parameters.

		Existing parameters stored in Configuration will be deleted after string has been parsed
		successfully without syntax errors.
		If string is empty, then all existing parameters are deleted.

		\param config character string containing plain-text parameter expressions.

		\throw std::invalid_argument if string has syntax errors

	**/
	inline
	void
	loadFromString(const std::string& config)
	{
		ParameterMap new_parameters;

		const static std::string WHITESPACE_CHARS = getWhitespaceChars();

		enum class State {START, LABEL, VALUE, COMMENT, STRING};

		/*
			START state -- start of FSM.
			Decides to go to LABEL or COMMENT state without moving forward.

			LABEL state -- extracts label from source string.
			Goes to VALUE state if encounters assignment character =

			VALUE state -- extracts value from source string.
			Goes to STRING state if start string delimiter " is encountered.
			Goes to START state if encounters end of statement character ;
			Also, this state evaluates the parameter-value statement and adds parameter to
			configuration if end of statement character is reached.

			STRING state -- extracts string value from source string.
			Goes back to VALUE state if encounters un-escaped " character (ending string delimiter).

			COMMENT state -- extracts and discards comment lines that start with // characters.
			Goes to START state if reaches end of line.
		*/

		State curr_state = State::START;
		State last_state = curr_state;
		State next_state = curr_state;

		char curr_char = 0;
		char last_char = 0;

		std::size_t curr_line = 0;

		std::string label, value;

		auto iter = config.begin();

		while(iter != config.end())
		{
			curr_state = next_state;

			curr_char = *iter;

			if(curr_char == NEWLINE_CHAR)
			{
				curr_line++;
			}

			switch(curr_state)
			{
				//----------------------------------------------------------------------------------
				case State::START:
				{
					auto next_iter = iter+1;

					if(next_iter == config.end())
					{
						iter++;
						break;
					}

					if(curr_char == START_COMMENT_CHAR && (*next_iter) == START_COMMENT_CHAR)
					{
						//go to comment state

						next_state = State::COMMENT;
						last_state = curr_state;
					}
					else if(WHITESPACE_CHARS.find_first_of(curr_char) != std::string::npos)
					{
						//ignore whitespace characters

						iter++;
					}
					else
					{
						//go to label state

						next_state = State::LABEL;
						last_state = curr_state;
					}

					break;
				}
				//----------------------------------------------------------------------------------
				case State::LABEL:
				{
					auto next_iter = iter+1;

					if(next_iter == config.end())
					{
						throw std::invalid_argument
						(
							"Configuration::loadFromString(const std::string& config) -- "
							"line " +
							std::to_string(curr_line)+
							": "
							"Parameter label ended prematurely without assignment operator (=)"
						)
						;
					}

					if
					(
						curr_char == START_COMMENT_CHAR    &&
						next_iter != config.end()          &&
						(*next_iter) == START_COMMENT_CHAR
					)
					{
						throw std::invalid_argument
						(
							"Configuration::loadFromString(const std::string& config) -- "
							"line " +
							std::to_string(curr_line)+
							": "
							"Parameter label ended prematurely into comment"
						)
						;
					}

					if(curr_char == END_PARAMETER_CHAR)
					{
						throw std::invalid_argument
						(
							"Configuration::loadFromString(const std::string& config) -- "
							"line " +
							std::to_string(curr_line)+
							": "
							"Parameter label-value assignment ended prematurely at ;"
						)
						;
					}

					if
					(
						WHITESPACE_CHARS.find_first_of(curr_char) == std::string::npos &&
						WHITESPACE_CHARS.find_first_of(last_char) != std::string::npos &&
						curr_char != ASSIGNMENT_CHAR
					)
					{
						throw std::invalid_argument
						(
							"Configuration::loadFromString(const std::string& config) -- "
							"line " +
							std::to_string(curr_line)+
							": "
							"Parameter label has possible whitespace within it"
						)
						;
					}

					if(WHITESPACE_CHARS.find_first_of(curr_char) == std::string::npos && curr_char != ASSIGNMENT_CHAR)
					{
						//extract label character

						label += curr_char;
					}

					if(curr_char == ASSIGNMENT_CHAR)
					{
						//go to value state

						next_state = State::VALUE;
						last_state = curr_state;
					}

					iter++;
					break;
				}
				//----------------------------------------------------------------------------------
				case State::VALUE:
				{
					auto next_iter = iter+1;

					if(next_iter == config.end())
					{
						throw std::invalid_argument
						(
							"Configuration::loadFromString(const std::string& config) -- "
							"line " +
							std::to_string(curr_line)+
							": "
							"Parameter value ended prematurely without semicolon (;)"
						)
						;
					}

					if
					(
						curr_char == START_COMMENT_CHAR    &&
						next_iter != config.end()          &&
						(*next_iter) == START_COMMENT_CHAR
					)
					{
						throw std::invalid_argument
						(
							"Configuration::loadFromString(const std::string& config) -- "
							"line " +
							std::to_string(curr_line)+
							": "
							"Parameter value ended prematurely into comment"
						)
						;
					}

					if
					(
						WHITESPACE_CHARS.find_first_of(curr_char) == std::string::npos &&
						curr_char != STRING_DELIMITER_CHAR &&
						curr_char != END_PARAMETER_CHAR
					)
					{
						//extract value character

						value += curr_char;
					}

					if(curr_char == STRING_DELIMITER_CHAR)
					{
						//go to string value state

						next_state = State::STRING;
						last_state = curr_state;
					}
					else if(curr_char == END_PARAMETER_CHAR)
					{

						//evaluate extracted parameter label and value pair

						if(last_state == State::STRING) //string
						{
							new_parameters[label] = Value(value);
						}
						else if //boolean true
						(
							value == "true" ||
							value == "TRUE"
						)
						{
							new_parameters[label] = Value(bool(true));
						}
						else if //boolean false
						(
							value == "false" ||
							value == "FALSE"
						)
						{
							new_parameters[label] = Value(bool(false));
						}
						else //integral or floating point values
						{
							Value num_value;
							std::size_t pos = 0;

								//check floating point

							if(value.find_first_of(".eE") != std::string::npos)
							{
								try
								{
									num_value = std::stod(value, &pos);

									if(pos != value.size())
									{
										throw
										std::invalid_argument
										(
											"Configuration::loadFromString(const std::string& config) -- "
											"line " +
											std::to_string(curr_line)+
											": "
											"value of parameter " +
											label +
											" could not be converted into numerical value"
										);
									}
								}
								catch(const std::out_of_range& error)
								{
									throw
									std::out_of_range
									(
										"Configuration::loadFromString(const std::string& config) -- "
										"line " +
										std::to_string(curr_line)+
										": "
										"value of parameter " +
										label +
										" could not be converted into numerical value without precision loss"
									);
								}
								catch(const std::invalid_argument& error)
								{
									throw
									std::invalid_argument
									(
										"Configuration::loadFromString(const std::string& config) -- "
										"line " +
										std::to_string(curr_line)+
										": "
										"value of parameter " +
										label +
										" could not be converted into numerical value"
									);
								}
							}
								//check integer
							else
							{
									// !!! Using nested try catch this way to size fit integer with
									// str->int functions is messy crude way of doing it.
									// -- MM 2020-07-31-22.37
								try
								{
									num_value = std::stoi(value, &pos);

									if(pos != value.size())
									{
										throw
										std::invalid_argument
										(
											"Configuration::loadFromString(const std::string& config) -- "
											"line " +
											std::to_string(curr_line)+
											": "
											"value of parameter " +
											label +
											" could not be converted into numerical value"
										);
									}
								}
								catch(const std::invalid_argument& error)
								{
									throw
									std::invalid_argument
									(
										"Configuration::loadFromString(const std::string& config) -- "
										"line " +
										std::to_string(curr_line)+
										": "
										"value of parameter " +
										label +
										" could not be converted into numerical value"
									);
								}
								catch(const std::out_of_range& error)
								{

									try
									{
										num_value = std::stol(value, &pos);
									}
									catch(const std::out_of_range& error)
									{
										try
										{
											num_value = std::stoll(value, &pos);
										}
										catch(const std::out_of_range& error)
										{
											throw
											std::out_of_range
											(
												"Configuration::loadFromString(const std::string& config) -- "
												"line " +
												std::to_string(curr_line)+
												": "
												"value of parameter " +
												label +
												" could not be converted into numerical value without precision loss"
											);
										}
									}


								}

							}

							new_parameters[label] = num_value;
						}

						label.clear();
						value.clear();

						auto next_iter = iter+1;

						if(next_iter != config.end())
						{
							next_state = State::START;
							last_state = curr_state;
						}
					}

					iter++;
					break;
				}
				//----------------------------------------------------------------------------------
				case State::COMMENT:
				{
					if(curr_char == NEWLINE_CHAR)
					{
						//go to start state

						next_state = State::START;
						last_state = curr_state;
					}

					iter++;
					break;
				}
				//----------------------------------------------------------------------------------
				case State::STRING:
				{
					if
					(
						!(curr_char == STRING_DELIMITER_CHAR && last_char != ESCAPE_CHAR)
					)
					{
						//extract string value character

						value += curr_char;
					}

					if(curr_char == STRING_DELIMITER_CHAR && last_char != ESCAPE_CHAR)
					{
						//go back to value state

						next_state = State::VALUE;
						last_state = curr_state;
					}

					iter++;
					break;
				}
			}

			last_char = curr_char;
		}

		parameters = new_parameters;
	}

	/**
		\brief loads configuration parameters from plain-text file

		Existing parameters stored in Configuration will be deleted after file has been parsed
		successfully without syntax errors.
		If file is empty, then all existing parameters are deleted.

		\param filename name of plain-text file where from parameters will be loaded.
		The file contents will be left untouched.

		\throw std::runtime_error if fails to open/read file.
		\throw std::invalid_argument if filename is empty

		\note this method loads the entire contents of a file into memory before parsing it.
		As such, opening massive text files (>1GB) with this method is ill-advised unless memory
		is available.
	**/
	inline
	void
	loadFromFile(const std::string& filename)
	{
		if(filename.empty())
		{
			throw std::invalid_argument
			(
				"Configuration::loadFromFile(const std::string& filename) --"
				"filename cannot be empty"
			);
		}

		std::ifstream file(filename, std::ifstream::in|std::ifstream::binary);

		if(!file.good())
		{
			throw std::runtime_error
			(
				"Configuration::loadFromFile(const std::string& filename) -- "
				"failed to open/read file"
			);
		}

		file.seekg(0, std::ifstream::end);

		std::size_t file_size = file.tellg();

		std::string buffer(file_size, 0);

		file.seekg(0);

		file.read(&buffer[0], file_size);

		try
		{
			loadFromString(buffer);
		}
		catch(const std::exception& error)
		{
			throw std::runtime_error
			(
				"Configuration::loadFromFile(const std::string& filename) -- "
				"error parsing file \""+
				filename+
				"\" -- "+
				error.what()
			);
		}
	}
};

} //namespace ortis

#endif // SCFG_CONFIGURATION_HPP
