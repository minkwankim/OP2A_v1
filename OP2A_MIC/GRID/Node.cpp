/*
 * Open-source multi-Physics Phenomena Analyzer (OP2A) ver. 1.0
 * 
 *         Copyright (c) 2014-2016 MINKWAN KIM
 *
 * Node.cpp
 *
 *  Created on: 7 Apr 2016
 *      Author: a1633125
 *
 *	@brief Brief description of the file
 */

#include "Node.hpp"
#include <cmath>


namespace GRID {

Node::Node()
: id(-1), x(DIM, 0.0)
{

}

Node::~Node()
{
	// TODO Auto-generated destructor stub
}



NodeCart::NodeCart(int n, double x_start, double x_end)
:N(n), x0(x_start), xe(x_end), x(n+1)
{
	dx[0] = (xe - x0) / N;
	for (int lvl=1; lvl <= GRID_MAX_REFINEMENT_LVL; lvl++) dx[lvl] = dx[lvl-1] / 2.0;


	x[0] = x0;
	for (int i = 1; i <= N; i++)	x[i] = x[i-1] + dx[0];
}

NodeCart::~NodeCart()
{

}

int NodeCart::lowerIndex(double a)
{
	return (int((a-x0)/dx[0]));
}

int NodeCart::upperIndex(double a)
{
	double aux1 = (a-x0)/dx[0];
	int    aux2 = aux1;

	if ((aux1 - aux2) == 0) return (aux2);
	else					return (aux2+1);
}


double NodeCart::Xc_i(int i, int ii)
{
	int lvl = log2(ii);
	int sub_index = ii - pow(2, lvl);

	double xi = x[i] + 0.5*dx[lvl];
	xi += dx[lvl]*sub_index;

	return (xi);
}


} /* namespace GRID */
