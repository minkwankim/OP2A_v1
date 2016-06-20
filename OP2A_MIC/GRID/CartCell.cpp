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

#include "CartCell.hpp"
#include "grid_definitions.hpp"

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
		std::vector<double> Xi(Nx[0]+1);
		std::vector<double> Yj(Nx[1]+1);

		double dX = (Xe[0] - Xs[0]) / Nx[0];
		double dY = (Xe[1] - Xs[1]) / Nx[1];

		Xi[1] = Xs[0] + 0.5*dX;
		Yj[1] = Xs[1] + 0.5*dY;

		for (int i = 2; i <= Nx[0]; i++) Xi[i] = Xi[1] + dX*(i-1.0);
		for (int j = 2; j <= Nx[1]; j++) Yj[j] = Yj[1] + dY*(j-1.0);

		initialize(Nx[0], Nx[1], true);
		CellCart *celldata = new CellCart[(Nx[0]+1)*(Nx[1]+1)];
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
	}
	else
	{
		std::vector<double> Xi(Nx[0]+1);
		std::vector<double> Yj(Nx[1]+1);
		std::vector<double> Zk(Nx[2]+1);


		double dX = (Xe[0] - Xs[0]) / Nx[0];
		double dY = (Xe[1] - Xs[1]) / Nx[1];
		double dZ = (Xe[2] - Xs[2]) / Nx[2];


		Xi[1] = Xs[0] + 0.5*dX;
		Yj[1] = Xs[1] + 0.5*dY;
		Yj[2] = Xs[2] + 0.5*dZ;


		for (int i = 2; i <= Nx[0]; i++) Xi[i] = Xi[1] + dX*(i-1.0);
		for (int j = 2; j <= Nx[1]; j++) Yj[j] = Yj[1] + dY*(j-1.0);
		for (int k = 2; k <= Nx[2]; k++) Zk[k] = Zk[1] + dZ*(k-1.0);


		initialize(Nx[0], Nx[1], Nx[2], true);
		CellCart *celldata = new CellCart[(Nx[0]+1)*(Nx[1]+1)*(Nx[2]+1)];
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
	}
}


void CartCell::refineCell(CellCart* cell)
{
	CellCart* childrens;
	childrens = cell->refine();

	for (int i = 0; i < GRID_REFINEMENT; i++)
		add(childrens[i].i, childrens[i].j, childrens[i].k, childrens[i].lvl, childrens[i].subIndex, &childrens[i]);
}






} /* namespace GRID */

