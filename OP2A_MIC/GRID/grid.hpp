/*
 * Open-source multi-Physics Phenomena Analyzer (OP2A) ver. 1.0
 * 
 *         Copyright (c) 2014-2016 MINKWAN KIM
 *
 * grid.hpp
 *
 *  Created on: 7 Apr 2016
 *      Author: a1633125
 *
 *	@brief Brief description of the file
 */

#ifndef GRID_GRID_HPP_
#define GRID_GRID_HPP_

#include <vector>

#include "Node.hpp"
#include "Face.hpp"
#include "Cell.hpp"
#include "../COMM/map1D.hpp"


namespace GRID {


class Grid{

public:
	Grid();
	~Grid();

public:
	int NNM;
	int NFM;
	int NCM;
	int NGM;
	std::vector<Node> nodes;
	std::vector<Face> faces;
	std::vector<Cell> cells;

	Common::Map1D<int, Node*> nodeIDMap;
	Common::Map1D<int, Face*> faceIDMap;
	Common::Map1D<int, Cell*> cellIDMap;


	Node* NODE(unsigned int n);
	Face* FACE(unsigned int n);
	Cell* CELL(unsigned int n);
};


void precessingGridSU2(const  std::string& mesh_file_name_SU2);


}



#endif /* GRID_GRID_HPP_ */
