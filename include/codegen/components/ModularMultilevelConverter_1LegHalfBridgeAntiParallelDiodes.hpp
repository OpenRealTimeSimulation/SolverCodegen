
/*

Copyright (C) 2019-2021 Matthew Milton, Michele Difronzo, Dhiman Chowdhury

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

#ifndef ORTIS_MODULARMULTILEVELCONVERTER_1LEGHALFBRIDGEANTIPARALLELDIODES_HPP
#define ORTIS_MODULARMULTILEVELCONVERTER_1LEGHALFBRIDGEANTIPARALLELDIODES_HPP

//==============================================================================================================================
//==============================================================================================================================

#include <string>
#include <vector>

#include "codegen/components/Component.hpp"

//==============================================================================================================================
//==============================================================================================================================

namespace lblmc
{

//==============================================================================================================================
//==============================================================================================================================

/**
	\brief Component code generator for a 1-leg Modular Multilevel Converter (MMC) with half-bridge switching modules using
	anti-parallel diodes across switches

	\author Matthew Milton, Michele Difronzo, Dhiman Chowdhury

	\date Created 2021.10.19

	Model ported over from ModularMultilevelConverter1Leg component of the LB-LMC Matlab toolbox.
**/
class ModularMultilevelConverter_1LegHalfBridgeAntiParallelDiodes : public Component
{

//==============================================================================================================================
// FIELDS

private:

	//parameters

	double dt; ///< time step length (sec) for discretization

	double csm; ///< capacitance of switching module capacitor

	double larm; ///< inductance of arm inductor

	double rarm; ///< series resistance of arm inductor

	double vcap_init; ///< initial voltage of switching module capacitors

	double iind_init; ///< initial current for arm inductors (positive to negative)

	double vth; ///< conduction threshold voltage for diodes

	double ith; ///< conduction threshold current for diodes

	unsigned int num_sm_arm; ///< number of switching modules per arm

	constexpr static double conductance = 1.0e-6; ///< small (high-Z) conductance across arms in companion model

	//node connection indices

	std::vector<Component::Terminal> terminals; ///< terminals of component (positive DC, negative DC, and leg/phase A terminals)

	//source contribution ids

	std::vector<Component::Source> sources; ///< companion sources for upper and lower arms

	//model data fields

	std::vector<Component::Field> parameters; ///< model input parameters

	std::vector<Component::Field> constants; ///< model internal constants

	std::vector<Component::Field> persistents; ///< model persistent/state fields

	std::vector<Component::Field> temporaries; ///< model temporary fields

	std::vector<Component::Field> signal_inputs; ///< model input signals

	std::vector<Component::Field> signal_outputs; ///< model output signals

	// enumerations

	/**
		conduction modes of a switching module
	**/
	enum SwitchingModuleConduction : unsigned char
	{
		OPEN      = 0, ///< module is open

		BYPASSED  = 1, ///< module is bypassed in arm

		INSERTED  = 2, ///< module capacitor is inserted into arm

		SHORT     = 3, ///< module is shorted out

		UNDECIDED = 4  ///< conduction unknown yet
	};

	/**
		conduction states of arm diodes in series
	**/
	enum DiodeConduction : unsigned char
	{
		NO_DIODES_CONDUCTING     = 0, ///< no arm diodes are conducting

		TOP_DIODES_CONDUCTING    = 1, ///< top diodes of modules are conducting

		BOTTOM_DIODES_CONDUCTING = 2 ///< bottom diodes of modules are conducting
	};

public:

//==============================================================================================================================
// CONSTRUCTORS

	ModularMultilevelConverter_1LegHalfBridgeAntiParallelDiodes(std::string comp_name);

	ModularMultilevelConverter_1LegHalfBridgeAntiParallelDiodes
	(
		std::string comp_name,
		double dt,
		unsigned int num_sm_arm,
		double csm,
		double larm,
		double rarm,
		double vcap_init,
		double iind_init,
		double vth,
		double ith
	);

	ModularMultilevelConverter_1LegHalfBridgeAntiParallelDiodes
	(
		const ModularMultilevelConverter_1LegHalfBridgeAntiParallelDiodes& base
	);

//==============================================================================================================================
// ACCESSORS

	inline
	std::string
	getType() const
	{
		return std::string
		(
			"ModularMultilevelConverter_1LegHalfBridgeAntiParallelDiodes"
		);
	}

	inline
	unsigned int
	getNumberOfTerminals() const
	{
		return terminals.size();
	}

	inline
	unsigned int
	getNumberOfSources() const
	{
		return sources.size();
	}

	inline
	void
	getSourceIds(std::vector<unsigned int>& ids) const
	{
		ids.clear();
		ids.push_back(sources[0].id);
		ids.push_back(sources[1].id);
	}

	inline
	void
	getResistiveCompanionElements(std::vector<ResistiveCompanionElement>& elements) const
	{
		elements.clear();
	}

	inline
	void
	setTerminalConnections
	(
		unsigned int p,
		unsigned int n,
		unsigned int a
	)
	{
		terminals[0].node_index = p;
		terminals[1].node_index = n;
		terminals[2].node_index = a;
	}

	inline
	void
	setTerminalConnections(const std::vector<unsigned int>& indices)
	{
		if(indices.size() < 3)
		{
			throw
			std::invalid_argument("ModularMultilevelConverter_1LegHalfBridgeAntiParallelDiodes::setTerminalConnections(.) -- argument indices must have at least 3 node indices");
		}

		terminals[0].node_index = indices[0];
		terminals[1].node_index = indices[1];
		terminals[2].node_index = indices[2];
	}

	inline
	void
	getTerminalConnections(std::vector<unsigned int>& term_ids) const
	{
        term_ids = {terminals[0].node_index, terminals[1].node_index, terminals[2].node_index};
	}

	inline
	void
	setParameters
	(
		std::string comp_name,
		double dt,
		unsigned int num_sm_arm,
		double csm,
		double larm,
		double rarm,
		double vcap_init,
		double iind_init,
		double vth,
		double ith
	)
	{
		this->comp_name = comp_name;

		if(comp_name.empty())
		{
			throw std::invalid_argument("ModularMultilevelConverter_1LegHalfBridgeAntiParallelDiodes::constructor(.): comp_name must be a valid, non-empty C++ label");
		}

		this->dt = dt;
		this->num_sm_arm = num_sm_arm;
		this->csm = csm;
		this->larm = larm;
		this->rarm = rarm;
		this->vcap_init = vcap_init;
		this->iind_init = iind_init;
		this->vth = vth;
		this->ith = ith;
	}

	inline
	void
	setIntegrationMethod(std::string method) {}

	inline
	std::string
	getIntegrationMethod() const { return std::string("euler_forward"); }

	inline
	std::vector<std::string>
	getSupportedOutputs() const
	{
		std::vector<std::string> ret
		{
			"lega_inductor_currents",
			"lega_capacitor_voltages"
		};

		return ret;
	}

//==============================================================================================================================
// NODAL STAMPING

	void
	stampConductance(SystemConductanceGenerator& gen);

	void
	stampSources(SystemSourceVectorGenerator& gen);

//==============================================================================================================================
// CODE GENERATORS

	std::string
	generateParameters();

	std::string
	generateFields();

	std::string
	generateInputs();

	std::string
	generateOutputs(std::string output = "ALL");

	std::string
	generateOutputsUpdateBody(std::string output="ALL");

	std::string
	generateUpdateBody();

//==============================================================================================================================

private:

	/**
		\brief sets up specific model fields for parameters, persistent/temp variables, signals I/O, and more
	**/
	void
	setupFields();

//==============================================================================================================================


};

//==============================================================================================================================
//==============================================================================================================================

} //namespace lblmc

#endif // ORTIS_MODULARMULTILEVELCONVERTER_1LEGHALFBRIDGEANTIPARALLELDIODES_HPP
