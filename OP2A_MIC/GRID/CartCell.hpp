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

#if DIM == 2
#define findStencil findStencil_2D
#define refineAdjustment refineAdjustment_2D
#else
#define findStencil findStencil_3D
#define refineAdjustment refineAdjustment_3D
#endif


namespace GRID {

class CartCell: public DataCart<CellCart> {
public:
	CartCell();
	~CartCell();

public:
	void createBackgroundMesh(std::vector<double> Xs, std::vector<double> Xe, std::vector<int> Nx, std::vector<int> BC);
	void refineCell(CellCart* cell);
	void refineAdjustment_2D(std::vector<CellCart*> children);



	CellCart* findStencil_2D(CellCart* cell, int stencilIndex);
	CellCart* findStencil_3D(CellCart* cell, int stencilIndex);








};

} /* namespace GRID */

#endif /* GRID_CARTCELL_HPP_ */
