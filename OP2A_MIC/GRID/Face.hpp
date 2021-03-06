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
#include <stddef.h>


class CellCart;

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



class FaceCart {
public:
	FaceCart();
	~FaceCart();

public:
	std::vector<double> xc;
	double vol;
	int direction;

	int bc;
	int type;

	double distWall;
	double nDotWall;

	CellCart* cl;
	CellCart* cr;


	FaceCart* parent;
	std::vector<FaceCart*>	children;
	bool hasChildren;
};





} /* namespace GRID */

#endif /* GRID_FACE_HPP_ */
