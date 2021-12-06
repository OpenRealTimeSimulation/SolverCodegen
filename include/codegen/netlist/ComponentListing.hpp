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

#ifndef LBLMC_COMPONENTLISTING_HPP
#define LBLMC_COMPONENTLISTING_HPP

#include <vector>
#include <string>
#include <utility>

namespace lblmc
{

/**
	\brief defines an instance of a component within a LB-LMC system model netlist

	Objects of this class only store the netlist definition of a component which is used by other
	systems to construct a LB-LMC system model implementation.

	\see NetlistLoader for loader that loads these definitions from a netlist file
	\see Netlist for class used to store netlists of system models using this class
	\author Matthew Milton
	\date 2019
**/
class ComponentListing
{
private:

	std::string type; ///< type of component
	std::string label; ///< label of the component
	std::vector<double> parameters; ///< list of parameters for component
	std::vector<unsigned int> terminal_connections; ///< list of node network connections

public:

	/**
		\brief default constructor
	**/
	ComponentListing();

	/**
		\brief copy constructor
	**/
	ComponentListing(const ComponentListing& base);

	/**
		\brief move constructor
	**/
	ComponentListing(ComponentListing&& base);

	/**
		\brief parameter constructor
	**/
	ComponentListing
	(
		std::string& type,
		std::string& label,
		std::vector<double>& parameters,
		std::vector<unsigned int>& terminal_connections
	);

	/**
		\brief parameter constructor with move semantics
	**/
	ComponentListing
	(
		std::string&& type,
		std::string&& label,
		std::vector<double>&& parameters,
		std::vector<unsigned int>&& terminal_connections
	);

	/**
		\brief parameter constructor

		This constructor takes a netlist line for a component as a string to construct this object.

		\see setFromNetlistLine(const std::string&) for syntax of string

		\param listing string storing netlist for a component listing
	**/
	explicit ComponentListing(const std::string& listing);

	ComponentListing& operator=(const ComponentListing& base);

	/**
		\brief sets listing object from component listing line taken from a plaintext netlist
		\param listing string storing netlist for a component listing

		The syntax for a component line is as follows:\n
		ComponentType label(param1, param2, ..., paramP) {node1, node2, ..., nodeN}

		An example for a capacitor (DT=50ns, C=300mF; on nodes 1 and 2) is:\n
		Capacitor cap_1(50e-9, 300e-3) { 1, 2 }

		A future version of the component line syntax may support code generation options in
		square brackets.

	**/
	void setFromNetlistLine(const std::string& listing);

	void setType(std::string t);

	void setLabel(std::string l);

	/**
		\brief (re)sets parameters to new ones given

		\param p the vector of parameters to set from.  The argument p is not altered
		by this method.

		\see setParameters(std::vector<unsigned int>&& p) for faster set method that
		moves contents of p to this object which may empty p.
	**/
	void setParameters(const std::vector<double>& p);

	/**
		\brief (re)sets parameters to new ones given

		\param p the vector of parameters to set from.  Note that the contents
		of p will be moved to this object, where p may become empty afterward.

		\see setParameters(const std::vector<unsigned int>& p) for set method that
		does not alter the argument p.
	**/
	void setParameters(std::vector<double>&& p);

	void addParameter(double p);

	/**
		\brief resets terminal connections to new ones given

		\param tc the vector of terminal connections to set from.  The argument tc is not altered
		by this method.

		\see setTerminalConnections(std::vector<unsigned int>&& tc) for faster set method that
		moves contents of tc to this object which may empty tc.
	**/
	void setTerminalConnections(const std::vector<unsigned int>& tc);

	/**
		\brief resets terminal connections to new ones given

		\param tc the vector of terminal connections to set from.  Note that the contents
		of tc will be moved to this object, where tc may become empty afterward.

		\see setTerminalConnections(const std::vector<unsigned int>& tc) for set method that
		does not alter the argument tc.
	**/
	void setTerminalConnections(std::vector<unsigned int>&& tc);

	void addTerminalConnection(unsigned int tc);

	const std::string& getType() const;

	const std::string& getLabel() const;

	/**
		\brief gets parameter at given zeroth index
		\param p zeroth index of where parameter is stored in component collection
		\return constant reference to the parameter at index p
		\throw error if p is out of bounds
	**/
	double getParameter(unsigned int p) const;

	const std::vector<double>& getParameters() const;

	/**
		\brief gets the number of parameters component has
	**/
	unsigned int getParametersCount() const;

	/**
		\brief gets terminal connection index at given zeroth position
		\param tc zeroth position of where terminal connection index is stored in component collection
		\return constant reference to the terminal connection index at position tc
		\throw error if tc is out of bounds
	**/
	unsigned int getTerminalConnection(unsigned int tc) const;

	const std::vector<unsigned int>& getTerminalConnections() const;

	/**
		\brief gets the number of terminal connections component has
	**/
	unsigned int getTerminalConnectionsCount() const;

};

} //namespace lblmc

#endif // LBLMC_NETLISTCOMPONENT_HPP
