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

#ifndef LBLMC_NETLIST_HPP
#define LBLMC_NETLIST_HPP

#include <vector>
#include <string>
#include <utility>

#include "codegen/netlist/ComponentListing.hpp"

namespace lblmc
{

/**
	\brief defines the netlist for a LB-LMC system model

	Objects of this class only store a definition of a netlist which is used by other systems to
	construct a LB-LMC system model implementation.

	\author Matthew Milton
	\date 2019
**/
class Netlist
{
private:

	std::string model_name; ///< name of the system model taken from netlist
	std::vector<ComponentListing> components; ///< netlist definitions of model components
	unsigned int num_nodes; ///< number of nodes in system model

public:

	/**
		\brief default constructor
	**/
	Netlist() :
		model_name(),
		components(),
		num_nodes(0)
	{}

	/**
		\brief copy constructor
	**/
	Netlist(const Netlist& base) :
		model_name(base.model_name),
		components(base.components),
		num_nodes(base.num_nodes)
	{}

	/**
		\brief move constructor
	**/
	Netlist(Netlist&& base) :
		model_name(std::move(base.model_name)),
		components(std::move(base.components)),
		num_nodes(std::move(base.num_nodes))
	{}

	Netlist& operator=(const Netlist& base)
	{
		model_name = base.model_name;
		components = base.components;
		num_nodes  = base.num_nodes;

        return *this;
	}

	Netlist& operator=(const Netlist&& base)
	{
		model_name = std::move(base.model_name);
		components = std::move(base.components);
		num_nodes  = std::move(base.num_nodes);

        return *this;
	}

	inline
	void setModelName(std::string mn)
	{
		model_name = mn;
	}

	inline
	void addComponent(const ComponentListing& comp)
	{
		for(const auto& term_conn : comp.getTerminalConnections())
		{
			if(term_conn > num_nodes)
			{
				num_nodes = term_conn;
			}
		}

		components.push_back(comp);
	}

	inline
	void addComponent(ComponentListing&& comp)
	{
		for(const auto& term_conn : comp.getTerminalConnections())
		{
			if(term_conn > num_nodes)
			{
				num_nodes = term_conn;
			}
		}

		components.push_back(comp);
	}

	inline
	const unsigned int& getNumberOfNodes() const
	{
		return num_nodes;
	}

	inline
	const std::string& getModelName() const
	{
		return model_name;
	}

	/**
		\brief gets netlist component at given zeroth index
		\param c zeroth index of where component is stored in netlist collection
		\return constant reference to the netlist component at index c
		\throw error if c is out of bounds
	**/
	inline
	const ComponentListing& getComponent(unsigned int c) const
	{
		return components.at(c);
	}

	inline
	const std::vector<ComponentListing>& getComponents() const
	{
		return components;
	}

	/**
		\brief gets number of components this netlist has
	**/
	inline
	unsigned int getComponentsCount() const
	{
		return components.size();
	}

	inline
	bool hasComponent(const std::string& component_label) const
	{
		for (auto& comp : components)
		{
			if(comp.getLabel() == component_label)
			{
				return true;
			}
		}

		return false;
	}
};

} //namespace lblmc

#endif // LBLMC_NETLIST_HPP
