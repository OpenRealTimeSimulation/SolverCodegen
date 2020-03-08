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

#ifndef LBLMC_COMPONENTFACTORY_HPP
#define LBLMC_COMPONENTFACTORY_HPP

#include <vector>
#include <memory>
#include <utility>
#include <string>
#include <unordered_map>
#include <stdexcept>

#include "codegen/components/Component.hpp"
#include "codegen/netlist/producers/ComponentProducer.hpp"
#include "codegen/netlist/ComponentListing.hpp"

namespace lblmc
{

/**
	\brief constructs LB-LMC component code generators from given component type and parameters

	Component (code generator) factories construct component code generators from given component
	parameters and type, then returning an observing pointer to the constructed object.
	These factories do not construct the generator objects themselves, instead using
	ComponentProducer objects to constructor them, which are registered under a factory under the
	component type.
	This allows the factories to be flexible and support new component generators as they are
	created without modifying the factory class.

	\author Matthew Milton
	\date 2019
**/
class ComponentFactory
{

public:

	typedef std::unique_ptr<Component> ComponentPtr;
	typedef std::unique_ptr<ComponentProducer> ComponentProducerPtr;
	typedef std::unordered_map<std::string, ComponentProducerPtr> ComponentProducerRegistry;

private:

	ComponentProducerRegistry producer_registry;  ///< registry that maps a component type to producer of its component generator

public:

	/**
		\brief default constructor
	**/
	ComponentFactory();
	/**
		\brief copy constructor (deleted)

		copying of these objects are not allowed.
	**/
	ComponentFactory(const ComponentFactory& base) = delete;

	/**
		\brief move constructor

		\param base factory whose contents will be moved to the constructed object.  Contents
		of base might be empty afterward.
	**/
	ComponentFactory(ComponentFactory&& base);

	/**
		\brief registers a component (code generator) producer

		\param producer raw pointer to producer object that will construct component code generators
		of of type component_type.  The object pointed to should be allocated onto heap memory
		using new operator so as to avoid out of scope errors.
		If another producer is registered under same component type, it will be replaced by this new
		producer.
		Note that the factory will take ownership of the producer so nothing else should delete the
		producer.
	**/
	void registerComponentProducer(ComponentProducer* producer);

	/**
		\brief registers a component (code generator) producer to given type name

		\param producer unique pointer to producer object that will construct component code
		generators of of type component_type.  The producer object pointed to should be allocated
		on the heap memory using new operator or std::make_unique<>() function.
		If another producer is registered under same component type, it will be replaced by this new
		producer.
		Note that the factory will take ownership of the producer so nothing else should delete the
		producer.  The unique_ptr passed will be nulled after this method call.
	**/
	void registerComponentProducer(ComponentProducerPtr&& producer);

	#if 0
	/**
		\brief registers a component (code generator) producer to given type name

		\param component_type component type as string that producer will be registered under.
		\param producer constant reference to object that will construct component code generators
		of of type component_type.
		If another producer is registered under component_type, it will be replaced by this new
		producer.
		Note that the factory will create a copy of the producer and leave the original alone.
	**/
	void registerComponentProducer(std::string component_type, const ComponentProducer& producer);
	#endif

	/**
		\brief registers component generator producers that are builtin to the codegen library
	**/
	void registerBuiltinComponentProducers();

	/**
		\brief checks if a component producer is registered under given name
		\param component_type name of the component producer to check
		\return true if producer registered under given name; false otherwise
	**/
	bool isComponentProducerRegistered(std::string component_type);

	/**
		\brief gets the component code generator producer from given type name

		Call isComponentProducerRegistered(std::string) before calling this method to avoid
		runtime error if producer does not exist.

		\param component_type type name that producer is registered under
		\return constant raw observing pointer to producer if it exists.  The
		returned pointer should not be used to delete the producer.
		\throw error if producer doesn't exist
	**/
	const ComponentProducer* getComponentProducer(std::string component_type);

	/**
		\brief produces a component code generator object based on given component listing in netlist
		\param listing ComponentListing listing describing component for which the created generator will create.
		\return unique pointer that owns the component code generator that is produced by this method.
		\throw error if producer doesn't exist for given listing, or listing is malformed for producer.
	**/
	ComponentPtr produceComponent(const ComponentListing& listing);

};

} //namespace lblmc

#endif // LBLMC_COMPONENTFACTORY_HPP
