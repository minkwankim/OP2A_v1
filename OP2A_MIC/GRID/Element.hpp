/*
 * Open-source multi-Physics Phenomena Analyzer (OP2A) ver. 1.0
 * 
 *         Copyright (c) 2014-2016 MINKWAN KIM
 *
 * Element.hpp
 *
 *  Created on: 2 Jun 2016
 *      Author: a1633125
 *
 *	@brief Brief description of the file
 */

#ifndef GRID_ELEMENT_HPP_
#define GRID_ELEMENT_HPP_

#include <vector>

void ElementTRI_RealToMaster(std::vector<double>&x1, std::vector<double>&x2, std::vector<double>&x3, std::vector<double>&x, std::vector<double>& x_master);
bool ElementTRI_Inside(std::vector<double>&x_master);

void ElementTETRA_ToMaster(std::vector<double>&x1, std::vector<double>&x2, std::vector<double>&x3, std::vector<double>&x4, std::vector<double>&x, std::vector<double>& x_master);
bool ElementTERRA_Inside(std::vector<double>&x_master);


void ElementQUAD_RealToMaster(std::vector<double>&x1, std::vector<double>&x2, std::vector<double>&x3, std::vector<double>&x4, std::vector<double>&x, std::vector<double>& x_master);
bool ElementQUAD_Inside(std::vector<double>&x_master);


#endif /* GRID_ELEMENT_HPP_ */
