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

namespace GRID {

Node::Node()
: id(-1)
{
	for (int dim = 0; dim < DIM; dim++)	x[dim]	= 0.0;
}

Node::~Node()
{
	// TODO Auto-generated destructor stub
}



} /* namespace GRID */
