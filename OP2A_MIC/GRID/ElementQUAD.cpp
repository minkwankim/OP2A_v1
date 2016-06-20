/*
 * Open-source multi-Physics Phenomena Analyzer (OP2A) ver. 1.0
 * 
 *         Copyright (c) 2014-2016 MINKWAN KIM
 *
 * ElementQUAD.cpp
 *
 *  Created on: 3 Jun 2016
 *      Author: a1633125
 *
 *	@brief Brief description of the file
 */


#include <vector>

void ElementQUAD_RealToMaster(std::vector<double>&x1, std::vector<double>&x2, std::vector<double>&x3, std::vector<double>&x4,
                              std::vector<double>&x, std::vector<double>& x_master)
{

	double det_part;
	det_part = (x4[1]-x2[1])*x[0] - (x4[0]-x2[0])*x[1] + (x4[0]*x2[1]-x2[0]*x4[1]);

	if (det_part <= 0.0)
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
	else
	{
		double x3_x2 = x3[0] - x2[0];
		double x3_x4 = x3[0] - x4[0];
		double x2_x4 = x2[0] + x4[0];
		double  x_x3 = x[0]  + x3[0];

		double y3_y2 = x3[1] - x2[1];
		double y3_y4 = x3[1] - x4[1];
		double y2_y4 = x2[1] + x4[1];
		double  y_y3 = x[1]  + x3[1];

		double aux1 = x3_x4*y3_y2 - x3_x2*y3_y4;

		double rhs1 = x_x3 - x2_x4;
		double rhs2 = y_y3 - y2_y4;

		x_master[0] = (y3_y2*rhs1  - x3_x2*rhs2) / aux1;
		x_master[1] = (-y3_y4*rhs1 + x3_x4*rhs2) / aux1;
	}
}



bool ElementQUAD_Inside(std::vector<double>&x_master)
{
	if (x_master[0] >= 0 && x_master[0] <= 1.0 && x_master[1] >= 0.0 && x_master[1] <= 0.0)	return (true);
	else return (false);

}

