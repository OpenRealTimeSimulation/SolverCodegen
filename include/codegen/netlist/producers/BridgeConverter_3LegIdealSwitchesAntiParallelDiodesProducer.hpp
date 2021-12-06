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

#ifndef LBLMC_BRIDGECONVERTER_3LEGIDEALSWITCHESANTIPARALLELDIODESPRODUCER_HPP
#define LBLMC_BRIDGECONVERTER_3LEGIDEALSWITCHESANTIPARALLELDIODESPRODUCER_HPP

#include <string>
#include <vector>
#include <memory>

#include "codegen/components/Component.hpp"
#include "codegen/netlist/ComponentListing.hpp"
#include "codegen/netlist/producers/ComponentProducer.hpp"

namespace lblmc
{

class BridgeConverter_3LegIdealSwitchesAntiParallelDiodesProducer : public ComponentProducer
{

public:

	BridgeConverter_3LegIdealSwitchesAntiParallelDiodesProducer();
	BridgeConverter_3LegIdealSwitchesAntiParallelDiodesProducer(const BridgeConverter_3LegIdealSwitchesAntiParallelDiodesProducer& base);
	BridgeConverter_3LegIdealSwitchesAntiParallelDiodesProducer(BridgeConverter_3LegIdealSwitchesAntiParallelDiodesProducer&& base);

	std::unique_ptr<Component> operator()(const ComponentListing& component_def) const;
};

} //namespace lblmc

#endif // LBLMC_BRIDGECONVERTER_3LEGIDEALSWITCHESANTIPARALLELDIODESPRODUCER_HPP




