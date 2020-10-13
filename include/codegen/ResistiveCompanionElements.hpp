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

#ifndef LBLMC_RESISTIVECOMPANIONELEMENTS_HPP
#define LBLMC_RESISTIVECOMPANIONELEMENTS_HPP

namespace lblmc
{

/**
	\brief Describes a basic resistive companion element for a Component
	(current source with parallel conductance (or ideal voltage source) and associated id and name)

	\deprecated This class is being phased out in use
**/
class ResistiveCompanionElement
{

public:

	std::string name;             ///< unique name of element; default is ""
	unsigned int p;               ///< terminal connection / node index for positive terminal of element; default is 0
	unsigned int n;               ///< terminal connection / node index for negative terminal of element; default is 0
	double fixed_source_value;    ///< element's source value if source is fixed (otherwords, not functional); default is 0.0
	double parallel_conductance;  ///< parallel conductance of the element if present; default is 0.0
	unsigned int source_id;       ///< unique id of element's source in a system; default is 0
	bool is_fixed;                ///< indicates if fixed or functional source in element (true for fixed, false for functional); default is false
	bool has_no_source;           ///< indicates that element has no source (for purely resistive elements); default is false
	bool is_ideal_voltage_source;  ///< indicates that element is an ideal voltage source (instead of current source); default is false

	ResistiveCompanionElement() :
		name(), p(0), n(0), fixed_source_value(0.0), parallel_conductance(0),
		source_id(0), is_fixed(false), has_no_source(false), is_ideal_voltage_source(false) {}

	ResistiveCompanionElement
	(
		std::string name,
		unsigned int p,
		unsigned int n,
		double fixed_source_value,
		double parallel_conductance,
		unsigned int source_id,
		bool is_fixed,
		bool has_no_source,
		bool is_ideal_voltage_source
	) :
		name(name), p(p), n(n), fixed_source_value(fixed_source_value),
		parallel_conductance(parallel_conductance), source_id(source_id),
		is_fixed(is_fixed), has_no_source(has_no_source),
		is_ideal_voltage_source(is_ideal_voltage_source)
	{}
};

typedef ResistiveCompanionElement RCE;   ///< typedef alias for ResistiveCompanionElement

/**
	\brief Describes a controlled source resistive companion element for a Component

	Controlled sources are ideal voltage and current sources which are controlled by a measured
	voltage or current (multiplied by a gain) across/through terminals elsewhere in a system.  These
	components cannot be modeled by typical 2-terminal resistive companion elements (current source
	with parallel conductance, or ideal voltage source), so they are described by special resistive
	companion elements of 4 terminals with this type.

	\see VoltageControlledCurrentSource code generator class for creating such a component
	\see VoltageControlledVoltageSource code generator class for creating such a component
	\see CurrentControlledCurrentSource code generator class for creating such a component
	\see CurrentControlledVoltageSource code generator class for creating such a component

	\deprecated This class is being phased out in use

**/
class ResistiveCompanionControlledSourceElement
{

public:

	/**
		enumerates the controlled source type
	**/
	enum class Type : int
	{
		UNDEFINED                         = -1,
		VOLTAGE_CONTROLLED_CURRENT_SOURCE =  0,
		VOLTAGE_CONTROLLED_VOLTAGE_SOURCE =  1,
		CURRENT_CONTROLLED_CURRENT_SOURCE =  2,
		CURRENT_CONTROLLED_VOLTAGE_SOURCE =  3
	};

	std::string name;       ///< unique name of element; default is ""
	unsigned int p_source;  ///< node index of positive terminal for the source itself
	unsigned int n_source;  ///< node index of negative terminal for the source itself
	unsigned int p_measure; ///< node index of positive terminal for the measured quantity
	unsigned int n_measure; ///< node index of negative terminal for the measured quantity
	double gain; ///< gain of the source from measured quantity (source_magnitude = gain*measurement)
	ResistiveCompanionControlledSourceElement::Type type; ///< type of the controlled source

	ResistiveCompanionControlledSourceElement() :
		name(), p_source(0), n_source(0), p_measure(0), n_measure(0), gain(0.0), type(Type::UNDEFINED)
	{}

	ResistiveCompanionControlledSourceElement
	(
		std::string name,
		unsigned int p_source,
		unsigned int n_source,
		unsigned int p_measure,
		unsigned int n_measure,
		double gain,
		ResistiveCompanionControlledSourceElement::Type type
	) :
		name(name), p_source(p_source), n_source(n_source), p_measure(p_measure), n_measure(n_measure),
		gain(gain), type(type)
	{}


};

typedef ResistiveCompanionControlledSourceElement RCCSE;

} //namespace lblmc

#endif // LBLMC_RESISTIVECOMPANIONELEMENTS_HPP

