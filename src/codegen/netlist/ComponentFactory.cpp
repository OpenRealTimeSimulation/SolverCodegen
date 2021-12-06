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

#include "codegen/netlist/ComponentFactory.hpp"
#include <stdexcept>

namespace lblmc
{

ComponentFactory::ComponentFactory() :
	producer_registry()
{}

ComponentFactory::ComponentFactory(ComponentFactory&& base) :
	producer_registry(std::move(base.producer_registry))
{}

void ComponentFactory::registerComponentProducer(ComponentProducer* producer)
{
	if(producer == nullptr || producer == 0)
		throw std::invalid_argument("ComponentFactory::registerComponentProducer(ComponentProducer*) -- given pointer to producer cannot be null");

	producer_registry.erase(producer->getType());
	producer_registry[producer->getType()] = ComponentProducerPtr(producer);
}


void ComponentFactory::registerComponentProducer(ComponentProducerPtr&& producer)
{
	producer_registry.erase(producer->getType());
	producer_registry[producer->getType()] = ComponentProducerPtr(std::move(producer));
}

#if 0
void ComponentFactory::registerComponentProducer(std::string component_type, const ComponentProducer& producer)
{
	producer_registry.erase(component_type);
	producer_registry[component_type] = ComponentProducerPtr(producer.clone());
}
#endif


bool ComponentFactory::isComponentProducerRegistered(std::string component_type)
{
	const auto iter = producer_registry.find(component_type);

	return ( iter != producer_registry.end() );
}

const ComponentProducer* ComponentFactory::getComponentProducer(std::string component_type)
{
	try
	{
		return producer_registry.at(component_type).get();
	}
	catch(...)
	{
		throw std::out_of_range(std::string("ComponentFactory::getComponentProducer() -- producer doesn't exist for given component type: ")+component_type );
	}

}

ComponentFactory::ComponentPtr ComponentFactory::produceComponent(const ComponentListing& listing)
{
	const auto& producer = *(getComponentProducer(listing.getType()));

	return std::move(producer(listing));
}

} //namespace lblmc
