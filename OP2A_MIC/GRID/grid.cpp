/*
 * Open-source multi-Physics Phenomena Analyzer (OP2A) ver. 1.0
 * 
 *         Copyright (c) 2014-2016 MINKWAN KIM
 *
 * grid.cpp
 *
 *  Created on: 7 Apr 2016
 *      Author: a1633125
 *
 *	@brief Brief description of the file
 */

#include "grid.hpp"
#include "../COMM/assert_mk.hpp"


namespace GRID {


GridSetup::GridSetup()
: gridFactor(1.0), isAxisymmetric(false)
{

}

GridSetup::~GridSetup()
{

}


GridMPI::GridMPI()
	: numLines(0),
	  lines(GRID_MAX_NUM_LINES, std::vector<int>(2, 0)),
	  sendGhostIndex(GRID_MAX_NUM_CPU, std::vector<int>(2, 0)),
	  receiveGhostIndex(GRID_MAX_NUM_CPU, std::vector<int>(2, 0)),
	  sendGhost(NULL),
	  receiveGhost(NULL)
{

}

GridMPI::~GridMPI()
{

}



Grid::Grid()
	:NDIM(0), NNM(0), NFM(0), NCM(0), NGM(0)
{

}

Grid::~Grid()
{

}



Node& Grid::NODE(int nID)
{
	op_assert(nID > 0);

	int nPos = whereisNode[nID];
	return (nodes[nPos]);
	//return (nodeIDMap.find(nID));
}


Face& Grid::FACE(int fID)
{
	op_assert(fID > 0);

	int fPos = whereisFace[fID];
	return(faces[fPos]);
	//return (faceIDMap.find(f));
}

Cell& Grid::CELL(int cID)
{
	if (cID < 0)
	{
		int cPos = whereisGhost[-cID];
		return (cells_ghost[cPos]);
	}
	else
	{
		int cPos = whereisCell[cID];
		return (cells[cPos]);
	}
	//return (cellIDMap.find(c));
}



}
