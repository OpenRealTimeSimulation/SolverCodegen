/*

Copyright (C) 2020 Matthew Milton

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

#ifndef LBLMC_SUBSYSTEMSIMULATIONENGINEGENERATOR_HPP
#define LBLMC_SUBSYSTEMSIMULATIONENGINEGENERATOR_HPP

#include <string>
#include <vector>
#include <map>

#include "codegen/SystemConductanceGenerator.hpp"
#include "codegen/SystemSourceVectorGenerator.hpp"
#include "codegen/SystemSolverGenerator.hpp"
#include "codegen/SimulationEngineGenerator.hpp"

namespace lblmc
{

/**
	\brief Generates top-level code for a LB-LMC simulation solver engine of a network subsystem

	This generator is used to generate solver engine functions for subsystems of a network system
	that is decomposed using Nodal Decomposition under LB-LMC method.  The subsystem is expected
	to have ports which are to be interfaced to the ports of other subsystems in a system during
	simulation.  The execution flow of the solver function generated with this generator follows
	a different order than that of the monolithic system solver; This subsystem solver flow is as
	such:

	1. read port exchanges from other subsystems\n
	2. aggregrate component source contributions and port exchanges into b vector\n
	3. solve subsystem Gx=b for x and update solution (subsystem and internal) outputs\n
	4. read signal inputs for components\n
	5. update models/behavior of each component from solution x, past states, and signal inputs\n
	6. compute port exchanges of this subsystem to be passed to other subsystem solvers\n
	7. output subsystem and internal solutions of past time step t along with port exchanges of
	current time step t+dt, where dt is length of time step.

	\note This class does not check if the engine code it generates is valid C++.  It is up to the
	user to ensure the given information to the generator is correct to ensure the generated code
	is valid.

	\author Matthew Milton
	\date 2020

	\see SimulationEngineGenerator for a generator that creates solver engines for monolithic
	systems
**/
class SubsystemSimulationEngineGenerator : public SimulationEngineGenerator
{

public:

	/**
		\brief declares a port of a subsystem

		A port of a subsystem is two paired terminals where the subsystem is connected to the rest
		of a system network.  A subsystem must have 1 or more ports to rest of system; otherwise,
		it is not a subsystem but a system of itself.

		These objects are used to merely define the 2-terminal ports of the subsystem by their
		terminals' node indices.
	**/
	struct Port
	{
		unsigned int id; ///< numerical unique id of the port within entire system; defaults to -1 for invalid id
		unsigned int p; ///< positive terminal of the port in subsystem specifically; defaults to -1 for invalid index
		unsigned int n; ///< negative terminal of the port in subsystem specifically; defaults to -1 for invalid index

		Port() : id(-1), p(-1), n(-1) {}
		Port(unsigned int id, unsigned int p, unsigned int n) : id(id), p(p), n(n) {}
	};

	/**
		\brief defines the Norton model of a subsystem as seen from a port

		A port of a subsystem is two paired terminals where the subsystem is connected to the rest
		of a system network.  A subsystem must have 1 or more ports to rest of system; otherwise,
		it is not a subsystem but a system of itself.

		A Norton model at a port is defined as a conductance Y across the two terminals of a port,
		along with a transconductance dependent voltage-controlled current source T and a functional
		current source H in parallel to this conductance. For a subsystem, the Norton port model
		represents the equivalency of the subsystem as seen from the port, where Y is the
		equivalent conductance, T is the coupling conductance seen from other ports on the
		subsystem, and H is the weighted summation of all source contributions in the subsystem.

		These objects are used to define the port models of a subsystem, these models being computed
		by SubsystemSimulationEngineGenerator.

		\see NortonPort for a component generator that can realize this model into an actual
		component
	**/
	struct PortModel
	{
		unsigned int id;                                   ///< port id of this model; defaults to -1 for invalid id
		double conductance;                                ///< conductance Y of the subsystem at the port; defaults to 1.0
		std::map<unsigned int, double> transconductances;  ///< transconductances T of subsystem at the port from other ports, mapped to id of the other ports
		std::map<unsigned int, double> source_gains; ///< weight gains for each source in subsystem to summate the port source H, mapped to source id of the component contribution sources in subsystem

		PortModel() : id(-1), conductance(1.0), transconductances(), source_gains() {}
	};

private:

	std::vector<Port> ports; ///< ports across where subsystem is decomposed from rest of a system
	std::map< unsigned int, std::map<unsigned int, double> > source_gains; ///< source weight gains for each port H, mapped to each port by id; gains are mapped to source id's of component contribution sources in subsystem ( key port id -> value ( key source id -> value gain) )
	std::map< unsigned int, unsigned int> port_source_ids; ///< ids of port sources attached to subsystem from other subsystems, each source id mapped to id of associated port (key port_id -> value source_id)

public:

	/**
		\brief default constructor (deleted)
	**/
	SubsystemSimulationEngineGenerator() = delete;

	/**
		\brief parameter constructor
		\param model_name string name of the model the engine is for; name must be C++ compatible label that is not null/empty
		\param num_solutions the number of solutions the generated engine solves for
		\param num_ports the number of ports subsystem has that are to be connected to other subsystems; must be >= 1
	**/
	SubsystemSimulationEngineGenerator
	(
		std::string model_name,
		unsigned int num_solutions
	);

	/**
		\brief copy constructor
	**/
	SubsystemSimulationEngineGenerator(const SubsystemSimulationEngineGenerator& base);

	/**
		\brief resets the generator
		\param model_name string name of the model the engine is for; name must be C++ compatible label that is not null/empty
		\param num_solutions the number of solutions the generated engine solves for
	**/
	void reset(std::string model_name, unsigned int num_solutions);

	/**
		\brief adds a port for the subsystem

		if a port is already in subsystem with same id as that given to this method,
			it will be replaced by the given port

		\param port the port to add to the subsystem
		\throw invalid_argument error if port has invalid id or node indices (-1)
	**/
	void addPort(const Port& port);

	/**
		\brief sets the ports for the subsystem
		\param ports vector of ports to add for the subsystem
		\throw invalid_argument error if ports have invalid id's or node indices (-1)
	**/
	void setPorts(const std::vector<Port>& ports);


	/**
		\param id of the port to get
		\return port that corresponds to given id
		\throw out_of_range error if id does not exist in the port
	**/
	const Port& getPort(unsigned int id) const;

	/**
		\return ports of the subsystem which for a solver engine will be generated
	**/
	inline const std::vector<Port>& getPorts() const {return ports;}

	/**
		\brief computes port models of the subsystem at its ports

		This method should be called only after all components of this subsystem have stamped their
		conductances and sources into the subsystem set of equations Gx=b.

		\return vector of all port models computed

		\throw error if subsystem has no ports or conductance matrix is singular
	**/
	std::vector<PortModel> computePortModels() const;

	/**
		\brief stamps the port model of another subsystem into this subsystem's set of equations

		This method takes the port model of another subsystem and adds its contributions
		to the conductance and source vector of this subsystem

		\param port_model port model from other subsystem
	**/
	void stampOthersPortModel(const PortModel& port_model);

	/**
		\brief stamps the port models of another subsystem to this subsystem's set of equations

		This method takes the port models of another subsystem and adds their contributions
		to the conductance and source vector of this subsystem

		\param port_model port models in vector from other subsystem
	**/
	void stampOthersPortModels(const std::vector<PortModel>& port_models);

	/**
		\brief adds source gains of this subsystem back into itself via a port model

		This method takes the port model computed from this subsystem and adds the source
		equation gains of the model to this subsystem.

		\param port_model port model from this subsystem
	**/
	void addOwnSourceGains(const PortModel& port_model);

	/**
		\brief adds source gains of this subsystem back into itself via port models

		This method takes the port model computed from this subsystem and adds the source
		equation gains of the model to this subsystem.

		\param port_model port models in vector, from this subsystem
	**/
	void addOwnSourceGains(const std::vector<PortModel>& port_models);

	/**
		\brief generates string defining code for equation of a port's source contribution H to
		be injected into another subsystem

		\param port_id the id of the port for which the equation will be generated
		\return string containing C++ statement for the equation being generated
		\throw out_of_range exception if port_id does not correspond with any port in subsystem
	**/
	std::string generatePortSourceEquation(unsigned int port_id) const;

	/**
		\brief generates C++ code for equations computing port source injections from subsystem
		\return string containing C++ code for the equations
	**/
	std::string generatePortSourceEquations() const;

	/**
		\brief generates valid function parameter (argument) list for port source injections from
		other subsystems
		\return string containing valid C++ parameter list of the input port injections for the
		simulation engine top-level function
	**/
	std::string generatePortSourceInputParameterList() const;

	/**
		\brief generates valid function parameter (argument) list for port source injections to
		other subsystems
		\return string containing valid C++ parameter list of the output port injections for the
		simulation engine top-level function
	**/
	std::string generatePortSourceOutputParameterList() const;

	/**
		\brief generates valid parameter (argument) list for the simulation engine top-level function

		The generated parameter list will contain the engine's solutions, signal inputs and outputs
		of components, and any other parameter items generated from the information given to the
		generator object.

		\return string containing valid C++ parameter list for the simulation engine top-level function
	**/
	std::string generateCFunctionParameterList() const;

	/**
		\brief generates valid C++ code string of the simulation engine that can be inlined into existing C++ code
		\param zero_bound value indicating how close a system conductance matrix element must be to zero to be discarded for reduced calculations
		\return string containing valid, inlineable C++ code for the simulation engine
	**/
    std::string generateCInlineCode(double zero_bound = 1.0e-12) const;

    /**
		\brief generates valid C++ code string of the simulation engine as a C++ function definition
		\param zero_bound value indicating how close a system conductance matrix element must be to zero to be discarded for reduced calculations
		\return string containing valid C++ function definition for the simulation engine
	**/
	std::string generateCFunction(double zero_bound = 1.0e-12) const;

	/**
		\brief generates valid C++ code string of the simulation engine as a C++ function definition exported to a header file
		\param filename name of the header file that will contain the engine definition, including directory path and file extension
		\param zero_bound value indicating how close a system conductance matrix element must be to zero to be discarded for reduced calculations
	**/
	void generateCFunctionAndExport(std::string filename, double zero_bound = 1.0e-12) const;

};

} //namespace lblmc

#endif // LBLMC_SUBSYSTEMSIMULATIONENGINEGENERATOR_HPP

