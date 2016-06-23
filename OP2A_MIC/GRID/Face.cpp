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
: id(-1), area(0.0), area_inv(0.0), bc(-1), bcindex(-1), nnodes(0), type(-1), x(DIM, 0.0), n(DIM, std::vector<double>(DIM, 0.0)), nodes(GRID_FACE_MAXNNODES, -1)
{
#ifdef CFD
	distWall = 0.0;
	nDotWall = 0.0;

	for (int c = 0; c < GRID_FACE_MAXSTENCIL; c++) cr[c] = -1;
	for (int c = 0; c < GRID_FACE_MAXSTENCIL; c++) cl[c] = -1;
#endif

}

Face::~Face() {
	// TODO Auto-generated destructor stub
}



/*
 * FaceCart Class
 */
// Constructor and destructor
FaceCart::FaceCart()
: xc(DIM, 0.0), vol(0.0), direction(-1), bc(-1), type(-1), hasChildren(false), distWall(0.0), nDotWall(0.0)
{
	cl = NULL;
	cr = NULL;

	parent = NULL;
	children.reserve(GRID_REFINEMENT/2);
}

FaceCart::~FaceCart()
{

}









} /* namespace GRID */
