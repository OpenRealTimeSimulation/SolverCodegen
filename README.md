<img src="./ortis_logo_300dpi.png" width="300"/>

# ORTiS Solver C++ Code Generation Tools for Real-Time Simulation of Power Electronic Systems 

Copyright (C) 2019-2022 Matthew Milton and Others

Code generation tools for creation of CPU or FPGA real-time simulation C++ solvers of nonlinear electrical and power electronic systems.

These tools are part of the Open Real-Time Simulation (ORTiS) framework.

Utilizing solvers generated by these tools along with FPGA High-Level Synthesis (HLS) and execution, real-time simulation of sizable power electronic systems with high frequency dynamics at 35 or less nanosecond time steps are achievable.

Solvers generated by the tools utilize the Latency Based Linear Multi-step Compound (LB-LMC) simulation method to solve system models.  

Solvers are generated as custom-tailored C++ code for a given system model defined in plain-text netlists.  User defined components in the systems are supported through C++ extensions to the codegen tool library.  Multi-FPGA solver creation for decomposed systems is supported.

The tools include the LB-LMC C++ Solver Code Generation C++ Library along with a Command Line Interface (CLI) tool to allow creation of simulation solvers via either C++ programming or command shell environments.

## Requirements

These tools require C++14 or higher complaint compiler suite to build, such as GCC.

Besides the C++ standard library, the tools depend on the following third-party libaries to compile:

  * Eigen 3 -- Linear Algebra Template Library
http://eigen.tuxfamily.org/

Solvers generated by the tools are C++03 complaint and do not have any dependencies.

High Level Synthesis (HLS) of C++ solvers into Register Transfer Level (RTL) designs for FPGA execution is supported using Xilinx Vivado HLx suite for Xilinx FPGA devices.  Solver FPGA cores created with HLS can be utilized on National Instruments FPGA-based platforms, Xilinx FPGA evaluation kits, and other platforms that incorporate Xilinx FPGAs.

## License

Solver C++ Code Generation Tools are licensed under the GNU General Public License (GPL) v3.0 (https://www.gnu.org/licenses/).

## Getting Started

To get started with the ORTiS Solver Codegen tools to generate electrical system solvers in C++ for simulation, we recommend to use the precompiled Command Line Interface (CLI) tool under Releases found here:
https://github.com/OpenRealTimeSimulation/SolverCodegen/releases

The CLI tool comes with a user guide which explains in detail how to use the tool and the generated solver code produced by the tool.  Electrical components supported by the tools is listed in the guide.  The current user guide for v0.9 of the tools is attached here for convenience:
[ortis_codegen_cli_guide-0.9.pdf](https://github.com/OpenRealTimeSimulation/SolverCodegen/files/8312269/ortis_codegen_cli_guide-0.9.pdf)

**In Summary**
1. You write a netlist file describing the electrical system you wish to simulate, following the syntax supported by the Solver Codegen tools
2. Then, call the CLI tool from the command prompt, passing the netlist file to the tool
3. The CLI tool will then generate a C++ function template for the solver of the electrical system described in your netlist
4. With the solver function template, you can run an offline simulation of the electrical system within a custom C++ testbench or from a simulation tool such as Mathworks Matlab/Simulink, Gnu Octave, Plexim PLECS, or many others that support C++ code.
5. For real-time simulation, the solver C++ function templates can be compiled into executables for real-time CPUs, or high level synthesized into HDL cores for FPGA execution (Xilinx Vivado HLS).

If you need references for C++, commercial simulators, and FPGA tools, see these third-party links:
-  [C++ References](https://en.cppreference.com/w/)
- [Simulink documentation](https://www.mathworks.com/help/simulink/)
- [PLECS documentation](https://www.plexim.com/download/documentation)
- [GNU Octave documentation](https://octave.org/doc/v6.4.0/)
- [Xilinx Vivado high level synthesis documentation](https://www.xilinx.com/support/documentation-navigation/design-hubs/dh0012-vivado-high-level-synthesis-hub.html)

## Literature 
  
**The ORTiS solver code generation tools themselves are presented in:**

* M. Milton and A. Benigni, "ORTiS solver codegen: C++ code generation tools for high performance, FPGA-based, real-time simulation of power electronic systems," SoftwareX, vol. 13, Jan. 2021.
doi: 10.1016/j.softx.2021.100660
url: https://www.sciencedirect.com/science/article/pii/S2352711021000054

* M. Milton and A. Benigni, "Software and Synthesis Development Libraries for Power Electronic System Real-Time Simulation," *2019 IEEE Electric Ship Technologies Symposium (ESTS)*, Arlington VA, 2019, pp. 368-376, Aug. 2019.
doi: 10.1109/ESTS.2019.8847940
url: https://ieeexplore.ieee.org/document/8847940

**The code generation tools have been applied in the following recent work:**

* M. Milton, M. Difronzo, D. Chowdhury, H. Ginn III, and A. Benigni, ``A Model of MMCs for Power Electronic System High-Performance Real-Time Simulation,''  2022 Open Source Modelling and Simulation of Energy Systems (OSMSES), Aachen, Germany, April 2022.
doi: 10.1109/OSMSES54027.2022.9769141
url: https://ieeexplore.ieee.org/document/9769141 

* M. Difronzo, Md. M. Biswas, M. Milton, H. Ginn III, and A. Benigni, ``System Level Real-Time Simulation and Hardware-in-the-Loop Testing of MMCs,'' Energies, vol. 14, no. 11, May 2021.
doi:  10.3390/en14113046 
url: https://www.mdpi.com/1996-1073/14/11/3046

* M. Vygoder, M. Milton, J. Gudex, R. Cuzner, and A. Benigni, ``A Hardware-in-the-Loop Platform for DC Protection,'' IEEE Journal of Emerging and Selected Topics in Power Electronics, vol. 9, no.3, pp. 2605-2619, June 2021.
doi:  10.1109/JESTPE.2020.3017769
url: https://ieeexplore.ieee.org/document/9171341

* M. Milton, A. Benigni, and A. Monti, "Real-Time Multi-FPGA Simulation of Energy Conversion Systems," in IEEE Trans. Energy Conversion, vol. 34, no.4, pp. 2198-2208, Dec. 2019.
doi:  10.1109/TEC.2019.2938811
url: https://ieeexplore.ieee.org/document/8822485

* M. Vygoder, J. Gudex, R. Cuzner, M. Milton, and A. Benigni, ``Real Time Simulation of Transient Overvoltage and Common-Mode during Line-to-Ground Fault in DC Ungrounded Systems,'' IECON 2019 -- 45th Annual Conference of the IEEE Industrial Electronics Society, Lisbon, Portugal, 2019, pp. 6451-6456.
doi: 10.1109/IECON.2019.8927034
url: https://ieeexplore.ieee.org/document/8927034

* M. Milton, M. Vygoder, J. Gudex, R. Cuzner, and A. Benigni, ``Power Electronic System Real-Time Simulation on National Instruments FPGA Platforms,'' 2019 IEEE Electric Ship Technologies Symposium (ESTS), Arlington VA, 2019, pp. 32-38.
doi:  10.1109/ESTS.2019.8847934
url: https://ieeexplore.ieee.org/document/8847934
  
**The LB-LMC simulation solver algorithm applied in generated solvers was first proposed in:**  

 * A. Benigni and A. Monti, "A Parallel Approach to Real-Time Simulation of Power Electronics Systems," in *IEEE Transactions on Power Electronics*, vol. 30, no. 9, pp. 5192-5206, Sept. 2015.
doi: 10.1109/TPEL.2014.2361868
url: https://ieeexplore.ieee.org/document/6918539

**FPGA execution of the LB-LMC solver algorithm is demonstrated in the following papers:** 

* M. Difronzo, M. Milton, M. Davidson and A. Benigni, "Hardware-in-the-loop testing of high switching frequency power electronics converters," *2017 IEEE Electric Ship Technologies Symposium (ESTS)*, Arlington, VA, 2017, pp. 299-304.
doi: 10.1109/ESTS.2017.8069297
url: https://ieeexplore.ieee.org/document/8069297

 * M. Milton, A. Benigni and J. Bakos, "System-Level, FPGA-Based, Real-Time Simulation of Ship Power Systems," in *IEEE Transactions on Energy Conversion*, vol. 32, no. 2, pp. 737-747, June 2017.
doi: 10.1109/TEC.2017.2692525
url: https://ieeexplore.ieee.org/document/7894204
 
 * Milton, M. A.(2016). *A Comparison Of FPGA Implementation Of Latency-Based Solvers For Power Electronic System Real-Time Simulation*. (Master's thesis). Retrieved from https://scholarcommons.sc.edu/etd/3903

## Acknowledgements
* Matthew Milton (University of South Carolina) - solver codegen tools creator and lead developer
* Michele Difronzo (University of South Carolina) - component model developer
* Dhiman Chowdhury (University of South Carolina) - component model developer
* Mark Vygoder (University of Wisconsin-Milwaukee) - component model developer
* Andrea Benigni (FZ-Juelich, RWTH Aachen) - original creator of the LB-LMC solver algorithm



