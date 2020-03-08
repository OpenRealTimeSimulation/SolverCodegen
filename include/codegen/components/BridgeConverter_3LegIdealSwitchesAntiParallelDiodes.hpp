/*

Copyright (C) 2019-2020 Matthew Milton, Mark Vygoder

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

#ifndef LBLMC_BRIDGECONVERTER_3LEGIDEALSWITCHESANTIPARALLELDIODES_HPP
#define LBLMC_BRIDGECONVERTER_3LEGIDEALSWITCHESANTIPARALLELDIODES_HPP

#include <string>
#include <vector>

#include "codegen/components/Component.hpp"

namespace lblmc
{

/**
	\brief Component code generator for a bridge 1-Leg Switching Power Electronic Converter using
	ideal switches with ideal anti-parallel diodes across each switch

	The component expects a bipolar DC side and a 1-leg DC or AC side.  The DC side contains
	2 capacitors for bipolar configuration.  The 1-leg side contains series inductor(s) per leg.
	The half-bridge switching elements are ideal switches with ideal diodes across them.  Each
	switch can be controlled independently from the other and allow for shorts across DC bus.

	\see HalfBridgeConverter3Phase_IdealSwitches for simpler component of similar converter

	\note This component generator class is currently under construction and not usable yet.

	\author Matthew Milton, Mark Vygoder

**/
class BridgeConverter_3LegIdealSwitchesAntiParallelDiodes : public Component
{

private:

	double DT;   ///< time step length (s)
	double RIN;  ///< DC capacitor side input terminal resistance
	double GIN;  ///< inverse of RIN (DC capacitor side input terminal resistance)
	double RSW;  ///< conducting resistance of ideal switch, diode
	double R;    ///< series resistance of AC/DC inductor side terminal
	double C;    ///< DC capacitor side capacitance
	double L;    ///< AC/DC inductor side inductance
	double VTH;  ///< forward bias threshold voltage for switch diode to conduct; not voltage drop of diode (diode is ideal switch)
	double ITH;  ///< forward bias threshold current for switch diode to conduct; set to constant 0.0 always

	unsigned int P, G, N, A, B, Ct;
	unsigned int source_id_P, source_id_N, source_id_A, source_id_B, source_id_C;

public:

	BridgeConverter_3LegIdealSwitchesAntiParallelDiodes(std::string comp_name);
	BridgeConverter_3LegIdealSwitchesAntiParallelDiodes
	(
		std::string comp_name,
		double dt,
		double dc_filter_capacitance,
		double leg_inductance,
		double leg_resistance,
		double diode_threshold_voltage
	);
	BridgeConverter_3LegIdealSwitchesAntiParallelDiodes(const BridgeConverter_3LegIdealSwitchesAntiParallelDiodes& base);

	inline std::string getType() const { return std::string("BridgeConverter_3LegIdealSwitchesAntiParallelDiodes");}

	inline unsigned int getNumberOfTerminals() const { return 6; }
	inline unsigned int getNumberOfSources() const { return 5; }

	void getSourceIds(std::vector<unsigned int>& ids) const;
	void getResistiveCompanionElements(std::vector<ResistiveCompanionElement>& elements) const;

	inline void setTerminalConnections
	(
		unsigned int p,
		unsigned int g,
		unsigned int n,
		unsigned int a,
		unsigned int b,
		unsigned int c
	) { P = p; G = g; N = n; A = a; B = b; Ct = c; }

	inline void getTerminalConnections(std::vector<unsigned int>& term_ids) const
	{
        term_ids = {P, G, N, A, B, Ct};
	}

	void setParameters
	(
		double dt,
		double dc_filter_capacitance,
		double leg_inductance,
		double leg_resistance,
		double diode_threshold_voltage
	);

	inline void setIntegrationMethod(std::string method) {}
	inline std::string getIntegrationMethod() const { return std::string("euler_forward"); }

	inline std::vector<std::string> getSupportedOutputs() const
	{
		std::vector<std::string> ret
		{
			"cp_voltage",
			"cn_voltage",
			"la_current",
			"lb_current",
			"lc_current"
		};

		return ret;
	}

	void stampConductance(SystemConductanceGenerator& gen);
	void stampSources(SystemSourceVectorGenerator& gen);
	std::string generateParameters();
	std::string generateFields();
	std::string generateInputs();
	std::string generateOutputs(std::string output = "ALL");
	std::string generateOutputsUpdateBody(std::string output="ALL");
	std::string generateUpdateBody();

};

} //namespace lblmc

#endif // LBLMC_BRIDGECONVERTER_3LEGIDEALSWITCHESANTIPARALLELDIODES_HPP



