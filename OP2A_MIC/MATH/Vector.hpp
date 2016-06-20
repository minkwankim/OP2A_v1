/*
 * Open-source multi-Physics Phenomena Analyzer (OP2A) ver. 1.0
 * 
 *         Copyright (c) 2014-2016 MINKWAN KIM
 *
 * VectorBasic.hpp
 *
 *  Created on: 22 Apr 2016
 *      Author: a1633125
 *
 *	@brief Brief description of the file
 */

#ifndef MATH_VECTOR_HPP_
#define MATH_VECTOR_HPP_


#include <vector>

#include "../COMM/common.hpp"
#include "../COMM/noninstantiable.hpp"

class Vector
{
public:
	Vector();
	Vector(int ND);
	Vector(std::vector<double> start);
	Vector(std::vector<double> start, std::vector<double> end);
	Vector(Vector start, Vector end);

	~Vector();

public:
	int size();
	std::vector<double> exportData();
	void importData(std::vector <double>& data);
	void startEnd(std::vector<double> start, std::vector<double> end);
	void startEnd(Vector start, Vector end);


	double& operator() (const unsigned int i);
	double& operator[] (const unsigned int i);

	Vector&	operator= (std::vector<double>& A);

	void normalize();
	double length();
	void rotate(const double angle, int direction);



private:
	std::vector<double> m_data;
	int m_I;

};

double dotProduct(Vector& a, Vector& b);
double dotProduct(std::vector<double>& a, std::vector<double>& b);

Vector crossProduct(Vector& a, Vector& b);
Vector normalFromThreePoint(Vector& a, Vector& b, Vector& c);


#endif /* MATH_VECTOR_HPP_ */
