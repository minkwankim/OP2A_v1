/*
 * Open-source multi-Physics Phenomena Analyzer (OP2A) ver. 1.0
 * 
 *         Copyright (c) 2014-2016 MINKWAN KIM
 *
 * Node.hpp
 *
 *  Created on: 7 Apr 2016
 *      Author: a1633125
 *
 *	@brief Brief description of the file
 */

#ifndef GRID_NODE_HPP_
#define GRID_NODE_HPP_


#include "grid_definitions.hpp"
#include "../COMM/MultiVector.hpp"

namespace GRID {

class Node {
public:
	Node();
	~Node();


public:
	int id;
	std::vector<double> x;
};


class NodeCart {
public:
	explicit NodeCart(int n, double x_start, double x_end);

	~NodeCart();

public:
	int lowerIndex(double a);
	int upperIndex(double a);
	double Xc_i(int i, int ii);



public:
	std::vector<double> x;


protected:
	int N;
	double x0;
	double xe;
	double dx[GRID_MAX_REFINEMENT_LVL+1];
};




} /* namespace GRID */

#endif /* GRID_NODE_HPP_ */
