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
#include <vector>
#include <stddef.h>



namespace GRID {

class Cell {
public:
	Cell();
	~Cell();


public:
	int    id;
	double vol;
	double vol_inv;
	double charLength;

	int nnodes;
	int nodes[GRID_CELL_MAXNNODES];

	int nfaces;
	int faces[GRID_CELL_MAXNFACES];

	int type;
	int bc;

	std::vector<double> x;

#ifdef CFD
	double distWall;
#endif

#ifdef PIC
	std::vector< std::vector<std::vector <double> > > shapeFnInverseMatrix;
	std::vector< double> regionCoeff;
#endif

};



class CellCart
{
public:
	CellCart();
	~CellCart();

public:
	int i;
	int j;
	int k;
	int subIndex;
	int lvl;

	std::vector<double> xc;
	std::vector<double> dx;
	double vol;

	int bc;
	int type;

	CellCart* 				parent;
	std::vector<CellCart*>	children;
	bool hasChildren;
	bool needToRefine;


public:
	CellCart* refine();
	void coarse();
	std::vector < std::vector<double> > nodePos();
};


} /* namespace GRID */

#endif /* GRID_CELL_HPP_ */
