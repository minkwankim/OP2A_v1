/*
 * Open-source multi-Physics Phenomena Analyzer (OP2A) ver. 1.0
 * 
 *         Copyright (c) 2014-2016 MINKWAN KIM
 *
 * Cell.cpp
 *
 *  Created on: 7 Apr 2016
 *      Author: a1633125
 *
 *	@brief Brief description of the file
 */

#include "Cell.hpp"

namespace GRID {

Cell::Cell()
: id(-1), vol(0.0), vol_inv(0.0), nnodes(0), nfaces(0), type(-1)
{
	for (int n = 0; n < GRID_CELL_MAXNNODES; n++) nodes[n] = 0;
	for (int f = 0; f < GRID_FACE_MAXNNODES; f++) faces[f] = 0;



}

Cell::~Cell() {
	// TODO Auto-generated destructor stub
}

} /* namespace GRID */
