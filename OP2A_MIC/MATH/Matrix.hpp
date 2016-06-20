/*
 * Open-source multi-Physics Phenomena Analyzer (OP2A) ver. 1.0
 * 
 *         Copyright (c) 2014-2016 MINKWAN KIM
 *
 * Matrix.hpp
 *
 *  Created on: 21 Apr 2016
 *      Author: a1633125
 *
 *	@brief Brief description of the file
 */

#ifndef MATH_MATRIX_HPP_
#define MATH_MATRIX_HPP_

#include <vector>
#include <cmath>
#include <omp.h>
#include "../COMM/error_codes.hpp"
#include "../COMM/error_exception.hpp"



class Matrix {
public:
	Matrix();
	Matrix(const unsigned int I, const unsigned int J);
	Matrix(const std::vector< std::vector< double> >& data_matrix);

	~Matrix();

public:
	void resize(unsigned int I, unsigned int J);
	int sizeI();
	int sizeJ();

	void ones();
	void ones(unsigned int I, unsigned int J);

	void zeros();
	void zeros(unsigned int I, unsigned int J);

	void diag();
	void diag(unsigned int I);
	void diag(unsigned int I, double value);

	void removeRow(unsigned int I);
	void removeColumn(unsigned int J);
	double sum();

	std::vector < std::vector <double> > exportData();
	void importData(std::vector < std::vector <double> >& data);


public:
	Matrix& minorMatrix(const int r, const int c);


public:
	double& operator() (const unsigned int i, const unsigned int j);
	Matrix&	operator= (std::vector< std::vector<double> >& A);


protected:
	unsigned int m_I;
	unsigned int m_J;
	std::vector < std::vector<double> > m_data;
};


/*
 * Linear Algebra functions
 */
Matrix minorMatrix(Matrix& A, const int r, const int c);
double detMatrix(Matrix& A);
Matrix invMatrix(Matrix& A);
Matrix invMatrix2(Matrix& A);
Matrix confactorMatrix(Matrix& A);
Matrix adjointMatrix(Matrix& A);








#endif /* MATH_MATRIX_HPP_ */
