/*
 * Open-source multi-Physics Phenomena Analyzer (OP2A) ver. 1.0
 * 
 *         Copyright (c) 2014-2016 MINKWAN KIM
 *
 * ElementTetra.cpp
 *
 *  Created on: 2 Jun 2016
 *      Author: a1633125
 *
 *	@brief Brief description of the file
 */


#include "../MATH/Matrix.hpp"

void ElementTETRA_ToMaster(std::vector<double>&x1, std::vector<double>&x2, std::vector<double>&x3, std::vector<double>&x4,
                                 std::vector<double>&x, std::vector<double>& x_master)
{
	Matrix M(3,3);

	M(0, 0) = x2[0] - x1[0];
	M(0, 1) = x3[0] - x1[0];
	M(0, 2) = x4[0] - x1[0];

	M(1, 0) = x2[1] - x1[1];
	M(1, 1) = x3[1] - x1[1];
	M(1, 2) = x4[1] - x1[1];

	M(2, 0) = x2[2] - x1[2];
	M(2, 1) = x3[2] - x1[2];
	M(2, 2) = x4[2] - x1[2];


	Matrix Minv = invMatrix(M);

	double x_x1 = x[0] - x1[0];
	double y_y1 = x[1] - x1[1];
	double z_z1 = x[2] - x1[2];

	x_master[0] = Minv(0,0)*x_x1 + Minv(0,1)*y_y1 + Minv(0,2)*z_z1;
	x_master[1] = Minv(1,0)*x_x1 + Minv(1,1)*y_y1 + Minv(1,2)*z_z1;
	x_master[2] = Minv(2,0)*x_x1 + Minv(2,1)*y_y1 + Minv(2,2)*z_z1;
}

bool ElementTETRA_Inside(std::vector<double>&x_master)
{
	if (x_master[0] >= 0 && x_master[1] >= 0 && x_master[2] >= 0 && (x_master[0]+ x_master[1] + x_master[2]) <= 1.0)	return (true);
	else return (false);
}
