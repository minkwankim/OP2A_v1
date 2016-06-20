/*
 * Open-source multi-Physics Phenomena Analyzer (OP2A) ver. 1.0
 * 
 *         Copyright (c) 2014-2016 MINKWAN KIM
 *
 * Matrix.cpp
 *
 *  Created on: 21 Apr 2016
 *      Author: a1633125
 *
 *	@brief Brief description of the file
 */
#include <limits>
#include "Matrix.hpp"
#include "mkl.h"


Matrix::Matrix()
:m_I(0), m_J(0)
{

}

Matrix::Matrix(const unsigned int i, const unsigned int j)
:m_I(i), m_J(j), m_data(i, std::vector<double>(j, 0.0))
{


}

Matrix::Matrix(const std::vector< std::vector< double> >& data_matrix)
:m_I(data_matrix.size()), m_J(data_matrix[0].size()), m_data(data_matrix)
{



}


Matrix::~Matrix()
{
	// TODO Auto-generated destructor stub
}


/*
 * Basic Functions
 */
void Matrix::resize(unsigned int i, unsigned int j)
{
	m_I = i;
	m_J = j;

	m_data.resize(i);
	for (int i = 0; i <= m_I; i++) m_data[i].resize(j);
}

int Matrix::sizeI()
{
	return (m_I);
}

int Matrix::sizeJ()
{
	return (m_J);
}


void Matrix::ones()
{
#pragma omp parallel for
	for (int i = 0; i <= m_I-1; i++)
	{
#pragma ivdep
		for (int j = 0; j <= m_J-1; j++)
		{
			m_data[i][j] = 1.0;
		}
	}
}


void Matrix::ones(unsigned int I, unsigned int J)
{
	resize(I, J);

#pragma omp parallel for
	for (int i = 0; i <= m_I-1; i++)
	{
#pragma ivdep
		for (int j = 0; j <= m_J-1; j++)
		{
			m_data[i][j] = 1.0;
		}
	}
}


void Matrix::zeros()
{
#pragma omp parallel for
	for (int i = 0; i <= m_I-1; i++)
	{
#pragma ivdep
		for (int j = 0; j <= m_J-1; j++)
		{
			m_data[i][j] = 0.0;
		}
	}
}


void Matrix::zeros(unsigned int I, unsigned int J)
{
	resize(I, J);

#pragma omp parallel for
	for (int i = 0; i <= m_I-1; i++)
	{
#pragma ivdep
		for (int j = 0; j <= m_J-1; j++)
		{
			m_data[i][j] = 0.0;
		}
	}
}


void Matrix::diag()
{
	if (m_I != m_J)	Common::ExceptionError(FromHere(), "You need a square matrix to create a diagonal matrix", Common::ErrorCodes::ExceedLimit());


#pragma omp parallel for
	for (int i = 0; i <= m_I-1; i++)
	{
#pragma ivdep
		for (int j = 0; j <= m_J-1; j++)
		{
			m_data[i][j] = 0.0;
		}

		m_data[i][i] = 1.0;
	}
}


void Matrix::diag(unsigned int I)
{
	resize(I, I);

#pragma omp parallel for
	for (int i = 0; i <= m_I-1; i++)
	{
#pragma ivdep
		for (int j = 0; j <= m_J-1; j++)
		{
			m_data[i][j] = 0.0;
		}

		m_data[i][i] = 1.0;
	}
}


void Matrix::diag(unsigned int I, double value)
{
#pragma omp parallel for
	for (int i = 0; i <= m_I-1; i++)
	{
#pragma ivdep
		for (int j = 0; j <= m_J-1; j++)
		{
			m_data[i][j] = 0.0;
		}

		m_data[i][i] = value;
	}
}



void Matrix::removeRow(unsigned int I)
{
	if (I >= m_I)	Common::ExceptionError(FromHere(), "Exceed the row size of a Matrix", Common::ErrorCodes::ExceedLimit());

	int inew, jnew;
	std::vector <std::vector <double> >  data_temp(m_I-1, std::vector<double>(m_J)) ;

#pragma omp parallel for private(inew, jnew)
	for (int i = 0; i <= m_I-1; i++)
	{
		if (i != I)
		{
			if (i < I)	inew	 = i;
			if (i > I)	inew	 = i - 1;

#pragma ivdep
			for (int j = 0; j <= m_J-1; j++)
			{
				data_temp[inew][j] = m_data[i][j];
			}
		}
	}

	resize(m_I-1, m_J);
	m_data = data_temp;
}


void Matrix::removeColumn(unsigned int J)
{
	if (J >= m_J) Common::ExceptionError(FromHere(), "Exceed the column size of a Matrix", Common::ErrorCodes::ExceedLimit());

	int inew, jnew;
	std::vector <std::vector <double> >  data_temp(m_I, std::vector<double>(m_J-1)) ;

#pragma omp parallel for private(inew, jnew)
	for (int i = 0; i <= m_I-1; i++)
	{
#pragma ivdep
		for (int j = 0; j <= m_J-1; j++)
		{
			if (j != J)
			{
				if (j < J)	jnew	 = j;
				if (j > J)	jnew	 = j - 1;

				data_temp[i][jnew] = m_data[i][j];
			}
		}
	}

	resize(m_I, m_J-1);
	m_data = data_temp;
}


double Matrix::sum()
{
	double o_sum = 0.0;
	std::vector<double> o_sumMP(m_I, 0.0);

#pragma omp parallel for
	for (int i = 0; i <= m_I-1; i++)
	{
		double sum_temp;
#pragma omp parallel for reduction(+: sum_temp)
		for (int j = 0; j <= m_J-1; j++)
		{
			sum_temp += m_data[i][j];
		}

		o_sumMP[i] = sum_temp;
	}


#pragma omp parallel for reduction(+: o_sum)
	for (int i = 0; i <= m_I-1; i++)
	{
		o_sum += o_sumMP[i];
	}

	return (o_sum);
}


std::vector < std::vector <double> > Matrix::exportData()
{
	return(m_data);
}

void Matrix::importData(std::vector < std::vector <double> >& data)
{
	m_data = data;
	m_I = data.size();
	m_J = data[0].size();
}



/*
 * Linear Algebra
 */
Matrix& Matrix::minorMatrix(const int r, const int c)
{
	removeRow(r);
	removeColumn(c);

	return *this;
}

Matrix minorMatrix(Matrix& A, const int r, const int c)
{
	Matrix C = A;
	C.minorMatrix(r, c);
	return (C);
}





double detMatrix(Matrix& A)
{
	int m = A.sizeI();
	double det;

	if (m != A.sizeJ()) Common::ExceptionError(FromHere(), "Matrix should be a square matrix to calculate determinant", Common::ErrorCodes::ExceedLimit());
	if (m == 0) Common::ExceptionError(FromHere(), "Input matrix has Zero-dimension", Common::ErrorCodes::ExceedLimit());


	if (m == 1)
	{
		det = A(0, 0);
	}
	else if ( m == 2)
	{
		det = A(0,0)*A(1,1) - A(0,1)*A(1,2);
	}
	else if (m <= 5)
	{
		det = 0.0;

		for (int i = 0; i <= m-1; i++)
		{
			Matrix tempMatrix = minorMatrix(A, 0, i);

			double temp1 = pow(-1.0, i);
			double temp2 = detMatrix(tempMatrix);

			det += temp1*A(0, i) * temp2;
		}
	}
	else
	{
		det = 0.0;

		#pragma omp parallel for reduction(+: det)
		for (int i = 0; i <= m-1; i++)
		{
			Matrix tempMatrix = minorMatrix(A, 0, i);

			double temp1 = pow(-1.0, i);
			double temp2 = detMatrix(tempMatrix);

			det += temp1*A(0, i) * temp2;
		}
	}


	/*
	 * Error Check
	 */
	if (det != det) Common::ExceptionError(FromHere(), "Input matrix has no determinant value:NaN", Common::ErrorCodes::ExceedLimit());
	if (det == std::numeric_limits<double>::infinity()) Common::ExceptionError(FromHere(), "Input matrix has no determinant value:Inf", Common::ErrorCodes::ExceedLimit());

	return (det);
}


Matrix invMatrix(Matrix& A)
{
	int m = A.sizeI();
	if (m != A.sizeJ()) Common::ExceptionError(FromHere(), "Matrix should be a square matrix to calculate inverse matrix", Common::ErrorCodes::ExceedLimit());
	if (m == 0) Common::ExceptionError(FromHere(), "Input matrix has Zero-dimension", Common::ErrorCodes::ExceedLimit());


	Matrix Ainv(m,m);

	int 	i, j;
	int 	info;
	int 	*ipiv;

	info			= 0;
	ipiv			= new int [A.sizeI()+1];
	int 	lwork	= m * m;

	double *work	= new double [lwork];
	double *element = new double [lwork];

	// Store element in Column-major order
#pragma ivdep
	for (i = 0; i <= A.sizeI()-1; i++)
		for (j = 0; j <= A.sizeJ()-1; j++)
			element[i + j*m] = A(i,j);


	dgetrf(&m, &m, element, &m, ipiv, &info);
	dgetri(&m, element, &m, ipiv, work, &lwork, &info);

	int k = 0;
	for (j = 0; j <= A.sizeI()-1; j++)
	{
		for (i = 0; i <= A.sizeI()-1; i++)
		{
			Ainv(i,j) = element[k];
			k++;
		}
	}

	delete[] ipiv;
	delete[] work;
	delete[] element;

	return (Ainv);
}


Matrix confactorMatrix(Matrix& A)
{
	int m = A.sizeI();
	if (m != A.sizeJ()) Common::ExceptionError(FromHere(), "Matrix should be a square matrix to calculate confactor matrix", Common::ErrorCodes::ExceedLimit());
	if (m == 0) Common::ExceptionError(FromHere(), "Input matrix has Zero-dimension", Common::ErrorCodes::ExceedLimit());


	Matrix res(m, m);


#pragma omp parallel
	{
		for (int i = 0; i <= A.sizeI()-1; i++)
		{
#pragma ivdep
			for (int j = 0; j <= A.sizeI()-1; j++)
			{
				Matrix ap = minorMatrix(A, i, j);
				double d = detMatrix(ap);

				res(i,j) = pow(-1.0, i+j) * d;
			}
		}
	}

	return res;
}


Matrix adjointMatrix(Matrix& A)
{
	int m = A.sizeI();
	if (m != A.sizeJ()) Common::ExceptionError(FromHere(), "Matrix should be a square matrix to calculate adjoint matrix", Common::ErrorCodes::ExceedLimit());
	if (m == 0) Common::ExceptionError(FromHere(), "Input matrix has Zero-dimension", Common::ErrorCodes::ExceedLimit());

	Matrix res(m , m);
	Matrix ap = confactorMatrix(A);

#pragma omp parallel
	{
		for (int i = 0; i <= A.sizeI()-1; i++)
		{
#pragma ivdep
			for (int j = 0; j <= A.sizeI()-1; j++)
			{
				res(i, j) = ap(j, i);
			}
		}
	}

	return res;
}



Matrix invMatrix2(Matrix& A)
{
	int m = A.sizeI();
	if (m != A.sizeJ()) Common::ExceptionError(FromHere(), "Matrix should be a square matrix to calculate inverse matrix", Common::ErrorCodes::ExceedLimit());
	if (m == 0) Common::ExceptionError(FromHere(), "Input matrix has Zero-dimension", Common::ErrorCodes::ExceedLimit());

	Matrix res(m, m);
	double 	d = detMatrix(A);


	if (m == 1)		// 1 X 1 MATRIX
	{
		res(0,0) = 1/d;
	}
	else if (m == 2)	// 2 X 2 MATRIX
	{
		res(0,0) =  A(1,1) / d;
		res(0,1) = -A(0,1) / d;

		res(1,0) = -A(1,0) / d;
		res(1,1) =  A(0,0) / d;
	}
	else				// MORE THAN 3 X 3 MATRIX
	{
		// IT USES ADJOINT METHOD
		Matrix ai = adjointMatrix(A);

#pragma omp parallel
	{
		for (int i = 0; i <= m-1; i++)
		{
#pragma ivdep
			for (int j = 0; j <= m-1; j++)
			{
					res(i,j)	= ai(i,j) / d;
			}
		}
	}
	}

	return res;
}






/*
 * Operator Definitions
 */

double& Matrix::operator() (const unsigned int i, const unsigned int j)
{
	if (i >= m_I) Common::ExceptionError(FromHere(), "Exceed the row size of a Matrix", Common::ErrorCodes::ExceedLimit());
	if (j >= m_J) Common::ExceptionError(FromHere(), "Exceed the column size of a Matrix", Common::ErrorCodes::ExceedLimit());

	return (m_data[i][j]);
}


Matrix& Matrix::operator= (std::vector< std::vector<double> >& A)
{
	if (m_I !=  A.size() || m_J != A[0].size())	resize(A.size(), A[0].size());

	m_data	= A;
	return *this;
}


