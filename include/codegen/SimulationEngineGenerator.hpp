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

#ifndef LBLMC_SIMULATIONENGINEGENERATOR_HPP
#define LBLMC_SIMULATIONENGINEGENERATOR_HPP

#include <string>
#include <vector>

#include "codegen/SystemConductanceGenerator.hpp"
#include "codegen/SystemSourceVectorGenerator.hpp"
#include "codegen/SystemSolverGenerator.hpp"

namespace lblmc
{

/**
	\brief stores settings for the LB-LMC Simulation Engine Code Generator
	\note as of March 02, 2019, only a subset of these settings are supported
**/
struct SimulationEngineGeneratorParameters
{
	// General code generation settings
	bool codegen_solver_templated_function_enable; ///< enables making the generated solver function into a template; default is false
	bool codegen_solver_templated_real_type_enable; ///< enables templating the generated solver function's real type; depends on codegen_solver_templated_function_enable being true; default is false

	// Xilinx (Vivado) High-Level Synthesis settings
	bool         xilinx_hls_enable;       ///< enable code generation for Xilinx HL synthesis; default is false
	double       xilinx_hls_clock_period; ///< set desired clock execution period for synthesized design; default is 50e-9 (50ns)
	bool         xilinx_hls_latency_enable; ///< enable setting of clock cycle latency; default if false
	unsigned int xilinx_hls_latency_min;  ///< set minimum number of clock cycles to execute; default is 0
	unsigned int xilinx_hls_latency_max;  ///< set maximum number of clock cycles to execute; default is 0
	bool         xilinx_hls_inline;       ///< enable inlining of the generated code into top-level design; default is true

	// Fixed Point settings
	bool         fixed_point_enable;         ///< enable use of fixed point for real numbers; default is false
	unsigned int fixed_point_word_width;  ///< set word width in bits of the fixed point words; default is 64
	unsigned int fixed_point_int_width;   ///< set the integral width in bits of the fixed point words; default is 32

	// Inverted Conductance Matrix Optimizations
	bool inv_conduct_matrix_rescale_enable;     ///< enable rescaling of the inverted conductance matrix by a power of 2 scalar; default is false
	unsigned int inv_conduct_matrix_divider; ///< set power of 2 divider scalar for the inverted conductance matrix; default is 2

	// Input/Output Signal settings
	bool io_signal_output_enable;  ///< enable use of output signals; default is true
	bool io_source_vector_output_enable; ///< enable output of the system source vector b; default is false
	bool io_component_sources_output_enable; ///< enable output of component source values as array (*not* same as b); default is false

	SimulationEngineGeneratorParameters() :
		codegen_solver_templated_function_enable(false),
		codegen_solver_templated_real_type_enable(false),
		xilinx_hls_enable(false),
		xilinx_hls_clock_period(50.0e-9),
		xilinx_hls_latency_enable(false),
		xilinx_hls_latency_min(0),
		xilinx_hls_latency_max(0),
		xilinx_hls_inline(true),
		fixed_point_enable(false),
        fixed_point_word_width(64),
        fixed_point_int_width(32),
		inv_conduct_matrix_rescale_enable(false),
        inv_conduct_matrix_divider(2),
		io_signal_output_enable(true),
		io_source_vector_output_enable(false),
		io_component_sources_output_enable(false)
	{}

};

/**
	\brief Generates top-level code for a LB-LMC simulation solver engine

	\note This class does not check if the engine code it generates is valid C++.  It is up to the
	user to ensure the given information to the generator is correct to ensure the generated code
	is valid.

	\author Matthew Milton
	\date 2019
**/
class SimulationEngineGenerator
{

protected:

	std::string model_name;
	unsigned int num_solutions;
	std::vector<std::string> comp_parameters;
	std::vector<std::string> comp_fields;
	std::vector<std::string> comp_inputs;
	std::vector<std::string> comp_outputs;
	std::vector<std::string> comp_outputs_update_bodies;
	std::vector<std::string> comp_update_bodies;
	SystemConductanceGenerator conductance_matrix_gen;
	SystemSourceVectorGenerator source_vector_gen;

	SimulationEngineGeneratorParameters parameters;

public:

	/**
		\brief default constructor (deleted)
	**/
	SimulationEngineGenerator() = delete;

	/**
		\brief parameter constructor
		\param model_name string name of the model the engine is for; name must be C++ compatible label that is not null/empty
		\param num_solutions the number of solutions the generated engine solves for
	**/
	SimulationEngineGenerator
	(
		std::string model_name,
		unsigned int num_solutions
	);

	/**
		\brief copy constructor
	**/
	SimulationEngineGenerator(const SimulationEngineGenerator& base);

	/**
		\brief resets the generator
		\param model_name string name of the model the engine is for; name must be C++ compatible label that is not null/empty
		\param num_solutions the number of solutions the generated engine solves for
	**/
	virtual void reset(std::string model_name, unsigned int num_solutions);

	/**
		\brief sets name of the model the generated engine is for
		\param model_name string name of the model the engine is for; name must be C++ compatible label that is not null/empty
	**/
	void setModelName(std::string model_name);

	/**
		\return name of the model the engine is for
	**/
	std::string getModelName() const;

	/**
		\return number of solutions that the engine will solve
	**/
	inline unsigned int getNumberOfSolutions() const { return num_solutions; }

	inline void setParameters(SimulationEngineGeneratorParameters param) { parameters = param; }

	const SimulationEngineGeneratorParameters& getParameters() const { return parameters; }

	/**
		\return reference to generator's internal Conductance Matrix generator
	**/
	SystemConductanceGenerator&  getConductanceGenerator();

	/**
		\return reference to generator's internal Source Vector generator
	**/
	SystemSourceVectorGenerator& getSourceVectorGenerator();

	/**
		\brief inserts C++ code string for a component's literal (const static) parameters

		The inserted code is lines of C++ code defining the component's constant parameters. An
		example includes:\n
		<pre>
		const static real DT = 500.0e-9;
		const static real L  = 10.0e-6;
		const static int  LEVELS = 7;
		</pre>

		\param code string containing code for a component's literal parameters in valid C++
	**/
	void insertComponentParametersCode(std::string& code);

	/**
		\brief inserts C++ code string for a component's fields (internal variables and states)

		The inserted code is lines of C++ code defining the component's variables and fields such as
		states which are static.  An example includes:\n
		<pre>
		static real cap_voltage = 0;
		static real ind_current = 0;
		static bool sw_past     = false;
		</pre>

		\param code string containing code for a component's fields in valid C++
	**/
	void insertComponentFieldsCode(std::string& code);

	/**
		\brief inserts C++ code string for a component's non-system-solution input signals

		The inserted code is lines of C++ code defining input signals for a component as a parameter
		or argument list for a function.  Each signal should be separated by a comma ',' and the
		last signal must not have an ending comma.  Moreover, the signals should avoid having
		defined assigned default values (i.e. avoid type signal = default_value). An example
		includes:\n
		<pre>
		real sine_in,
		real duty_in,
		bool gate_signal_in,
		int operation_mode_in
		</pre>

		\param code string containing code for a component's input signals in valid C++
	**/
	void insertComponentInputsCode(std::string& code);

	/**
		\brief inserts C++ code string for a component's non-source-contribution output signals

		The inserted code is lines of C++ code defining output signals from a component as a
		parameter or argument list for a function.  Each signal should be separated by a comma ','
		and the last signal must not have an ending comma.  Moreover, the signals should avoid
		having defined assigned default values (i.e. avoid type signal = default_value). The signals
		should be either pointers (*), fixed-sized arrays ([size]), or references (&). An example
		includes:\n
		<pre>
		real* ind_current_out,
		real  cap_voltages_out[2],
		bool&  status_out
		</pre>

		\param code string containing code for a component's output signals in valid C++
	**/
	void insertComponentOutputsCode(std::string& code);

	/**
		\brief insertes C++ code string for a component's update method body for its output signals

		The inserted update code should correspond to output signals inserted by method
		insertComponentOutputsCode(std::string&).

		\param code string containing update code for a component's output signals in valid C++
	**/
	void insertComponentOutputsUpdateBody(std::string& code);

	/**
		\brief inserts C++ code string for a component's update method body

		The inserted code is lines of C++ code defining operations to update the state and behavior
		of the component in question.  This code should process any input and output signals for
		the component that are included with methods insertComponentInputsCode() and
		insertComponentOutputsCode(), and take as input the system solution and output the
		component's source contributions.  Any temporary variables in the update code should not
		clash with any other code's variables.  An example includes:\n
		<pre>
		bc[5] = (x[2]-x[1]) * (G_component) * duty_component_in;
		current_component_out = bc[5];
		</pre>

		\param code string containing code for a component's update method body in valid C++
	**/
	void insertComponentUpdateBody(std::string& code);

	/**
		\brief generates valid parameter (argument) list for the simulation engine top-level function

		The generated parameter list will contain the engine's solutions, signal inputs and outputs
		of components, and any other parameter items generated from the information given to the
		generator object.

		\return string containing valid C++ parameter list for the simulation engine top-level function
	**/
	virtual std::string generateCFunctionParameterList() const;

	/**
		\brief generates valid C++ code string of the simulation engine that can be inlined into existing C++ code
		\param zero_bound value indicating how close a system conductance matrix element must be to zero to be discarded for reduced calculations
		\return string containing valid, inlineable C++ code for the simulation engine
	**/
    virtual std::string generateCInlineCode(double zero_bound = 1.0e-12) const;

    /**
		\brief generates valid C++ code string of the simulation engine as a C++ function definition
		\param zero_bound value indicating how close a system conductance matrix element must be to zero to be discarded for reduced calculations
		\return string containing valid C++ function definition for the simulation engine
	**/
	virtual std::string generateCFunction(double zero_bound = 1.0e-12) const;

	/**
		\brief generates valid C++ code string of the simulation engine as a C++ function definition exported to a header file
		\param filename name of the header file that will contain the engine definition, including directory path and file extension
		\param zero_bound value indicating how close a system conductance matrix element must be to zero to be discarded for reduced calculations
	**/
	virtual void generateCFunctionAndExport(std::string filename, double zero_bound = 1.0e-12) const;

};

} //namespace lblmc

#endif // LBLMC_SIMULATIONENGINEGENERATOR_HPP
