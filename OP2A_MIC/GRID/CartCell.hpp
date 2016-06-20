/*
 * Open-source multi-Physics Phenomena Analyzer (OP2A) ver. 1.0
 * 
 *         Copyright (c) 2014-2016 MINKWAN KIM
 *
 * CartCell.hpp
 *
 *  Created on: 17 Jun 2016
 *      Author: a1633125
 *
 *	@brief Brief description of the file
 */

#ifndef GRID_CARTCELL_HPP_
#define GRID_CARTCELL_HPP_


#include "Cell.hpp"
#include "../DATA/DataCart.hpp"

namespace GRID {

class CartCell: public DataCart<CellCart> {
public:
	CartCell();
	~CartCell();

public:
	void createBackgroundMesh(std::vector<double> Xs, std::vector<double> Xe, std::vector<int> Nx, std::vector<int> BC);
	void refineCell(CellCart* cell);


};

} /* namespace GRID */

#endif /* GRID_CARTCELL_HPP_ */
