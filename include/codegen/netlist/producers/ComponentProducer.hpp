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

#ifndef LBLMC_COMPONENTPRODUCER_HPP
#define LBLMC_COMPONENTPRODUCER_HPP

#include <string>
#include <vector>
#include <memory>
#include <utility>
#include <stdexcept>

#include "codegen/components/Component.hpp"
#include "codegen/netlist/ComponentListing.hpp"

namespace lblmc
{

/**
	\brief pure abstract root class for objects that create code generators for LB-LMC components

	This class is unusable by itself.  It must be inherited from to be used.

	\brief Matthew Milton
	\date 2019
**/
class ComponentProducer
{
protected:

	std::string type;
	std::string producer_name;
	unsigned int num_parameters;
	unsigned int num_terminals;

public:

	ComponentProducer();

	ComponentProducer(const ComponentProducer& base);

	ComponentProducer(ComponentProducer&& base);

	const std::string& getType() const;

	virtual std::unique_ptr<Component> operator()(const ComponentListing& component_def) const = 0;

protected:


	bool isTypeValid(const ComponentListing& component_def) const;

	bool isLabelValid(const ComponentListing& component_def) const;

	bool isNumberOfParametersValid(const ComponentListing& component_def) const;

	bool isNumberOfTerminalsValid(const ComponentListing& component_def) const;

	void assertTypeValid(const ComponentListing& component_def) const;

	void assertLabelValid(const ComponentListing& component_def) const;

	void assertNumberOfParametersValid(const ComponentListing& component_def) const;

	void assertNumberOfTerminalsValid(const ComponentListing& component_def) const;

	void assertNetlistComponentInstanceValid(const ComponentListing& component_def) const;



};

} //namespace lblmc

#endif // LBLMC_COMPONENTPRODUCER_HPP
