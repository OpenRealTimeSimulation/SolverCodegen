/*

Copyright (C) 2019-2021 Matthew Milton

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

#include <memory>
#include <utility>

#include "codegen/netlist/ComponentFactory.hpp"

#include "codegen/netlist/producers/CapacitorProducer.hpp"
#include "codegen/netlist/producers/CurrentSourceProducer.hpp"
#include "codegen/netlist/producers/FunctionalCurrentSourceProducer.hpp"
#include "codegen/netlist/producers/FunctionalVoltageSourceProducer.hpp"
#include "codegen/netlist/producers/BridgeConverter3LegIdealSwitchesProducer.hpp"
#include "codegen/netlist/producers/InductorProducer.hpp"
#include "codegen/netlist/producers/MutualInductance3Producer.hpp"
#include "codegen/netlist/producers/ResistorProducer.hpp"
#include "codegen/netlist/producers/SeriesRLIdealSwitchProducer.hpp"
#include "codegen/netlist/producers/VoltageSourceProducer.hpp"
#include "codegen/netlist/producers/IdealVoltageSourceProducer.hpp"
#include "codegen/netlist/producers/IdealFunctionalVoltageSourceProducer.hpp"
#include "codegen/netlist/producers/NortonPortProducer.hpp"
#include "codegen/netlist/producers/VoltageControlledCurrentSourceProducer.hpp"
#include "codegen/netlist/producers/ModularMultilevelConverter_HalfBridgeModulesProducer.hpp"
#include "codegen/netlist/producers/BridgeConverter_1LegIdealSwitchesAntiParallelDiodesProducer.hpp"
#include "codegen/netlist/producers/BridgeConverter_3LegIdealSwitchesAntiParallelDiodesProducer.hpp"
#include "codegen/netlist/producers/DualActiveBridgeConverter_IdealSwitchesProducer.hpp"
#include "codegen/netlist/producers/ModularMultilevelConverter_1LegHalfBridgeAntiParallelDiodesProducer.hpp"

namespace lblmc
{

void ComponentFactory::registerBuiltinComponentProducers()
{
	registerComponentProducer( new CapacitorProducer() );
	registerComponentProducer( new CurrentSourceProducer() );
	registerComponentProducer( new FunctionalCurrentSourceProducer() );
	registerComponentProducer( new FunctionalVoltageSourceProducer() );
	registerComponentProducer( new BridgeConverter3LegIdealSwitchesProducer() );
    registerComponentProducer( new InductorProducer() );
    registerComponentProducer( new MutualInductance3Producer() );
    registerComponentProducer( new ResistorProducer() );
    registerComponentProducer( new SeriesRLIdealSwitchProducer() );
    registerComponentProducer( new VoltageSourceProducer() );
    registerComponentProducer( new IdealVoltageSourceProducer() );
    registerComponentProducer( new IdealFunctionalVoltageSourceProducer() );
    registerComponentProducer( new NortonPortProducer() );
    registerComponentProducer( new VoltageControlledCurrentSourceProducer() );
    registerComponentProducer( new ModularMultilevelConverter_HalfBridgeModulesProducer() );
    registerComponentProducer( new BridgeConverter_1LegIdealSwitchesAntiParallelDiodesProducer() );
    registerComponentProducer( new BridgeConverter_3LegIdealSwitchesAntiParallelDiodesProducer() );
    registerComponentProducer( new DualActiveBridgeConverter_IdealSwitchesProducer() );
    registerComponentProducer( new ModularMultilevelConverter_1LegHalfBridgeAntiParallelDiodesProducer() );
}

} //namespace lblmc
