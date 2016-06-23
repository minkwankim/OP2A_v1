/*
 * Open-source multi-Physics Phenomena Analyzer (OP2A) ver. 1.0
 * 
 *         Copyright (c) 2014-2016 MINKWAN KIM
 *
 * CartCell.cpp
 *
 *  Created on: 17 Jun 2016
 *      Author: a1633125
 *
 *	@brief Brief description of the file
 */
#include <algorithm>

#include "CartCell.hpp"
#include "grid_definitions.hpp"
#include "../UTIL/vectorstasticsfn.hpp"


namespace GRID {

CartCell::CartCell(){
	initializeDescendant(GRID_MAX_REFINEMENT_LVL, GRID_REFINEMENT);
}


CartCell::~CartCell() {
	// TODO Auto-generated destructor stub
}



// Create background mesh (Basic grid)
void CartCell::createBackgroundMesh(std::vector<double> Xs, std::vector<double> Xe, std::vector<int> Nx, std::vector<int> BC)
{
	if(DIM == 2)
	{
		std::vector<double> Xi(Nx[0]+2);
		std::vector<double> Yj(Nx[1]+2);

		double dX = (Xe[0] - Xs[0]) / Nx[0];
		double dY = (Xe[1] - Xs[1]) / Nx[1];

		Xi[0] = Xs[0] - 0.5*dX;
		Yj[0] = Xs[1] - 0.5*dY;

		for (int i = 1; i <= Nx[0]+1; i++) Xi[i] = Xi[i-1] + dX;
		for (int j = 1; j <= Nx[1]+1; j++) Yj[j] = Yj[j-1] + dY;

		initialize(Nx[0], Nx[1], true);
		CellCart *celldata  = new CellCart[(Nx[0]+1)*(Nx[1]+1)];
		CellCart *ghostdata = new CellCart[2*(Nx[0]+Nx[1])];

		CellCart *cell;
		int n = 0;

		for (int j = 1; j <= Nx[1]; j++)
		{
			for (int i = 1; i <= Nx[0]; i++)
			{
				cell = &celldata[n];

				cell->i = i;
				cell->j = j;
				cell->k = 0;
				cell->subIndex = 0;
				cell->lvl = 0;

				cell->xc[0] = Xi[i];
				cell->xc[1] = Yj[j];

				cell->dx[0] = dX;
				cell->dx[1] = dY;
				cell->vol   = dX * dY;

				cell->bc = BC_INTERIOR;
				cell->type = 0;

				cell->parent = NULL;
				cell->hasChildren = false;
				cell->needToRefine = false;
				n++;

				add(i,j,cell);
			}
		}

		// Ghost cell setting
		n = 0;
		for (int j = 1; j <= Nx[1]; j++)
		{
			int i;

			// East
			i = 0;
			cell = &ghostdata[n];
			cell->i = i;
			cell->j = j;
			cell->k = 0;
			cell->subIndex = 0;
			cell->lvl = 0;

			cell->xc[0] = Xi[i];
			cell->xc[1] = Yj[j];

			cell->dx[0] = dX;
			cell->dx[1] = dY;
			cell->vol   = 0.0;

			cell->bc = BC[BC_EAST];
			cell->type = -1;

			cell->parent = NULL;
			cell->hasChildren = false;
			cell->needToRefine = false;
			n++;

			add(i,j,cell, false);


			// West
			i = Nx[0] +1;
			cell = &ghostdata[n];
			cell->i = i;
			cell->j = j;
			cell->k = 0;
			cell->subIndex = 0;
			cell->lvl = 0;

			cell->xc[0] = Xi[i];
			cell->xc[1] = Yj[j];

			cell->dx[0] = dX;
			cell->dx[1] = dY;
			cell->vol   = 0.0;

			cell->bc = BC[BC_WEST];
			cell->type = -1;

			cell->parent = NULL;
			cell->hasChildren = false;
			cell->needToRefine = false;
			n++;

			add(i,j,cell, false);
		}

		for (int i = 1; i <= Nx[0]; i++)
		{
			int j;

			// South
			j = 0;
			cell = &ghostdata[n];
			cell->i = i;
			cell->j = j;
			cell->k = 0;
			cell->subIndex = 0;
			cell->lvl = 0;

			cell->xc[0] = Xi[i];
			cell->xc[1] = Yj[j];

			cell->dx[0] = dX;
			cell->dx[1] = dY;
			cell->vol   = 0.0;

			cell->bc = BC[BC_SOUTH];
			cell->type = -1;

			cell->parent = NULL;
			cell->hasChildren = false;
			cell->needToRefine = false;
			n++;

			add(i,j,cell, false);

			// North
			j = Nx[1] + 1;
			cell = &ghostdata[n];
			cell->i = i;
			cell->j = j;
			cell->k = 0;
			cell->subIndex = 0;
			cell->lvl = 0;

			cell->xc[0] = Xi[i];
			cell->xc[1] = Yj[j];

			cell->dx[0] = dX;
			cell->dx[1] = dY;
			cell->vol   = 0.0;

			cell->bc = BC[BC_NORTH];
			cell->type = -1;

			cell->parent = NULL;
			cell->hasChildren = false;
			cell->needToRefine = false;
			n++;

			add(i,j,cell, false);
		}

		m_hasGhostData = true;
	}
	else
	{
		std::vector<double> Xi(Nx[0]+2);
		std::vector<double> Yj(Nx[1]+2);
		std::vector<double> Zk(Nx[2]+2);


		double dX = (Xe[0] - Xs[0]) / Nx[0];
		double dY = (Xe[1] - Xs[1]) / Nx[1];
		double dZ = (Xe[2] - Xs[2]) / Nx[2];


		Xi[0] = Xs[0] - 0.5*dX;
		Yj[0] = Xs[1] - 0.5*dY;
		Zk[0] = Xs[2] - 0.5*dZ;


		for (int i = 1; i <= Nx[0]+1; i++) Xi[i] = Xi[i-1] + dX;
		for (int j = 1; j <= Nx[1]+1; j++) Yj[j] = Yj[j-1] + dY;
		for (int k = 1; k <= Nx[2]+1; k++) Zk[k] = Zk[k-1] + dZ;


		initialize(Nx[0], Nx[1], Nx[2], true);
		CellCart *celldata  = new CellCart[(Nx[0]+1)*(Nx[1]+1)*(Nx[2]+1)];
		CellCart *ghostdata = new CellCart[2*(Nx[0]*Nx[1] + Nx[1]*Nx[2] + Nx[2]*Nx[0])];

		CellCart *cell;
		int n = 0;

		for (int k = 1; k <= Nx[2]; k++)
		{
			for (int j = 1; j <= Nx[1]; j++)
			{
				for (int i = 1; i <= Nx[0]; i++)
				{
					cell = &celldata[n];

					cell->i = i;
					cell->j = j;
					cell->k = k;
					cell->subIndex = 0;
					cell->lvl = 0;

					cell->xc[0] = Xi[i];
					cell->xc[1] = Yj[j];
					cell->xc[2] = Zk[k];


					cell->dx[0] = dX;
					cell->dx[1] = dY;
					cell->dx[2] = dZ;
					cell->vol   = dX * dY * dZ;

					cell->bc = BC_INTERIOR;
					cell->type = 0;

					cell->parent = NULL;
					cell->hasChildren = false;
					cell->needToRefine = false;
					n++;

					add(i,j,k,cell);
				}
			}
		}



		n = 0;
		// TOP and Bottom
		for (int j = 1; j <= Nx[1]; j++)
		{
			for (int i = 1; i <= Nx[0]; i++)
			{

				// Bottom
				int  k = 0;
				cell = &ghostdata[n];
				cell->i = i;
				cell->j = j;
				cell->k = k;
				cell->subIndex = 0;
				cell->lvl = 0;

				cell->xc[0] = Xi[i];
				cell->xc[1] = Yj[j];
				cell->xc[2] = Zk[k];

				cell->dx[0] = dX;
				cell->dx[1] = dY;
				cell->dx[2] = dZ;
				cell->vol   = 0;

				cell->bc = BC[BC_BOTTOM];
				cell->type = -1;

				cell->parent = NULL;
				cell->hasChildren = false;
				cell->needToRefine = false;
				n++;

				add(i,j,k,cell, false);

				// Top
				k = Nx[2] +1;
				cell = &ghostdata[n];
				cell->i = i;
				cell->j = j;
				cell->k = k;
				cell->subIndex = 0;
				cell->lvl = 0;

				cell->xc[0] = Xi[i];
				cell->xc[1] = Yj[j];
				cell->xc[2] = Zk[k];

				cell->dx[0] = dX;
				cell->dx[1] = dY;
				cell->dx[2] = dZ;
				cell->vol   = 0;

				cell->bc = BC[BC_TOP];
				cell->type = -1;

				cell->parent = NULL;
				cell->hasChildren = false;
				cell->needToRefine = false;
				n++;

				add(i,j,k,cell, false);
			}
		}

		// EAST and WEST
		for (int k = 1; k <= Nx[2]; k++)
		{
			for (int j = 1; j <= Nx[1]; j++)
			{
				// EAST
				int i = 0;
				cell = &ghostdata[n];

				cell->i = i;
				cell->j = j;
				cell->k = k;
				cell->subIndex = 0;
				cell->lvl = 0;

				cell->xc[0] = Xi[i];
				cell->xc[1] = Yj[j];
				cell->xc[2] = Zk[k];


				cell->dx[0] = dX;
				cell->dx[1] = dY;
				cell->dx[2] = dZ;
				cell->vol   = dX * dY * dZ;

				cell->bc = BC[BC_EAST];
				cell->type = -1;

				cell->parent = NULL;
				cell->hasChildren = false;
				cell->needToRefine = false;
				n++;

				add(i,j,k,cell, false);


				// WEST
				i = Nx[0]+1;
				cell = &ghostdata[n];

				cell->i = i;
				cell->j = j;
				cell->k = k;
				cell->subIndex = 0;
				cell->lvl = 0;

				cell->xc[0] = Xi[i];
				cell->xc[1] = Yj[j];
				cell->xc[2] = Zk[k];


				cell->dx[0] = dX;
				cell->dx[1] = dY;
				cell->dx[2] = dZ;
				cell->vol   = dX * dY * dZ;

				cell->bc = BC[BC_WEST];
				cell->type = -1;

				cell->parent = NULL;
				cell->hasChildren = false;
				cell->needToRefine = false;
				n++;

				add(i,j,k,cell, false);
			}
		}


		// SOUTH and NORTH
		for (int k = 1; k <= Nx[2]; k++)
		{
			for (int i = 1; i <= Nx[0]; i++)
			{
				// SOUTH
				int j = 0;
				cell = &ghostdata[n];

				cell->i = i;
				cell->j = j;
				cell->k = k;
				cell->subIndex = 0;
				cell->lvl = 0;

				cell->xc[0] = Xi[i];
				cell->xc[1] = Yj[j];
				cell->xc[2] = Zk[k];


				cell->dx[0] = dX;
				cell->dx[1] = dY;
				cell->dx[2] = dZ;
				cell->vol   = dX * dY * dZ;

				cell->bc = BC[BC_SOUTH];
				cell->type = -1;

				cell->parent = NULL;
				cell->hasChildren = false;
				cell->needToRefine = false;
				n++;

				add(i,j,k,cell, false);


				// NORTH
				j = Nx[1]+1;
				cell = &ghostdata[n];

				cell->i = i;
				cell->j = j;
				cell->k = k;
				cell->subIndex = 0;
				cell->lvl = 0;

				cell->xc[0] = Xi[i];
				cell->xc[1] = Yj[j];
				cell->xc[2] = Zk[k];


				cell->dx[0] = dX;
				cell->dx[1] = dY;
				cell->dx[2] = dZ;
				cell->vol   = dX * dY * dZ;

				cell->bc = BC[BC_NORTH];
				cell->type = -1;

				cell->parent = NULL;
				cell->hasChildren = false;
				cell->needToRefine = false;
				n++;

				add(i,j,k,cell, false);
			}
		}

		m_hasGhostData = true;
	}
}


void CartCell::refineCell(CellCart* cell)
{
	if (cell->hasChildren == false && cell->lvl < GRID_MAX_REFINEMENT_LVL)
	{
		CellCart* childrens;
		childrens = cell->refine();

		for (int i = 0; i < GRID_REFINEMENT; i++)
			add(childrens[i].i, childrens[i].j, childrens[i].k, childrens[i].lvl, childrens[i].subIndex, &childrens[i]);


		std::vector<CellCart*> children2(GRID_REFINEMENT, NULL);
		for (int i = 0; i < GRID_REFINEMENT; i++) children2[i] = &childrens[i];

		refineAdjustment(children2);
	}
}


void CartCell::refineAdjustment_2D(std::vector<CellCart*> children)
{
	CellCart* neighbor1;
	CellCart* neighbor2;
	CellCart* neighbor3;
	CellCart* neighbor4;

	for (int i = 0; i < GRID_REFINEMENT; i++)
	{
		neighbor1 = findStencil(children[i], BC_NORTH);
		neighbor2 = findStencil(children[i], BC_EAST);
		neighbor3 = findStencil(children[i], BC_WEST);
		neighbor4 = findStencil(children[i], BC_SOUTH);

		if (neighbor1->hasChildren == false && neighbor1->type >= 0 && neighbor1->lvl < children[i]->lvl-1)
		{
			refineCell(neighbor1);
		}

		if (neighbor2->hasChildren == false && neighbor2->type >= 0 && neighbor2->lvl < children[i]->lvl-1)
		{
			refineCell(neighbor2);
		}

		if (neighbor3->hasChildren == false && neighbor3->type >= 0 && neighbor3->lvl < children[i]->lvl-1)
		{
			refineCell(neighbor3);
		}

		if (neighbor4->hasChildren == false && neighbor4->type >= 0 && neighbor4->lvl < children[i]->lvl-1)
		{
			refineCell(neighbor4);
		}
	}
}


void CartCell::refineAdjustment_3D(std::vector<CellCart*> children)
{
	CellCart* neighbor1;
	CellCart* neighbor2;
	CellCart* neighbor3;
	CellCart* neighbor4;
	CellCart* neighbor5;
	CellCart* neighbor6;


	for (int i = 0; i < GRID_REFINEMENT; i++)
	{
		neighbor1 = findStencil(children[i], BC_NORTH);
		neighbor2 = findStencil(children[i], BC_EAST);
		neighbor3 = findStencil(children[i], BC_WEST);
		neighbor4 = findStencil(children[i], BC_SOUTH);
		neighbor5 = findStencil(children[i], BC_BOTTOM);
		neighbor6 = findStencil(children[i], BC_TOP);

		if (neighbor1->hasChildren == false && neighbor1->type >= 0 && neighbor1->lvl < children[i]->lvl-1)
		{
			refineCell(neighbor1);
		}

		if (neighbor2->hasChildren == false && neighbor2->type >= 0 && neighbor2->lvl < children[i]->lvl-1)
		{
			refineCell(neighbor2);
		}

		if (neighbor3->hasChildren == false && neighbor3->type >= 0 && neighbor3->lvl < children[i]->lvl-1)
		{
			refineCell(neighbor3);
		}

		if (neighbor4->hasChildren == false && neighbor4->type >= 0 && neighbor4->lvl < children[i]->lvl-1)
		{
			refineCell(neighbor4);
		}

		if (neighbor5->hasChildren == false && neighbor5->type >= 0 && neighbor5->lvl < children[i]->lvl-1)
		{
			refineCell(neighbor5);
		}

		if (neighbor6->hasChildren == false && neighbor6->type >= 0 && neighbor6->lvl < children[i]->lvl-1)
		{
			refineCell(neighbor6);
		}



	}
}


CellCart* CartCell::findStencil_2D(CellCart* cell, int stencilIndex)
{

	if (cell == NULL) return(NULL);

	CellCart* neighbor = NULL;


	if (cell->lvl == 0)
	{
		switch(stencilIndex)
		{
		case BC_WEST:
			neighbor = operator()(cell->i-1, cell->j, cell->k);
			break;

		case BC_SOUTH:
			neighbor = operator()(cell->i, cell->j-1, cell->k);
			break;

		case BC_EAST:
			neighbor = operator()(cell->i+1, cell->j, cell->k);
			break;

		case BC_NORTH:
			neighbor = operator()(cell->i, cell->j+1, cell->k);
			break;
		}
	}
	else
	{
		int flag = cell->subIndex % GRID_REFINEMENT;

		switch(flag)
		{
		case 0:
			switch(stencilIndex)
			{
			case BC_WEST:
				neighbor = findStencil_2D(cell->parent, stencilIndex);
				if (neighbor != NULL && neighbor->hasChildren == true) neighbor = neighbor->children[1];
				break;

			case BC_SOUTH:
				neighbor = findStencil_2D(cell->parent, stencilIndex);
				if (neighbor != NULL && neighbor->hasChildren == true) neighbor = neighbor->children[3];
				break;

			case BC_EAST:
				neighbor = operator()(cell->i, cell->j, cell->k, cell->lvl, cell->subIndex+1);
				break;

			case BC_NORTH:
				neighbor = operator()(cell->i, cell->j, cell->k, cell->lvl, cell->subIndex+3);
				break;
			}
			break;

		case 1:
			switch(stencilIndex)
			{
			case BC_WEST:
				neighbor = operator()(cell->i, cell->j, cell->k, cell->lvl, cell->subIndex-1);
				break;

			case BC_SOUTH:
				neighbor = findStencil_2D(cell->parent, stencilIndex);
				if (neighbor != NULL && neighbor->hasChildren == true) neighbor = neighbor->children[2];
				break;

			case BC_EAST:
				neighbor = findStencil_2D(cell->parent, stencilIndex);
				if (neighbor != NULL && neighbor->hasChildren == true) neighbor = neighbor->children[0];
				break;

			case BC_NORTH:
				neighbor = operator()(cell->i, cell->j, cell->k, cell->lvl, cell->subIndex+1);
				break;
			}
			break;

		case 2:
			switch(stencilIndex)
			{
			case BC_WEST:
				neighbor = operator()(cell->i, cell->j, cell->k, cell->lvl, cell->subIndex+1);
				break;

			case BC_SOUTH:
				neighbor = operator()(cell->i, cell->j, cell->k, cell->lvl, cell->subIndex-1);
				break;

			case BC_EAST:
				neighbor = findStencil_2D(cell->parent, stencilIndex);
				if (neighbor != NULL && neighbor->hasChildren == true) neighbor = neighbor->children[3];
				break;

			case BC_NORTH:
				neighbor = findStencil_2D(cell->parent, stencilIndex);
				if (neighbor != NULL && neighbor->hasChildren == true) neighbor = neighbor->children[1];
				break;
			}
			break;

		case 3:
			switch(stencilIndex)
			{
			case BC_WEST:
				neighbor = findStencil_2D(cell->parent, stencilIndex);
				if (neighbor != NULL && neighbor->hasChildren == true) neighbor = neighbor->children[2];
				break;

			case BC_SOUTH:
				neighbor = operator()(cell->i, cell->j, cell->k, cell->lvl, cell->subIndex-3);
				break;

			case BC_EAST:
				neighbor = operator()(cell->i, cell->j, cell->k, cell->lvl, cell->subIndex-1);
				break;

			case BC_NORTH:
				neighbor = findStencil_2D(cell->parent, stencilIndex);
				if (neighbor != NULL && neighbor->hasChildren == true) neighbor = neighbor->children[0];
				break;
			}
			break;
		}
	}

	return (neighbor);
}



CellCart* CartCell::findStencil_3D(CellCart* cell, int stencilIndex)
{

	if (cell == NULL) return(NULL);

	CellCart* neighbor = NULL;


	if (cell->lvl == 0)
	{
		switch(stencilIndex)
		{
		case BC_WEST:
			neighbor = operator()(cell->i-1, cell->j, cell->k);
			break;

		case BC_SOUTH:
			neighbor = operator()(cell->i, cell->j-1, cell->k);
			break;

		case BC_EAST:
			neighbor = operator()(cell->i+1, cell->j, cell->k);
			break;

		case BC_NORTH:
			neighbor = operator()(cell->i, cell->j+1, cell->k);
			break;

		case BC_BOTTOM:
			neighbor = operator()(cell->i, cell->j, cell->k-1);
			break;

		case BC_TOP:
			neighbor = operator()(cell->i, cell->j, cell->k+1);
			break;
		}
	}
	else
	{
		int flag = cell->subIndex % GRID_REFINEMENT;

		switch(flag)
		{
		case 0:
			switch(stencilIndex)
			{
			case BC_WEST:
				neighbor = findStencil(cell->parent, stencilIndex);
				if (neighbor != NULL && neighbor->hasChildren == true) neighbor = neighbor->children[1];
				break;

			case BC_SOUTH:
				neighbor = findStencil(cell->parent, stencilIndex);
				if (neighbor != NULL && neighbor->hasChildren == true) neighbor = neighbor->children[3];
				break;

			case BC_EAST:
				neighbor = operator()(cell->i, cell->j, cell->k, cell->lvl, cell->subIndex+1);
				break;

			case BC_NORTH:
				neighbor = operator()(cell->i, cell->j, cell->k, cell->lvl, cell->subIndex+3);
				break;

			case BC_BOTTOM:
				neighbor = findStencil(cell->parent, stencilIndex);
				if (neighbor != NULL && neighbor->hasChildren == true) neighbor = neighbor->children[4];
				break;

			case BC_TOP:
				neighbor = operator()(cell->i, cell->j, cell->k, cell->lvl, cell->subIndex+4);
				break;
			}
			break;

		case 1:
			switch(stencilIndex)
			{
			case BC_WEST:
				neighbor = operator()(cell->i, cell->j, cell->k, cell->lvl, cell->subIndex-1);
				break;

			case BC_SOUTH:
				neighbor = findStencil(cell->parent, stencilIndex);
				if (neighbor != NULL && neighbor->hasChildren == true) neighbor = neighbor->children[2];
				break;

			case BC_EAST:
				neighbor = findStencil(cell->parent, stencilIndex);
				if (neighbor != NULL && neighbor->hasChildren == true) neighbor = neighbor->children[0];
				break;

			case BC_NORTH:
				neighbor = operator()(cell->i, cell->j, cell->k, cell->lvl, cell->subIndex+1);
				break;

			case BC_BOTTOM:
				neighbor = findStencil(cell->parent, stencilIndex);
				if (neighbor != NULL && neighbor->hasChildren == true) neighbor = neighbor->children[5];
				break;

			case BC_TOP:
				neighbor = operator()(cell->i, cell->j, cell->k, cell->lvl, cell->subIndex+4);
				break;
			}
			break;

		case 2:
			switch(stencilIndex)
			{
			case BC_WEST:
				neighbor = operator()(cell->i, cell->j, cell->k, cell->lvl, cell->subIndex+1);
				break;

			case BC_SOUTH:
				neighbor = operator()(cell->i, cell->j, cell->k, cell->lvl, cell->subIndex-1);
				break;

			case BC_EAST:
				neighbor = findStencil(cell->parent, stencilIndex);
				if (neighbor != NULL && neighbor->hasChildren == true) neighbor = neighbor->children[3];
				break;

			case BC_NORTH:
				neighbor = findStencil(cell->parent, stencilIndex);
				if (neighbor != NULL && neighbor->hasChildren == true) neighbor = neighbor->children[1];
				break;

			case BC_BOTTOM:
				neighbor = findStencil(cell->parent, stencilIndex);
				if (neighbor != NULL && neighbor->hasChildren == true) neighbor = neighbor->children[6];
				break;

			case BC_TOP:
				neighbor = operator()(cell->i, cell->j, cell->k, cell->lvl, cell->subIndex+4);
				break;

			}
			break;

		case 3:
			switch(stencilIndex)
			{
			case BC_WEST:
				neighbor = findStencil(cell->parent, stencilIndex);
				if (neighbor != NULL && neighbor->hasChildren == true) neighbor = neighbor->children[2];
				break;

			case BC_SOUTH:
				neighbor = operator()(cell->i, cell->j, cell->k, cell->lvl, cell->subIndex-3);
				break;

			case BC_EAST:
				neighbor = operator()(cell->i, cell->j, cell->k, cell->lvl, cell->subIndex-1);
				break;

			case BC_NORTH:
				neighbor = findStencil(cell->parent, stencilIndex);
				if (neighbor != NULL && neighbor->hasChildren == true) neighbor = neighbor->children[0];
				break;

			case BC_BOTTOM:
				neighbor = findStencil(cell->parent, stencilIndex);
				if (neighbor != NULL && neighbor->hasChildren == true) neighbor = neighbor->children[7];
				break;

			case BC_TOP:
				neighbor = operator()(cell->i, cell->j, cell->k, cell->lvl, cell->subIndex+4);
				break;

			}
			break;


		case 4:
			switch(stencilIndex)
			{
			case BC_WEST:
				neighbor = findStencil(cell->parent, stencilIndex);
				if (neighbor != NULL && neighbor->hasChildren == true) neighbor = neighbor->children[5];
				break;

			case BC_SOUTH:
				neighbor = findStencil(cell->parent, stencilIndex);
				if (neighbor != NULL && neighbor->hasChildren == true) neighbor = neighbor->children[7];
				break;

			case BC_EAST:
				neighbor = operator()(cell->i, cell->j, cell->k, cell->lvl, cell->subIndex+1);
				break;

			case BC_NORTH:
				neighbor = operator()(cell->i, cell->j, cell->k, cell->lvl, cell->subIndex+3);
				break;

			case BC_BOTTOM:
				neighbor = operator()(cell->i, cell->j, cell->k, cell->lvl, cell->subIndex-4);
				break;

			case BC_TOP:
				neighbor = findStencil(cell->parent, stencilIndex);
				if (neighbor != NULL && neighbor->hasChildren == true) neighbor = neighbor->children[0];
				break;
			}
			break;

		case 5:
			switch(stencilIndex)
			{
			case BC_WEST:
				neighbor = operator()(cell->i, cell->j, cell->k, cell->lvl, cell->subIndex-1);
				break;

			case BC_SOUTH:
				neighbor = findStencil(cell->parent, stencilIndex);
				if (neighbor != NULL && neighbor->hasChildren == true) neighbor = neighbor->children[6];
				break;

			case BC_EAST:
				neighbor = findStencil(cell->parent, stencilIndex);
				if (neighbor != NULL && neighbor->hasChildren == true) neighbor = neighbor->children[4];
				break;

			case BC_NORTH:
				neighbor = operator()(cell->i, cell->j, cell->k, cell->lvl, cell->subIndex+1);
				break;

			case BC_BOTTOM:
				neighbor = operator()(cell->i, cell->j, cell->k, cell->lvl, cell->subIndex-4);
				break;

			case BC_TOP:
				neighbor = findStencil(cell->parent, stencilIndex);
				if (neighbor != NULL && neighbor->hasChildren == true) neighbor = neighbor->children[1];
				break;
			}
			break;

		case 6:
			switch(stencilIndex)
			{
			case BC_WEST:
				neighbor = operator()(cell->i, cell->j, cell->k, cell->lvl, cell->subIndex+1);
				break;

			case BC_SOUTH:
				neighbor = operator()(cell->i, cell->j, cell->k, cell->lvl, cell->subIndex-1);
				break;

			case BC_EAST:
				neighbor = findStencil(cell->parent, stencilIndex);
				if (neighbor != NULL && neighbor->hasChildren == true) neighbor = neighbor->children[7];
				break;

			case BC_NORTH:
				neighbor = findStencil(cell->parent, stencilIndex);
				if (neighbor != NULL && neighbor->hasChildren == true) neighbor = neighbor->children[5];
				break;

			case BC_BOTTOM:
				neighbor = operator()(cell->i, cell->j, cell->k, cell->lvl, cell->subIndex-4);
				break;

			case BC_TOP:
				neighbor = findStencil(cell->parent, stencilIndex);
				if (neighbor != NULL && neighbor->hasChildren == true) neighbor = neighbor->children[2];
				break;
			}
			break;

		case 7:
			switch(stencilIndex)
			{
			case BC_WEST:
				neighbor = findStencil(cell->parent, stencilIndex);
				if (neighbor != NULL && neighbor->hasChildren == true) neighbor = neighbor->children[6];
				break;

			case BC_SOUTH:
				neighbor = operator()(cell->i, cell->j, cell->k, cell->lvl, cell->subIndex-3);
				break;

			case BC_EAST:
				neighbor = operator()(cell->i, cell->j, cell->k, cell->lvl, cell->subIndex-1);
				break;

			case BC_NORTH:
				neighbor = findStencil(cell->parent, stencilIndex);
				if (neighbor != NULL && neighbor->hasChildren == true) neighbor = neighbor->children[4];
				break;

			case BC_BOTTOM:
				neighbor = operator()(cell->i, cell->j, cell->k, cell->lvl, cell->subIndex-4);
				break;

			case BC_TOP:
				neighbor = findStencil(cell->parent, stencilIndex);
				if (neighbor != NULL && neighbor->hasChildren == true) neighbor = neighbor->children[0];
				break;
			}
			break;
		}
	}

	return (neighbor);
}



} /* namespace GRID */

