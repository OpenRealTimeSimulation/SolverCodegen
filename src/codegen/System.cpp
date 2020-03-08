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

#include "codegen/System.hpp"
#include "codegen/components/Component.hpp"
#include "codegen/SimulationEngineGenerator.hpp"

#include <algorithm>
#include <stdexcept>
#include <utility>

namespace lblmc
{

System::System(std::string model_name) :
	name(model_name),
	num_nodes(0),
	num_ideal_voltage_sources(0),
	components()
{

	if(model_name == std::string())
	{
		throw std::invalid_argument("System::constructor -- model_name cannot be null");
	}

}

System::System(System&& base) :
	name(std::move(base.name)),
	num_nodes(std::move(base.num_nodes)),
	num_ideal_voltage_sources(std::move(base.num_ideal_voltage_sources)),
	components(std::move(base.components))
{}

const std::string& System::getName() const
{
	return name;
}

const unsigned int& System::getNumberOfNodes() const
{
	return num_nodes;
}

const unsigned int& System::getNumberOfIdealVoltageSources() const
{
	return num_ideal_voltage_sources;
}

unsigned int System::getNumberOfSolutions() const
{
	return num_nodes + num_ideal_voltage_sources;
}

unsigned int System::getNumberOfComponents() const
{
	return components.size();
}

Component& System::addComponent(Component* component)
{
	if(component == nullptr)
		throw std::invalid_argument("System::addComponent(Component*): component cannot be null or nonexistent");

	components.push_back(std::unique_ptr<Component>{component});

	unsigned int largest_node_index = components.back()->getLargestTerminalConnectionIndex();
    if(largest_node_index >= num_nodes)
	{
		num_nodes = largest_node_index;
	}

	num_ideal_voltage_sources += components.back()->getNumberOfIdealVoltageSources();

	return *(components.back());
}


Component& System::addComponent(std::unique_ptr<Component>& component)
{
	if(component == nullptr)
		throw std::invalid_argument("System::addComponent(std::unique_ptr<Component>&): component cannot be null or nonexistent");

	components.push_back(std::move(component));

	unsigned int largest_node_index = components.back()->getLargestTerminalConnectionIndex();
    if(largest_node_index >= num_nodes)
	{
		num_nodes = largest_node_index;
	}

	num_ideal_voltage_sources += components.back()->getNumberOfIdealVoltageSources();

	return *(components.back());
}

Component& System::addComponent(std::unique_ptr<Component>&& component)
{
	if(component == nullptr)
		throw std::invalid_argument("System::addComponent(std::unique_ptr<Component>&): component cannot be null or nonexistent");

	components.push_back(std::move(component));

	unsigned int largest_node_index = components.back()->getLargestTerminalConnectionIndex();
    if(largest_node_index >= num_nodes)
	{
		num_nodes = largest_node_index;
	}

	num_ideal_voltage_sources += components.back()->getNumberOfIdealVoltageSources();

	return *(components.back());
}

Component* System::getComponent(std::string name)
{
	auto iter = components.begin();

	while(iter != components.end())
	{
		if( (*iter)->getName() == name )
			return (*iter).get();
		else
			iter++;
	}

	return nullptr;
}

const Component* System::getComponent(std::string name) const
{
	auto iter = components.begin();

	while(iter != components.end())
	{
		if( (*iter)->getName() == name )
			return (*iter).get();
		else
			iter++;
	}

	return nullptr;
}

const std::vector<std::unique_ptr<Component>>& System::getComponents() const
{
	return components;
}

bool System::areComponentNamesUnique() const
{
	for(auto i = components.begin(); i != components.end(); i++)
	{
		for(auto j = i+1; j != components.end(); j++)
		{
            if( (*i)->getName() == (*j)->getName() )
				return false;
		}
	}

	return true;
}

} //namespace lblmc
