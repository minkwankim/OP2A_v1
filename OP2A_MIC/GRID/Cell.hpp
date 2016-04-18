/*
 * Open-source multi-Physics Phenomena Analyzer (OP2A) ver. 1.0
 * 
 *         Copyright (c) 2014-2016 MINKWAN KIM
 *
 * Cell.hpp
 *
 *  Created on: 7 Apr 2016
 *      Author: a1633125
 *
 *	@brief Brief description of the file
 */

#ifndef GRID_CELL_HPP_
#define GRID_CELL_HPP_

#include "grid_definitions.hpp"




namespace GRID {

class Cell {
public:
	Cell();
	~Cell();


public:
	int    id;
	double vol;
	double vol_inv;

	int nnodes;
	int nodes[GRID_CELL_MAXNNODES];

	int nfaces;
	int faces[GRID_CELL_MAXNFACES];

	int type;
};

} /* namespace GRID */

#endif /* GRID_CELL_HPP_ */
