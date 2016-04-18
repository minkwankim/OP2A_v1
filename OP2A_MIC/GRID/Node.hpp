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

namespace GRID {

class Node {
public:
	Node();
	~Node();


public:
	int id;
	double x[DIM];
};




} /* namespace GRID */

#endif /* GRID_NODE_HPP_ */
