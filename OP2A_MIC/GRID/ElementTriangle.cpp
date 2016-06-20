/*
 * Open-source multi-Physics Phenomena Analyzer (OP2A) ver. 1.0
 * 
 *         Copyright (c) 2014-2016 MINKWAN KIM
 *
 * ElementTriangle.cpp
 *
 *  Created on: 2 Jun 2016
 *      Author: a1633125
 *
 *	@brief Brief description of the file
 */


#include <vector>

void ElementTRI_RealToMaster(std::vector<double>&x1, std::vector<double>&x2, std::vector<double>&x3, std::vector<double>&x, std::vector<double>& x_master)
{
	double x3_x1 = x3[0] - x1[0];
	double x2_x1 = x2[0] - x1[0];

	double y3_y1 = x3[1] - x1[1];
	double y2_y1 = x2[1] - x1[1];

	double x_x1  = x[0]  - x1[0];
	double y_y1  = x[1]  - x1[1];


	double aux = 1.0 / (x3_x1*y2_y1 - x2_x1*y3_y1);

	x_master[0] = (-y3_y1*x_x1 + x3_x1*y_y1) * aux;
	x_master[1] = ( y2_y1*x_x1 - x2_x1*y_y1) * aux;
}

bool ElementTRI_Inside(std::vector<double>&x_master)
{
	if (x_master[0] >= 0 && x_master[1] >= 0 && (x_master[0]+ x_master[1]) <= 1.0)	return (true);
	else return (false);

}
