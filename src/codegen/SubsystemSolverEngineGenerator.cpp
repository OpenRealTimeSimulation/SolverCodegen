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

#include "codegen/SubsystemSolverEngineGenerator.hpp"

#include <vector>
#include <string>
#include <map>
#include <stdexcept>
#include <sstream>
#include <fstream>
#include <cstddef>
#include <iomanip>
#include <cmath>

#include <iostream> //for debugging here

#include "codegen/ArrayObject.hpp"
#include "codegen/CodeGenDataTypes.hpp"

namespace lblmc
{

SubsystemSolverEngineGenerator::SubsystemSolverEngineGenerator
(
	std::string model_name,
	unsigned int num_solutions
) :
	SolverEngineGenerator(model_name, num_solutions),
	ports(),
	source_gains(),
	port_source_ids()
{
	if(model_name == "")
		throw std::runtime_error("SubsystemSolverEngineGenerator::constructor(): model_name cannot be null or empty");

	if(num_solutions == 0)
		throw std::runtime_error("SubsystemSolverEngineGenerator::constructor(): num_solutions must be positive nonzero value");
}

SubsystemSolverEngineGenerator::SubsystemSolverEngineGenerator(const SubsystemSolverEngineGenerator& base) :
	SolverEngineGenerator(base),
	ports(base.ports),
	source_gains(base.source_gains),
	port_source_ids(base.port_source_ids)
{}

void SubsystemSolverEngineGenerator::reset(std::string model_name, unsigned int num_solutions)
{
	if(model_name == "")
		throw std::runtime_error("SubsystemSolverEngineGenerator::reset(): model_name cannot be null or empty");

	if(num_solutions == 0)
		throw std::runtime_error("SubsystemSolverEngineGenerator::reset(): num_solutions must be positive nonzero value");

	this->model_name = model_name;
	this->num_solutions = num_solutions;
	this->comp_parameters.clear();
	this->comp_fields.clear();
	this->comp_inputs.clear();
	this->comp_outputs.clear();
	this->comp_outputs_update_bodies.clear();
	this->comp_update_bodies.clear();
	this->conductance_matrix_gen = SystemConductanceGenerator(num_solutions);
	this->source_vector_gen = SystemSourceVectorGenerator(num_solutions);
	this->ports.clear();
	this->source_gains.clear();
	this->port_source_ids.clear();
}

void SubsystemSolverEngineGenerator::addPort(const Port& port)
{
	if(port.id == -1 || port.p == -1 || port.n == -1)
	{
		throw std::invalid_argument("SubsystemSolverEngineGenerator::addPort(port) -- port cannot contain invalid id or indices (-1)");
	}

	bool nonexistent = true;
	for(auto& p : ports)
	{
		if(p.id == port.id)
		{
			p = port;
			nonexistent = false;
			break;
		}
	}

	if(nonexistent)
	{
		ports.push_back(port);
	}
}

void SubsystemSolverEngineGenerator::setPorts(const std::vector<Port>& ports)
{
	for(const auto& p : ports)
	{
		if(p.id == -1 || p.p == -1 || p.n == -1)
		{
			throw std::invalid_argument("SubsystemSolverEngineGenerator::addPorts(ports) -- ports cannot contain invalid id or indices (-1)");
		}
	}

	this->ports = ports;
}

const SubsystemSolverEngineGenerator::Port& SubsystemSolverEngineGenerator::getPort(unsigned int id) const
{
	for(const auto& p : ports)
	{
		if(p.id == id)
		{
			return p;
		}
	}

	throw std::out_of_range("SubsystemSolverEngineGenerator::getPort(id) -- port does not exist for given id");
}

std::vector<SubsystemSolverEngineGenerator::PortModel> SubsystemSolverEngineGenerator::computePortModels() const
{
	if(ports.empty())
	{
		throw std::runtime_error("SubsystemSolverEngineGenerator::computePortModels() -- subsystem has no ports for which to compute models");
	}

	unsigned int num_ports = ports.size();
	unsigned int dimension = conductance_matrix_gen.getDimension();
	unsigned int num_sources = source_vector_gen.getNumSources();

	std::vector<SubsystemSolverEngineGenerator::PortModel> port_models(num_ports);

	for(unsigned int i = 0; i < num_ports; i++)
	{
		port_models[i].id = ports[i].id;
	}

	MatrixRMXd gprobe = MatrixRMXd::Zero(dimension+num_ports, dimension+num_ports);
	const MatrixRMXd& conduct_matrix = conductance_matrix_gen.asEigen3Matrix();

		//init gprobe from subsystem conductance matrix

	for(unsigned int r = 0; r < dimension; r++)
	{
		for(unsigned int c = 0; c < dimension; c++)
		{
			gprobe(r,c) = conduct_matrix(r,c);
		}
	}

		//add incidences (-1, 1) for the ideal voltage probe sources into gprobe

	for(unsigned int rc = 0; rc < num_ports; rc++)
	{

        if( ports[rc].p != 0 )
		{

			gprobe( ports[rc].p-1, dimension+rc ) = 1.0;
			gprobe( dimension+rc, ports[rc].p-1) = 1.0;

			if( ports[rc].n != 0)
			{
				gprobe( ports[rc].n-1, dimension+rc ) = -1.0;
				gprobe( dimension+rc, ports[rc].n-1) = -1.0;
			}
		}
		else
		{
			if( ports[rc].n != 0)
			{
				gprobe( ports[rc].n-1, dimension+rc ) = 1.0;
				gprobe( dimension+rc, ports[rc].n-1) = 1.0;
			}
		}
	}

		//init solution, source, and zero vector

	VectorRMXd vzeroed = VectorRMXd::Zero(dimension+num_ports);
	VectorRMXd bprobe = vzeroed;
	VectorRMXd xprobe = vzeroed;

		//compute conductances and transconductances

	for(unsigned int i = 0; i < num_ports; i++) // iterate probes
	{
		bprobe(dimension+i) = 1.0;

		xprobe = gprobe.fullPivLu().solve(bprobe); // x = G\b with full pivot LU factorization

		for(unsigned int j = 0; j < num_ports; j++) // iterate port models
		{
			auto& mdl = port_models[j];

			if(mdl.id == ports[i].id)
			{
				mdl.conductance = xprobe(dimension+i);
			}
			else
			{
                //mdl.transconductances[i] = xprobe(dimension+i); //changed 2021-01-15-17.52
                mdl.transconductances[i+1] = xprobe(dimension+i);
			}

		}

		bprobe = vzeroed;
	}

		//compute port source gains

	for(unsigned int i = 0; i < num_ports; i++) //iterate probes/models
	{
		auto& mdl = port_models[i];

		for(unsigned int s = 0; s < num_sources; s++) //iterate sources; may need to init s to 1
		{
			//  update bprobe for present source at s, like in pseudo code below:
			// vector nodes = source_vector_gen.getNodeIndicesForSource(s);
			// bprobe (at nodes) = +/- 1.0;

			const auto& source_nodes = source_vector_gen.getSourceNodesById(s+1);

			if(source_nodes[0] != 0)
			{
				bprobe(source_nodes[0]-1) += 1.0;

				if(source_nodes[1] != 0)
				{
					bprobe(source_nodes[1]-1) += -1.0;
				}
			}
			else
			{
				if(source_nodes[1] != 0)
				{
					bprobe(source_nodes[1]-1) += 1.0;
				}
			}

			xprobe = gprobe.fullPivLu().solve(bprobe); // x = G\b with full pivot LU factorization
			mdl.source_gains[s] = xprobe(dimension+i);

			bprobe = vzeroed;
		}
	}

	return port_models;
}

void SubsystemSolverEngineGenerator::stampOthersPortModel(const PortModel& port_model)
{
    for(const auto& port : ports)
	{
		if(port_model.id == port.id)
		{

			if(port_model.source_gains.size() != 0)
			{
				port_source_ids[port_model.id] = source_vector_gen.insertSource(port.p, port.n);
			}

			conductance_matrix_gen.stampConductance( abs(double(port_model.conductance)), port.p, port.n);

            for(const auto& xconduct_pair : port_model.transconductances)
			{
                const auto& other_port = getPort( xconduct_pair.first );
                conductance_matrix_gen.stampTransconductance
                (
					xconduct_pair.second,
					other_port.p,
					other_port.n,
					port.p,
					port.n
				);
			}

			return;
		}
	}

	throw std::invalid_argument("SubsystemSolverEngineGenerator::stampOthersPortModel(port_model) -- given port model does not correspond to any port of this subsystem");
}

void SubsystemSolverEngineGenerator::stampOthersPortModels(const std::vector<PortModel>& port_models)
{
	try
	{
		for(const auto& port_model : port_models)
		{
			stampOthersPortModel(port_model);
		}
	}
	catch(...)
	{
		throw std::invalid_argument("SubsystemSolverEngineGenerator::stampOthersPortModels(port_models) -- one or more of the given port models does not correspond to any port of this subsystem");
	}
}

void SubsystemSolverEngineGenerator::addOwnSourceGains(const PortModel& port_model)
{
	for(const auto& port : ports)
	{
		if( (port_model.id == port.id) && port_model.source_gains.size() != 0 )
		{
			source_gains[port_model.id] = port_model.source_gains;

			return;
		}
	}

	throw std::invalid_argument("SubsystemSolverEngineGenerator::addOwnSourceGains(port_model) -- given port model does not correspond to any port of this subsystem");
}

void SubsystemSolverEngineGenerator::addOwnSourceGains(const std::vector<PortModel>& port_models)
{
	for(const auto& port_model : port_models)
	{
		addOwnSourceGains(port_model);
	}
}

std::string SubsystemSolverEngineGenerator::generatePortSourceEquation(unsigned int port_id) const
{
	std::stringstream sstrm;
	sstrm << std::setprecision(16) << std::fixed << std::scientific;

	try
	{
		const auto& gains = source_gains.at(port_id);

		if(gains.size() == 0) //nothing to do, so return empty string
		{
			return std::string();
		}

		sstrm << "port_inject_" << port_id << "_out = ";

		auto gain_pair_begin_iter = gains.begin();

		sstrm << "b_components["<<(gain_pair_begin_iter->first)<<"]*real("<<(gain_pair_begin_iter->second)<<")";

		++gain_pair_begin_iter;

		for(auto gain_pair_iter = gain_pair_begin_iter; gain_pair_iter != gains.end(); gain_pair_iter++)
		{
            sstrm << " + " <<
            "b_components["<<(gain_pair_iter->first)<<"]*real("<<(gain_pair_iter->second)<<")";
		}

		sstrm << ";\n\n";

		return sstrm.str();

	}
	catch(const std::out_of_range& error)
	{
		throw std::out_of_range
		("SubsystemSolverEngineGenerator::generatePortSourceEquation(port_id) -- given port_id does not correspond to any port of the subsystem");
	}


	return std::string();
}

std::string SubsystemSolverEngineGenerator::generatePortSourceEquations() const
{
	std::string equations;

	for(const auto& source_gain_map : source_gains)
	{
		equations += generatePortSourceEquation( source_gain_map.first );
	}

	return equations;
}

std::string SubsystemSolverEngineGenerator::generatePortSourceInputParameterList() const
{
	if(!port_source_ids.empty())
	{
		std::stringstream sstrm;

		auto begin_iter = port_source_ids.begin();

		sstrm << "real port_inject_"<<(begin_iter->first)<<"_in";

		for(auto iter = ++begin_iter; iter != port_source_ids.end(); iter++ )
		{
			sstrm << ",\n";
			sstrm << "real port_inject_"<<(iter->first)<<"_in";
		}

		return sstrm.str();
	}

	return std::string();

}

std::string SubsystemSolverEngineGenerator::generatePortSourceOutputParameterList() const
{
	if(!source_gains.empty())
	{
		std::stringstream sstrm;

		auto begin_iter = source_gains.begin();

		sstrm << "real& port_inject_"<<(begin_iter->first)<<"_out";

		for(auto iter = ++begin_iter; iter != source_gains.end(); iter++)
		{
            sstrm << ",\n";
            sstrm << "real& port_inject_"<<(iter->first)<<"_out";
		}

		return sstrm.str();
	}

	return std::string();
}


std::string SubsystemSolverEngineGenerator::generateCFunctionParameterList() const
{
	std::stringstream sstrm;

	lblmc::ArrayObject x_out("real", "x_out", "", {num_solutions});
	sstrm << x_out.generateArgument();

	std::string port_src_output_params = generatePortSourceOutputParameterList();
	if(!port_src_output_params.empty())
	{
		sstrm << ",\n" << port_src_output_params;
	}

	if( parameters.io_signal_output_enable && !comp_outputs.empty())
	{
		sstrm << ",\n";

		auto iter = comp_outputs.begin();

		sstrm << *iter;

		for(iter = iter+1; iter != comp_outputs.end(); iter++)
		{
            sstrm << ",\n"
                  << *iter;
		}
	}

	std::string port_src_input_params = generatePortSourceInputParameterList();
	if(!port_src_input_params.empty())
	{
		sstrm << ",\n" << port_src_input_params;
	}

	if(!comp_inputs.empty())
	{
		sstrm << ",\n";

		auto iter = comp_inputs.begin();

		sstrm << *iter;

		for(iter = iter+1; iter != comp_inputs.end(); iter++)
		{
            sstrm << ",\n"
                  << *iter;
		}
	}

	if(parameters.io_source_vector_output_enable == true)
	{
		sstrm << ",\n";

		lblmc::ArrayObject b_out("real", "b_out", "", {num_solutions});
		sstrm << b_out.generateArgument();

	}

	if(parameters.io_component_sources_output_enable == true)
	{
		sstrm << ",\n";

		lblmc::ArrayObject src_out("real", "sources_out", "", {source_vector_gen.getNumSources()});
		sstrm << src_out.generateArgument();
	}

	return sstrm.str();
}

std::string SubsystemSolverEngineGenerator::generateCInlineCode(double zero_bound) const
{
	std::stringstream sstrm;

	SystemConductanceGenerator invg_gen(conductance_matrix_gen);
	invg_gen.invertSelf();
	const double * invg = invg_gen.asArray();

	unsigned int num_components = source_vector_gen.getNumSources();

	SystemSolverGenerator solver_gen(invg, num_solutions, num_components, zero_bound);

	std::string buf;

	//codegen xilinx HLS features
	if(parameters.xilinx_hls_enable)
	{
		sstrm << "//clock period=" << parameters.xilinx_hls_clock_period << "\n";

		if(parameters.xilinx_hls_inline)
		{
			sstrm << "#pragma HLS inline\n";
		}

		if(parameters.xilinx_hls_latency_enable)
		{
			sstrm << "#pragma HLS latency min="<<parameters.xilinx_hls_latency_min<<
			         " max="<<parameters.xilinx_hls_latency_max<<"\n";
		}

		sstrm << "\n";
	}

	sstrm << "//MODEL PARAMETERS\n\n";

	for(auto i : comp_parameters)
	{
		sstrm << i << "\n";
	}
	sstrm << "\n";

	sstrm << "//COMPONENT FIELDS AND STATES\n\n";

	for(auto i : comp_fields)
	{
		sstrm << i << "\n";
	}
	sstrm << "\n";

	sstrm << "//MODEL SOLUTIONS\n\n";

	sstrm
	<< "static real b["<<num_solutions<<"];\n"
	<< "static real x["<<num_solutions+1<<"];\n"
	<< "static real b_components["<<num_components<<"];\n\n";

	sstrm << "//INVERTED CONDUCTANCE MATRIX G^-1\n\n";

	buf = invg_gen.asCLiteral("inv_g");
	sstrm << buf << "\n\n";

	sstrm << "//READ PORT INJECTIONS FROM OTHER SUBSYSTEMS H(n-1)\n\n";

	for(const auto& id_pair : port_source_ids)
	{
		sstrm <<
		"b_components["<<id_pair.second-1<<"]"<<" = "<<"port_inject_"<<id_pair.first<<"_in"<<";\n";
	}
	sstrm << "\n";

	sstrm << "//AGGREGRATE COMPONENT SOURCE CONTRIBUTIONS b(n-1)\n\n";

	source_vector_gen.asCInlineCode(buf);
	sstrm << buf << "\n\n";

	sstrm << "//MODEL UPDATE SOLUTIONS x(n)=G^-1 * b(n-1)\n\n";

	solver_gen.generateCInlineCode(buf, "inv_g");
	sstrm << buf << "\n\n";

	sstrm << "//COMPONENT SOURCE CONTRIBUTION UPDATES b_comp(n)\n\n";

	for(auto i : comp_update_bodies)
	{
		sstrm << i << "\n";
	}
	sstrm << "\n";

	if(parameters.io_signal_output_enable)
	{
		sstrm << "//MODEL OUTPUT SIGNAL UPDATES y(n)\n\n";

		for(auto i : comp_outputs_update_bodies)
		{
			sstrm << i << "\n";
		}
		sstrm << "\n";
	}

	return sstrm.str();
}

std::string SubsystemSolverEngineGenerator::generateCFunction(double zero_bound) const
{
	std::stringstream sstrm;

	if(parameters.codegen_solver_templated_function_enable == true)
	{
        sstrm
        << "template< int instance";

        if(parameters.codegen_solver_templated_real_type_enable == true)
		{
			sstrm
			<< ", typename real";
		}

		sstrm
		<< " >\n";
	}

	sstrm
	<< "void "<<model_name<<"_solver\n"
	<< "(\n";

	sstrm
	<< generateCFunctionParameterList()
	<< "\n)\n"
	<< "{\n";

	std::string buf;
	buf = generateCInlineCode(zero_bound);
	sstrm << buf;

	if(parameters.io_source_vector_output_enable == true)
	{
		for(unsigned int i = 0; i < num_solutions; i++)
		{
			sstrm << "b_out["<<i<<"] = b["<<i<<"];\n";
		}
	}

	sstrm << "\n";

	if(parameters.io_component_sources_output_enable == true)
	{
		for(unsigned int i = 0; i < source_vector_gen.getNumSources(); i++)
		{
			sstrm << "sources_out["<<i<<"] = b_components["<<i<<"];\n";
		}
	}

	sstrm << "\n";

	sstrm << "//UPDATE PORT INJECTIONS TO OTHER SUBSYSTEMS\n\n";

	sstrm << generatePortSourceEquations() << "\n";

	sstrm << "//UPDATE OUTPUTS\n\n";

	for(unsigned int i = 0; i < num_solutions; i++)
	{
		sstrm << "x_out["<<i<<"] = x["<<i+1<<"];\n";
	}

	sstrm
	<< "\n}";

	return sstrm.str();
}

void SubsystemSolverEngineGenerator::generateCFunctionAndExport(std::string filename, double zero_bound) const
{
	if(filename == "")
		throw std::invalid_argument("SubsystemSolverEngineGenerator::generateCFunctionAndExport(): filename cannot be null or empty");

	std::fstream file;

	std::string fname = filename;

	try
	{
		file.open(fname.c_str(), std::fstream::out | std::fstream::trunc);
	}
	catch(...)
	{
		throw std::runtime_error("SubsystemSolverEngineGenerator::generateCFunctionAndExport(): failed to open or create source files");
	}

	file <<
			"/**\n"
			" *\n"
			" * LBLMC Vivado HLS Simulation Engine for FPGA Designs\n"
			" *\n"
			" * Auto-generated by SubsystemSolverEngineGenerator Object\n"
			" *\n"
			" */\n\n";

	file << "#ifndef " << model_name << "_SIMULATIONENGINE_HPP" << "\n";
	file << "#define " << model_name << "_SIMULATIONENGINE_HPP" << "\n";

	file << "\n\n";

	if( !( parameters.codegen_solver_templated_real_type_enable == true &&
		parameters.codegen_solver_templated_function_enable == true ) )
	{
		if(parameters.fixed_point_enable)
		{
			if(parameters.xilinx_hls_enable)
			{
				file <<
				"#include <ap_fixed.h>\n" <<
				"typedef ap_fixed<"<<parameters.fixed_point_word_width<<", "<<
				parameters.fixed_point_int_width<<", AP_RND> real;\n\n";

			}
			else
			{
				file << "//platform-agnostic fixed point not supported yet. Using double real values\n"<<
				"typedef double real;\n\n";
			}
		}
		else
		{
			file << "typedef double real;\n\n";
		}
	}

	if(parameters.codegen_solver_templated_function_enable == false)
	{
		file << "inline\n";
	}

    std::string buf;
    buf = generateCFunction(zero_bound);
    file << buf << "\n\n";

	file << "\n#endif";

	file.close();

}

} //namespace lblmc

