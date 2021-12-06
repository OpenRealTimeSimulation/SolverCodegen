/*

Copyright (C) 2017-2020 Matthew Milton

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
	\author Matthew Milton
	\date Summer 2017 - Fall 2020
**/
#ifndef SYSTEMCONDUCTANCEGENERATOR_HPP
#define SYSTEMCONDUCTANCEGENERATOR_HPP

#include <vector>
#include <string>

#include "codegen/CodeGenDataTypes.hpp"

namespace lblmc
{

/**
	\brief Generates the square conductance matrix for a system model simulated in LB-LMC

	This class is intended for use of computing the conductance matrix of a system in
	offline simulation only.  The conductance matrix that is managed by this class
	can be used in math tools to develop the resistance matrix that is to be used
	in LB-LMC FPGA simulation.

	This class is used to compute conductance matrix by passing its matrix to LB-LMC
	components to stamp in their conductances.  After the conductance matrix is stamped,
	the matrix can be exported by this class's instances to file or memory to be processed for
	development of a LB-LMC simulation engine that is RTL synthesizable.

	\note This class is NOT intended for RTL Synthesis.

 **/
class SystemConductanceGenerator
{
private:
	MatrixRMXd matrix;
	unsigned int dimension;

public:

	SystemConductanceGenerator() = delete;

	/**
	 * parameter constructor
	 * \param dimension non-zero dimension of square conductance matrix (length or width)
	 */
	SystemConductanceGenerator(unsigned int dimension);

	/**
	 * initialization constructor
	 *
	 * Initializes conductance matrix to that given.  This constructor expects the dimension to match up
	 * to get given matrix
	 *
	 * \param dimension non-zero dimension of square conductance matrix (length or width)
	 * \param base matrix to initialize conductance matrix to
	 */
	SystemConductanceGenerator(unsigned int dimension, const MatrixRMXd& base);

	/**
	 * copy constructor
	 * \param base conductance matrix to copy from
	 */
	SystemConductanceGenerator(const SystemConductanceGenerator& base);

	/**
	 * resets conductance matrix to given matrix and dimensions
	 *
	 * If base's pointer is equal to zero, this method clears the matrix to all ZERO values.
	 *
	 * \param dimension non-zero dimension of square conductance matrix (length or width)
	 * \param base conductance matrix to copy from
	 */
	void reset(unsigned int dimension);

	/**
	 * resets conductance matrix to given matrix and dimensions
	 *
	 * If base's pointer is equal to zero, this method clears the matrix to all ZERO values.
	 *
	 * \param dimension non-zero dimension of square conductance matrix (length or width)
	 * \param base conductance matrix to copy from
	 */
	void reset(unsigned int dimension, const MatrixRMXd& base);

	/**
	 * resets conductance matrix to that of given SystemConductance
	 * \param base
	 */
	void reset(const SystemConductanceGenerator& base);

	/**
	 * returns conductance matrix as an observer pointer
	 * \return pointer to conductance matrix data
	 */
	double* asPointer();

	/**
	 * returns conductance matrix as an observer array
	 *
	 * same as asPointer()
	 *
	 * \see asPointer()
	 *
	 * \return return conductance matrix as an array
	 */
	double* asArray();

	/**
	 * returns the conductance matrix as a Eigen3 Matrix Type
	 * \return
	 */
	MatrixRMXd& asEigen3Matrix();

	const MatrixRMXd& asEigen3Matrix() const;

	/**
	 * gets dimension of square conductance matrix
	 * \return dimension of matrix
	 */
	unsigned int getDimension() const;

	/**
		\brief stamps a given conductance into conductance matrix for given node indices

		<pre>
		p ---/\/\/\--- n
		</pre>

		\param conductance the conductance to stamp into matrix
		\param p the index of the node where positive terminal of conductance resides
		\param n the index of the node where negative terminal of conductance resides
	**/
	void stampConductance(double conductance, unsigned int p, unsigned int n = 0);

	/**
		\brief stamps a transconductance of a voltage-controlled current source (VCCS) into conductance matrix for given node indices

		Terminals m and n are the voltage measured side while p and q are the current source side.

		<pre>
		 m -------   --------- p
		         +   | I = g*Vp
		         -   V
		 n -------   --------- q
		 </pre>

		\param transconductance the transconductance to stamp into matrix
		\param m the index of the positive voltage terminal of VCCS
		\param n the index of the negative voltage terminal of VCCS
		\param p the index of the input current terminal of VCCS
		\param q the index of the output current terminal of VCCS
	**/
	void stampTransconductance(double transconductance, unsigned int m, unsigned int n, unsigned int p, unsigned int q);


	/**
		\brief stamps transconductances of a coupled 2-port system into conductance matrix for given node indices

		This method is similar to stampTransconductance() but acts as two voltage-controlled current sources coupled together.

		<pre>
		 m --------  --------- p
		I=g12*Vpq |  | I = g21*Vmn
		          V  V
		 q --------  --------- q
		 </pre>

		\param transconductance12 the transconductance at port 1 due to port 2
		\param transconductance21 the transconductance at port 2 due to port 1
		\param m the index of the positive terminal of port 1
		\param n the index of the negative terminal of port 1
		\param p the index of the positive terminal of port 2
		\param q the index of the negative terminal of port 2
	**/
	void stampTransconductance2(double transconductance12, double transconductance21, unsigned int m, unsigned int n, unsigned int p, unsigned int q);

	/**
		\brief stamps a partial conductance into conductance matrix at given indices in matrix

		Unlike the other stamp methods, such as stampConductance(), this method stamps a value
		into the conductance matrix at only the given indices of the matrix, rather than stamping
		a full conductance for indexed terminals.  This method is used to perform custom conductance
		stamping that is not covered by the other stamp methods.

		\param conductance the conductance to stamp into matrix; can be positive or negative
		\param r the nonzero row index of the conductance matrix to stamp into
		\param c the nonzero column index of the conductance matrix to stamp into

	**/
	void stampPartialConductance(double conductance, unsigned int r, unsigned int c);

	/**
		\brief stamps the incidence constants for an ideal voltage source into conductance matrix
		for	modified nodal analysis

		\param solution_id the non-zero index in solution vector for where the source's solution is
		to be placed; in other words, the index for row and column for where incidence constants are
		to be placed.  This index should not be set equal to any node index in the system.
		\param p the index of the node where positive terminal of source resides
		\param n the index of the node where negative terminal of source resides
	**/
	void stampIdealVoltageSourceIncidence(unsigned int solution_id, unsigned int p, unsigned int n);

	/**
		\brief checks if generated matrix is invertible (non-singular)
		\return true if invertible (non-singular); false if non-invertible (singular)
	**/
	bool isInvertible() const;

	/**
	 * inverts the conductance matrix and stores the result into itself
	 * \throw std::runtime_error if matrix is singular (non-invertible)
	 * \see isInvertible() to check if matrix is invertible
	 */
	void invertSelf();

	/**
		\brief inverts the conductance matrix and returns the result

		This method does not alter the matrix; for that, see invertSelf().

		\throw std::runtime_error if matrix is singular (non-invertible)
		\see isInvertible() to check if matrix is invertible
		\return conductance matrix generator with the inverted matrix
	**/
	SystemConductanceGenerator invert() const;

	/**
	 * generates a sparsity pattern of conductance matrix and returns pattern as a printable string
	 *
	 * In the SPY, 'X' is a non-zero value, '.' is a zero value
	 *
	 * @return string that stores the sparsity pattern
	 */
	std::string spy() const;

	/**
	 * generates a sparsity pattern of conductance matrix and stores into a text file given by filename
	 *
	 * In the SPY, 'X' is a non-zero value, '.' is a zero value
	 *
	 * @param filename filename of the text file to store sparsity pattern
	 */
	void exportSpy(std::string filename) const;

	/**
		\brief converts the conductance matrix to a printable character string
		\return a constant string of the converted matrix; same as buffer.c_str();
	**/
	std::string asString() const;

	/**
	 * exports the conductance matrix to a MATLAB ASCII double text file
	 *
	 * The exported file can be opened in MATLAB/Octave using load 'file.txt' command
	 * MATLAB/Octave expects ASCII matrix files to be saved as text file, not as .MAT.
	 * .MAT files are binary and interpreted as such by MATLAB/Octave.
	 *
	 * @param filename filename of the text file to store matrix
	 */
	void exportAsASCIIMatlab(std::string filename) const;

	/**
	 * exports the conductance matrix to a Comma Separated Value (CSV) text file
	 *
	 * The exported file can be opened by many tools that support CSV, including:
	 * Mathworks MATLAB / GNU Octave (dlmread, csvread)\n
	 * MS Excel\n
	 * Kst2\n
	 * Gnuplot\n
	 * and many more.
	 *
	 * @param filename filename of the text file to store matrix
	 */
	void exportAsCSV(std::string filename) const;

	/**
	 * exports the conductance matrix to a C/C++ Header file containing a constant 2D array
	 *
	 * The exported header file can be used in projects based on the LB-LMC codebase
	 *
	 * @param filename filename of the header file to store matrix as array, without file extension; the actual filename will be "<filename>.hpp"
	 * @param mat_name C/C++ compatible name for the matrix array in header file; with no spaces
	 * \deprecated This method might be replaced or removed in future versions of the library
	 */
	void exportAsCHeader(std::string filename, std::string mat_name) const;

	/**
	 * imports a matrix from a given MATLAB ASCII text file into the conductance matrix
	 *
	 * The imported MATLAB ASCII file must have been generated with MATLAB command: save file.txt matrix -ascii -double
	 *
	 * In this version, the imported matrix from the file is expected to be square and same dimension as this object.
	 * Also, this version does not check the file for proper formatting, so beware!
	 *
	 * @param filename filename of the matlab ASCII text file from which to import the matrix
	 */
	void importFromASCIIMatlab(std::string filename);

	/**
		\brief exports the conductance matrix as C/C++ code definition for a literal (const static) array

		The exported code is insertable into generated C/C++ source code

		\param buf the string buffer that will store the code definition of the literal array
		\param mat_name C/C++ compatible name for the matrix array; with no spaces
	**/
	std::string asCLiteral(std::string mat_name) const;

};

} //namespace lblmc

#endif //SYSTEMCONDUCTANCEGENERATOR_HPP
