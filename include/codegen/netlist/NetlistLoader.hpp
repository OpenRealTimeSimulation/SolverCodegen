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

#ifndef LBLMC_NETLISTLOADER_HPP
#define LBLMC_NETLISTLOADER_HPP

#include <string>
#include <vector>
#include <map>
#include <istream>

#include "codegen/netlist/Netlist.hpp"
#include "codegen/netlist/ComponentListing.hpp"

namespace lblmc
{


/**
	\brief Loads a LB-LMC Model Definition from a plain text netlist file or string

	\todo develop this class

	A netlist of a LB-LMC system model defines the components and network connections between these
	components that make up the model.

	The NetlistLoader requires the following the syntax rules for a given netlist:

	Each command, comment, and component definition in netlist must exist in a single line ending
	with newline.

	Comments are started by a % character.

	Commands are started by a # character.

	Components are started by type-name of component.

	The model in netlist must be given a name using command #name model_name which must be called
	before any other command or component definition, like so:
	<pre>
	#name rlc_circuit_model
	</pre>

	Model constants can be defined in the netlist using command #const const_name const_value, like
	so:
	<pre>
	#const DT 50.0e-9
	#const Resistance 10.0
	</pre>
	The name of constant cannot be a command or comment and must consist of alphanumerical and
	underscore characters.  Constants must be defined before components
	The constant is referenced in netlist by inserting the constant's name, like so:
	<pre>
	#const Res 100.0
	%...
	Resistor R1(Res) {2 3}
	</pre>
	The constant must be defined before it is used in netlist

	The syntax for a component in the name follows:
	<pre>
	component_type name (parameter list) { node indices }
	</pre>
	An example for a 2 terminal capacitor is:
	<pre>
	% 1st param is time step in seconds, 2nd param is capacitance in Farads
	Capacitor cap1 (10e-6, 10e-3) { 1, 2 }
	</pre>

	Indices must be positive from 0 onwards.  The index 0 indicates the system model's common/ground
	point.

**/
class NetlistLoader
{

public:

	NetlistLoader();
	NetlistLoader(const NetlistLoader& base) = delete;

	/**
		\brief loads a netlist from input stream
		\param strm input stream from where netlist is coming from;
		note that the given stream will be modified by this method
		\return Netlist object defining the netlist
		\throw throws error if netlist from stream is malformed
	**/
	Netlist loadFromStream(std::istream& strm);

	/**
		\brief loads a netlist from character text string
		\param netlist_str string containing the netlist
		\return Netlist object defining the netlist
		\throw throws error if netlist string is malformed
	**/
	Netlist loadFromString(const std::string& netlist_str);

	/**
		\brief loads a netlist from text file given by its filename
		\param filename file name of the netlist file
		\return Netlist object defining the netlist
		\throw throws error if file is malformed
	**/
	Netlist loadFromFile(const std::string& filename);

private:

	const static std::string WHITESPACE_CHARS;
	const static std::string BAD_START_CHARS;
	const static std::string VALID_NAME_CHARS;
	const static std::string VALID_NUMBER_CHARS;

	enum class LineType : int
	{
		ERROR     = -2,                // line is generally malformed (bad syntax)
		LINE_START_ERROR = -1,         // line is malformed with incorrect character (bad syntax)
		EMPTY     =  0,                // line is empty
		COMMENT   =  1,                // line is comment
		NAME      =  2,                // line is name command
		CONSTANT  =  3,                // line is constant command
		SUBSYSTEM =  4,                // line is subsystem command
		EXPOSE_COMPANION_ELEMENTS = 5, // line is expose companion elements command
		COMPONENT =  6                 // line is component definition
	};

	LineType checkLineType(const std::string& line, size_t& line_pos);
	std::string extractModelName(const std::string& line, const size_t& line_pos);
	std::string extractConstantValue(const std::string& line, const size_t& line_pos, std::string& name);
	ComponentListing extractComponent(const std::string& line, const std::map<std::string,std::string>& constants);

};

} //namespace lblmc

#endif // LBLMC_NETLISTLOADER_HPP
