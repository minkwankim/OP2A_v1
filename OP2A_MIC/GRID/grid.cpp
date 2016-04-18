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


namespace GRID {



Grid::Grid()
	:NNM(0), NFM(0), NCM(0), NGM(0)
{

}

Grid::~Grid()
{

}



Node* Grid::NODE(unsigned int n)
{
	return (nodeIDMap.find(n));
}


Face* Grid::FACE(unsigned int f)
{
	return (faceIDMap.find(f));
}

Cell* Grid::CELL(unsigned int c)
{
	return (cellIDMap.find(c));
}

}
