/*
 * Open-source multi-Physics Phenomena Analyzer (OP2A) ver. 1.0
 * 
 *         Copyright (c) 2014-2016 MINKWAN KIM
 *
 * Face.hpp
 *
 *  Created on: 7 Apr 2016
 *      Author: a1633125
 *
 *	@brief Brief description of the file
 */

#ifndef GRID_FACE_HPP_
#define GRID_FACE_HPP_

#include "grid_definitions.hpp"
#include <vector>

namespace GRID {

class Face {
public:
	Face();
	~Face();

public:
	int id;
	std::vector<double> x;
	double area;
	double area_inv;

	int bcindex;
	int bc;

	int nnodes;
	std::vector<int> nodes;

	int type;

	std::vector< std::vector<double> > n;



#ifdef CFD
	double distWall;
	double nDotWall;

	int cr[GRID_FACE_MAXSTENCIL];
	int cl[GRID_FACE_MAXSTENCIL];
#endif








};

} /* namespace GRID */

#endif /* GRID_FACE_HPP_ */
