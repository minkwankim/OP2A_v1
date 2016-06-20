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
#include "../COMM/MultiVector.hpp"

namespace GRID {

Cell::Cell()
: id(-1), vol(0.0), vol_inv(0.0), nnodes(0), nfaces(0), type(-1), bc(-1), x(DIM, 0.0), charLength(0.0)
{
	for (int n = 0; n < GRID_CELL_MAXNNODES; n++) nodes[n] = 0;
	for (int f = 0; f < GRID_FACE_MAXNNODES; f++) faces[f] = 0;

#ifdef CFD
	distWall = 0.0;
#endif

}

Cell::~Cell() {
	// TODO Auto-generated destructor stub
}


CellCart::CellCart()
: i(0), j(0), k(0), subIndex(0), lvl(0), bc(-1), type(-1), vol(0.0), xc(DIM, 0.0), dx(DIM, 0.0), parent(NULL), hasChildren(false), needToRefine(false)
{
	children.reserve(GRID_REFINEMENT);
}


CellCart::~CellCart()
{
	for (int ic = 0; ic < children.size(); ic++)  children[ic] = NULL;
}


CellCart* CellCart::refine()
{
	CellCart* cell_children;
	cell_children = new CellCart [GRID_REFINEMENT];

	children.resize(GRID_REFINEMENT);
	for (int ic = 0; ic < children.size(); ic++)  children[ic] = &cell_children[ic];
	hasChildren  = true;
	needToRefine = false;

	CellCart* parentPtr = this;
	for (int ic = 0; ic < children.size(); ic++)  children[ic]->parent = parentPtr;

	int startIndex = subIndex * 4;

	for (int ic = 0; ic < children.size(); ic++)
	{
		children[ic]->i = i;
		children[ic]->j = j;
		children[ic]->k = k;

		children[ic]->lvl = lvl +1;
		children[ic]->subIndex = startIndex + ic;

		for (int itr_d = 0; itr_d < DIM; itr_d++)	children[ic]->dx[itr_d] = dx[itr_d] / 2.0;

		children[ic]->bc = bc;
		children[ic]->type = type;
		children[ic]->hasChildren = false;
		children[ic]->needToRefine = false;
	}

	if (DIM == 2)
	{
		for (int ic = 0; ic < children.size(); ic++)
		{
			switch (ic)
			{
			case 0:
				children[ic]->xc[0] = xc[0] - children[ic]->dx[0]/2.0;
				children[ic]->xc[1] = xc[1] - children[ic]->dx[1]/2.0;
				break;

			case 1:
				children[ic]->xc[0] = xc[0] + children[ic]->dx[0]/2.0;
				children[ic]->xc[1] = xc[1] - children[ic]->dx[1]/2.0;
				break;

			case 2:
				children[ic]->xc[0] = xc[0] + children[ic]->dx[0]/2.0;
				children[ic]->xc[1] = xc[1] + children[ic]->dx[1]/2.0;
				break;

			case 3:
				children[ic]->xc[0] = xc[0] - children[ic]->dx[0]/2.0;
				children[ic]->xc[1] = xc[1] + children[ic]->dx[1]/2.0;
				break;
			}

			children[ic]->vol = vol / 4.0;
		}
	}
	else
	{
		for (int ic = 0; ic < children.size(); ic++)
	    {
			switch (ic)
			{
			case 0:
				children[ic]->xc[0] = xc[0] - children[ic]->dx[0]/2.0;
				children[ic]->xc[1] = xc[1] - children[ic]->dx[1]/2.0;
				children[ic]->xc[2] = xc[2] - children[ic]->dx[2]/2.0;
				break;

			case 1:
				children[ic]->xc[0] = xc[0] + children[ic]->dx[0]/2.0;
				children[ic]->xc[1] = xc[1] - children[ic]->dx[1]/2.0;
				children[ic]->xc[2] = xc[2] - children[ic]->dx[2]/2.0;
				break;

			case 2:
				children[ic]->xc[0] = xc[0] + children[ic]->dx[0]/2.0;
				children[ic]->xc[1] = xc[1] + children[ic]->dx[1]/2.0;
				children[ic]->xc[2] = xc[2] - children[ic]->dx[2]/2.0;
				break;

			case 3:
				children[ic]->xc[0] = xc[0] - children[ic]->dx[0]/2.0;
				children[ic]->xc[1] = xc[1] + children[ic]->dx[1]/2.0;
				children[ic]->xc[2] = xc[2] - children[ic]->dx[2]/2.0;
				break;

			case 4:
				children[ic]->xc[0] = xc[0] - children[ic]->dx[0]/2.0;
				children[ic]->xc[1] = xc[1] - children[ic]->dx[1]/2.0;
				children[ic]->xc[2] = xc[2] + children[ic]->dx[2]/2.0;
				break;

			case 5:
				children[ic]->xc[0] = xc[0] + children[ic]->dx[0]/2.0;
				children[ic]->xc[1] = xc[1] - children[ic]->dx[1]/2.0;
				children[ic]->xc[2] = xc[2] + children[ic]->dx[2]/2.0;
				break;

			case 6:
				children[ic]->xc[0] = xc[0] + children[ic]->dx[0]/2.0;
				children[ic]->xc[1] = xc[1] + children[ic]->dx[1]/2.0;
				children[ic]->xc[2] = xc[2] + children[ic]->dx[2]/2.0;
				break;

			case 7:
				children[ic]->xc[0] = xc[0] - children[ic]->dx[0]/2.0;
				children[ic]->xc[1] = xc[1] + children[ic]->dx[1]/2.0;
				children[ic]->xc[2] = xc[2] + children[ic]->dx[2]/2.0;
				break;
			}

			children[ic]->vol = vol / 8.0;
		}
	}

	return (cell_children);
}


void CellCart::coarse()
{
	children.clear();
	hasChildren  = false;
	needToRefine = false;

}

std::vector < std::vector<double> > CellCart::nodePos()
{
	std::vector < std::vector<double> > nodes;

	if (DIM == 2)
	{
		Common::MultiVector::resize2D(nodes, 4, 2);

		nodes[0][0] = xc[0] - 0.5*dx[0];
		nodes[0][1] = xc[1] - 0.5*dx[1];

		nodes[1][0] = xc[0] + 0.5*dx[0];
		nodes[1][1] = xc[1] - 0.5*dx[1];

		nodes[2][0] = xc[0] + 0.5*dx[0];
		nodes[2][1] = xc[1] + 0.5*dx[1];

		nodes[3][0] = xc[0] - 0.5*dx[0];
		nodes[3][1] = xc[1] + 0.5*dx[1];
	}
	else
	{
		Common::MultiVector::resize2D(nodes, 8, 3);

		nodes[0][0] = xc[0] - 0.5*dx[0];
		nodes[0][1] = xc[1] - 0.5*dx[1];
		nodes[0][2] = xc[2] - 0.5*dx[2];

		nodes[1][0] = xc[0] + 0.5*dx[0];
		nodes[1][1] = xc[1] - 0.5*dx[1];
		nodes[1][2] = xc[2] - 0.5*dx[2];

		nodes[2][0] = xc[0] + 0.5*dx[0];
		nodes[2][1] = xc[1] + 0.5*dx[1];
		nodes[2][2] = xc[2] - 0.5*dx[2];

		nodes[3][0] = xc[0] - 0.5*dx[0];
		nodes[3][1] = xc[1] + 0.5*dx[1];
		nodes[3][2] = xc[2] - 0.5*dx[2];

		nodes[4][0] = xc[0] - 0.5*dx[0];
		nodes[4][1] = xc[1] - 0.5*dx[1];
		nodes[4][2] = xc[2] + 0.5*dx[2];

		nodes[5][0] = xc[0] + 0.5*dx[0];
		nodes[5][1] = xc[1] - 0.5*dx[1];
		nodes[5][2] = xc[2] + 0.5*dx[2];

		nodes[6][0] = xc[0] + 0.5*dx[0];
		nodes[6][1] = xc[1] + 0.5*dx[1];
		nodes[6][2] = xc[2] + 0.5*dx[2];

		nodes[7][0] = xc[0] - 0.5*dx[0];
		nodes[7][1] = xc[1] + 0.5*dx[1];
		nodes[7][2] = xc[2] + 0.5*dx[2];
	}

	return(nodes);
}



} /* namespace GRID */
