/*
 * Open-source multi-Physics Phenomena Analyzer (OP2A) ver. 1.0
 * 
 *         Copyright (c) 2014-2016 MINKWAN KIM
 *
 * processingGrid.cpp
 *
 *  Created on: 22 Apr 2016
 *      Author: a1633125
 *
 *	@brief Brief description of the file
 */


#include <fstream>
#include <sstream>
#include <iostream>
#include <cstring>
#include <iomanip>


#include "grid.hpp"
#include "../MATH/CalArea.hpp"
#include "../COMM/error_codes.hpp"
#include "../COMM/error_exception.hpp"
#include "../MATH/Vector.hpp"


namespace GRID {



void processingGrid(Grid& grid)
{
// Nodes
	if (grid.configutation.gridFactor != 1.0)
	{
#pragma ivdep
		for (int n = 0; n < grid.NNM; n++)
		{
			for (int k = 0; k < grid.NDIM; k++)
			{
				grid.nodes[n].x[k] /= grid.configutation.gridFactor;
			}
		}

	}

	if (grid.configutation.isAxisymmetric == true && grid.NDIM == 2)
	{
#pragma ivdep
		for (int n = 0; n < grid.NNM; n++)
		{
			grid.nodes[n].x[1] = fabs(grid.nodes[n].x[1]) + GRID_AXISMEMTRIC_Y;
		}
	}
	else if (grid.NDIM == 3)
	{
		grid.configutation.isAxisymmetric = false;
	}






// Faces
#pragma ivdep
	for (int f = 0; f < grid.NFM; f++)
	{
		Vector x1;
		Vector x2;
		Vector x3;

		switch (grid.faces[f].type)
		{
		case LINE:
			grid.faces[f].area	= CalArea::line(grid.NODE(grid.faces[f].nodes[0]).x, grid.NODE(grid.faces[f].nodes[1]).x);

			// Tangential vector
			x1.startEnd(grid.NODE(grid.faces[f].nodes[0]).x, grid.NODE(grid.faces[f].nodes[1]).x);
			x1.normalize();
			grid.faces[f].n[GRID_TAN1] = x1.exportData();

			// Normal Vector
			grid.faces[f].n[GRID_NOR][0] =  grid.faces[f].n[GRID_TAN1][1];
			grid.faces[f].n[GRID_NOR][1] = -grid.faces[f].n[GRID_TAN1][0];
			break;

		case TRI3:
			grid.faces[f].area	= CalArea::triangle(grid.NODE(grid.faces[f].nodes[0]).x,
					                                grid.NODE(grid.faces[f].nodes[1]).x,
												    grid.NODE(grid.faces[f].nodes[2]).x);

			// Normal vector
			x1.startEnd(grid.NODE(grid.faces[f].nodes[1]).x, grid.NODE(grid.faces[f].nodes[0]).x);
			x1.normalize();

			x2.startEnd(grid.NODE(grid.faces[f].nodes[2]).x, grid.NODE(grid.faces[f].nodes[0]).x);
			x2.normalize();

			x3 = crossProduct(x1, x2);
			grid.faces[f].n[GRID_NOR] = x3.exportData();

			// Tangential Vector
			grid.faces[f].n[GRID_TAN1] = x1.exportData();

			x2 = crossProduct(x3, x1);
			grid.faces[f].n[GRID_TAN2] = x2.exportData();
			break;


		case QUAD4:
			grid.faces[f].area	= CalArea::quadrilateral(grid.NODE(grid.faces[f].nodes[0]).x,
					                                     grid.NODE(grid.faces[f].nodes[1]).x,
														 grid.NODE(grid.faces[f].nodes[2]).x,
														 grid.NODE(grid.faces[f].nodes[3]).x);
			// Normal vector
			x1.startEnd(grid.NODE(grid.faces[f].nodes[1]).x, grid.NODE(grid.faces[f].nodes[0]).x);
			x1.normalize();

			x2.startEnd(grid.NODE(grid.faces[f].nodes[2]).x, grid.NODE(grid.faces[f].nodes[0]).x);
			x2.normalize();

			x3.startEnd(grid.NODE(grid.faces[f].nodes[3]).x, grid.NODE(grid.faces[f].nodes[0]).x);
			x3.normalize();

			Vector x4 = normalFromThreePoint(x1, x2, x3);
			x4.normalize();

			grid.faces[f].n[GRID_NOR] = x4.exportData();

			// Tangential Vector
			grid.faces[f].n[GRID_TAN1] = x2.exportData();

			x1 = crossProduct(x4, x2);
			grid.faces[f].n[GRID_TAN2] = x1.exportData();
			break;
		}

		for (int d = 0; d < grid.NDIM; d++)
		{
			grid.faces[f].x[d] = 0.0;
			for (int n = 0; n < grid.faces[f].nnodes; n++)	grid.faces[f].x[d] += grid.NODE(grid.faces[f].nodes[n]).x[d];
			grid.faces[f].x[d] /= grid.faces[f].nnodes;
		}

		grid.faces[f].area_inv = 1.0 / grid.faces[f].area;
	}



	// Cell
#pragma ivdep
	for (int c = 0; c < grid.NCM; c++)
	{
		// Find Xc
		for (int d = 0; d < grid.NDIM; d++)
		{
			grid.cells[c].x[d] = 0.0;
			for (int n = 0; n < grid.cells[c].nnodes; n++)	grid.cells[c].x[d] += grid.NODE(grid.cells[c].nodes[n]).x[d];
			grid.cells[c].x[d] /= grid.cells[c].nnodes;
		}


		// Calculate Volume/Area and Characteristic length
		std::vector<double> xf1(grid.NDIM);
		std::vector<double> xf2(grid.NDIM);
		std::vector<double> xf3(grid.NDIM);
		std::vector<double> xf4(grid.NDIM);

		double aux1, aux2;

		switch (grid.cells[c].type)
		{
		case TRI3:
			grid.cells[c].vol	= CalArea::triangle(grid.NODE(grid.cells[c].nodes[0]).x,
													grid.NODE(grid.cells[c].nodes[1]).x,
													grid.NODE(grid.cells[c].nodes[2]).x);

			grid.cells[c].charLength = sqrt(grid.cells[c].vol);
			break;

		case QUAD4:
			grid.cells[c].vol	= CalArea::quadrilateral(grid.NODE(grid.cells[c].nodes[0]).x,
													     grid.NODE(grid.cells[c].nodes[1]).x,
													     grid.NODE(grid.cells[c].nodes[2]).x,
														 grid.NODE(grid.cells[c].nodes[3]).x);

			aux1 = CalArea::line(grid.NODE(grid.cells[c].nodes[1]).x, grid.NODE(grid.cells[c].nodes[0]).x);
			aux2 = CalArea::line(grid.NODE(grid.cells[c].nodes[3]).x, grid.NODE(grid.cells[c].nodes[0]).x);
			grid.cells[c].charLength = fmin(aux1, aux2);
			break;

		case TETRA4:
			grid.cells[c].vol	= CalArea::tetrahedron(grid.NODE(grid.cells[c].nodes[0]).x,
													   grid.NODE(grid.cells[c].nodes[1]).x,
													   grid.NODE(grid.cells[c].nodes[2]).x,
													   grid.NODE(grid.cells[c].nodes[3]).x);

			for (int k = 0; k < grid.NDIM; k++)	xf1[k]	= (grid.NODE(grid.cells[c].nodes[0]).x[k] + grid.NODE(grid.cells[c].nodes[1]).x[k] + grid.NODE(grid.cells[c].nodes[2]).x[k]) / 3.0;
			for (int k = 0; k < grid.NDIM; k++)	xf2[k]	= (grid.NODE(grid.cells[c].nodes[0]).x[k] + grid.NODE(grid.cells[c].nodes[1]).x[k] + grid.NODE(grid.cells[c].nodes[3]).x[k]) / 3.0;
			for (int k = 0; k < grid.NDIM; k++)	xf3[k]	= (grid.NODE(grid.cells[c].nodes[1]).x[k] + grid.NODE(grid.cells[c].nodes[2]).x[k] + grid.NODE(grid.cells[c].nodes[3]).x[k]) / 3.0;
			for (int k = 0; k < grid.NDIM; k++)	xf4[k]	= (grid.NODE(grid.cells[c].nodes[0]).x[k] + grid.NODE(grid.cells[c].nodes[3]).x[k] + grid.NODE(grid.cells[c].nodes[2]).x[k]) / 3.0;


			aux1 = CalArea::line(grid.cells[c].x, xf1);
			aux1 = fmin(CalArea::line(grid.cells[c].x, xf2), aux1);
			aux1 = fmin(CalArea::line(grid.cells[c].x, xf3), aux1);
			aux1 = fmin(CalArea::line(grid.cells[c].x, xf4), aux1);

			grid.cells[c].charLength = 2.0 * aux1;
			break;

		case HEXA8:
			grid.cells[c].vol	= CalArea::hexahedron(grid.NODE(grid.cells[c].nodes[0]).x,
													  grid.NODE(grid.cells[c].nodes[1]).x,
													  grid.NODE(grid.cells[c].nodes[2]).x,
													  grid.NODE(grid.cells[c].nodes[3]).x,
													  grid.NODE(grid.cells[c].nodes[4]).x,
													  grid.NODE(grid.cells[c].nodes[5]).x,
													  grid.NODE(grid.cells[c].nodes[6]).x,
													  grid.NODE(grid.cells[c].nodes[7]).x);

			aux1 = CalArea::line(grid.NODE(grid.cells[c].nodes[1]).x, grid.NODE(grid.cells[c].nodes[0]).x);
			aux2 = CalArea::line(grid.NODE(grid.cells[c].nodes[2]).x, grid.NODE(grid.cells[c].nodes[0]).x);

			aux1 = fmin(aux1, aux2);
			aux2 = CalArea::line(grid.NODE(grid.cells[c].nodes[3]).x, grid.NODE(grid.cells[c].nodes[0]).x);

			grid.cells[c].charLength = fmin(aux1, aux2);
			break;

		case PRISM6:
			grid.cells[c].vol	= CalArea::prism(grid.NODE(grid.cells[c].nodes[0]).x,
											     grid.NODE(grid.cells[c].nodes[1]).x,
											     grid.NODE(grid.cells[c].nodes[2]).x,
												 grid.NODE(grid.cells[c].nodes[3]).x,
												 grid.NODE(grid.cells[c].nodes[4]).x,
												 grid.NODE(grid.cells[c].nodes[5]).x);

			for (int k = 0; k < grid.NDIM; k++)	xf1[k]	= grid.NODE(grid.cells[c].nodes[1]).x[k] - grid.NODE(grid.cells[c].nodes[0]).x[k];
			for (int k = 0; k < grid.NDIM; k++)	xf2[k]	= grid.NODE(grid.cells[c].nodes[2]).x[k] - grid.NODE(grid.cells[c].nodes[0]).x[k];
			for (int k = 0; k < grid.NDIM; k++)	xf3[k]	= grid.NODE(grid.cells[c].nodes[3]).x[k] - grid.NODE(grid.cells[c].nodes[0]).x[k];

			aux1 =  pow(0.5*(xf1[1]*xf2[2] - xf1[2]*xf2[1]), 2.0);
			aux1 += pow(-0.5*(xf1[0]*xf2[2] - xf1[2]*xf2[0]), 2.0);
			aux1 += pow(0.5*(xf1[0]*xf2[1] - xf1[1]*xf2[0]), 2.0);
			aux1 = pow(aux1, 0.25);

			aux2 = CalArea::line(grid.NODE(grid.cells[c].nodes[3]).x, grid.NODE(grid.cells[c].nodes[0]).x);


			grid.cells[c].charLength = fmin(aux1, aux2);
			break;

		case PYRAMID5:
			grid.cells[c].vol	= CalArea::pyramid(grid.NODE(grid.cells[c].nodes[0]).x,
												   grid.NODE(grid.cells[c].nodes[1]).x,
												   grid.NODE(grid.cells[c].nodes[2]).x,
												   grid.NODE(grid.cells[c].nodes[3]).x,
												   grid.NODE(grid.cells[c].nodes[4]).x);


			grid.cells[c].charLength = pow(grid.cells[c].vol, 1.0/6.0);
			break;
		}

		grid.cells[c].vol_inv = 1.0 / grid.cells[c].vol;

	}

	// Create Ghost cells
	int counterNGM = 0;
	for (int f = 0; f < grid.NFM; f++)
	{
		if (grid.faces[f].cr[0] == 0 || grid.faces[f].cl[0] == 0) counterNGM++;
	}
	grid.cells_ghost.resize(counterNGM);
	grid.whereisGhost.resize(counterNGM+1);

	int gID = 1;
	int gPos = 0;
	for (int f = 0; f < grid.NFM; f++)
	{
		if (grid.faces[f].cr[0] == 0)
		{
			grid.faces[f].cr[0] = -gID;

			grid.cells_ghost[gPos].id     = -gID;
			grid.cells_ghost[gPos].nnodes = grid.faces[f].nnodes;
			for (int n = 0; n < grid.cells_ghost[gPos].nnodes; n++) grid.cells_ghost[gPos].nodes[n] = grid.faces[f].nodes[n];

			grid.cells_ghost[gPos].nfaces = 1;
			grid.cells_ghost[gPos].faces[0] = grid.faces[f].id;

			grid.cells_ghost[gPos].bc = grid.faces[f].bc;
			grid.cells_ghost[gPos].type = -1;

			grid.cells_ghost[gPos].vol = grid.faces[f].area;
			grid.cells_ghost[gPos].vol_inv = grid.faces[f].area_inv;

			for (int k = 0; k < grid.NDIM; k++)
			{
				grid.cells_ghost[gPos].x[k] = 2.0*grid.faces[f].x[k] - grid.CELL(grid.faces[f].cl[0]).x[k];
			}

			//grid.cellIDMap.insert(-counterNGM, &grid.cells_ghost[counterNGM-1]);
			if (gPos + 1 < grid.whereisGhost.size()) grid.whereisGhost.resize(gPos+1);
			grid.whereisGhost[gID] = gPos;

			gPos++;
			gID++;
		}
		else if (grid.faces[f].cl[0] == 0)
		{
			grid.faces[f].cl[0] = grid.faces[f].cr[0];

			for (int k1 = 0; k1 < grid.NDIM; k1++)
			{
				for (int k2 = 0; k2 < grid.NDIM; k2++)
				{
					grid.faces[f].n[k1][k2]	= -grid.faces[f].n[k1][k2];
				}
			}
			grid.faces[f].cr[0] = -gID;


			grid.cells_ghost[gPos].id     = -gID;
			grid.cells_ghost[gPos].nnodes = grid.faces[f].nnodes;
			for (int n = 0; n < grid.cells_ghost[gPos].nnodes; n++) grid.cells_ghost[gPos].nodes[n] = grid.faces[f].nodes[n];

			grid.cells_ghost[gPos].nfaces   = 1;
			grid.cells_ghost[gPos].faces[0] = grid.faces[f].id;

			grid.cells_ghost[gPos].bc   = grid.faces[f].bc;
			grid.cells_ghost[gPos].type = -1;

			grid.cells_ghost[gPos].vol     = grid.faces[f].area;
			grid.cells_ghost[gPos].vol_inv = grid.faces[f].area_inv;

			for (int k = 0; k < grid.NDIM; k++)
			{
				grid.cells_ghost[gPos].x[k] = 2.0*grid.faces[f].x[k] - grid.CELL(grid.faces[f].cl[0]).x[k];
			}

			//grid.cellIDMap.insert(-counterNGM, &grid.cells_ghost[counterNGM-1]);
			if (gPos + 1 < grid.whereisGhost.size()) grid.whereisGhost.resize(gPos+1);
			grid.whereisGhost[gID] = gPos;

			gPos++;
			gID++;
		}
	}

	grid.NGM = counterNGM;

	Cell CL;
	Cell CR;
	Cell CLL;
	Cell CRR;

#ifdef CFD
	// Calculate distance to wall
	// 1. Initizlize variables
	std::vector<unsigned int> walkBound(grid.whereisCell.size(), 0);			// Walk bound of cell ID
	std::vector<Face*>        nearWallFace(grid.whereisCell.size(), NULL);	    // Near wall face of cell ID
	for (int c = 0; c < grid.NCM; c++) grid.cells[c].distWall	= 1.E10;

	// 2. Calculate distance of 1st Wall layer
	int	num_WallFaces	= 0;
	int	num_WalkCells	= 0;

	for (int g = 0; g < grid.NGM; g++)
	{
		if (isWallTypeBC(grid.cells_ghost[g].bc))
		{
			num_WallFaces++;

			Face wallFace       = grid.FACE(grid.cells_ghost[g].faces[0]);
			Cell firstLayerCell = grid.CELL(wallFace.cl[0]);

			if (firstLayerCell.id > 0) // Non-ghost
			{
				walkBound[firstLayerCell.id] = 1;
				nearWallFace[firstLayerCell.id] = &wallFace;
				num_WalkCells++;

				firstLayerCell.distWall = CalArea::line(firstLayerCell.x, wallFace.x);
			}
			else
			{
				std::ostringstream error_message;
				error_message << "Face ID:" << wallFace.id << " has problem in stencil. CL cannot be ghost";
				Common::ExceptionError(FromHere(), error_message.str(), Common::ErrorCodes::NotSupportedType());
			}
		}
	}

	// 3. Calculate path
	int num_Pass = 0;
	Face curr_Face;
	Cell test_Cell;

	while (num_WalkCells > 0 && num_Pass <= GRID_MAX_NUM_PATH)
	{
		num_Pass++;
		num_WalkCells = 0;	// Initialize number of walk cells

		for (int c = 0; c < grid.NCM; c++)
		{
			if (walkBound[grid.cells[c].id] == num_Pass)
			{
				for (int f = 0; f < grid.cells[c].nfaces; f++)
				{
					curr_Face = grid.FACE(grid.cells[c].faces[f]);

					// Find test cell
					if (curr_Face.cl[0] != grid.cells[c].id)	test_Cell = grid.CELL(curr_Face.cl[0]);
					else										test_Cell = grid.CELL(curr_Face.cr[0]);

					// Only for inner cell(Not ghost cells)
					if (test_Cell.id > 0)
					{
						if (walkBound[test_Cell.id] != 1)	// Not for the first laye of walls
						{
							if (nearWallFace[test_Cell.id] == NULL)	// Case: Near wall face is not allocated
							{
								nearWallFace[test_Cell.id]	= &curr_Face;
								walkBound[test_Cell.id]	    = num_Pass + 1;
								num_WalkCells++;

								test_Cell.distWall = CalArea::line(test_Cell.x, curr_Face.x);
							}
							else if (nearWallFace[test_Cell.id] != nearWallFace[grid.cells[c].id])
							{
								double dist = CalArea::line(test_Cell.x, nearWallFace[grid.cells[c].id]->x);

								if (dist < test_Cell.distWall)
								{
									nearWallFace[test_Cell.id] = nearWallFace[grid.cells[c].id];
									walkBound[test_Cell.id]	   = num_Pass + 1;
									num_WalkCells++;

									test_Cell.distWall	= dist;
								}
							}
						}
					}
				}
			}
		}
	}

	// 4. Treatment for ghost cells
	for (int g = 0; g < grid.NGM; g++)
	{
		curr_Face	= grid.FACE(grid.cells_ghost[g].faces[0]);
		grid.cells_ghost[g].distWall = grid.CELL(curr_Face.cl[0]).distWall;
	}

	// 5. Calculate distance and n_dot_wallVector for each faces
	Face*	wall_Face;

	for (int f = 0; f < grid.NFM; f++)
	{
		CL = grid.CELL(grid.faces[f].cl[0]);
		CR = grid.CELL(grid.faces[f].cr[0]);

		grid.faces[f].distWall	= 0.5 * (CL.distWall + CR.distWall);
		wall_Face = nearWallFace[CL.id];

		double n_dot_wall = dotProduct(grid.faces[f].n[GRID_NOR], wall_Face->n[GRID_NOR]);
		grid.faces[f].nDotWall = 1.0 - fabs(n_dot_wall);
	}

	// Find stencil
	Vector xfc(grid.NDIM);
	Vector normal_vector(grid.NDIM);
	double max_dot;
	double dot;

	std::vector<std::vector <int> > neighborList;
	neighborList.resize(grid.NCM + 1);

	for (int c = 0; c < grid.NCM; c++)
	{
		neighborList[grid.cells[c].id].resize(grid.cells[c].nfaces);

		for (int f = 0; f < grid.cells[c].nfaces; f++)
		{
			if (grid.FACE(grid.cells[c].faces[f]).cl[0] == grid.cells[c].id)
			{
				neighborList[grid.cells[c].id][f] = grid.FACE(grid.cells[c].faces[f]).cr[0];
			}
			else
			{
				neighborList[grid.cells[c].id][f] = grid.FACE(grid.cells[c].faces[f]).cl[0];
			}
		}
	}



	for (int f = 0; f < grid.NFM; f++)
	{
		CL	= grid.CELL(grid.faces[f].cl[0]);
		CR	= grid.CELL(grid.faces[f].cr[0]);

		// 1: FIND CLL
		if (CL.id > 0)
		{
			CLL.id 	= 0;
			max_dot = -1.0;

			// GET NORMAL VECTOR TOWARD TO CL
			for (int k = 0; k < grid.NDIM; k++)	normal_vector[k]	= -grid.faces[f].n[GRID_NOR][k];

			// CALCULATE DISTANCE TO CELL-CENTER OF EACH NEIGHBORING CELL
			Cell trial_Cell;

			for (int j = 0; j < neighborList[CL.id].size(); j++)
			{
				trial_Cell = grid.CELL(neighborList[CL.id][j]);
				xfc.startEnd(grid.faces[f].x, trial_Cell.x);
				xfc.normalize();

				dot = dotProduct(normal_vector, xfc);

				if (dot > max_dot)
				{
					max_dot = dot;
					CLL = trial_Cell;
				}
			}

			if (CLL.id == 0)
			{
				CLL	= CL;
			}
		}
		else
		{
			CLL = CL;
		}



		// 2: FIND CRR
		if (CR.id > 0)
		{
			CRR.id  = 0;
			max_dot = -1.0;

			// GET NORMAL VECTOR TOWARD TO CR
			for (int k = 0; k < grid.NDIM-1; k++)	normal_vector[k]	= grid.faces[f].n[GRID_NOR][k];

			// CALCULATE DISTANCE TO CELL-CENTER OF EACH NEIGHBORING CELL
			Cell trial_Cell;


			for (int j = 0; j < neighborList[CR.id].size(); j++)
			{
				trial_Cell = grid.CELL(neighborList[CR.id][j]);
				xfc.startEnd(grid.faces[f].x, trial_Cell.x);
				xfc.normalize();

				dot = dotProduct(normal_vector, xfc);

				if (dot > max_dot)
				{
					max_dot = dot;
					CRR     = trial_Cell;
				}
			}

			if (CRR.id == 0)
			{
				CRR	= CR;
			}
		}
		else
		{
			CRR = CR;
		}


		grid.faces[f].cl[1] = CLL.id;
		grid.faces[f].cr[1] = CRR.id;
	}
#endif



}












}

