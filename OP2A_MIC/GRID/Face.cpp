/*
 * Open-source multi-Physics Phenomena Analyzer (OP2A) ver. 1.0
 * 
 *         Copyright (c) 2014-2016 MINKWAN KIM
 *
 * Face.cpp
 *
 *  Created on: 7 Apr 2016
 *      Author: a1633125
 *
 *	@brief Brief description of the file
 */

#include "Face.hpp"

namespace GRID {

Face::Face()
: id(-1), area(0.0), area_inv(0.0), bc(-1), bcindex(-1), nnodes(0), type(-1)
{
	for (int dim = 0; dim < DIM-1; dim++) x[dim] = 0.0;
	for (int n = 0; n < GRID_FACE_MAXNNODES; n++)	nodes[n] = -1;

#ifdef CFD
	distToWall = 0.0;
	nDotWall = 0.0;

	for (c = 0; c < GRID_FACE_MAXSTENCIL; c++) cr[c] = -1;
	for (c = 0; c < GRID_FACE_MAXSTENCIL; c++) cl[c] = -1;
#endif



}

Face::~Face() {
	// TODO Auto-generated destructor stub
}

} /* namespace GRID */
