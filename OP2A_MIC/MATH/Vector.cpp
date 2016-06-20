/*
 * Open-source multi-Physics Phenomena Analyzer (OP2A) ver. 1.0
 * 
 *         Copyright (c) 2014-2016 MINKWAN KIM
 *
 * VectorBasic.cpp
 *
 *  Created on: 22 Apr 2016
 *      Author: a1633125
 *
 *	@brief Brief description of the file
 */

#include "Vector.hpp"

#include "../COMM/error_codes.hpp"
#include "../COMM/error_exception.hpp"



Vector::Vector()
:m_I(0)
{
	m_data.reserve(3);
}

Vector::Vector(int ND)
: m_data(ND, 0.0), m_I(ND)
{

}

Vector::Vector(std::vector<double> start)
{
	int m = start.size();
	if (m != 2 && m != 3) Common::ExceptionError(FromHere(), "2D or 3D are only valid in Vector", Common::ErrorCodes::NotSupportedType());

	m_data = start;
	m_I = m;
}


Vector::Vector(std::vector<double> start, std::vector<double> end)
{
	int m = start.size();
	if (end.size() != m) Common::ExceptionError(FromHere(), "The dimensions of two points should be same to form vector", Common::ErrorCodes::Mismatch());
	if (m != 2 && m != 3) Common::ExceptionError(FromHere(), "2D or 3D are only valid in Vector", Common::ErrorCodes::NotSupportedType());

	m_data.resize(m);
	for (int i = 0; i < m; i++) m_data[i] = end[i] - start[i];

	m_I = m;
}


Vector::Vector(Vector start, Vector end)
{
	int m = start.size();
	if (end.size() != m) Common::ExceptionError(FromHere(), "The dimensions of two points should be same to form vector", Common::ErrorCodes::Mismatch());
	if (m != 2 && m != 3) Common::ExceptionError(FromHere(), "2D or 3D are only valid in Vector", Common::ErrorCodes::NotSupportedType());

	m_data.resize(m);
	for (int i = 0; i < m; i++) m_data[i] = end[i] - start[i];

	m_I = m;
}

Vector::~Vector()
{

}


int Vector::size()
{
	return (m_data.size());
}

std::vector<double> Vector::exportData()
{
	return (m_data);
}


void Vector::importData(std::vector <double>& data)
{
	m_data = data;
}


void Vector::startEnd(std::vector<double> start, std::vector<double> end)
{
	int m = start.size();
	if (end.size() != m) Common::ExceptionError(FromHere(), "The dimensions of two points should be same to form vector", Common::ErrorCodes::Mismatch());
	if (m != 2 && m != 3) Common::ExceptionError(FromHere(), "2D or 3D are only valid in Vector", Common::ErrorCodes::NotSupportedType());

	m_data.resize(m);
	for (int i = 0; i < m; i++) m_data[i] = end[i] - start[i];

	m_I = m;
}

void Vector::startEnd(Vector start, Vector end)
{
	int m = start.size();
	if (end.size() != m) Common::ExceptionError(FromHere(), "The dimensions of two points should be same to form vector", Common::ErrorCodes::Mismatch());
	if (m != 2 && m != 3) Common::ExceptionError(FromHere(), "2D or 3D are only valid in Vector", Common::ErrorCodes::NotSupportedType());

	m_data.resize(m);
	for (int i = 0; i < m; i++) m_data[i] = end[i] - start[i];

	m_I = m;
}




double& Vector::operator() (const unsigned int i)
{
	if (i >= m_I) Common::ExceptionError(FromHere(), "Exceed the size of Vector", Common::ErrorCodes::ExceedLimit());
	return (m_data[i]);
}

double& Vector::operator[] (const unsigned int i)
{
	if (i >= m_I) Common::ExceptionError(FromHere(), "Exceed the size of Vector", Common::ErrorCodes::ExceedLimit());
	return (m_data[i]);
}




Vector& Vector::operator= (std::vector<double>& A)
{
	if (m_I !=  A.size())	m_data.resize(A.size());

	m_data	= A;
	return *this;
}


void Vector::normalize()
{
	double len = 0.0;
	for (int i = 0; i < m_I; i++) len += m_data[i]*m_data[i];

	len = sqrt(len);
	for (int i = 0; i < m_I; i++) m_data[i] /= len;
}


double Vector::length()
{
	double len = 0.0;
	for (int i = 0; i < m_I; i++) len += m_data[i]*m_data[i];

	len = sqrt(len);
	return(len);
}


void Vector::rotate(const double angle, int direction)
{
	double cos_theta = cos(angle);
	double sin_theta = sin(angle);

	if (m_I == 2)
	{
		double x = cos_theta*m_data[0] - sin_theta*m_data[1];
		double y = sin_theta*m_data[0] + cos_theta*m_data[1];

		m_data[0] = x;
		m_data[1] = y;
	}
	else
	{
		double x, y, z;
		switch(direction)
		{
		case 0:
			x = m_data[0];
			y = cos_theta*m_data[1] - sin_theta*m_data[2];
			z = sin_theta*m_data[1] + cos_theta*m_data[2];
			break;

		case 1:
			x = cos_theta*m_data[0] + sin_theta*m_data[2];
			y = m_data[1];
			z = -sin_theta*m_data[0] + cos_theta*m_data[2];
			break;

		case 2:
			x = cos_theta*m_data[0] - sin_theta*m_data[1];
			y = sin_theta*m_data[0] + cos_theta*m_data[1];
			z = m_data[2];
			break;
		}

		m_data[0] = x;
		m_data[1] = y;
		m_data[2] = z;
	}
}




double dotProduct(Vector& a, Vector& b)
{
	if (a.size() != b.size())
		Common::ExceptionError(FromHere(), "The dimensions of two points should be same to form vector", Common::ErrorCodes::Mismatch());


	int m = a.size();
	double result = 0.0;
	for (int i = 0; i < m; i++) result += a(i) * b(i);

	return (result);
}


double dotProduct(std::vector<double>& a, std::vector<double>& b)
{
	Vector A(a);
	Vector B(b);

	return (dotProduct(A, B));
}


Vector crossProduct(Vector& a, Vector& b)
{
	int m = a.size();
	if (b.size() != m) Common::ExceptionError(FromHere(), "The dimensions of two points should be same to form vector", Common::ErrorCodes::Mismatch());
	if (m != 3) Common::ExceptionError(FromHere(), "3D vector is required for cross product", Common::ErrorCodes::NotSupportedType());

	Vector res(3);

	res[0] = a[1]*b[2] - a[2]*b[1];
	res[1] = a[2]*b[0] - a[0]*b[2];
	res[2] = a[0]*b[1] - a[1]*b[0];

	return (res);
}


Vector normalFromThreePoint(Vector& a, Vector& b, Vector& c)
{
	Vector ab(a, b);
	Vector ac(a, c);

	Vector res(3);
	res	= crossProduct(ab, ac);

	return (res);
}





