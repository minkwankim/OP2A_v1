/*
 * Open-source multi-Physics Phenomena Analyzer (OP2A) ver. 1.0
 * 
 *         Copyright (c) 2014-2016 MINKWAN KIM
 *
 * GridCart.hpp
 *
 *  Created on: 17 Jun 2016
 *      Author: a1633125
 *
 *	@brief Brief description of the file
 */

#ifndef GRID_GRIDCART_HPP_
#define GRID_GRIDCART_HPP_

#include <string>
#include "CartCell.hpp"

namespace GRID {

class GridCart {
public:
	GridCart();
	~GridCart();


};

void writeGridCartTecplot(const  std::string& title, CartCell& cells);


} /* namespace GRID */

#endif /* GRID_GRIDCART_HPP_ */
