/*

Copyright (C) 2018-2020 Matthew Milton

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

/**

	\file CodeGenDataTypes.hpp

	Provides data types to be used only in code generation for LB-LMC engine development

	Many of the data types here may not be FPGA/HDL synthesizable

	\author Matthew Milton
	\date Spring 2018

 **/

#ifndef LBLMC_CODEGENDATATYPES_HPP
#define LBLMC_CODEGENDATATYPES_HPP

#include <Eigen/Dense>

namespace lblmc
{

///////////////////////////////////////////////////////////////////////////////////////////////////
// Eigen3 Linear Algebra Library Typedefs (http://eigen.tuxfamily.org)

typedef Eigen::Matrix<double,
		Eigen::Dynamic,Eigen::Dynamic,
		Eigen::RowMajor>
MatrixRMXd; ///< Dynamically-allocated row-major double Eigen3 matrix type

typedef Eigen::Matrix<double,
		Eigen::Dynamic, 1>
VectorRMXd; ///< dynamically-allocated row-major double Eigen3 vector type

///////////////////////////////////////////////////////////////////////////////////////////////////

} //namespace lblmc

#endif // LBLMC_CODEGENDATATYPES_HPP
